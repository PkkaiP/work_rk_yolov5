
TARGET = YoloCamera
TEMPLATE = app

QT += widgets multimedia multimediawidgets

SOURCES += main.cpp

include($$PWD/yolocamera.pri)

#LIBS+=-lopencv_core -lopencv_objdetect -lopencv_highgui -lopencv_videoio -lopencv_imgproc -lopencv_imgcodecs -lOpenCL -lpthread

#temp file
DESTDIR         = $$PWD/app_bin
MOC_DIR         = $$PWD/build/qcamera
OBJECTS_DIR     = $$PWD/build/qcamera

HEADERS +=
