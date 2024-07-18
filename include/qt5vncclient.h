//
// Created by xiawei on 2024/6/15.
//

#ifndef DMA_QT5VNCCLIENT_H
#define DMA_QT5VNCCLIENT_H

#include <iostream>
#include <thread>
#include<string>
#include <shared_mutex>
#include "rfbclient.h"
class qt5vncclient {

};

class{
public:
    std::unique_ptr<unsigned char[]> bmpData;
    int size;
} bmpData;



class VncViewer
{
    //if you want to make use of signals/slots, uncomment the line bellow:
    //Q_OBJECT
public:
    VncViewer(){
        serverIp = "127.0.0.1";
        std::cout<<"请输入端口:"<<std::endl;
        std::cin>>serverPort;
        start();
    }

    virtual ~VncViewer() {};
    void start();
     unsigned char* bmpData = new unsigned char[1024*1000*5];
     long long int size ;
    std::string serverIp;
    int serverPort;
    std::thread *vncThread() const;
    rfbClient *cl;
    void KeyPressUp(uint32_t key);
    void KeyPressDown(uint32_t key);
    void KeyPress(uint32_t key,int delay) {
        KeyPressDown(key);
        Sleep(delay);
        KeyPressUp(key);
        Sleep(20);
    }

    void MouseMove(int x,int y);
    void MouseButton(int x,int y,int button,bool press){
        if (!press){
            button=0;
        }
        this->x=x;
        this->y=y;
        SendPointerEvent(cl,x,y,button);
    };
    void MouseLeftDown(){
        MouseButton(x,y,rfbButton1Mask,true);
    };
    void MouseLeftUp(){
        MouseButton(x,y,rfbButton1Mask,false);
    };
    void MouseRightDown(){
        MouseButton(x,y,rfbButton3Mask,true);
    };
    void MouseRightUp(){
        MouseButton(x,y,rfbButton3Mask,false);
    };
    void MouseMiddleDown(){
        MouseButton(x,y,rfbButton2Mask,true);
    };
    void MouseMiddleUp(){
        MouseButton(x,y,rfbButton2Mask,false);
    };
    void MouseWheelUp(){
        MouseButton(x,y,rfbButton4Mask,true);
    };
    void MouseWheelDown(){
        MouseButton(x,y,rfbButton5Mask,true);
    };
    void finishedFramebufferUpdate(rfbClient *cl);

private:

    int x=0;
    int y=0;
    std::thread *m_vncThread;
    static void finishedFramebufferUpdateStatic(rfbClient *cl);




};
void Initialize(VncViewer *vncViewer,char *argv[]);

#endif //DMA_QT5VNCCLIENT_H
