#pragma once
#include <iostream>
#include <chrono>

inline std::chrono::milliseconds utlsGetCurMiliStamp()
{
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch());
}