#pragma once

#include "CommonDef.h"

class ObstacleModel
{
public:
    /**
     * @brief 构造函数，初始化障碍物地图
     * @param canvasInfo 包含所有模块和已存在线的画布信息
     * @param gridSize 栅格化地图的网格大小
     * @param inflationRadius 障碍物膨胀半径（以格子为单位）
     */
    ObstacleModel(const CompAndCanvasInfo &canvasInfo, float gridSize = 5.0f, int inflationRadius = 1);

    /**
     * @brief 检查某个栅格坐标是否是障碍物
     * @param gridX 栅格的X坐标
     * @param gridY 栅格的Y坐标
     * @return 如果是障碍物点，则返回 true
     */
    bool isObstacle(int gridX, int gridY) const;

    /**
     * @brief 检查某个世界坐标点是否在障碍物内
     * @param point 要检查的点
     * @return 如果是障碍物点，则返回 true
     */
    bool isObstacle(const CustomPoint &point) const;

    /**
     * @brief 将栅格坐标转换为世界坐标
     */
    CustomPoint gridToWorld(int gridX, int gridY) const;

    /**
     * @brief 将世界坐标转换为栅格坐标
     */
    CustomPoint worldToGrid(const CustomPoint &point) const;

    // --- Getters ---
    float getWidth() const;
    float getHeight() const;
    float getGridSize() const;
    int getGridWidth() const;
    int getGridHeight() const;

private:
    std::vector<std::vector<bool>> gridMap;
    float canvasWidth;
    float canvasHeight;
    float gridSize;
    int gridWidth;
    int gridHeight;
    int inflationRadius;

    /**
     * @brief 将所有模块添加到栅格地图中
     */
    void addModulesAsObstacles(const std::vector<CompPartInfo> &modules);

    /**
     * @brief 将所有已存在线段添加到栅格地图中
     */
    void addLinesAsObstacles(const std::vector<std::vector<CustomPoint>> &lines);

    /**
     * @brief 在栅格地图上标记一个点为障碍物，并应用膨胀
     */
    void setObstacle(int x, int y);
};