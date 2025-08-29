#include "RouteNodes.h"
#include "ObstacleModel.h"
#include "AStarPlanner.h"
#include "PathOptimizer.h"
#include <iostream>

void RouteNodes(const CompAndCanvasInfo &wsg, std::vector<CustomPoint> &path)
{
    // 1. 创建障碍物模型
    // gridSize: 网格大小，较小值更精确但慢。5.0是一个不错的起点。
    // inflationRadius: 障碍物膨胀半径(格子数)，确保路径不贴边。
    std::cout << "1. Creating obstacle model..." << std::endl;
    ObstacleModel model(wsg, 5.0f, 2);

    // 2. 创建A*规划器实例
    AStarPlanner planner(model);

    // 3. 寻找初始路径
    std::cout << "2. Finding initial path with A*..." << std::endl;
    std::vector<CustomPoint> initialPath = planner.findPath(wsg.startNode, wsg.endNode);

    if (initialPath.empty())
    {
        std::cerr << "Error: Could not find a path." << std::endl;
        path.clear();
        return;
    }
    std::cout << "   Initial path found with " << initialPath.size() << " points." << std::endl;

    // 4. 创建路径优化器实例
    PathOptimizer optimizer(model);

    // 5. 优化路径
    std::cout << "3. Optimizing path..." << std::endl;
    path = optimizer.optimize(initialPath);
    std::cout << "   Path optimized to " << path.size() << " points." << std::endl;

    // 6. 最终路径已存入输出参数 `path`
}