//
// Created by   on 03/05/17.
//

#include "WMDynamixelNode.h"
#include "WMDynamixel.h"

dynamixel::PortHandler *portHandler;
dynamixel::PacketHandler *packetHandler;

ros::Publisher dynamixelPublisher;

std::vector<WMDynamixel> dynamixelArray;

int main(int argc, char **argv){
	
	ros::init(argc, argv, "wm_dynamixel_node");
	
	ros::NodeHandle dynamixelHandler;
	
	//initialise ros subscriber
	ros::Subscriber dynamixelSubscriber = dynamixelHandler.subscribe("dynamixel_cmd", 1, &WriteAcceleration);
	
	//initialise ros subscriber
	ros::Subscriber newDynamixelSubscriber = dynamixelHandler.subscribe("dynamixel_init", 1, &addDynamixel);
	
	//initialise ros publisher for data feedback
	dynamixelPublisher = dynamixelHandler.advertise<std_msgs::Float64MultiArray>("dynamixel_pos", 1);
	
	//initialise port
	if(!InitPort(PORTNAME, BAUDRATE)){
		ROS_INFO("Error initialising RS485 port");
		return 0;
	}else{
		ROS_INFO("RS485 Initialised!");
		
		//run loop
		nodeLoop();
	}
	
	return 0;
}

void nodeLoop() {
	ros::Rate loop_rate(10);
	int iCount = 0;
	ROS_INFO("Going in node loop.");
	while(ros::ok()){
		ros::spinOnce();
		//ReadFeedback();
		iCount ++;
		loop_rate.sleep();
	}
}

void ReadFeedback() {
	for (int index=0; index < dynamixelArray.size(); index++) {
		dynamixelArray[index].publishPosition(dynamixelPublisher);
	}
}

void WriteAcceleration(std_msgs::Float64MultiArrayConstPtr msg) {
	int ID = (int)msg->data[0];
	for (int index=0; index < dynamixelArray.size(); index++) {
		if(dynamixelArray[index].getID() == ID){
			ROS_INFO("received command for dynamixel %i",ID);
			dynamixelArray[index].setVelocity(msg->data[1]);
			break;
		}
	}
}

void addDynamixel(std_msgs::Float64MultiArrayConstPtr msg) {
	int ID = (int)msg->data[0];
	double offset = msg->data[1];
	int resolution = (int)msg->data[2];
	
	ROS_INFO("Try to add a dynamixel with ID %i, offset %f and coef %i.",ID,offset,resolution);
	
	for (int index=0; index < dynamixelArray.size(); index++) {
		if(dynamixelArray[index].getID() == ID){
			ROS_INFO("Will update a dynamixel with ID %i, offset %f and coef %i.",ID,offset,resolution);
			dynamixelArray[index].updateDynamixel(ID, offset, resolution);
			break;
		}
	}
	
	ROS_INFO("Will add a dynamixel with ID %i, offset %f and coef %i.",ID,offset,resolution);
	dynamixelArray.push_back(WMDynamixel(ID, offset, resolution));
}

bool InitPort(const char *PortName, int BaudRate) {
	if (portHandler == NULL || packetHandler == NULL) {
		// Link port
		portHandler = dynamixel::PortHandler::getPortHandler(PortName);

		// Link packet
		packetHandler = dynamixel::PacketHandler::getPacketHandler(1.0);

		// Open port
		if (!portHandler->openPort()) {
			return false;
		}

		// Set port baudrate
		if (!portHandler->setBaudRate(BaudRate)) {
			return false;
		}
	}
	return true;
}

bool write1BDynamixel(int ID, int iAddress, int iValue){
	int dxl_comm_result;
	uint8_t dxl_error = 0;
	
	dxl_comm_result = packetHandler->write1ByteTxRx(portHandler, ID, iAddress, iValue, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return false;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return false;
	}
	
	return true;
}

bool write2BDynamixel(int ID, int iAddress, int iValue){
	int dxl_comm_result;
	uint8_t dxl_error = 0;
	
	dxl_comm_result = packetHandler->write2ByteTxRx(portHandler, ID, iAddress, iValue, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return false;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return false;
	}
	return true;
}

int read1BDynamixel(int ID, int iAddress) {
	int dxl_comm_result;
	uint8_t dxl_error = 0;
	
	uint16_t dxl_present_position;
	// Read present position
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, ID, iAddress, &dxl_present_position, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS)
	{
		packetHandler->printTxRxResult(dxl_comm_result);
		return false;
	}
	else if (dxl_error != 0)
	{
		packetHandler->printRxPacketError(dxl_error);
		return false;
	}
	return dxl_present_position;
}

int read2BDynamixel(int ID, int iAddress) {
	int dxl_comm_result;
	uint8_t dxl_error = 0;
	
	uint16_t dxl_present_position;
	// Read present position
	dxl_comm_result = packetHandler->read2ByteTxRx(portHandler, ID, iAddress, &dxl_present_position, &dxl_error);
	if (dxl_comm_result != COMM_SUCCESS) {
		packetHandler->printTxRxResult(dxl_comm_result);
		return false;
	} else if (dxl_error != 0) {
		packetHandler->printRxPacketError(dxl_error);
		return false;
	}
	return dxl_present_position;
}