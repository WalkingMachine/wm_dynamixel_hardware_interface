//
// Created by lucas on 29/06/17.
//

#ifndef PROJECT_WMDYNAMIXEL_H
#define PROJECT_WMDYNAMIXEL_H

#include "WMDynamixelNode.h"

#define PI 3.1415926535897932
#define DELAY 10000

class WMDynamixel {
public:
	WMDynamixel(int Id, double offset, int resolution);
	void updateDynamixel(int Id, double offset, int resolution);
	void setVelocity(double newVelocity);

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
	double _coeficient;
	
	void initDynamixel();
};

#endif //PROJECT_WMDYNAMIXEL_H
