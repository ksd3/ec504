#pragma once

#include "headers/vec3.h"
#include <vector>

enum KDAxis {
    X = 0,
    Y = 1,
    Z = 2,
    N = 3
};

struct KDNode {
    //non-leaf node (has splitting)
    KDAxis axis;
    float split;

    KDNode* left;
    KDNode* right;

    //leaf node (no splitting)
    bool isLeaf;
    std::vector<Vec3> data;

    KDNode() : axis(KDAxis::X), left(nullptr), right(nullptr), isLeaf(false) {}
};

class KDTree {
    KDNode* root;

public:
    void build_kdtree(int maxDepth, const std::vector<Vec3>& points);

    Vec3 findNearestneighbor(const Vec3& p) const;
    std::vector<Vec3> findpointsinradius(const Vec3& minDim, const Vec3& maxDim) const;

    void clear_memory(const Vec3& ref) const;
};

