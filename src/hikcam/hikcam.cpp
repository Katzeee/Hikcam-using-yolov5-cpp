#include "hikcam.h"

// void HikCam::camInit(std::string _ipAddr, WORD _servicePort, std::string _userNmae, std::string _userPassword)
//{
// ipAddr = _ipAddr;
// servicePort = _servicePort;
// userName = _userNmae;
// userPassword = _userPassword;
//}

HikCam::HikCam(std::string _ipAddr, WORD _servicePort, std::string _userNmae, std::string _userPassword) : ipAddr(_ipAddr), servicePort(_servicePort), userName(_userNmae), userPassword(_userPassword)
{
    NET_DVR_Init();
    NET_DVR_USER_LOGIN_INFO logInfo = {0};
    NET_DVR_DEVICEINFO_V40 deviceInfo = {0};
    logInfo.bUseAsynLogin = useAsynLogin;
    logInfo.wPort = servicePort;
    memcpy(logInfo.sDeviceAddress, ipAddr.c_str(), NET_DVR_DEV_ADDRESS_MAX_LEN);
    memcpy(logInfo.sUserName, userName.c_str(), NAME_LEN);
    memcpy(logInfo.sPassword, userPassword.c_str(), NAME_LEN);
    logUserID = NET_DVR_Login_V40(&logInfo, &deviceInfo);
    if (logUserID < 0)
    {
        std::cout << "LOGIN ERROR!\t"
                  << "ERRORID:" << NET_DVR_GetLastError() << std::endl;
        NET_DVR_Cleanup();
        throw "Init Error";
    }
}

int HikCam::startStream(HWND _hPlayWnd, LONG _lChannel, DWORD _dwLinkMode, DWORD _bBlocked, DWORD _dwDisplayBufNum)
{
    NET_DVR_PREVIEWINFO previewInfo = {0};
    previewInfo.hPlayWnd = _hPlayWnd; // no window handle
    previewInfo.lChannel = _lChannel; // channel number
    previewInfo.dwLinkMode = _dwLinkMode;
    previewInfo.bBlocked = _bBlocked;
    previewInfo.dwDisplayBufNum = _dwDisplayBufNum;

    if (NET_DVR_RealPlay_V40(logUserID, &previewInfo, globalRealDataCallBack_V30, static_cast<void *>(this)) < 0)
    {
        std::cout << "NET_DVR_RealPlay_V40 ERROR!\t"
                  << "ERRORID:" << NET_DVR_GetLastError() << std::endl;
        return -1;
    }
    while (1)
        ;
    return 0;
}

void CALLBACK globalDecCBFun(int _nPort, char *_pBuf, int _nSize, FRAME_INFO *_pFrameInfo, void *_nUser, int _nReserved2)
{
    HikCam *pHikcam = static_cast<HikCam *>(_nUser);
    static cv::Mat globalBGRImage;
    if (_pFrameInfo->nType == T_YV12)
    {
        // std::cout << "the frame infomation is T_YV12" << std::endl;
        if (globalBGRImage.empty())
        {
            globalBGRImage.create(_pFrameInfo->nHeight, _pFrameInfo->nWidth, CV_8UC3);
        }
        cv::Mat YUVImage(_pFrameInfo->nHeight + _pFrameInfo->nHeight / 2, _pFrameInfo->nWidth, CV_8UC1, (unsigned char *)_pBuf);

        cv::cvtColor(YUVImage, globalBGRImage, cv::COLOR_YUV2BGR_YV12);



        cv::resize(globalBGRImage, globalBGRImage, cv::Size(), 0.3, 0.3);
        clock_t start = clock();
        std::vector<torch::Tensor> r = yolo->prediction(globalBGRImage);
        clock_t ends = clock();
        std::cout << "Running Time : " << (double)(ends - start) / CLOCKS_PER_SEC << std::endl;
        std::cout << r << std::endl;
        globalBGRImage = yolo->drawRectangle(globalBGRImage, r[0], labels); // using r[0] because r is std::vector
        cv::putText(globalBGRImage, cv::String(std::to_string(PlayM4_GetCurrentFrameRate(pHikcam->palyPort))), cv::Point(50, 50), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 2, cv::Scalar::all(255));
        write2Json(r[0]);

        cv::imshow("RGBImage1", globalBGRImage);
        cv::waitKey(1);

        YUVImage.~Mat();
    }
    else
    {
        cv::destroyWindow("RGBImage1");
    }
}

