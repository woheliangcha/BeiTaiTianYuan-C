#pragma once

#include "ObstacleModel.h"
#include <vector>

// 用于优先队列的比较器
struct CompareNode;

class AStarPlanner
{
public:
    /**
     * @brief 构造函数
     * @param model 包含了障碍物信息的地图模型
     */
    AStarPlanner(const ObstacleModel &model);

    /**
     * @brief 寻找从起点到终点的路径
     * @param start 起点坐标
     * @param end 终点坐标
     * @return 返回包含路径上所有关键点的向量。如果找不到路径，返回空向量。
     */
    std::vector<CustomPoint> findPath(const CustomPoint &start, const CustomPoint &end);

private:
    // A* 算法内部节点结构
    struct Node;

    // 将比较器声明为友元，以便它可以访问 Node 的私有成员
    friend struct ::CompareNode;

    const ObstacleModel &obstacleModel;

    /**
     * @brief 计算两个点之间的启发式距离（曼哈顿距离）
     */
    float heuristic(int x1, int y1, int x2, int y2) const;

    /**
     * @brief 从关闭列表中重建最终路径
     */
    std::vector<CustomPoint> reconstructPath(Node *endNode);
};