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

int HikCam::getStream()
{
    NET_DVR_PREVIEWINFO previewInfo = {0};
    previewInfo.hPlayWnd = 0;  // no window handle
    previewInfo.lChannel = 33; // channel number
    previewInfo.dwLinkMode = 0;
    previewInfo.bBlocked = 1;
    previewInfo.dwDisplayBufNum = 1;
    if (NET_DVR_RealPlay_V40(logUserID, &previewInfo, globalRealDataCallBack_V30, static_cast<void*>(this)) < 0)
    {
        std::cout << "NET_DVR_RealPlay_V40 ERROR!\t"
                  << "ERRORID:" << NET_DVR_GetLastError() << std::endl;
        return -1;
    }
    while(1);
    return 0;
}

void CALLBACK globalDecCBFun(int nPort, char *pBuf, int nSize, FRAME_INFO *pFrameInfo, void *nUser, int nReserved2)
{
    HikCam* pHikcam = static_cast<HikCam*>(nUser);
    static cv::Mat globalBGRImage;
    if (pFrameInfo->nType == T_YV12)
    {
        //std::cout << "the frame infomation is T_YV12" << std::endl;
        if (globalBGRImage.empty())
        {
            globalBGRImage.create(pFrameInfo->nHeight, pFrameInfo->nWidth, CV_8UC3);
        }
        cv::Mat YUVImage(pFrameInfo->nHeight + pFrameInfo->nHeight / 2, pFrameInfo->nWidth, CV_8UC1, (unsigned char *)pBuf);

        cv::cvtColor(YUVImage, globalBGRImage, cv::COLOR_YUV2BGR_YV12);

        cv::resize(globalBGRImage, globalBGRImage, cv::Size(), 0.3, 0.3);
        clock_t start = clock();
        std::vector<torch::Tensor> r = yolo->prediction(globalBGRImage);
		clock_t ends = clock();
		std::cout << "Running Time : " << (double)(ends - start) / CLOCKS_PER_SEC << std::endl;
        std::cout << r << std::endl;
        globalBGRImage = yolo->drawRectangle(globalBGRImage, r[0], labels); // using r[0] because r is std::vector
        cv::putText(globalBGRImage, cv::String(std::to_string(PlayM4_GetCurrentFrameRate(pHikcam->nPort))), cv::Point(50, 50), cv::FONT_HERSHEY_SCRIPT_SIMPLEX, 2, cv::Scalar::all(255));

        cv::imshow("RGBImage1", globalBGRImage);
        cv::waitKey(1);

        YUVImage.~Mat();
    }
    else
    {
        cv::destroyWindow("RGBImage1");
    }
}

void CALLBACK globalRealDataCallBack_V30(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser)
{
    //std::cout << time(NULL) << "\t(private_v30)Get data,the size is " << dwBufSize;
    HikCam* pHikcam = static_cast<HikCam*>(pUser);
    switch (dwDataType)
    {
    case NET_DVR_SYSHEAD:                  //系统头
        if (!PlayM4_GetPort(&pHikcam->nPort)) //获取播放库未使用的通道号
        {
            std::cout << "PlayM4_GetPort ERROR!\t"
                      << "ERRORID:" << PlayM4_GetLastError(pHikcam->nPort);
            break;
        }
        if (!PlayM4_SetStreamOpenMode(pHikcam->nPort, 0))
        {
            std::cout << "PlayM4_SetStreamOpenMode ERROR!\t"
                      << "ERRORID:" << PlayM4_GetLastError(pHikcam->nPort);
            break;
        }
        else
        {
            std::cout << "StreamOpenMode: " << PlayM4_GetStreamOpenMode(pHikcam->nPort);
        }
        if (dwBufSize > 0)
        {
            if (!PlayM4_OpenStream(pHikcam->nPort, pBuffer, dwBufSize, 1024 * 100000))
            {
                std::cout << "PlayM4_OpenStream ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->nPort);
                break;
            }
            //设置解码回调函数 仅仅解码不显示
            // if (!PlayM4_SetDecCallBack(pHikcam->nPort,globalDecCBFun))
            //{
            //    dRet=PlayM4_GetLastError(pHikcam->nPort);
            //    break;
            //}
            //设置解码回调函数 解码且显示
            if (!PlayM4_SetDecCallBackExMend(pHikcam->nPort, globalDecCBFun, NULL, 0, static_cast<void*>(pHikcam)))
            {
                std::cout << "PlayM4_SetDecCallBackExMend ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->nPort);
                break;
            }
            if (!PlayM4_Play(pHikcam->nPort, 0))
            {
                std::cout << "PlayM4_Play ERROR!\t"
                          << "ERRORID:" << PlayM4_GetLastError(pHikcam->nPort);
                break;
            }
            else
            {
                std::cout << "success to set play mode" << std::endl;
            }
        }
        break;
    case NET_DVR_STREAMDATA:
        if (dwDataType == NET_DVR_STREAMDATA) //码流数据
        {
            if (dwBufSize > 0 && pHikcam->nPort != -1)
            {
                if (!PlayM4_InputData(pHikcam->nPort, pBuffer, dwBufSize))
                {
                    std::cout << "fail input data" << std::endl;
                    std::cout << PlayM4_GetLastError(pHikcam->nPort) << std::endl;
                    // usleep(10000);
                }
                else
                {
                    //std::cout << "success input data" << std::endl;
                }
            }
        }
        break;
    }
}