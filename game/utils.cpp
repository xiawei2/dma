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

std::vector<int64_t> 取空白Arr(int num) {
    std::vector<int64_t> res;
    for (size_t i = 0; i < num; i++)
    {
        res.push_back(0);
    }
    return res;
}

ByteArr 取空白ByteArr(int num) {
    ByteArr res;
    for (size_t i = 0; i < num; i++)
    {
        res.push_back(0);
    }
    return res;
}

ByteArr 倒转ByteArr(std::vector<BYTE> bytes) {
    ByteArr res;
    for (size_t i = 0; i < bytes.size(); i++)
    {
        res.push_back(bytes[bytes.size() - i - 1]);
    }
    return res;
}

template<typename... Args>
std::vector<BYTE> addBytes(const vector<BYTE> &oldArray, const Args &... newArrayArr) {
    std::vector<BYTE> result = oldArray;

    // 使用折叠表达式累加新数组的长度
    (result.insert(result.end(), newArrayArr.begin(), newArrayArr.end()), ...);
    return result;
}

ByteArr IntToBytes(DWORD64 num) {
    vector<BYTE>bytes;
    for (int i = 0; i < 8; i++)
    {
        int offset = i * 8;
        BYTE byteTmp = (num >> offset) & 0xFF;
        bytes.push_back(byteTmp);
    }
    return bytes;
}

ByteArr AnsiToUnicode(string str) {
    ByteArr Ret;
    DWORD dwNum = MultiByteToWideChar(936, 0, str.c_str(), -1, NULL, 0);
    unsigned char * pwText;
    pwText = new  unsigned char[dwNum * 2];
    MultiByteToWideChar(936, 0, str.c_str(), -1, (LPWSTR)pwText, dwNum * 2);

    for (size_t i = 0; i < dwNum * 2; i++)
    {
        Ret.push_back(pwText[i]);
    }
    Ret.push_back(0);
    Ret.push_back(0);
    return Ret;
}

ByteArr _IntToBytes(DWORD64 num, int lenght) {
    vector<BYTE>bytes;
    for (int i = 0; i < lenght; i++)
    {
        int offset = i * 8;
        BYTE byteTmp = (num >> offset) & 0xFF;
        bytes.push_back(byteTmp);
    }
    return bytes;
}

ByteArr AddBytes(std::vector<BYTE> oldBytes, std::vector<BYTE> newBytes) {
    ByteArr bytes = oldBytes;
//if (oldBytes.size() == 0)
//{
//	return bytes;
//}
    if (newBytes.size() == 0)
    {
        return bytes;
    }

    for (u_int64 i = 1; i <= newBytes.size(); i++)
    {
        bytes.push_back(newBytes[i - (u_int64)1]);
    }

    return bytes;
}
ByteArr AddBytes(std::vector<BYTE> oldBytes, std::vector<BYTE> newBytes,std::vector<BYTE> newBytes2) {
    ByteArr bytes = oldBytes;
//if (oldBytes.size() == 0)
//{
//	return bytes;
//}
    if (newBytes.size() == 0)
    {
        return bytes;
    }

    for (u_int64 i = 1; i <= newBytes.size(); i++)
    {
        bytes.push_back(newBytes[i - (u_int64)1]);
    }
    for (u_int64 i = 1; i <= newBytes2.size(); i++)
    {
        bytes.push_back(newBytes2[i - (u_int64)1]);
    }

    return bytes;
}
long long int GetTime() {
    // 获取当前时间点
    auto now = std::chrono::system_clock::now();

    // 转换为毫秒
    auto now_ms = std::chrono::time_point_cast<std::chrono::milliseconds>(now);
    auto value_ms = now_ms.time_since_epoch();
    long duration_ms = value_ms.count();
    return duration_ms;
}




