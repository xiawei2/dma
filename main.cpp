//
// Created by 17129 on 2024/6/14.
//
#include "Auto.h"
#include "fastCall.h"

void 神话公告(wstring 内容, int 类型) {
    if (内容 == L"") return;
    LPCWSTR TempStr;
    内容 = L" " + 内容 + L" ";
    TempStr = 内容.c_str();
    std::vector<int64_t> 公告数组 = 取空白Arr(100);
    公告数组[7] = (INT64) TempStr;
    公告数组[23] = 0;
    公告数组[31] = 65535;  //颜色A
    if (类型 == NULL) {
        公告数组[35] = 38;  //普通
    } else {
        公告数组[35] = 17;  //系统
    }
    auto codeCave = mem.FindCodecave(1024);


    std::vector<BYTE> 汇编数据 = {72, 131, 236, 100};
    汇编数据 = AddBytes(汇编数据, {72, 184}, IntToBytes(人物CALL));
    汇编数据 = AddBytes(汇编数据, {255, 208});
    汇编数据 = AddBytes(汇编数据, {72, 163}, IntToBytes(codeCave));
    汇编数据 = AddBytes(汇编数据, {72, 131, 196, 100});


    mem.WriteArray(codeCave, 汇编数据);
    mem.GetShellcode().call_function(汇编数据.data(), (void *) 汇编CALL, "DNF.exe");
    auto ptr = mem.readLong(codeCave);
    printf("公告地址:%llx\n", ptr);
    //*(INT64*)((INT64)公告数组 + 108) = 55512;//颜色B
//    FastCall(神话CALL, (INT64)公告数组);
}

std::wstring stringToWString(const std::string &str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> myconv;
    return myconv.from_bytes(str);
}

int main() {
//    InitCode();
    auto vnc =gameMove.vncViewer;
    while (true) {

        auto bmpdata = vnc.bmpData;
        FILE *f = fopen("framebuffer.bmp", "wb");
        for (int i = 0; i < vnc.size; ++i) {
            fputc(bmpdata[i],f);
        }
        fputc(0,f);
        fclose(f);
        getchar();
//        Sleep(1000);
    }
//    while (true) {
//        if (gameMap.IsSkillFlush(0)){
//            printf("技能冷却完成\n");
//        }else{
//            printf("技能冷却中\n");
//        }
//        Sleep(100);
//    }


}
//    std::cout << "Value: " << prt <<