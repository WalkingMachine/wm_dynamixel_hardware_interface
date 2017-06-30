//
// Created by philippe on 03/05/17.
//

#ifndef PROJECT_WMDynamixelNode_H
#define PROJECT_WMDynamixelNode_H

#include <ros/ros.h>

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Float64MultiArray.h"

#include "dynamixel_sdk.h"

#define PORTNAME "/dev/ttyUSB0"
#define BAUDRATE 57600

void nodeLoop();
bool InitPort(const char *PortName, int BaudRate);
void WriteAcceleration(std_msgs::Float64MultiArrayConstPtr msg);
void addDynamixel(std_msgs::Float64MultiArrayConstPtr msg);
void ReadFeedback();
	
bool write1BDynamixel(int ID, int iAddress, int iValue);
bool write2BDynamixel(int ID, int iAddress, int iValue);
int read1BDynamixel(int ID, int iAddress);

int read2BDynamixel(int ID, int iAddress, bool *returnError);

#endif //PROJECT_WMDynamixelNode_H
