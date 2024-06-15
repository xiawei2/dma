//
// Created by xiawei on 2024/6/15.
//

#ifndef DMA_QT5VNCCLIENT_H
#define DMA_QT5VNCCLIENT_H

#include <iostream>
#include <thread>

#include "rfbclient.h"
class qt5vncclient {

};
class VncViewer
{
    //if you want to make use of signals/slots, uncomment the line bellow:
    //Q_OBJECT
public:
    virtual ~VncViewer() {};
    void start();
    std::string serverIp;
    int serverPort;
    std::thread *vncThread() const;
    rfbClient *cl;

private:

    std::thread *m_vncThread;
    static void finishedFramebufferUpdateStatic(rfbClient *cl);
    void finishedFramebufferUpdate(rfbClient *cl);
};
void Initialize(VncViewer *vncViewer,char *argv[]);

#endif //DMA_QT5VNCCLIENT_H
