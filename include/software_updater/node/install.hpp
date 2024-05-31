#pragma once

#include <filesystem>

#include <behaviortree_cpp/action_node.h>
#include <behaviortree_cpp/utils/timer_queue.h>

class Install : public BT::StatefulActionNode
{
public:
    Install(const std::string &name, const BT::NodeConfig &config)
        : BT::StatefulActionNode(name, config)
        , timer_waiting_(false)
    {
    }

    ~Install() override
    {
        halt();
    }

    BT::NodeStatus onStart() override
    {
        unsigned msec = 0;
        if (!getInput("msec", msec))
        {
            throw BT::RuntimeError("Missing parameter [msec] in SleepNode");
        }

        if (msec <= 0)
        {
            return BT::NodeStatus::SUCCESS;
        }

        setStatus(BT::NodeStatus::RUNNING);

        timer_waiting_ = true;

        timer_id_ = timer_.add(std::chrono::milliseconds(msec), [this](bool aborted) {
            std::unique_lock<std::mutex> lk(delay_mutex_);
            if (!aborted)
            {
                emitWakeUpSignal();
            }
            timer_waiting_ = false;
        });

        setOutput("result", true);
        return BT::NodeStatus::RUNNING;
    }

    BT::NodeStatus onRunning() override
    {
        return timer_waiting_ ? BT::NodeStatus::RUNNING : BT::NodeStatus::SUCCESS;
    }

    void onHalted() override
    {
        timer_waiting_ = false;
        timer_.cancel(timer_id_);
    }

    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<unsigned>("msec"), BT::OutputPort<bool>("result") };
    }

private:
    BT::TimerQueue<> timer_;
    uint64_t timer_id_;

    std::atomic_bool timer_waiting_ = false;
    std::mutex delay_mutex_;
};