#pragma once

#include <atomic>

#include <behaviortree_cpp/action_node.h>
#include <behaviortree_cpp/utils/timer_queue.h>

namespace software_updater::node
{

class InstallNode : public BT::StatefulActionNode
{
public:
    InstallNode(const std::string &name, const BT::NodeConfig &config)
        : BT::StatefulActionNode(name, config)
        , timer_waiting_(false)
    {
    }

    ~InstallNode() override
    {
        halt();
    }

    static BT::PortsList providedPorts(void)
    {
        return { BT::InputPort<unsigned>("msec"), BT::OutputPort<bool>("result") };
    }

    BT::NodeStatus onStart(void) override;
    BT::NodeStatus onRunning(void) override;
    void onHalted(void) override;

private:
    BT::TimerQueue<> timer_;
    uint64_t timer_id_;

    std::atomic_bool timer_waiting_ = false;
    std::mutex delay_mutex_;
};

} // namespace software_updater::node