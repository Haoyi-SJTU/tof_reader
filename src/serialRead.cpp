#include <stdio.h>
#include <string.h>
#include "iostream"
#include <boost/thread/thread.hpp>

#include <ros/ros.h>
#include "geometry_msgs/PointStamped.h"

unsigned int StringToDEC(char *rdata, int len)
{
  unsigned int i;
  unsigned int wdata = 0;
  char temp[4];

  for (i = 0; i < len - 1; i++)
  {
    if ((*(rdata + i) >= '0') && (*(rdata + i) <= '9'))
    {
      temp[i] = *(rdata + i) - '0'; //字符串转换成数字
      // std::cout<<"单个数字"<< int(temp[i]) <<std::endl;
    }
    else
    {
      // std::cout<<"TOF数据格式错误!"<<std::endl;
      // continue;
      break;
    }
  }
  switch (i)
  {
  case 1:
    wdata = temp[i - 1];
    break;
  case 2:
    wdata = temp[i - 2] * 10 + temp[i - 1];
    break;
  case 3:
    wdata = temp[i - 3] * 100 + temp[i - 2] * 10 + temp[i - 1];
    break;
  case 4:
    wdata = temp[i - 4] * 1000 + temp[i - 3] * 100 + temp[i - 2] * 10 + temp[i - 1];
    break;
  default:
    break;
  }
  return wdata;
}


int main(int argc, char **argv)
{
  unsigned int counter = 0;        //有效tof数据计数器
  unsigned int tof_queue[3] = {0}; // tof数据队列，用于数据平滑
  char open_command[] = "s7-1";
  // char stop_command[] = "s7-0";
  char serialPortFilename[] = "/dev/ttyUSB0";
  char readBuffer[5];
  unsigned int tof_result = 0;

  ros::init(argc, argv, "tof_talker"); //节点初始化
  ros::NodeHandle h;                   //创建节点句柄
  geometry_msgs::PointStamped msg;
  ros::Publisher tof_pub = h.advertise<geometry_msgs::PointStamped>("tof_talker", 1);

  while (ros::ok())
  {
    FILE *serPort = fopen(serialPortFilename, "r+"); //	以读/写方式打开文件，该文件必须存在
    if (serPort == NULL)
    {
      std::cout << "没打开串口" << std::endl;
      continue;
    }
    fwrite(open_command, 2, 5, serPort);
    fread(readBuffer, sizeof(char), 5, serPort);

    if (sizeof(readBuffer) != 0)
    {
      unsigned int temp = StringToDEC(readBuffer, 5);
      // std::cout << "转换结果     " << temp << std::endl;
      // std::cout << "开始     " << readBuffer << std::endl;
      if (temp >0 && temp < 2500)//数据在0-2500mm内的有效
      {
        counter++;
        switch (counter)//对计数器数字分类，前两次用来填充队列，不产生结果
        {
        case 0:
          break;
        case 1:
          tof_queue[0] = temp;
          break;
        case 2:
          tof_queue[0] = tof_queue[1];
          tof_queue[1] = temp;
          break;
        default:
          tof_queue[(counter - 1) % 3] = temp; //建立长度为3的队列，把新产生的数据存进队列
          // tof_result = 0.6 * tof_queue[2] + tof_queue[1] * 0.2 + tof_queue[0] * 0.2;
          tof_result = (tof_queue[2] + tof_queue[1] + tof_queue[0]) / 3; //队列数据求均值
          // std::cout << "平均结果     " << tof_result << std::endl;
          msg.header.stamp = ros::Time::now(); // currentTime;
          msg.point.x = tof_result;
          msg.point.y = temp;
          // msg.point.z = 0;
          tof_pub.publish(msg);
          break;
        }
      }
    }
    fclose(serPort);
  }
  return 0;
}
