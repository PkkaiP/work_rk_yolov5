#include "qtcamera.h"
#include <QApplication>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPixmap>
#include <QPainter>
#include <QPicture>
#include "imageutil.h"

#define FONT_SIZE 20

qtCamera::qtCamera()
{
    QVBoxLayout *vLayout = new QVBoxLayout();
    const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
    QFont font;
    font.setPixelSize(FONT_SIZE);
    resize(availableGeometry.width(), availableGeometry.height());
    //resize(800, 600);//test

    //可用相机列表
    const QList<QCameraInfo> availableCameras = QCameraInfo::availableCameras();
    for (const QCameraInfo &cameraInfo : availableCameras)
    {
        qDebug() << cameraInfo.description();
        if (cameraInfo.description().contains("USB", Qt::CaseSensitive) || cameraInfo.description().contains("UVC", Qt::CaseSensitive)
                || cameraInfo.description().contains("rkisp", Qt::CaseSensitive))
        {
            //USB摄像头
            QPushButton *camera = new QPushButton;
            camera->setText(cameraInfo.description());
            camera->setFont(font);
            camera->setCheckable(true);
            if (cameraInfo == QCameraInfo::defaultCamera())
            {
                camera->setDefault(true);
            }
            else
            {
                camera->setDefault(false);
            }

            //启动相机
            connect(camera, SIGNAL(clicked(bool)), this, SLOT(on_cameraClick()));
            vLayout->addWidget(camera);

            m_cameraInfo = cameraInfo;
            break;
        }
    }

    //退出按钮
    QPushButton *exitButton = new QPushButton;
    exitButton->setText(tr("Exit"));
    exitButton->setFont(font);
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(on_exitClicked()));

    //按钮垂直布局
    vLayout->addWidget(exitButton);
    vLayout->setAlignment(Qt::AlignTop);

    //创建一个label用于显示图像
    m_lableShowImg = new QLabel();

    //整体水平布局
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->setMargin(0);
    hlayout->addWidget(m_lableShowImg, 0, Qt::AlignTop);
    hlayout->addLayout(vLayout);
    hlayout->setStretch(0,10);
    hlayout->setStretch(1,1);
    hlayout->setContentsMargins(10, 10, 10, 10);

    QWidget *widget = new QWidget;
    widget->setLayout(hlayout);
    setCentralWidget(widget);
    setWindowFlags(Qt::FramelessWindowHint);

    yolov5_init(YOLOV5_MODEL_PATH, LABEL_NALE_TXT_PATH);
}

//切换相机
void qtCamera::on_cameraClick()
{
    //创建摄像头对象
    m_camera = new QCamera(m_cameraInfo);

    m_camera->unload();
    //配置摄像头的模式--捕获静止图像
    m_camera->setCaptureMode(QCamera::CaptureStillImage);

    //设置默认摄像头参数
    QCameraViewfinderSettings set;
    set.setResolution(640, 480);                 //设置显示分辨率
    set.setMaximumFrameRate(25);                 //设置帧率
    //m_camera->setViewfinderSettings(set);

    //自己用QPainter将每一帧视频画出来
    myvideosurface *surface = new myvideosurface(this);

    //设置取景器显示
    m_camera->setViewfinder(surface);
    connect(surface, SIGNAL(frameAvailable(QVideoFrame)), this, SLOT(rcvFrame(QVideoFrame)), Qt::DirectConnection);
    connect(this,SIGNAL(sendOneQImage(QImage)), this, SLOT(recvOneQImage(QImage)));

    //启动摄像头
    m_camera->start();
}

void qtCamera::rcvFrame(QVideoFrame m_currentFrame)
{
    m_currentFrame.map(QAbstractVideoBuffer::ReadOnly);
    //将视频帧转化成QImage,devicePixelRatio设备像素比，bytesPerLine一行的像素字节（1280*4=5120）
    QImage videoImg =  QImage(m_currentFrame.bits(),
                   m_currentFrame.width(),
                   m_currentFrame.height(),
                   QVideoFrame::imageFormatFromPixelFormat(m_currentFrame.pixelFormat())).copy();       //这里要做一个copy,因为char* pdata在emit后释放了
    //QMatrix matrix;
    //matrix.rotate(90);
    //videoImg = videoImg.transformed(matrix, Qt::FastTransformation);          //实现旋转图像
    //videoImg = videoImg.mirrored(false, true);                          //水平翻转，原始图片是反的
    //qDebug() <<  "image" << videoImg;  //可以看看输出啥东西
    //QString currentTime = QDateTime::currentDateTime().toString("yyyyMMddhhmmss");
    //QString savefile = QString("E:/study/QT/opencamera/opencamera/%1.jpg").arg(currentTime);
    //qDebug() << videoImg.save(savefile);
    m_currentFrame.unmap();                                         //释放map拷贝的内存
    QWidget::update();                                              //更新了，就会触发paintEvent画图

    emit sendOneQImage(videoImg); //发送信号
}

void qtCamera::recvOneQImage(QImage qImage)
{
    //qDebug() <<  "image" << qImage;
#if 1
    cv::Mat srcImg = ImageUtil::QImageToMat(qImage);
    cv::Mat dstImg;

    // 推理预测 输入每帧图像
    yolov5_detect(srcImg, dstImg);
    // 结果图缩放到768x432
    cv::Mat resize_dstImg;
    cv::resize(dstImg, resize_dstImg, cv::Size(768, 432), 0, 0, cv::INTER_LINEAR);

    QImage qDstImage = ImageUtil::MatToQImage(resize_dstImg);
    QPixmap tempPixmap = QPixmap::fromImage(qDstImage);
#else
    QPixmap tempPixmap = QPixmap::fromImage(qImage);
#endif
    m_lableShowImg->setPixmap(tempPixmap);
    m_lableShowImg->adjustSize();

}

void qtCamera::on_exitClicked()
{
    m_camera->stop();
    yolov5_deinit();
    qApp->exit(0);
}


