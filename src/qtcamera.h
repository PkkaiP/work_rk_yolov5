#ifndef QTCAMERA_H
#define QTCAMERA_H

#include <QCamera>
#include <QCameraInfo>
#include <QCameraViewfinder>
#include <QMainWindow>
#include <QtWidgets>
#include <QMatrix>
#include "myvideosurface.h"
//#include "rknn_yolo.h"
#include "postprocess.h"

class qtCamera : public QMainWindow
{
    Q_OBJECT

public:
    qtCamera();

private slots:
    void on_cameraClick();
    void on_exitClicked();

    void rcvFrame(QVideoFrame m_currentFrame);
    void recvOneQImage(QImage qImage);

signals:
    void sendOneQImage(QImage);


private:
    QCamera *m_camera;
    QCameraInfo m_cameraInfo;

    QImage m_videoImg;
    QLabel *m_lableShowImg;

};

#endif
