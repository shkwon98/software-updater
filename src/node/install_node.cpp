#include "software_updater/node/install_node.h"

#include <filesystem>

namespace software_updater::node
{

BT::NodeStatus InstallNode::onStart(void)
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

BT::NodeStatus InstallNode::onRunning(void)
{
    return timer_waiting_ ? BT::NodeStatus::RUNNING : BT::NodeStatus::SUCCESS;
}

void InstallNode::onHalted(void)
{
    timer_waiting_ = false;
    timer_.cancel(timer_id_);
}

} // namespace software_updater::node