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
#include "utils.h"
#include <opencv2/opencv.hpp>

namespace hik
{

    // label enum
    enum LABLE
    {
        PERSON = 0,
        BICYCLE,
        CAR,

    };

}

extern std::unique_ptr<yolov5> yolo;
extern std::map<int, std::string> labels;
extern std::unique_ptr<JsonDeal> jsonDeal;

void CALLBACK globalDecCBFun(int, char *, int, FRAME_INFO *, void *, int);
void CALLBACK globalRealDataCallBack_V30(LONG, DWORD, BYTE *, DWORD, void *);
void write2Json(torch::Tensor);

class HikCam
{
public:
    HikCam(std::string, WORD, std::string, std::string);
    ~HikCam() {}
    int startStream(HWND, LONG, DWORD, DWORD, DWORD);
    void write2Json();

private:
    HikCam();
    // ip address of device
    std::string ipAddr;
    // service port of device
    WORD servicePort = 8000;
    // login username of device
    std::string userName;
    // login password of device
    std::string userPassword;
    // use asyn login or not
    BOOL useAsynLogin = false;
    // login user ID, returned by login function
    int logUserID;
    // play port, returned by getport function
    LONG palyPort;
    friend void CALLBACK globalDecCBFun(int, char *, int, FRAME_INFO *, void *, int);
    friend void CALLBACK globalRealDataCallBack_V30(LONG, DWORD, BYTE *, DWORD, void *);
};
