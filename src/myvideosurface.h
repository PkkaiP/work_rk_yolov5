#ifndef MYVIDEOSURFACE_H
#define MYVIDEOSURFACE_H
#include <QAbstractVideoSurface>

class myvideosurface : public QAbstractVideoSurface
{
    Q_OBJECT

public:
    explicit myvideosurface(QObject *parent = nullptr);
    ~myvideosurface() Q_DECL_OVERRIDE;

    //QAbstractVideoSurface抽象基类的虚函数，会自动调用执行，大概在摄像头一开始，比主界面都要先跑起来
    bool present(const QVideoFrame &) Q_DECL_OVERRIDE;                             //每一帧画面将回到这里处理
    bool start(const QVideoSurfaceFormat &) Q_DECL_OVERRIDE;                       //只有摄像头开，就会调用
    void stop() Q_DECL_OVERRIDE;                                                    //出错就停止了
    bool isFormatSupported(const QVideoSurfaceFormat &) const Q_DECL_OVERRIDE;    //将视频流中像素格式转换成格式对等的图片格式，若无对等的格式，返回QImage::Format_Invalid
    QList<QVideoFrame::PixelFormat> supportedPixelFormats(QAbstractVideoBuffer::HandleType type = QAbstractVideoBuffer::NoHandle) const Q_DECL_OVERRIDE;

private:
    QVideoFrame m_currentFrame;                                         //视频帧

signals:
    void frameAvailable(QVideoFrame);                                   //将捕获的每一帧视频通过信号槽方式发出去
};

#endif // MYVIDEOSURFACE_H
