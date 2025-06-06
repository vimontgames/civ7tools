#include "stringconvert.h"

//--------------------------------------------------------------------------------------
wstring s2ws(const string& _str)
{
    int len;
    int slength = (int)_str.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, 0, 0);
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, _str.c_str(), slength, buf, len);
    wstring r(buf);
    delete[] buf;
    return r;
}

//--------------------------------------------------------------------------------------
string ws2s(const wstring& _str)
{
    int len;
    int slength = (int)_str.length() + 1;
    len = WideCharToMultiByte(CP_ACP, 0, _str.c_str(), slength, 0, 0, 0, 0);
    char* buf = new char[len];
    WideCharToMultiByte(CP_ACP, 0, _str.c_str(), slength, buf, len, 0, 0);
    string r(buf);
    delete[] buf;
    return r;
}

//--------------------------------------------------------------------------------------
bool isNumber(const std::string & str)
{
    if (str.empty())
        return false;

    size_t start = 0;
    if (str[0] == '-')
    {
        if (str.size() == 1) return false;
        start = 1;
    }

    for (size_t i = start; i < str.size(); ++i)
    {
        if (!std::isdigit(str[i]))
            return false;
    }

    return true;
}