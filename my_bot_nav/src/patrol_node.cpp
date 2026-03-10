#include <memory>
#include <chrono>
#include <functional> // Cần thiết cho std::bind
#include "rclcpp/rclcpp.hpp"
#include "rclcpp_action/rclcpp_action.hpp"
#include "nav2_msgs/action/navigate_to_pose.hpp"

using namespace std::chrono_literals;
using NavigateToPose = nav2_msgs::action::NavigateToPose;
using GoalHandleNav = rclcpp_action::ClientGoalHandle<NavigateToPose>;

class PatrolNode : public rclcpp::Node
{
public:
  PatrolNode() : Node("patrol_node")
  {
    this->client_ptr_ = rclcpp_action::create_client<NavigateToPose>(this, "navigate_to_pose");
    // Timer gọi hàm send_goal sau 1 giây để đảm bảo node đã khởi động xong
    this->timer_ = this->create_wall_timer(1000ms, std::bind(&PatrolNode::send_goal, this));
  }

  void send_goal()
  {
    this->timer_->cancel(); // Hủy timer để chỉ chạy 1 lần

    if (!this->client_ptr_->wait_for_action_server(10s)) {
      RCLCPP_ERROR(this->get_logger(), "Action server 'navigate_to_pose' not available. Check Nav2 stack!");
      rclcpp::shutdown();
      return;
    }

    auto goal_msg = NavigateToPose::Goal();
    goal_msg.pose.header.frame_id = "map";
    goal_msg.pose.header.stamp = this->now();
    
    // --- SET TỌA ĐỘ MỤC TIÊU ---
    // Lưu ý: Hãy chắc chắn tọa độ này nằm trong vùng 'trắng' (không có vật cản) trên bản đồ
    goal_msg.pose.pose.position.x = 2.0; 
    goal_msg.pose.pose.position.y = 0.5;
    goal_msg.pose.pose.orientation.w = 1.0; // w=1.0 nghĩa là không xoay (hướng 0 độ)

    RCLCPP_INFO(this->get_logger(), "Sending goal to x=2.0, y=0.5...");

    auto send_goal_options = rclcpp_action::Client<NavigateToPose>::SendGoalOptions();
    
    // 1. Callback khi Server phản hồi có nhận Goal hay không
    send_goal_options.goal_response_callback = 
      std::bind(&PatrolNode::goal_response_callback, this, std::placeholders::_1);

    // 2. Callback nhận Feedback liên tục (khoảng cách còn lại)
    send_goal_options.feedback_callback = 
      std::bind(&PatrolNode::feedback_callback, this, std::placeholders::_1, std::placeholders::_2);

    // 3. Callback khi hoàn thành (thành công hoặc thất bại)
    send_goal_options.result_callback = 
      std::bind(&PatrolNode::result_callback, this, std::placeholders::_1);
    
    this->client_ptr_->async_send_goal(goal_msg, send_goal_options);
  }

private:
  // --- Xử lý việc Server chấp nhận hay từ chối Goal ---
  void goal_response_callback(const GoalHandleNav::SharedPtr & goal_handle)
  {
    if (!goal_handle) {
      RCLCPP_ERROR(this->get_logger(), "Goal was REJECTED by server (maybe outside of map?)");
      rclcpp::shutdown();
    } else {
      RCLCPP_INFO(this->get_logger(), "Goal ACCEPTED by server, waiting for result...");
    }
  }

  // --- Xử lý Feedback (Optional nhưng rất hữu ích để debug) ---
  void feedback_callback(
    GoalHandleNav::SharedPtr,
    const std::shared_ptr<const NavigateToPose::Feedback> feedback)
  {
    // In ra khoảng cách còn lại (distance_remaining)
    RCLCPP_INFO(this->get_logger(), "Distance remaining: %.2f m", feedback->distance_remaining);
  }

  // --- Xử lý Kết quả cuối cùng ---
  void result_callback(const GoalHandleNav::WrappedResult & result)
  {
    switch (result.code) {
      case rclcpp_action::ResultCode::SUCCEEDED:
        RCLCPP_INFO(this->get_logger(), "Goal reached successfully!");
        break;
      case rclcpp_action::ResultCode::ABORTED:
        RCLCPP_ERROR(this->get_logger(), "Goal was aborted (Stuck or blocked?)");
        break;
      case rclcpp_action::ResultCode::CANCELED:
        RCLCPP_ERROR(this->get_logger(), "Goal was canceled");
        break;
      default:
        RCLCPP_ERROR(this->get_logger(), "Unknown result code");
        break;
    }
    rclcpp::shutdown(); // Tắt node
  }

  rclcpp_action::Client<NavigateToPose>::SharedPtr client_ptr_;
  rclcpp::TimerBase::SharedPtr timer_;
};

int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);
  auto node = std::make_shared<PatrolNode>();
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}