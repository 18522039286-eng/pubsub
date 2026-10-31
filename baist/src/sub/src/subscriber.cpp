#include <functional>
#include <memory>

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose_stamped.hpp"

class PoseSubscriber : public rclcpp::Node
{
public:
  PoseSubscriber() : Node("pose_subscriber")
  {
    auto qos = rclcpp::QoS(rclcpp::KeepLast(10)).reliable();

    pose_sub_ = this->create_subscription<geometry_msgs::msg::PoseStamped>(
      "/pose", qos,
      std::bind(&PoseSubscriber::poseCallback, this, std::placeholders::_1));

    RCLCPP_INFO(this->get_logger(), "subscriber started, waiting /pose ...");
  }

private:
  void poseCallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg)
  {
    RCLCPP_INFO(this->get_logger(),
      "recv: frame=%s x=%.2f y=%.2f z=%.2f",
      msg->header.frame_id.c_str(),
      msg->pose.position.x, msg->pose.position.y, msg->pose.position.z);
  }

  rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_sub_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<PoseSubscriber>());
  rclcpp::shutdown();
  return 0;
}