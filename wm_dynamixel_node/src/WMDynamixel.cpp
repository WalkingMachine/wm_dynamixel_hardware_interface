//
// Created by lucas on 29/06/17.
//

#include "WMDynamixel.h"

WMDynamixel::WMDynamixel(int Id, double offset, int resolution) {
	updateDynamixel(Id,offset,resolution);
}

void WMDynamixel::initDynamixel() {
	int pos;
	int goal = (int)(_offset/_coeficient);
	ROS_INFO("//set TORQUE to ON");
	write1BDynamixel(_ID, ADDR_P1_TORQUE_ENABLE,1);
	
	// -> FOR INITIAL POSITIONING <-
//	usleep(100000);
//
//	ROS_INFO("//set NORMAL mode");
//
//	usleep(100000);
//	write2BDynamixel(_ID, ADDR_P1_CW_LIMIT_2BYTES,0);
//	usleep(100000);
//	write2BDynamixel(_ID, ADDR_P1_CCW_LIMIT_2BYTES,4095);
//
//	usleep(100000);
//	ROS_INFO("//set SPEED to automatic");
//	write2BDynamixel(_ID, ADDR_P1_MOVING_SPEED_2BYTES,0);
//
//	usleep(100000);
//	ROS_INFO("//go to offset position : %i", goal);
//	write2BDynamixel(_ID, ADDR_P1_GOAL_POSITION_2BYTES,goal);
//
//	usleep(100000);
//	do{
//		pos = read2BDynamixel(_ID, ADDR_P1_PRESENT_POSITION_2BYTES);
//		ROS_INFO("waiting for goal : %i", pos);
//	}
//	while(pos > goal+2 || pos < goal-2);

	
	usleep(100000);

	ROS_INFO("//set WHEEL mode");
	
	write2BDynamixel(_ID, ADDR_P1_CW_LIMIT_2BYTES,0);
	write2BDynamixel(_ID, ADDR_P1_CCW_LIMIT_2BYTES,0);
	
	usleep(100000);
}

void WMDynamixel::setVelocity(double newVelocity) {
	ROS_INFO("receive new velocity");
	if(_isEnable){
		ROS_INFO("read new velocity");
		int iVelocity = (int)(newVelocity * 325.631013566);
		
		if(iVelocity<0){
			iVelocity = 1023 - iVelocity;
		}
		
		write2BDynamixel(_ID, ADDR_P1_MOVING_SPEED_2BYTES, iVelocity);
	}
}

void WMDynamixel::publishPosition(ros::Publisher pub) {
	if(_isEnable){
		//read datas
		std_msgs::Float64MultiArray msg;
		
		//add coef
		msg.data.push_back((double)_ID);
		msg.data.push_back(_coeficient * read2BDynamixel(_ID ,ADDR_P1_PRESENT_POSITION_2BYTES));
		msg.data.push_back(_coeficient * read2BDynamixel(_ID ,ADDR_P1_PRESENT_SPEED_2BYTES));
		msg.data.push_back(_coeficient * read2BDynamixel(_ID ,ADDR_P1_PRESENT_LOAD_2BYTES));
		
		//publish values
		pub.publish(msg);
	}
}

int WMDynamixel::getID(){
	return _ID;
}

void WMDynamixel::updateDynamixel(int Id, double offset, int resolution) {
	_ID = Id;
	_isEnable = false;
	_offset = offset;
	_coeficient = (2*PI)/resolution;
	ROS_INFO("Dynamixel added with ID %i, offset %f and coef %f.",_ID,_offset,_coeficient);
	initDynamixel();
	_isEnable = true;
	ROS_INFO("Initialised a dynamixel with ID %i, offset %f and coef %f.",_ID,_offset,_coeficient);
}
