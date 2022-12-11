#pragma once

#include "headers/vec3.h"
#include <vector>

struct OctNode
{
    //declare a 3D vector as defined in the header files
    Vec3 minimum_dimensions;
    Vec3 maximum_dimensions;
    Vec3 center; 

    //declare 8 children of the node - recursive definition of the octree
    OctNode* children[8];

    bool isLeaf; //this helps in setting up the leaf node: as in, no children=>leaf
    std::vector<Vec3> data; //declare vector of 3D vectors named data

    OctNode() : isLeaf(false) {} //by default, assume that the node is not a leaf

    float pythagoreandistancetopoint(const Vec3& test) //minimum pythagorean distance
    {
        Vec3 minimumvector;
        for(int i=0; i<3; i++)
        {
            //compute distance to the box edge - 0 if inside rectangle
            minimumvector[i] = test[i] - std::max(std::min(test[i], maximum_dimensions[i]), minimum_dimensions[i]);
        }
        return norm2(minimumvector);
    }

    bool intersectradius(const Vec3& minimum, const Vec3& maximum) const 
    {
        for(int i=0; i<3; i++)
        {
            if(maximum[i] < minimum_dimensions[i] || minimum[i] > maximum_dimensions[i])
                return false;
        }
        return true;
    }
};

class Octree
{
    OctNode* root;

public:
    void build_octree(int maxDepth, const Vec3& minimum_dimensions, const Vec3& maximum_dimensions);
    void insert_octree(const Vec3& p); 

    Vec3 findNearestneighbor(const Vec3& p) const;
    std::vector<Vec3> findpointsinradius(const Vec3& minimum_dimensions, const Vec3& maximum_dimensions) const;

    void clear_memory(const Vec3& ref) const;
};


