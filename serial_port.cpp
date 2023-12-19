#include <iostream>
#include <stdio.h>
#include <math.h>
#include "serial_port.hpp"
//#include <ros/ros.h> 

namespace robot_ctrl {

bool SerialPort::openSerialPort(std::string comport_name, int baud)
{
    fd = open(comport_name.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd < 0)
    {
        std::cerr<<"Serival port" << comport_name << "open fail!"<<std::endl;
        //ROS_ERROR("serial port: open failed!!!\n");

        return false;
    }
    else
    {
        std::cout<<"Serival port" << comport_name << "open success!"<<std::endl;
    }

    struct termios options;
    tcgetattr(fd, &options);//
    bzero(&options, sizeof (options));
    options.c_cflag |= (CLOCAL | CREAD);
    options.c_cflag &= ~CSIZE;
    options.c_cflag &= ~CRTSCTS;
    options.c_cflag = (options.c_cflag & ~CSIZE) | CS8; /*数据位:8*/
    options.c_cflag &= ~PARENB; /*校检位:无*/
    options.c_cflag &= ~CSTOPB; /*停止位:1*/
    switch (baud)
    {
        case 2400:
            cfsetispeed(&options, B2400);
            cfsetospeed(&options, B2400);
            break;
        case 4800:
            cfsetispeed(&options, B4800);
            cfsetospeed(&options, B4800);
            break;
        case 9600:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
        case 19200:
            cfsetispeed(&options, B19200);
            cfsetospeed(&options, B19200);
            break;
        case 38400:
            cfsetispeed(&options, B38400);
            cfsetospeed(&options, B38400);
            break;
        case 115200:
            cfsetispeed(&options, B115200);
            cfsetospeed(&options, B115200);
            break;

        default:
            cfsetispeed(&options, B9600);
            cfsetospeed(&options, B9600);
            break;
    }

    tcflush(fd, TCIFLUSH);
    if ((tcsetattr(fd, TCSANOW, &options)) != 0)
    {
        // std::cerr<<"Serival port com set error!"<<std::endl;
        return false;
    }

    return true;
}

//从端口读取信息

int SerialPort::readSerialPort(unsigned char* vBuffer, int vLen, int vOverTime)
{
    int ret = 0;
    int pos = 0;
    fd_set fs_read;
    struct timeval tv_timeout;
    FD_ZERO(&fs_read); //清空串口接受端口集
    FD_SET(fd, &fs_read); //设置串口接收端口集
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = 1000 * vOverTime;
    while (1)
    {
        pos = 0;
        while (FD_ISSET(fd, &fs_read))
        {
            //每次循环都要清空，否则不会检测到有变化
            FD_ZERO(&fs_read); //清空串口接受端口集
            FD_SET(fd, &fs_read); //设置串口接收端口集
            if (select(fd + 1, &fs_read, NULL, NULL, &tv_timeout))
            {
                ret = read(fd, vBuffer + pos, 1);
                if (ret == -1)
                {
                    break;
                }
                pos++;
                if (vLen <= pos)
                {
                    break;
                }
            }
            else
            {
                break;
            }
        }
        return pos;
    }
}

int SerialPort::readSerialPort(char* vBuffer, int vLen, int vOverTime)
{
    int ret = 0;
    int pos = 0;
    fd_set fs_read;
    struct timeval tv_timeout;
    FD_ZERO(&fs_read); //清空串口接受端口集
    FD_SET(fd, &fs_read); //设置串口接收端口集
    tv_timeout.tv_sec = 0;
    tv_timeout.tv_usec = 1000 * vOverTime;
    while (1)
    {
        pos = 0;
        while (FD_ISSET(fd, &fs_read))
        {
            //每次循环都要清空，否则不会检测到有变化
            FD_ZERO(&fs_read); //清空串口接受端口集
            FD_SET(fd, &fs_read); //设置串口接收端口集
            if (select(fd + 1, &fs_read, NULL, NULL, &tv_timeout))
            {
                ret = read(fd, vBuffer + pos, 1);

                //printf("%02X ", (unsigned char)chr[i]);
                
                if (ret == -1)
                {
                    break;
                }
                pos++;
                if (vLen <= pos)
                {
                    break;
                }
                
            }
            else
            {
                break;
            }
        }
        return pos;
    }
}

int SerialPort::writeSerialPort(unsigned char* vBuffer, int vLen)
{
    int iRet = 0;
    iRet = write(fd, (void*) vBuffer, vLen);

    if (iRet == vLen)
    {
        std::cout << "写入成功！"<< std::endl;
        return iRet;
    }
    else
    {
        tcflush(fd, TCOFLUSH);
        return -1;
    }
}

int SerialPort::writeSerialPort(char* vBuffer, int vLen)
{
    int iRet = 0;
    iRet = write(fd, (void*) vBuffer, vLen);

    if (iRet == vLen)
    {
        std::cout << "写入成功！"<< std::endl;
        return iRet;

    }
    else
    {
        tcflush(fd, TCOFLUSH);
        return -1;
    }
}

void SerialPort::closeSerialport(void) {
    if(fd >=0) {
        close(fd);
    }
}

} // namespace robot_ctrl