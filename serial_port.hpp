#ifndef _SERIAL_PORT_HPP_
#define _SERIAL_PORT_HPP_

#include <iostream>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <iomanip>
#include <termios.h> //set baud rate

namespace robot_ctrl {

class SerialPort
{
public:
    SerialPort(void)
    {
        fd = 0;
    }
    ~SerialPort(void){}

    bool openSerialPort(std::string comport_name = "/dev/ttyUSB0", int baud =115200);
    int readSerialPort(unsigned char* vBuffer, int vLen, int vOverTime);
    int readSerialPort(char* vBuffer, int vLen, int vOverTime);
    int writeSerialPort(unsigned char* vBuffer, int vLen);
    int writeSerialPort(char* vBuffer, int vLen);
    void closeSerialport(void);

private:

    int fd;
};

} // namespace robot_ctrl

#endif // _SERIAL_PORT_HPP_
