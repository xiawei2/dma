//
// Created by 17129 on 2024/6/14.
//

#include "Position.h"


int decode(int64_t address) {
    return mem.Read<int>(address);
}
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

Position::Position(int i, int i1, int i2 ) {
    x=i;
    y=i1;
    z=i2;
}
