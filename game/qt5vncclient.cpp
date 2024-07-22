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




// 假设你已经有了一个函数来获取图像宽度和高度
int GetImageWidth(rfbClient *client) { return client->width; }
int GetImageHeight(rfbClient *client) { return client->height; }
typedef struct {
    unsigned int biSize;
    int biWidth;
    int biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int biCompression;
    unsigned int biSizeImage;
    int biXPelsPerMeter;
    int biYPelsPerMeter;
    unsigned int biClrUsed;
    unsigned int biClrImportant;
} DIB_HEADER;


void VncViewer::finishedFramebufferUpdateStatic(rfbClient *cl)
{
    VncViewer *ptr = static_cast<VncViewer*>(rfbClientGetClientData(cl, nullptr));
    ptr->finishedFramebufferUpdate(cl);

}

void VncViewer::finishedFramebufferUpdate(rfbClient *client)
{

// 简化为只保存整个帧缓冲区
    int width = GetImageWidth(client);
    int height = GetImageHeight(client);
    rfbPixelFormat *pf = &client->format;

    // 检查是否为24位或32位颜色
    if (pf->bitsPerPixel != 24 && pf->bitsPerPixel != 32) {
        rfbClientLog("bpp = %d (!=24 or 32)\n", pf->bitsPerPixel);
        return ;
    }

//    FILE *f = fopen("framebuffer.bmp", "wb");
//    if (!f) {
//        rfbClientErr("Could not open framebuffer.bmp\n");
//        return;
//    }

    // BMP文件头
    uint8_t bmpFileHeader[14] = {
            'B', 'M', 0, 0,  0, 0, 0,
            0,   0,   0, 54, 0, 0, 0 // 54是接下来的信息头的大小（字节）
    };

    // BMP信息头
    DIB_HEADER bmpInfoHeader = {
            sizeof(DIB_HEADER), // 信息头大小
            width,              // 图像宽度
            height,             // 图像高度（注意BMP图像是倒序的）
            1,                  // 颜色平面数
            24,                 // 每个像素的位数
            0,                  // 压缩类型（0表示不压缩）
            static_cast<unsigned int>(width * height * 3), // 图像数据大小（字节）
            0,
            0,
            0,
            0 // 分辨率和颜色数等（对24位BMP不重要）
            //        0                     // 重要的颜色数（对24位BMP不重要）
    };

    // 计算文件大小
    bmpFileHeader[2] = (uint8_t)(bmpInfoHeader.biSizeImage + 54);
    bmpFileHeader[3] = (uint8_t)((bmpInfoHeader.biSizeImage + 54) >> 8);
    bmpFileHeader[4] = (uint8_t)((bmpInfoHeader.biSizeImage + 54) >> 16);
    bmpFileHeader[5] = (uint8_t)((bmpInfoHeader.biSizeImage + 54) >> 24);
    // 假设你已经有了bmpFileHeader和bmpInfoHeader的数据
    // 以及height, width, pf, client->frameBuffer等变量

    // 分配足够的内存以存储BMP数据
    size_t totalSize = sizeof(bmpFileHeader) + sizeof(bmpInfoHeader) +
                       height * width * (pf->bitsPerPixel / 8);
//    std::unique_ptr< char []> bmpData(new char[totalSize]);

    if (bmpData == NULL) {
        // 处理内存分配失败的情况
        return ;
    }

    // 将文件头和信息头复制到内存中
    memcpy(bmpData, bmpFileHeader, sizeof(bmpFileHeader));
    memcpy(bmpData + sizeof(bmpFileHeader), &bmpInfoHeader,
           sizeof(bmpInfoHeader));
    // 写入像素数据（注意BMP是倒序的）
    int index = sizeof(bmpFileHeader)+sizeof(bmpInfoHeader);
    for (int j = height - 1; j >= 0; j--) {
        for (int i = 0; i < width; i++) {
            unsigned char *p = client->frameBuffer +
                               (j * client->width + i) * (pf->bitsPerPixel / 8);
            // 如果需要处理32位颜色，请忽略alpha通道
            unsigned char b = (pf->bitsPerPixel == 32) ? p[0] : p[(pf->blueShift >> 3)];
            unsigned char g = (pf->bitsPerPixel == 32) ? p[1] : p[(pf->greenShift >> 3)];
            unsigned char r = (pf->bitsPerPixel == 32) ? p[2] : p[(pf->redShift >> 3)];
            // 计算像素数据的起始位置
            bmpData[index++] = b; // 蓝
            bmpData[index++] = g; // 绿
            bmpData[index++] = r;
            // 直接写入到内存块中
        }
    }
    this->size = index;
    // 使用完后记得释放内存
    return ;
}


void VncViewer::start()
{

    cl = rfbGetClient(8, 3, 4);
    int bitsPerSample = 8;
    int samplesPerPixel = 3;
    cl->format.depth = bitsPerSample*samplesPerPixel;
    cl->format.bitsPerPixel = 32;
    if (cl->format.bitsPerPixel == 8) {
        cl->format.redMax = 7;
        cl->format.greenMax = 7;
        cl->format.blueMax = 3;
        cl->format.redShift = 0;
        cl->format.greenShift = 3;
        cl->format.blueShift = 6;
    } else {
        cl->format.redMax = (1 << bitsPerSample) - 1;
        cl->format.greenMax = (1 << bitsPerSample) - 1;
        cl->format.blueMax = (1 << bitsPerSample) - 1;
        if(!cl->format.bigEndian) {
            cl->format.redShift = 0;
            cl->format.greenShift = bitsPerSample;
            cl->format.blueShift = bitsPerSample * 2;
        } else {
            if(cl->format.bitsPerPixel==8*3) {
                cl->format.redShift = bitsPerSample*2;
                cl->format.greenShift = bitsPerSample*1;
                cl->format.blueShift = 0;
            } else {
                cl->format.redShift = bitsPerSample*3;
                cl->format.greenShift = bitsPerSample*2;
                cl->format.blueShift = bitsPerSample;
            }
        }
    }
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
    size = 53 + cl->width * cl->height * 3;
    bmpData = new unsigned char [size];
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
    m_vncThread->detach();
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
