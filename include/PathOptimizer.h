#pragma once

#include "CommonDef.h"
#include "ObstacleModel.h"
#include <vector>

class PathOptimizer
{
public:
    /**
     * @brief 构造函数
     * @param model 包含了障碍物信息的地图模型，用于优化时检查碰撞
     */
    PathOptimizer(const ObstacleModel &model);

    /**
     * @brief 优化给定的路径
     * @param initialPath A*算法生成的初始路径
     * @return 返回优化后的路径
     */
    std::vector<CustomPoint> optimize(const std::vector<CustomPoint> &initialPath);

private:
    const ObstacleModel &obstacleModel;

    /**
     * @brief 移除路径中多余的拐点（三点共线的情况）
     */
    std::vector<CustomPoint> removeRedundantPoints(const std::vector<CustomPoint> &path);

    /**
     * @brief 尝试拉直路径以缩短长度并减少拐点
     */
    std::vector<CustomPoint> straightenPath(const std::vector<CustomPoint> &path);

    /**
     * @brief 检查两点之间的直线路径是否穿过障碍物
     */
    bool isLineObstructed(const CustomPoint &p1, const CustomPoint &p2);
};