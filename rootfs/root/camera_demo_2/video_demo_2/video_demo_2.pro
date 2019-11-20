QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    acq_img.c \
    core/cfg/wwk_main_cfg.c \
    core/cfg/wwk_source.c \
    core/wwk_alloc.c \
    core/wwk_larray.c \
    core/wwk_os.c \
    core/wwk_queue.c \
    core/wwk_str.c \
    core/wwk_str_encode.c \
    core/wwk_strbuf.c \
    core/wwk_uchar.c \
    core/wwk_wavehdr.c \
    main.cpp \
    mainwidget.cpp \
    v4l2_c/v4l2.c

HEADERS += \
    acq_img.h \
    core/cfg/wwk_main_cfg.h \
    core/cfg/wwk_source.h \
    core/wwk_alloc.h \
    core/wwk_larray.h \
    core/wwk_os.h \
    core/wwk_queue.h \
    core/wwk_str.h \
    core/wwk_str_encode.h \
    core/wwk_strbuf.h \
    core/wwk_type.h \
    core/wwk_uchar.h \
    core/wwk_wavehdr.h \
    mainwidget.h \
    v4l2_c/v4l2.h

FORMS += \
    mainwidget.ui

QMAKE_CFLAGS += -pthread

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
