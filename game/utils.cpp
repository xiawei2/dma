//
// Created by 17129 on 2024/6/14.
//

#include "utils.h"
std::wstring UnicodeToAnsi(std::vector<BYTE> bytes)
{
    DWORD size = bytes.size();
    CHAR* Unicode = new CHAR[size];
    for (size_t i = 0; i < size; i++)
    {
        Unicode[i] = bytes[i];
    }
    //UniCode到Ansi
    DWORD Count = WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, 0, 0, 0, FALSE);
    CHAR* Buffer = new CHAR[Count];
    WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, Buffer, Count, NULL, NULL);

    // string转wstring
    DWORD nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, NULL, 0);
    if (nLen == 0)
    {
        return NULL;
    }
    WCHAR* pResult = new WCHAR[nLen];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, pResult, nLen);

    return pResult;
}
#include "utils.h"
std::wstring UnicodeToAnsi(std::vector<PBYTE> bytes)
{
    DWORD size = bytes.size();
    CHAR* Unicode = new CHAR[size];
    for (size_t i = 0; i < size; i++)
    {
        Unicode[i] = *bytes[i];
    }
    //UniCode到Ansi
    DWORD Count = WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, 0, 0, 0, FALSE);
    CHAR* Buffer = new CHAR[Count];
    WideCharToMultiByte(936, 512, (LPCWCH)Unicode, -1, Buffer, Count, NULL, NULL);

    // string转wstring
    DWORD nLen = MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, NULL, 0);
    if (nLen == 0)
    {
        return NULL;
    }
    WCHAR* pResult = new WCHAR[nLen];
    MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, Buffer, -1, pResult, nLen);

    return pResult;
}
void split(const std::wstring& str, std::vector<std::wstring>& tokens, const std::wstring& delimiters)
{
    // Skip delimiters at beginning.
    std::wstring::size_type lastPos = str.find_first_not_of(delimiters, 0);
    // Find first "non-delimiter".
    std::wstring::size_type pos = str.find_first_of(delimiters, lastPos);
    while (std::wstring::npos != pos || std::wstring::npos != lastPos)
    {
        // Found a token, add it to the vector.
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        // Skip delimiters.  Note the "not_of"
        lastPos = str.find_first_not_of(delimiters, pos);
        // Find next "non-delimiter"
        pos = str.find_first_of(delimiters, lastPos);
    }
}


int rnd(int min, int max) {
    static std::random_device rd;  // 用于获取随机数的种子
    static std::mt19937 gen(rd()); // 使用Mersenne Twister算法生成随机数
    std::uniform_int_distribution<> dis(min, max); // 定义均匀分布的范围为[min, max]

    return dis(gen);
}
bool ArePointsEqual(Coordinate index1, Coordinate index2) {
    if (index1.z == index2.z && index1.x == index2.x && index1.y == index2.y) {
        return true;
    }
    return false;
}
std::string WideStringToString(const std::wstring& wstr);
int safeToIntExact(long long value);

std::string WideStringToString(const std::wstring& wstr) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.to_bytes(wstr);
}
int safeToIntExact(long long value) {
    if (value > INT_MAX || value < INT_MIN) {
        throw std::out_of_range("Value is out of int range");
    }
    return static_cast<int>(value);
}
