#pragma once

#include <iostream>
#include <cstring>
#include <string>
#include <unistd.h>
#include <ctime>
#include <cstdlib>
#include "HCNetSDK.h"
#include "LinuxPlayM4.h"
#include "yolov5.h"
#include <opencv2/opencv.hpp>

extern std::unique_ptr<yolov5> yolo;
extern std::map<int, std::string> labels;

void CALLBACK globalDecCBFun(int nPort, char *pBuf, int nSize, FRAME_INFO *pFrameInfo, void *nUser, int nReserved2);
void CALLBACK globalRealDataCallBack_V30(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);

class HikCam
{
public:
    HikCam(std::string _ipAddr, WORD _servicePort, std::string _userNmae, std::string _userPassword);
    ~HikCam(){}
    int getStream();

private:
    HikCam();
    std::string ipAddr;
    WORD servicePort = 8000;
    std::string userName;
    std::string userPassword;
    BOOL useAsynLogin = false;
    int logUserID;
    LONG nPort; // play port
    friend void CALLBACK globalDecCBFun(int nPort, char *pBuf, int nSize, FRAME_INFO *pFrameInfo, void *nUser, int nReserved2);
    friend void CALLBACK globalRealDataCallBack_V30(LONG lPlayHandle, DWORD dwDataType, BYTE *pBuffer, DWORD dwBufSize, void *pUser);
};