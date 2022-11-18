#include <stdio.h>
#include <fcntl.h>
#include <termios.h>
#include <unistd.h>
#include <errno.h>
#include <iostream>

int main(void)
{
    int fd; /*File Descriptor*/
    fd = open("/dev/ttyUSB0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
        printf("\n  Error! in Opening ttyUSB0  ");
    else
        std::cout<<"fd is "<<fd<<std::endl;
    struct termios SerialPortSettings;

    tcgetattr(fd, &SerialPortSettings);

    cfsetispeed(&SerialPortSettings, B9600); //设置波特率
    cfsetospeed(&SerialPortSettings, B9600);
    SerialPortSettings.c_cflag &= ~PARENB; //设置没有校验
    SerialPortSettings.c_cflag &= ~CSTOPB; //停止位 = 1
    // SerialPortSettings.c_cflag &= ~CSIZE;
    SerialPortSettings.c_cflag |= CS8; //设置数据位 = 8
    // SerialPortSettings.c_cflag &= ~CRTSCTS;
    // SerialPortSettings.c_cflag |= CREAD | CLOCAL;
    SerialPortSettings.c_iflag &= ~(IXON | IXOFF | IXANY); //关闭软件流动控制
    // SerialPortSettings.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); //设置操作模式
    // SerialPortSettings.c_oflag &= ~OPOST;
    // if ((tcsetattr(fd, TCSANOW, &SerialPortSettings)) != 0)
    //     printf("\n  ERROR ! in Setting attributes");
    // else
    //     printf("\n  波特率=9600 \n  停止位=1 \n  校验=none");

    //定义传输内容
    // char write_buffer[] = "s6-1#";
    // //传输字节数
    // int bytes_written = 0;

    //串口写数据
    // bytes_written = write(fd, write_buffer, sizeof(write_buffer));
    // printf("\n  %s written to ttyUSB0", write_buffer);
    // printf("\n  %d Bytes written to ttyUSB0", bytes_written);
    int bytes_read = 50;
    char read_buffer[50] = "0";
    int i = 0;
    // while (i < 50)
    {
        i++;
        bytes_read = read(fd, read_buffer, bytes_read);
        
        printf("位数 %d ", bytes_read);
        printf("\t 内容 %d \n ", *read_buffer);
    }

    close(fd);
    return 1;
}
