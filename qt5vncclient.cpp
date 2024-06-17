/*
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "qt5vncclient.h"

/*
* This is an example on how to make a simple VNC client with
* Qt5 Widgets. It suitable for desktop apps, but may not be
* good for mobile.
* It does not implement any form of cryptography,
* authentication support, client-side cursors or framebuffer
* resizing. If you want to make this a part of your
* application, please notice that you may need to change
* the while(true) loop to disconnect the client.
*
* To build this example with all the other components of
* libvncserver, you may need to explicitly define a C++
* compiler and the path to Qt libs when calling CMake.
* e.g. cmake -DCMAKE_PREFIX_PATH=~/Qt/5.15.2/gcc_64
* -DCMAKE_CXX_COMPILER=g++
*/






void VncViewer::finishedFramebufferUpdateStatic(rfbClient *cl)
{
    VncViewer *ptr = static_cast<VncViewer*>(rfbClientGetClientData(cl, nullptr));
    ptr->finishedFramebufferUpdate(cl);
}

void VncViewer::finishedFramebufferUpdate(rfbClient *cl)
{

}


void VncViewer::start()
{
    cl = rfbGetClient(8, 3, 4);
    cl->format.depth = 24;
    cl->format.depth = 16;
    cl->format.bitsPerPixel = 16;
    cl->format.redShift = 11;
    cl->format.greenShift = 5;
    cl->format.blueShift = 0;
    cl->format.redMax = 0x1f;
    cl->format.greenMax = 0x3f;
    cl->format.blueMax = 0x1f;
    cl->appData.compressLevel = 9;
    cl->appData.qualityLevel = 1;
    cl->appData.encodingsString = "tight ultra";
    cl->FinishedFrameBufferUpdate = finishedFramebufferUpdateStatic;
    cl->serverHost = strdup(serverIp.c_str());
    cl->serverPort = serverPort;
    cl->appData.useRemoteCursor = TRUE;

    rfbClientSetClientData(cl, nullptr, this);

    if (rfbInitClient(cl, 0, nullptr)) {
    } else {
        std::cout << "[INFO] disconnected" << std::endl;
        return;
    }

    m_vncThread = new std::thread([this]() {
        while (true) {
            int i = WaitForMessage(cl, 500);
            if (i < 0) {
                std::cout << "[INFO] disconnected" << std::endl;
                rfbClientCleanup(cl);
                break;
            }

            if (i && !HandleRFBServerMessage(cl)) {
                std::cout << "[INFO] disconnected" << std::endl;
                rfbClientCleanup(cl);
                break;
            }
        };
    });
}

void VncViewer::KeyPressDown(uint32_t key) {
    SendKeyEvent(cl,key, true);
}
void VncViewer::KeyPressUp(uint32_t key) {
    SendKeyEvent(cl,key, false);
}
void VncViewer::MouseMove(int x, int y) {
    MouseButton(x,y,0,false);
}




//char *argv[] = {"","127.0.0.1","5901"};
void Initialize(VncViewer *vncViewer,char *argv[])
{
    argv = argv;
    vncViewer->serverIp = std::string{argv[1]};
    vncViewer->serverPort = std::atoi(argv[2]);
    vncViewer->start();

}
