#pragma once
#pragma region CommonDef
#include <vector>
#include <cmath>

enum class Direction
{
    NORTH,
    SOUTH,
    EAST,
    WEST,
    NONE
};

struct CustomPoint
{
    float x = 0.0f;
    float y = 0.0f;

    CustomPoint() = default;
    CustomPoint(float x_val, float y_val) : x(x_val), y(y_val) {}

    bool operator==(const CustomPoint &other) const
    {
        return std::fabs(x - other.x) < 1e-3 && std::fabs(y - other.y) < 1e-3;
    }

    // For use in maps/sets
    struct Hash
    {
        std::size_t operator()(const CustomPoint &p) const
        {
            return std::hash<float>()(p.x) ^ std::hash<float>()(p.y);
        }
    };
};

struct CompPartInfo
{
    float width;
    float height;
    CustomPoint pos;
};

struct CompAndCanvasInfo
{
    std::vector<CompPartInfo> compPartInfo;
    std::vector<std::vector<CustomPoint>> existingLines;
    float graphicsceneWidth;
    float graphicsceneHeight;
    CustomPoint startNode;
    CustomPoint endNode;
};
#pragma endregion CommonDef