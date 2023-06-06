#ifndef IMAGEUTIL_H
#define IMAGEUTIL_H

#include <opencv2/opencv.hpp>
#include <QPixmap>

namespace ImageUtil
{
    cv::Mat QImageToMat(QImage image)
    {
        image = image.convertToFormat(QImage::Format_RGB888);
        cv::Mat tmp(image.height(), image.width(), CV_8UC3, (uchar *)image.bits(), image.bytesPerLine());
        cv::Mat result; // deep copy just in case (my lack of knowledge with open cv)
        cvtColor(tmp, result, CV_BGR2RGB);
        return result;
    }

    QImage MatToQImage(cv::Mat mat)
    {
        cv::cvtColor(mat, mat, CV_BGR2RGB);
        QImage qim((const unsigned char *)mat.data, mat.cols, mat.rows, mat.step,
                   QImage::Format_RGB888);
        return qim;
    }
};

#endif