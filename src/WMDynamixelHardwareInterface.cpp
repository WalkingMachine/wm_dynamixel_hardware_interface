//
// Created by philippe on 03/05/17.
//

#include "../include/WMDynamixelHardwareInterface.h"

namespace wm_dynamixel_hardware_interface {
// << ---- H I G H   L E V E L   I N T E R F A C E ---- >>
	
	bool WMDynamixelHardwareInterface::init(ros::NodeHandle &root_nh, ros::NodeHandle &robot_hw_nh) {
		using namespace hardware_interface;
		
		// Get parameters
		if (!robot_hw_nh.getParam("id", ID)) { return false; }

		if (!robot_hw_nh.getParam("offset", Offset)) { return false; }

        std::vector<std::string> Joints;
		if (!robot_hw_nh.getParam("joints", Joints)) { return false; }
		Name = Joints[0];
		

		// Initialise interface variables
		cmd = 0;	//command
		pos = 0;    //position
		vel = 0;    //velocity
		eff = 0;    //effort
		
		// Register interfaces
		joint_state_interface_.registerHandle(JointStateHandle(Name, &pos, &vel, &eff));
		joint_velocity_interface_.registerHandle(JointHandle(joint_state_interface_.getHandle(Name), &cmd));
		registerInterface(&joint_state_interface_);
		registerInterface(&joint_velocity_interface_);
		
		return true;
	}
	
	void WMDynamixelHardwareInterface::read(const ros::Time &time, const ros::Duration &period) {

	}
	
	void WMDynamixelHardwareInterface::write(const ros::Time &time, const ros::Duration &period) {

	}

}


PLUGINLIB_EXPORT_CLASS(wm_dynamixel_hardware_interface::WMDynamixelHardwareInterface, hardware_interface::RobotHW)