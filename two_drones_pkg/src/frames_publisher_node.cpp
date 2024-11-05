#include <ros/ros.h>
#include <ros/console.h>
#include <tf/transform_broadcaster.h>
#include <iostream>
#include <cmath>

class FramesPublisherNode{
 ros::NodeHandle nh;
 ros::Time startup_time;

 ros::Timer heartbeat;
 tf::TransformBroadcaster broadcaster;

 public:
  FramesPublisherNode(){
    startup_time = ros::Time::now();
    heartbeat = nh.createTimer(ros::Duration(0.02), &FramesPublisherNode::onPublish, this);
    heartbeat.start();
  }

  void onPublish(const ros::TimerEvent&){
    double time = (ros::Time::now() - startup_time).toSec();
    tf::Transform AV1World(tf::Transform::getIdentity());
    tf::Transform AV2World(tf::Transform::getIdentity());

    AV1World.setOrigin(tf::Vector3(cos(time), sin(time), 0.0));
        tf::Quaternion av1_rotation;
        av1_rotation.setRPY(0, 0, time);
        AV1World.setRotation(av1_rotation);
    AV2World.setOrigin(tf::Vector3(sin(time), 0.0, cos(2 * time)));
        tf::Quaternion av2_rotation;
        av2_rotation.setRPY(0, 0, 0);
        AV2World.setRotation(av2_rotation);

    static tf::TransformBroadcaster br;
    br.sendTransform(tf::StampedTransform(AV1World, ros::Time::now(), "world", "av1"));
    br.sendTransform(tf::StampedTransform(AV2World, ros::Time::now(), "world", "av2"));

    }
  };

int main(int argc, char** argv){

  ros::init(argc, argv, "frames_publisher_node");
  FramesPublisherNode node;
  ros::spin();
  return 0;
}
