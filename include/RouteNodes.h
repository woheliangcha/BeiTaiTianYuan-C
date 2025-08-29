#pragma once

#include "CommonDef.h"

/**
 * @brief 主函数，用于计算模块间的连线路径
 * @param wsg [in] 包含画布、模块、已有连线和起止点的信息
 * @param path [out] 计算得出的最终连线路径（包含起点、所有拐点和终点）
 */
void RouteNodes(const CompAndCanvasInfo &wsg, std::vector<CustomPoint> &path);