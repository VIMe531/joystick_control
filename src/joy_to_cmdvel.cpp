#include <rclcpp/rclcpp.hpp>
#include <sensor_msgs/msg/joy.hpp>
#include <geometry_msgs/msg/twist.hpp>

class JoyToCmdVel : public rclcpp::Node
{
public:
    JoyToCmdVel()
    : Node("joy_to_cmdvel_node")
    {
        this->declare_parameter("linear_axis", 1);
        this->declare_parameter("angular_axis", 3);
        this->declare_parameter("linear_scale", 1.0);
        this->declare_parameter("angular_scale", 1.0);

        joy_sub_ = this->create_subscription<sensor_msgs::msg::Joy>(
            "joy", 10, std::bind(&JoyToCmdVel::joy_callback, this, std::placeholders::_1));

        cmd_vel_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("cmd_vel", 10);

        RCLCPP_INFO(this->get_logger(), "joy_to_cmdVel node has started");
    }

private:
    void joy_callback(const sensor_msgs::msg::Joy::SharedPtr msg)
    {
        int linear_axis = this->get_parameter("linear_axis").as_int();
        int angular_axis = this->get_parameter("angular_axis").as_int();
        double linear_scale = this->get_parameter("linear_scale").as_double();
        double angular_scale = this->get_parameter("angular_scale").as_double();

        geometry_msgs::msg::Twist twist_msg;
        twist_msg.linear.x = (msg->axes.size() > linear_axis) ? msg->axes[linear_axis] * linear_scale : 0.0;
        twist_msg.angular.z = (msg->axes.size() > angular_axis) ? msg->axes[angular_axis] * angular_scale : 0.0;

        cmd_vel_pub_->publish(twist_msg);
    }

    rclcpp::Subscription<sensor_msgs::msg::Joy>::SharedPtr joy_sub_;
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr cmd_vel_pub_;
};

int main(int argc, char * argv[])
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<JoyToCmdVel>());
    rclcpp::shutdown();
    return 0;
}

