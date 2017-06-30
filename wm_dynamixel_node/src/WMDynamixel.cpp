//
// Created by lucas on 29/06/17.
//

#include "WMDynamixel.h"
WMDynamixel::WMDynamixel(int Id, double offset, int resolution) {
    updateDynamixel(Id, offset, resolution);
}

void WMDynamixel::initDynamixel() {
    int pos;
    int goal = (int) (_offset / _coefficient);
    ROS_INFO("//set TORQUE to ON");
    write1BDynamixel(_ID, ADDR_P1_TORQUE_ENABLE, 1);
    usleep(DELAY);

    /* -> FOR INITIAL POSITIONING <- */
//
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


    //set speed to 0
    write2BDynamixel(_ID, ADDR_P1_MOVING_SPEED_2BYTES, 0);
    usleep(DELAY);

    ROS_INFO("//set WHEEL mode");

    write2BDynamixel(_ID, ADDR_P1_CW_LIMIT_2BYTES, 0);
    write2BDynamixel(_ID, ADDR_P1_CCW_LIMIT_2BYTES, 0);

    usleep(DELAY);
}

void WMDynamixel::setVelocity(double newVelocity) {
    ROS_INFO("receive new velocity");
    if (_isEnable) {
        ROS_INFO("read new velocity");
        int iVelocity = (int) (newVelocity * 325.631013566);

        if (iVelocity < 0) {
            iVelocity = 1023 - iVelocity;
        }
        write2BDynamixel(_ID, ADDR_P1_MOVING_SPEED_2BYTES, iVelocity);
        _watchdog = time(0);
    }
}

bool WMDynamixel::publishPosition(ros::Publisher pub) {
    if (_isEnable) {
        //test watchdog
        watchdogMgr();

        bool dxl_error = false;
        //read datas
        std_msgs::Float64MultiArray msg;

        //add coef
        msg.data.push_back((double) _ID);
        usleep(DELAY);
        msg.data.push_back(_coefficient * read2BDynamixel(_ID, ADDR_P1_PRESENT_POSITION_2BYTES, &dxl_error));
        if (dxl_error) {
            ROS_INFO("Error reading position");
            return false;
        }
        usleep(DELAY);
        int iSpeed = read2BDynamixel(_ID, ADDR_P1_PRESENT_SPEED_2BYTES, &dxl_error);
        if (dxl_error) {
            ROS_INFO("Error reading speed");
            return false;
        }
        usleep(DELAY);
        if (iSpeed >= 1024) {
            iSpeed = -iSpeed + 1024;
        }
        msg.data.push_back((double) iSpeed / 325.631013566);
        msg.data.push_back(read2BDynamixel(_ID, ADDR_P1_PRESENT_LOAD_2BYTES, &dxl_error));
        if (dxl_error) {
            ROS_INFO("Error reading load");
            return false;
        }
        usleep(DELAY);

        //publish values
        pub.publish(msg);
    }
    return true;
}

int WMDynamixel::getID(){
    return _ID;
}

void WMDynamixel::updateDynamixel(int Id, double offset, int resolution) {
    _ID = Id;
    _isEnable = false;
    _offset = offset;
    _coefficient = (2 * PI) / resolution;
    ROS_INFO("Dynamixel added with ID %i, offset %f and coef %f.", _ID, _offset, _coefficient);
    initDynamixel();
    _isEnable = true;
    ROS_INFO("Initialised a dynamixel with ID %i, offset %f and coef %f.", _ID, _offset, _coefficient);
}

bool WMDynamixel::watchdogMgr() {
    long double sysTime = time(0);
    if (sysTime - WATCHDOG > _watchdog) {
        setVelocity(0);
        return true;
    }
    return false;
}
