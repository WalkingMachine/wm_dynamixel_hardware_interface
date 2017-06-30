//
// Created by lucas on 29/06/17.
//

#ifndef PROJECT_WMDYNAMIXEL_H
#define PROJECT_WMDYNAMIXEL_H

#include "WMDynamixelNode.h"
#include <ctime>

#define PI 3.1415926535897932
#define DELAY 10000
#define WATCHDOG 500000

class WMDynamixel {
public:
	WMDynamixel(int Id, double offset, int resolution);
	void updateDynamixel(int Id, double offset, int resolution);
	void setVelocity(double newVelocity);

    bool watchdogMgr();
    bool publishPosition(ros::Publisher pub);
	int getID();
	
private:
	//id of material dynamixel
	int _ID;
	
	//the dynamixel is actif
	bool _isEnable;
	
	//initial position of dynamixel
	double _offset;
	
	//number of positions for 2Pi Rad in dynamixel
    double _coefficient;

    //last write time
    long double _watchdog;

	void initDynamixel();
};

#endif //PROJECT_WMDYNAMIXEL_H
