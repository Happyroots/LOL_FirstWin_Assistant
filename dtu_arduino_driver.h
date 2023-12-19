#pragma once
#include <deque>

//#include "serial_port.hpp"
#include "conversions.h"
#include <QtSerialPort/QSerialPort>
#include <serialworker.h>
#include <QSerialPortInfo>

namespace sensor
{
    class dtu_arduino_driver{
        public:
            dtu_arduino_driver(void);
            ~dtu_arduino_driver(void);
            int run(void);
            bool serialConnect(void);

        private:
            void ParseData(const char chrBuf[], int length); 
        private:
            int rsp_ret_;
//            robot_ctrl::SerialPort dtu_motor_;
//            robot_ctrl::SerialPort arduino_motor_;
            QSerialPort dtu_motor_;
            QSerialPort arduino_motor_;

            std::string gps_port_;
            std::deque<char> rec_buf_;

            int rx_buffer_leng_;
            
    };
}
