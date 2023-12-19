#include <iostream>
#include "dtu_arduino_driver.h"
//#include "tf/transform_datatypes.h"//转换函数头文件

namespace sensor
{
    // 构建
    dtu_arduino_driver::dtu_arduino_driver(void)
    {
        rx_buffer_leng_ = 0;
    }
    // 析构
    dtu_arduino_driver::~dtu_arduino_driver(void) {
        dtu_motor_.close();
        arduino_motor_.close();
    }

    bool dtu_arduino_driver::serialConnect(void) {

        bool dtu_status = dtu_motor_.openSerialPort("/dev/ttyUSB0",9600);

        if(!dtu_status) {
            std::cout<<"dtu serial port /dev/ttyUSB0 open failed"<<std::endl;
            return false;
        }
        if(dtu_status)
        {
            std::cout<<"serial port open success"<<std::endl;
        }
        return true;
    }

    int dtu_arduino_driver::run(void) {
        char dtu_rec_buff[100];
        char arduino_rec_buff[100];
        // 读取DTU数据
        int dtu_rec_num = dtu_motor_.readSerialPort(dtu_rec_buff,100,0);
        ParseData(dtu_rec_buff,dtu_rec_num);

        const char data[] = "hello";
        
        std::cout << "正在输入\n" ;
        // 调用 writeSerialPort 函数
        int result = dtu_motor_.writeSerialPort(const_cast<char*>(data), sizeof(data) - 1);
    /*
        if (result == 0) {
            std::cout << "Data successfully written to serial port\n";
        } else {
            std::cerr << "Error writing to serial port\n";
        }
*/
        return 1;
        
    }

    void dtu_arduino_driver::ParseData(const char *chr, int length) {
        if (length > 0)
        {
            for(int i=0;i<length;++i) {
                printf("%02X ", (unsigned char)chr[i]);
            }
            printf("\n");
            std::cout << "length: " << length << std::endl;
        }
        
        /*
        下面就是解析数据，然后往Arduino写数据即可
        */
    }


}
