#HEADERS += $$PWD/dtu_arduino_driver.h
#HEADERS += $$PWD/conversions.h
HEADERS += $$PWD/serialworker.h
#HEADERS += $$PWD/serial_port.hpp

win32 {
    message("Building for Windows")
}

unix {
    message("Building for unix")
}

#SOURCES += $$PWD/dtu_arduino_driver.cpp
SOURCES += $$PWD/serialworker.cpp
#SOURCES += $$PWD/serial_port.cpp

#INCLUDEPATH += $$PWD
#DEPENDPATH += $$PWD
