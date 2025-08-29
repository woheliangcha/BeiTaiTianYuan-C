#include "PathOptimizer.h"
#include <cmath>

PathOptimizer::PathOptimizer(const ObstacleModel &model) : obstacleModel(model) {}

std::vector<CustomPoint> PathOptimizer::optimize(const std::vector<CustomPoint> &initialPath)
{
    if (initialPath.size() < 2)
    {
        return initialPath;
    }
    // 移除A*在栅格上产生的细微冗余点
    auto path = removeRedundantPoints(initialPath);
    // 核心优化步骤：通过跳点来拉直路径
    path = straightenPath(path);
    // 最后再清理一次可能产生的共线点
    return removeRedundantPoints(path);
}

std::vector<CustomPoint> PathOptimizer::removeRedundantPoints(const std::vector<CustomPoint> &path)
{
    if (path.size() < 3)
    {
        return path;
    }

    std::vector<CustomPoint> optimizedPath;
    optimizedPath.push_back(path.front());

    for (size_t i = 1; i < path.size() - 1; ++i)
    {
        const auto &p1 = optimizedPath.back();
        const auto &p2 = path[i];
        const auto &p3 = path[i + 1];

        // 检查三点是否共线
        // 由于是曼哈顿路径，只需检查x坐标或y坐标是否相同
        bool isCollinear = (std::abs(p1.x - p2.x) < 1e-3 && std::abs(p2.x - p3.x) < 1e-3) ||
                           (std::abs(p1.y - p2.y) < 1e-3 && std::abs(p2.y - p3.y) < 1e-3);

        if (!isCollinear)
        {
            optimizedPath.push_back(p2);
        }
    }
    optimizedPath.push_back(path.back());
    return optimizedPath;
}

bool PathOptimizer::isLineObstructed(const CustomPoint &p1, const CustomPoint &p2)
{
    CustomPoint g1 = obstacleModel.worldToGrid(p1);
    CustomPoint g2 = obstacleModel.worldToGrid(p2);

    int x1 = static_cast<int>(g1.x);
    int y1 = static_cast<int>(g1.y);
    int x2 = static_cast<int>(g2.x);
    int y2 = static_cast<int>(g2.y);

    int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (true)
    {
        if (obstacleModel.isObstacle(x1, y1))
            return true;
        if (x1 == x2 && y1 == y2)
            break;
        e2 = 2 * err;
        if (e2 >= dy)
        {
            err += dy;
            x1 += sx;
        }
        if (e2 <= dx)
        {
            err += dx;
            y1 += sy;
        }
    }
    return false;
}

std::vector<CustomPoint> PathOptimizer::straightenPath(const std::vector<CustomPoint> &path)
{
    if (path.size() < 3)
    {
        return path;
    }

    std::vector<CustomPoint> newPath;
    newPath.push_back(path.front());

    size_t currentIndex = 0;
    while (currentIndex < path.size() - 1)
    {
        size_t bestNextIndex = currentIndex + 1;
        for (size_t j = path.size() - 1; j > currentIndex + 1; --j)
        {
            // 尝试两种L型连接方式
            CustomPoint corner1(path[j].x, path[currentIndex].y);
            if (!isLineObstructed(path[currentIndex], corner1) && !isLineObstructed(corner1, path[j]))
            {
                bestNextIndex = j;
                break;
            }

            CustomPoint corner2(path[currentIndex].x, path[j].y);
            if (!isLineObstructed(path[currentIndex], corner2) && !isLineObstructed(corner2, path[j]))
            {
                bestNextIndex = j;
                break;
            }
        }

        // 如果找到了捷径
        if (bestNextIndex > currentIndex + 1)
        {
            // 决定添加哪个拐角点
            CustomPoint p_current = path[currentIndex];
            CustomPoint p_next = path[bestNextIndex];
            CustomPoint corner1(p_next.x, p_current.y);
            if (!isLineObstructed(p_current, corner1) && !isLineObstructed(corner1, p_next))
            {
                if (!(corner1 == p_current) && !(corner1 == p_next))
                {
                    newPath.push_back(corner1);
                }
            }
            else
            {
                CustomPoint corner2(p_current.x, p_next.y);
                if (!(corner2 == p_current) && !(corner2 == p_next))
                {
                    newPath.push_back(corner2);
                }
            }
        }
        else
        { // 没找到捷径，正常前进
            newPath.push_back(path[bestNextIndex]);
        }

        currentIndex = bestNextIndex;
    }

    // 确保终点被添加
    if (newPath.back() == path.back())
    {
        // ok
    }
    else
    {
        newPath.push_back(path.back());
    }

    return newPath;
}