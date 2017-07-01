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
    /**
     * Generate a new dynamixel with entered parameters
     * @param Id
     * @param offset
     * @param resolution
     */
	WMDynamixel(int Id, double offset, int resolution);
    /**
     *  Update dynamixel parameters
     * @param Id
     * @param offset
     * @param resolution
     */
	void updateDynamixel(int Id, double offset, int resolution);

    /**
     * Send a new velocity to the dynamixel (in Rad)
     * @param newVelocity
     */
	void setVelocity(double newVelocity);

    /**
     * Publish new dynamixel's position, velocity and load in a topic
     * @param pub
     * @return true if OK
     */
    bool publishPosition(ros::Publisher pub);

    /**
     *
     * @return getter for ID
     */
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

    //initialise dynamixel speed to 0 and toggle torque
	void initDynamixel();

    //test if watchdog is
    bool watchdogMgr();
};

#endif //PROJECT_WMDYNAMIXEL_H
