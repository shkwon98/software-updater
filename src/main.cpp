#include <deque>

#include <behaviortree_cpp/bt_factory.h>
#include <behaviortree_cpp/decorators/loop_node.h>
#include <behaviortree_cpp/loggers/bt_cout_logger.h>

#include "common.h"
#include "nodes/copy_update_package.hpp"
#include "nodes/extract_archive.hpp"
#include "nodes/find_update_package.hpp"

int main()
{
    BT::BehaviorTreeFactory factory;

    factory.registerNodeType<FindUpdatePackage>("FindUpdatePackage");
    factory.registerNodeType<CopyUpdatePackage>("CopyUpdatePackage");
    factory.registerNodeType<ExtractArchive>("ExtractArchive");

    auto tree = factory.createTreeFromFile("../my_tree.xml");

    BT::StdCoutLogger logger(tree);
    logger.enableTransitionToIdle(false);

    tree.tickWhileRunning();

    return 0;
}
