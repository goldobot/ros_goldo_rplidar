#include "ros/ros.h"
#include "ros/console.h"

#include "rplidar.h"
#include "std_msgs/String.h"

using namespace rp::standalone::rplidar;

RPlidarDriver * g_driver = nullptr;

int main(int argc, char **argv)
{
	g_driver = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);
	
	
	
	
  ros::init(argc, argv, "goldo_rplidar");

  ros::NodeHandle n("rplidar");

  //ros::Publisher chatter_pub = n.advertise<goldo_comm_uart::RawMessage>("stm32/out/raw", 1000);
  //ros::Subscriber raw_message_sub = n.subscribe<goldo_comm_uart::RawMessage>("stm32/in/raw", 1000, chatterCallback);

  
  ros::Rate loop_rate(1000);
  
  std::string port_name;
  int baudrate;
  n.getParam("port", port_name);
  n.getParam("baudrate", baudrate);


  if(IS_OK(g_driver->connect(port_name.c_str(), 115200)))
  {
  } else
  {
	  ROS_INFO_STREAM("Failed to connect to RPLidar on port: " << port_name);
	  return -1;
  }
  
  while (ros::ok())
  {
    ros::spinOnce();	
    loop_rate.sleep();	
  }

  return 0;
}
