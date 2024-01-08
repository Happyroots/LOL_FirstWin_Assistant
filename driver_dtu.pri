win32 {
    message("Building for Windows")
}

unix {
    message("Building for unix")
}

HEADERS += \
    $$PWD/drivers/serialworker.h

SOURCES += \
    $$PWD/drivers/serialworker.cpp