void CALLBACK globalRealDataCallBack_V30(LONG _lPlayHandle, DWORD _dwDataType, BYTE *_pBuffer, DWORD _dwBufSize, void *_pUser)
{
    // std::cout << time(NULL) << "\t(private_v30)Get data,the size is " << _dwBufSize;
    HikCam *pHikcam = static_cast<HikCam *>(_pUser);
    switch (_dwDataType)
    {
    case NET_DVR_SYSHEAD:                        //系统头
        if (!PlayM4_GetPort(&pHikcam->palyPort)) //获取播放库未使用的通道号
        {
            std::cout << "PlayM4_GetPort ERROR!\t"
                      << "ERRORID:" << PlayM4_GetLastError(pHikcam->palyPort);
            break;
        }
        if (!PlayM4_SetStreamOpenMode(pHikcam->palyPort, 0))
        {
            std::cout << "PlayM4_SetStreamOpenMode ERROR!\t"
                      << "ERRORID:" << PlayM4_GetLastError(pHikcam->palyPort);
            break;
        }
        else
        {
            std::cout << "StreamOpenMode: " << PlayM4_GetStreamOpenMode(pHikcam->palyPort);
        }
        if (_dwBufSize > 0)
        {
            if (!PlayM4_OpenStream(pHikcam->palyPort, _pBuffer, _dwBufSize, 1024 * 100000))
            {
                std::cout << "PlayM4_OpenStream ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->palyPort);
                break;
            }
            //设置解码回调函数 仅仅解码不显示
            // if (!PlayM4_SetDecCallBack(pHikcam->_nPort,globalDecCBFun))
            //{
            //    dRet=PlayM4_GetLastError(pHikcam->_nPort);
            //    break;
            //}
            //设置解码回调函数 解码且显示
            if (!PlayM4_SetDecCallBackExMend(pHikcam->palyPort, globalDecCBFun, NULL, 0, static_cast<void *>(pHikcam)))
            {
                std::cout << "PlayM4_SetDecCallBackExMend ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->palyPort);
                break;
            }
            if (!PlayM4_Play(pHikcam->palyPort, 0))
            {
                std::cout << "PlayM4_Play ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->palyPort);
                break;
            }
            else
            {
                std::cout << "success to set play mode" << std::endl;
            }
        }
        break;
    case NET_DVR_STREAMDATA:
        if (_dwBufSize > 0 && pHikcam->palyPort != -1)
        {
            if (!PlayM4_InputData(pHikcam->palyPort, _pBuffer, _dwBufSize))
            {
                std::cout << "fail input data" << std::endl;
                std::cout << PlayM4_GetLastError(pHikcam->palyPort) << std::endl;
            }
        }
        break;
    }
}

void write2Json(torch::Tensor _data)
{
    Json::Value jsondata;
    Json::Value personList;
    Json::Value person;
    auto curTime = utlsGetCurMiliStamp().count();
    jsondata["ShotTime"] = curTime;

    for (int i = 0; i < _data.size(0); i++)
    {
        int dataClass = _data[i][5].item().toInt();
        switch (dataClass)
        {
        case hik::PERSON:
            person["Gender"] = "X";
            person["LeftTopX"] = _data[i][0].item().toInt();
            person["LeftTopY"] = _data[i][1].item().toInt();
            person["RightBtmX"] = _data[i][2].item().toInt();
            person["RightBtmY"] = _data[i][3].item().toInt();
            person["AgeLowerLimit"] = 0;
            person["AgeUpLimit"] = 100;
            person["IsDriver"] = "X";
            personList.append(person);
            break;
        default:
            break;
        };
    }
    jsondata["personList"] = personList;
    std::string result;
    jsonDeal->cvtJson2String(jsondata, result);
    std::cout << result << std::endl;
}