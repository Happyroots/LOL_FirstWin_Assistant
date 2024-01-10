win32 {
    message("Building for Windows")
}

unix {
    message("Building for unix")
}

HEADERS += \
    $$PWD/drivers/dtu.h \
    $$PWD/drivers/serialworker.h

SOURCES += \
    $$PWD/drivers/dtu.cpp \
    $$PWD/drivers/serialworker.cpp
