//action_client
//sends poses to an action_server to make gazebo move in a certain path


#include <ros/ros.h>
#include <ps4_pkg/PathSrv.h>
#include <iostream>
#include <nav_msgs/Path.h>
#include <geometry_msgs/Pose.h>
#include <geometry_msgs/PoseStamped.h>

#define NO_ERROR 0;
using namespace std;

geometry_msgs::Quaternion convertPlanarPhi2Quaternion(double phi) {
    geometry_msgs::Quaternion quaternion;
    quaternion.x = 0.0;
    quaternion.y = 0.0;
    quaternion.z = sin(phi / 2.0);
    quaternion.w = cos(phi / 2.0);
    return quaternion;
}

int main(int argc, char *argv[]) {
	ros::init(argc, argv, "action_client");
	ros::NodeHandle n;
	ros::ServiceClient client = n.serviceClient<ps4_pkg::PathSrv>("action_server");
	geometry_msgs::Quaternion quat;
	
	while (!client.exists()) {
		ROS_INFO("waiting for service");
		ros::Duration(1.0).sleep();
	}
	ROS_INFO("connected to client service");
	ps4_pkg::PathSrv pathSrv;
	

	//GOTTA CREATE THE POSES HERE! LIKE
	geometry_msgs::PoseStamped poseStamped;
	geometry_msgs::Pose pose;


	pose.position.x = 1.0; // say desired x-coord is 1
	pose.position.y = 0.0;
	pose.position.z = 0.0; // let's hope so!
	pose.orientation.x = 0.0; //always, for motion in horizontal plane
	pose.orientation.y = 0.0; // ditto
	pose.orientation.z = 0.0; // implies oriented at yaw=0, i.e. along x axis
	pose.orientation.w = 1.0; //sum of squares of all components of unit quaternion is 1
	poseStamped.pose = pose;
	pathSrv.request.nav_path.poses.push_back(poseStamped);

	//POSE WITH QUARTENIONS ARE EASIER
	quat = convertPlanarPhi2Quaternion(1.57); // get a quaternion corresponding to this heading
	poseStamped.pose.orientation = quat;
	poseStamped.pose.position.y=1.0; // say desired y-coord is 1.0
	pathSrv.request.nav_path.poses.push_back(poseStamped);


	client.call(pathSrv);
	return NO_ERROR;
}



