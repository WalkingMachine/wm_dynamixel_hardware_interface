//
// Created by philippe on 03/05/17.
//

#include "WMDynamixelHardwareInterface.h"
#include <nodelet/nodelet.h>

namespace wm_dynamixel_hardware_interface {


    hardware_interface::VelocityJointInterface WMDynamixelHardwareInterface::joint_velocity_interface_;
    hardware_interface::JointStateInterface    WMDynamixelHardwareInterface::joint_state_interface_;

// << ---- H I G H   L E V E L   I N T E R F A C E ---- >>

	bool WMDynamixelHardwareInterface::init(ros::NodeHandle &root_nh, ros::NodeHandle &robot_hw_nh) {
		using namespace hardware_interface;
		
		// Get parameters
        Address = "";
        Baud = 0;
        Offset = 0;
        std::vector<std::string> Joints;
        robot_hw_nh.getParam("address", Address);
        robot_hw_nh.getParam("baudrate", Baud);
        if (!robot_hw_nh.getParam("id", ID)) { return false; }
        robot_hw_nh.getParam("offset", Offset);
        robot_hw_nh.getParam("resolution", resolution);
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

		// advertise publisher
		CtrlPub = robot_hw_nh.advertise<std_msgs::Float64MultiArray>( "dynamixel_cmd", 1 );
        InitPub = robot_hw_nh.advertise<std_msgs::Float64MultiArray>( "dynamixel_init", 1 );
		//GripperStatSub.
		StatSub = robot_hw_nh.subscribe( "dynamixel_pos", 1, &WMDynamixelHardwareInterface::StatusCB, this);

        std_msgs::Float64MultiArray msg;
        msg.data.push_back( ID );
        msg.data.push_back( Offset );
        msg.data.push_back( resolution );
        InitPub.publish( msg );
		return true;
	}
	
	void WMDynamixelHardwareInterface::read(const ros::Time &time, const ros::Duration &period) {
	}
	
	void WMDynamixelHardwareInterface::write(const ros::Time &time, const ros::Duration &period) {
        std_msgs::Float64MultiArray msg;
        msg.data.push_back( ID );
        msg.data.push_back( cmd );
	}

	void WMDynamixelHardwareInterface::StatusCB( std_msgs::Float64MultiArrayConstPtr msg ){
		if ( ID == (int)msg->data[0] ){
			pos = msg->data[1];
			vel = msg->data[2];
			eff = msg->data[3];
		}
	}


}
PLUGINLIB_EXPORT_CLASS(wm_dynamixel_hardware_interface::WMDynamixelHardwareInterface, hardware_interface::RobotHW)