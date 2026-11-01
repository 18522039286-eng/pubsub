#include <geometry_msgs/msg/pose_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <functional>
#include <memory>
class subscriber : public rclcpp::Node{
public:
    subscriber() : Node("pose_subscriber"){
        auto qos =rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
        pose_sub = this->create_subscription<geometry_msgs::msg::PoseStamped>(
            "/pose",qos,std::bind(&subscriber::posecallback,this,std::placeholders::_1)
        );
        RCLCPP_INFO(this->get_logger(),"waiting");
    }
private:
    void posecallback(const geometry_msgs::msg::PoseStamped::SharedPtr msg){
        RCLCPP_INFO(this->get_logger(),"receive:frame=%s,x=%.2f,y=%.2f",
        msg->header.frame_id.c_str(),msg->pose.position.x,msg->pose.position.y);
    }
    rclcpp::Subscription<geometry_msgs::msg::PoseStamped>::SharedPtr pose_sub;
};
int main(int argc,char ** argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<subscriber>());
    rclcpp::shutdown();
    return 0;
}