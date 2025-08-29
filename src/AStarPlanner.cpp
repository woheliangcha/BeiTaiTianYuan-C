#include "AStarPlanner.h"
#include <queue>
#include <unordered_map>
#include <algorithm> // for std::reverse
#include <vector>

// 内部节点结构定义
struct AStarPlanner::Node
{
    int x, y;
    float gCost, hCost;
    Node *parent;

    float fCost() const { return gCost + hCost; }

    bool operator==(const Node &other) const
    {
        return x == other.x && y == other.y;
    }
};

// 用于优先队列的比较器
struct CompareNode
{
    bool operator()(const AStarPlanner::Node *a, const AStarPlanner::Node *b) const
    {
        return a->fCost() > b->fCost();
    }
};

// 用于哈希表的哈希和等于函数
struct GridPointHash
{
    std::size_t operator()(const std::pair<int, int> &p) const
    {
        return std::hash<int>()(p.first) ^ (std::hash<int>()(p.second) << 1);
    }
};

AStarPlanner::AStarPlanner(const ObstacleModel &model) : obstacleModel(model) {}

float AStarPlanner::heuristic(int x1, int y1, int x2, int y2) const
{
    return std::abs(x1 - x2) + std::abs(y1 - y2);
}

std::vector<CustomPoint> AStarPlanner::reconstructPath(Node *endNode)
{
    std::vector<CustomPoint> path;
    Node *currentNode = endNode;
    while (currentNode != nullptr)
    {
        path.push_back(obstacleModel.gridToWorld(currentNode->x, currentNode->y));
        currentNode = currentNode->parent;
    }
    std::reverse(path.begin(), path.end());
    return path;
}

std::vector<CustomPoint> AStarPlanner::findPath(const CustomPoint &start, const CustomPoint &end)
{
    CustomPoint startGrid = obstacleModel.worldToGrid(start);
    CustomPoint endGrid = obstacleModel.worldToGrid(end);

    int startX = static_cast<int>(startGrid.x);
    int startY = static_cast<int>(startGrid.y);
    int endX = static_cast<int>(endGrid.x);
    int endY = static_cast<int>(endGrid.y);

    if (obstacleModel.isObstacle(startX, startY) || obstacleModel.isObstacle(endX, endY))
    {
        return {}; // 起点或终点在障碍物内
    }

    Node *startNode = new Node{startX, startY, 0.0f, heuristic(startX, startY, endX, endY), nullptr};

    std::priority_queue<Node *, std::vector<Node *>, CompareNode> openSet;
    std::unordered_map<std::pair<int, int>, Node *, GridPointHash> allNodes;

    openSet.push(startNode);
    allNodes[{startX, startY}] = startNode;

    int dx[] = {0, 0, 1, -1};
    int dy[] = {1, -1, 0, 0};

    std::vector<CustomPoint> path;
    bool pathFound = false;

    while (!openSet.empty())
    {
        Node *currentNode = openSet.top();
        openSet.pop();

        if (currentNode->x == endX && currentNode->y == endY)
        {
            path = reconstructPath(currentNode);
            pathFound = true;
            break; // 找到路径，跳出循环
        }

        for (int i = 0; i < 4; ++i)
        {
            int newX = currentNode->x + dx[i];
            int newY = currentNode->y + dy[i];

            if (obstacleModel.isObstacle(newX, newY))
            {
                continue;
            }

            float newGCost = currentNode->gCost + 1.0f;

            auto it = allNodes.find({newX, newY});

            if (it == allNodes.end() || newGCost < it->second->gCost)
            {
                Node *neighborNode;
                if (it == allNodes.end())
                {
                    float h = heuristic(newX, newY, endX, endY);
                    neighborNode = new Node{newX, newY, newGCost, h, currentNode};
                    allNodes[{newX, newY}] = neighborNode;
                }
                else
                {
                    neighborNode = it->second;
                    neighborNode->gCost = newGCost;
                    neighborNode->parent = currentNode;
                }
                openSet.push(neighborNode);
            }
        }
    }

    // 清理所有分配的节点内存
    for (auto const &[key, val] : allNodes)
    {
        delete val;
    }

    return path;
}