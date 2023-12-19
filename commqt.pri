HEADERS += $$PWD/dtu_arduino_driver.h

win32 {
    message("Building for Windows")
}

unix {
    message("Building for unix")
}

SOURCES += $$PWD/dtu_arduino_driver.cpp

#INCLUDEPATH += $$PWD
#DEPENDPATH += $$PWD
