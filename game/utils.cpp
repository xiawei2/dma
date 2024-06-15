//
// Created by 17129 on 2024/6/14.
//

#include "utils.h"
wstring UnicodeToAnsi(vector<BYTE> bytes)
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
wstring UnicodeToAnsi(vector<PBYTE> bytes)
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