//Some small test

#include <string>
#include <iostream>

#include "SerialPort.hpp"

using namespace LinuxSerial;

int main()
{
	std::string device0Name_ = "/dev/ttyUSB0";
	// std::string device1Name_ = "/dev/ttyS11";
	LinuxSerial::SerialPort serialPort0(device0Name_, LinuxSerial::BaudRate::B_57600);
	serialPort0.Open();

	// LinuxSerial::SerialPort serialPort1(device1Name_, LinuxSerial::BaudRate::B_57600);
	// serialPort1.Open();

	// serialPort0.Write("Hello");

	std::string readData;
	// serialPort1.Read(readData);
	serialPort0.Read(readData);

	std::cout << "read " << readData;
	return 0;
}
