#include "ObstacleModel.h"
#include <cmath>

ObstacleModel::ObstacleModel(const CompAndCanvasInfo &canvasInfo, float gridSize, int inflationRadius)
    : canvasWidth(canvasInfo.graphicsceneWidth),
      canvasHeight(canvasInfo.graphicsceneHeight),
      gridSize(gridSize),
      inflationRadius(inflationRadius)
{
    gridWidth = static_cast<int>(std::ceil(canvasWidth / this->gridSize));
    gridHeight = static_cast<int>(std::ceil(canvasHeight / this->gridSize));
    gridMap.resize(gridWidth, std::vector<bool>(gridHeight, false));

    addModulesAsObstacles(canvasInfo.compPartInfo);
    addLinesAsObstacles(canvasInfo.existingLines);
}

void ObstacleModel::setObstacle(int x, int y)
{
    for (int i = -inflationRadius; i <= inflationRadius; ++i)
    {
        for (int j = -inflationRadius; j <= inflationRadius; ++j)
        {
            int newX = x + i;
            int newY = y + j;
            if (newX >= 0 && newX < gridWidth && newY >= 0 && newY < gridHeight)
            {
                gridMap[newX][newY] = true;
            }
        }
    }
}

bool ObstacleModel::isObstacle(int gridX, int gridY) const
{
    if (gridX < 0 || gridX >= gridWidth || gridY < 0 || gridY >= gridHeight)
    {
        return true; // 边界外视为障碍物
    }
    return gridMap[gridX][gridY];
}

bool ObstacleModel::isObstacle(const CustomPoint &point) const
{
    CustomPoint gridCoords = worldToGrid(point);
    return isObstacle(static_cast<int>(gridCoords.x), static_cast<int>(gridCoords.y));
}

CustomPoint ObstacleModel::gridToWorld(int gridX, int gridY) const
{
    return CustomPoint(gridX * gridSize + gridSize / 2.0f, gridY * gridSize + gridSize / 2.0f);
}

CustomPoint ObstacleModel::worldToGrid(const CustomPoint &point) const
{
    return CustomPoint(std::floor(point.x / gridSize), std::floor(point.y / gridSize));
}

void ObstacleModel::addModulesAsObstacles(const std::vector<CompPartInfo> &modules)
{
    for (const auto &mod : modules)
    {
        CustomPoint startGrid = worldToGrid(mod.pos);
        CustomPoint endGrid = worldToGrid(CustomPoint(mod.pos.x + mod.width, mod.pos.y + mod.height));

        for (int x = static_cast<int>(startGrid.x); x <= static_cast<int>(endGrid.x); ++x)
        {
            for (int y = static_cast<int>(startGrid.y); y <= static_cast<int>(endGrid.y); ++y)
            {
                setObstacle(x, y);
            }
        }
    }
}

void ObstacleModel::addLinesAsObstacles(const std::vector<std::vector<CustomPoint>> &lines)
{
    for (const auto &polyline : lines)
    {
        for (size_t i = 0; i < polyline.size() - 1; ++i)
        {
            CustomPoint p1_world = polyline[i];
            CustomPoint p2_world = polyline[i + 1];

            CustomPoint g1 = worldToGrid(p1_world);
            CustomPoint g2 = worldToGrid(p2_world);

            int x1 = static_cast<int>(g1.x);
            int y1 = static_cast<int>(g1.y);
            int x2 = static_cast<int>(g2.x);
            int y2 = static_cast<int>(g2.y);

            int dx = std::abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
            int dy = -std::abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
            int err = dx + dy, e2;

            while (true)
            {
                setObstacle(x1, y1);
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
        }
    }
}

// --- Getters Implementation ---
float ObstacleModel::getWidth() const { return canvasWidth; }
float ObstacleModel::getHeight() const { return canvasHeight; }
float ObstacleModel::getGridSize() const { return gridSize; }
int ObstacleModel::getGridWidth() const { return gridWidth; }
int ObstacleModel::getGridHeight() const { return gridHeight; }