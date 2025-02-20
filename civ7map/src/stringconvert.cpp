#include "stringconvert.h"

//--------------------------------------------------------------------------------------
std::wstring s2ws(const std::string& _str)
{
    int len;
    int slength = (int)_str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}

//--------------------------------------------------------------------------------------
std::string ws2s(const std::wstring& _str)
{
    int len;
    int slength = (int)_str.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, _str.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, _str.c_str(), slength, buf, len, 0, 0);
    std::string r(buf);
    delete[] buf;
    return r;
}