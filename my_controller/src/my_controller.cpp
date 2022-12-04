# include <controller_interface/controller.h>
# include <hardware_interface/joint_command_interface.h>
# include <pluginlib/class_list_macros.h>
# include <std_msgs/Float64.h>

namespace my_controller_ns
{

    class MyPositionController : public controller_interface::Controller<hardware_interface::EffortJointInterface>
    {
        bool init(hardware_interface::EffortJointInterface* hw, ros::NodeHandle &n)
        {
            // get joint name from the parameter server
            std::string my_joint;
            if (!n.getParam("joint", my_joint)){
                ROS_ERROR("Could not find joint name");
                return false;
            }

            // get the joint object to use in the realtime loop
            joint_ = hw->getHandle(my_joint);   // throws on failure
            command_ = joint_.getPosition();    // set the current joint goal to the current joint position

            // load gain using gains set on parameter server
            if (!n.getParam("gain", gain_)){
                ROS_ERROR("Could not find the gain parameter value");
                return false;
            }

            // Start command subscriber
            sub_command_ = n.subscribe<std_msgs::Float64>("command", 1, &MyPositionController::setCommandCB, this);

            return true;
        }
    };

}