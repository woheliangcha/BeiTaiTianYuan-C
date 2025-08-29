#include "RouteNodes.h"
#include <iostream>
#include <vector>

void printPath(const std::vector<CustomPoint> &path)
{
    if (path.empty())
    {
        std::cout << "\nPath is empty." << std::endl;
        return;
    }
    std::cout << "\n--- Final Path Coordinates ---" << std::endl;
    for (const auto &p : path)
    {
        std::cout << "  (" << p.x << ", " << p.y << ")" << std::endl;
    }
    std::cout << "-----------------------------" << std::endl;
}

int main()
{
    // --- Test Case Setup (类似PDF用例2: 简单模块阻挡) ---
    CompAndCanvasInfo wsg;

    // 画布大小
    wsg.graphicsceneWidth = 500;
    wsg.graphicsceneHeight = 500;

    // 起点和终点 (A -> C)
    wsg.startNode = CustomPoint(50, 250);
    wsg.endNode = CustomPoint(450, 250);
    std::cout << "Planning route from (50, 250) to (450, 250)\n"
              << std::endl;

    // 添加一个障碍物模块 (B)
    std::cout << "Adding one obstacle module at (210, 190) with size 80x120" << std::endl;
    CompPartInfo obstacleModule;
    obstacleModule.width = 80;
    obstacleModule.height = 120;
    obstacleModule.pos = CustomPoint(210, 190); // 放在A和C之间
    wsg.compPartInfo.push_back(obstacleModule);

    // 添加一条已存在的线作为障碍物
    std::cout << "Adding one existing line as an obstacle\n"
              << std::endl;
    std::vector<CustomPoint> existingLine = {CustomPoint(100, 400), CustomPoint(400, 400), CustomPoint(400, 450)};
    wsg.existingLines.push_back(existingLine);

    // --- 调用核心函数 ---
    std::vector<CustomPoint> finalPath;
    RouteNodes(wsg, finalPath);

    // --- 打印结果 ---
    printPath(finalPath);

    return 0;
}