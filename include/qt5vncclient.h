//
// Created by xiawei on 2024/6/15.
//

#ifndef DMA_QT5VNCCLIENT_H
#define DMA_QT5VNCCLIENT_H

#include <iostream>
#include <thread>
#include<string>
#include "rfbclient.h"
class qt5vncclient {

};
class VncViewer
{
    //if you want to make use of signals/slots, uncomment the line bellow:
    //Q_OBJECT
public:
    VncViewer(){
        std::string ip;
        std::cout<<"请输入ip:"<<std::endl;
        getline(std::cin,serverIp);
        std::cout<<"请输入端口:"<<std::endl;
        std::cin>>serverPort;
        start();
    }
    virtual ~VncViewer() {};
    void start();
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
private:
    int x=0;
    int y=0;
    std::thread *m_vncThread;
    static void finishedFramebufferUpdateStatic(rfbClient *cl);
    void finishedFramebufferUpdate(rfbClient *cl);




};
void Initialize(VncViewer *vncViewer,char *argv[]);

#endif //DMA_QT5VNCCLIENT_H
