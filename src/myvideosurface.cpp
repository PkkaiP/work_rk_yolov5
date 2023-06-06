﻿#include "myvideosurface.h"
#include <QVideoFrame>                                                      //视频帧
#include <QVideoSurfaceFormat>                                              //视频的流格式
#include <QDebug>

myvideosurface::myvideosurface(QObject *parent)
    : QAbstractVideoSurface(parent)
{

}

myvideosurface::~myvideosurface()
{

}

//surfaceFormat()----Returns the format of a video surface.
bool myvideosurface::present(const QVideoFrame &frame)
{
    if (frame.isValid())
    {
        QVideoFrame cloneFrame(frame);                                      //每一帧视频都会进入present中，内部机制
        emit frameAvailable(cloneFrame);                                    //直接把视频帧发送出去
        return true;
    }
    stop();
    return false;
}

//这些虚函数，会自动被调用，start检测图像是否可以对等转换，每一帧有没有
bool myvideosurface::start(const QVideoSurfaceFormat &videoformat)
{
    //qDebug() << QVideoFrame::imageFormatFromPixelFormat(videoformat.pixelFormat());              //格式是RGB32
    if(QVideoFrame::imageFormatFromPixelFormat(videoformat.pixelFormat()) != QImage::Format_Invalid && !videoformat.frameSize().isEmpty())
    {
        QAbstractVideoSurface::start(videoformat);
        return true;
    }
    return false;
}

void myvideosurface::stop()
{
    QAbstractVideoSurface::stop();
}

//将视频流中像素格式转换成格式对等的图片格式，若无对等的格式，返回QImage::Format_Invalid
bool myvideosurface::isFormatSupported(const QVideoSurfaceFormat &videoformat) const
{
    //imageFormatFromPixelFormat()-----返回与视频帧像素格式等效的图像格式
    //pixelFormat()-----返回视频流中帧的像素格式
    return QVideoFrame::imageFormatFromPixelFormat(videoformat.pixelFormat()) != QImage::Format_Invalid;
}

//支持的像素格式
QList<QVideoFrame::PixelFormat> myvideosurface::supportedPixelFormats(QAbstractVideoBuffer::HandleType handleType) const
{
    if(handleType == QAbstractVideoBuffer::NoHandle){
        return QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32
                                                 << QVideoFrame::Format_ARGB32
                                                 << QVideoFrame::Format_ARGB32_Premultiplied
                                                 << QVideoFrame::Format_RGB565
                                                 << QVideoFrame::Format_RGB555;
        qDebug() << QList<QVideoFrame::PixelFormat>() << QVideoFrame::Format_RGB32;
    }
    else
    {
        return QList<QVideoFrame::PixelFormat>();
    }
}
