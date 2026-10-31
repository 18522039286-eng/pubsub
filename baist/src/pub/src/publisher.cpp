#include <rclcpp/rclcpp.hpp>
#include <functional>
#include <geometry_msgs/msg/pose_stamped.hpp>
#include <memory>
#include <chrono>
#include <cmath>
class posepublisher :public rclcpp::Node{
public:
   posepublisher(): Node("pose_publisher")
   {
       rate = this->declare_parameter<double>("rate",10.0);
       auto qos=rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
       pose_pub_ =this->create_publisher<geometry_msgs::msg::PoseStamped>("/pose",qos);
       plan_timer_ =this->create_wall_timer(
        std::chrono::duration<double>(1.0 /rate),
        std::bind(&posepublisher::planloop,this)
       );
       RCLCPP_INFO(this->get_logger(),"publisher started,rate=%.1f Hz",rate);
   } 
  private:
  void planloop(){
    geometry_msgs::msg::PoseStamped msg;
    msg.header.stamp = this->now();
    msg.header.frame_id ="map";
    msg.pose.position.x=1.0+0.1*static_cast<double>(count_);
    msg.pose.position.y=2.0;
    msg.pose.position.z=0.0;
    msg.pose.orientation.x = 0.0;
    msg.pose.orientation.y = 0.0;
    msg.pose.orientation.z = 0.0;
    msg.pose.orientation.w = 1.0;

    RCLCPP_INFO(this->get_logger(), "publish #%zu: x=%.2f y=%.2f",
      count_, msg.pose.position.x, msg.pose.position.y);

    pose_pub_->publish(msg);
    ++count_;
  }


  rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_pub_;
  rclcpp::TimerBase::SharedPtr plan_timer_;
  double rate = 10.0;
  size_t count_ = 0;
};



int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<posepublisher>());
  rclcpp::shutdown();
  return 0;
}