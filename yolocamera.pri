#-------------------------------------------------
#相机
#-------------------------------------------------

INCLUDEPATH         += $$PWD/src

HEADERS += \
    $$PWD/src/postprocess.h \
    $$PWD/src/qtcamera.h \
    $$PWD/src/myvideosurface.h \
    $$PWD/src/imageutil.h


SOURCES += \
    $$PWD/src/postprocess.cpp \
    $$PWD/src/qtcamera.cpp \
    $$PWD/src/myvideosurface.cpp \
    $$PWD/src/yolo.cpp

unix:!macx: LIBS += -L$$PWD/lib/opencv_3568/lib/ -lopencv_shape -lopencv_stitching \
                        -lopencv_objdetect -lopencv_calib3d -lopencv_features2d \
                        -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml \
                        -lopencv_imgproc -lopencv_flann  -lopencv_core

LIBS+= -L$$PWD/lib -lrknn_api
LIBS+= -L$$PWD/lib -lrga

INCLUDEPATH += $$PWD/lib/opencv_3568/include/
DEPENDPATH += $$PWD/lib/opencv_3568/include/
