#include "goldo_rplidar/StartMotor.h"
#include "goldo_rplidar/StopMotor.h"
#include "ros/console.h"
#include "ros/ros.h"
#include "rplidar.h"
#include "std_msgs/String.h"

using namespace rp::standalone::rplidar;

RPlidarDriver* g_driver = nullptr;

bool startMotorCallback(goldo_rplidar::StartMotor::Request& request,
                        goldo_rplidar::StartMotor::Response& response) {
  g_driver->startMotor();
  g_driver->startScan(0, 1);
  return true;
};

bool stopMotorCallback(goldo_rplidar::StopMotor::Request& request,
                       goldo_rplidar::StopMotor::Response& response) {
  g_driver->stop();
  g_driver->stopMotor();
  return true;
};

int main(int argc, char** argv) {
  g_driver = RPlidarDriver::CreateDriver(DRIVER_TYPE_SERIALPORT);

  ros::init(argc, argv, "goldo_rplidar");

  ros::NodeHandle n;

  //ros::Publisher raw_points_pub = n.advertise<goldo_comm_uart::RawMessage>("rplidar/raw", 1000);
  // ros::Subscriber raw_message_sub =
  // n.subscribe<goldo_comm_uart::RawMessage>("stm32/in/raw", 1000,
  // chatterCallback);

  auto start_motor_srv = n.advertiseService("start_motor", startMotorCallback);
  auto stop_motor_srv = n.advertiseService("stop_motor", stopMotorCallback);

  ros::Rate loop_rate(1000);

  std::string port_name;
  int baudrate;
  ros::param::get("~port", port_name);
  ros::param::get("~baudrate", baudrate);

  if (IS_OK(g_driver->connect(port_name.c_str(), 115200))) {
  } else {
    ROS_INFO_STREAM("Failed to connect to RPLidar on port: " << port_name);
    // return -1;
  }

  while (ros::ok()) {
    rplidar_response_measurement_node_hq_t nodes[16384];
    size_t count{sizeof(nodes)};
    auto op_result = g_driver->grabScanDataHq(nodes, count, 500);
    if (IS_OK(op_result)) {
      g_driver->ascendScanData(nodes, count);
      for (unsigned i = 0; i < count; i++) {
        double theta = nodes[i].angle_z_q14 * M_PI_2 / (1 << 14);
        double distance = nodes[i].dist_mm_q2 / 4.0f;
      }
    };
    ros::spinOnce();
  }

  return 0;
}
