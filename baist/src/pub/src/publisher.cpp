#include <geometry_msgs/msg/pose_stamped.hpp>
#include <rclcpp/rclcpp.hpp>
#include <memory>
class publisher : public rclcpp::Node{
public:
    publisher() :Node("pose_publisher"){
        auto qos =rclcpp::QoS(rclcpp::KeepLast(10)).reliable();
        pose_pub =this->create_publisher<geometry_msgs::msg::PoseStamped>("/pose",qos);
        timer_ =this->create_wall_timer(
          std::chrono::milliseconds(500),
          [this](){pub();}
        );
        RCLCPP_INFO(this->get_logger(),"over");
    }
private:
    void pub(){
        geometry_msgs::msg::PoseStamped msg;
        msg.header.stamp=this->now();
        msg.header.frame_id="baist";
        msg.pose.position.x=18.0;
        msg.pose.position.y=95.0;
        msg.pose.orientation.w=1.0;
        RCLCPP_INFO(this->get_logger(),"publish:x=%.2f,y=%.2f",msg.pose.position.x,msg.pose.position.y);
        pose_pub->publish(msg);
    }
    rclcpp::Publisher<geometry_msgs::msg::PoseStamped>::SharedPtr pose_pub;
    rclcpp::TimerBase::SharedPtr timer_;
};
int main(int argc,char **argv){
    rclcpp::init(argc,argv);
    rclcpp::spin(std::make_shared<publisher>());
    rclcpp::shutdown();
    return 0;
}