#ifndef SIMPLE_BT_NODES_H
#define SIMPLE_BT_NODES_H

#include "behaviortree_cpp/behavior_tree.h"

using BT::NodeStatus;

// Example of custom SyncActionNode (synchronous action)
// with an input port.
class SaySomething : public BT::SyncActionNode
{
public:
    SaySomething(const std::string &name, const BT::NodeConfig &config)
        : BT::SyncActionNode(name, config)
    {
    }

    // You must override the virtual function tick()
    NodeStatus tick() override
    {
        auto msg = getInput<std::string>("message");
        if (!msg)
        {
            throw BT::RuntimeError("missing required input [message]: ", msg.error());
        }

        std::cout << "Robot says: " << msg.value() << std::endl;
        return BT::NodeStatus::SUCCESS;
    }

    // It is mandatory to define this static method.
    static BT::PortsList providedPorts()
    {
        return { BT::InputPort<std::string>("message") };
    }
};

#endif // SIMPLE_BT_NODES_H
