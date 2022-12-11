#include "oct.h"

//build octree to a certain depth
static OctNode* _build_octree(int depth, const Vec3& minimum_dimensions, const Vec3& maximum_dimensions)
{
    OctNode* node = new OctNode;
    node->minimum_dimensions = minimum_dimensions;
    node->maximum_dimensions = maximum_dimensions;
    node->center = (minimum_dimensions + maximum_dimensions) * 0.5;

    if(depth == 0)
    {
        node->isLeaf = true;
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            Vec3 corner(
                (i & (1 << 0)) ? maximum_dimensions[0] : minimum_dimensions[0],
                (i & (1 << 1)) ? maximum_dimensions[1] : minimum_dimensions[1],
                (i & (1 << 2)) ? maximum_dimensions[2] : minimum_dimensions[2]
            );

            Vec3 childmin, childmax;
            for(int k=0; k<3; k++)
            {
                childmin[k] = std::min(node->center[k], corner[k]);
                childmax[k] = std::max(node->center[k], corner[k]);
            }

            node->children[i] = _build_octree(depth-1, childmin, childmax);
        }
    }
    return node;
}

void Octree::build_octree(int maxDepth, const Vec3& minimum_dimensions, const Vec3& maximum_dimensions)
{
    root = _build_octree(maxDepth, minimum_dimensions, maximum_dimensions);
}

//insert_octree a point in the octree
void Octree::insert_octree(const Vec3& p)
{
    OctNode* n = root;
    while(!n->isLeaf)
    {
        Vec3 v = p - n->center;
        int index = ((v[0] < 0 ? 0 : 1) << 0) |
                    ((v[1] < 0 ? 0 : 1) << 1) |
                    ((v[2] < 0 ? 0 : 1) << 2);

        n = n->children[index];
    }

    n->data.push_back(p);
}

// nearest neighbor search
static Vec3 _findNearest(OctNode* node, const Vec3& p, Vec3 best)
{
    if(node->isLeaf)
    {
        for(const Vec3& v : node->data)
        {
            if(norm2(v - p) < norm2(best - p))
                best = v;
        }
    }
    else
    {
        std::vector<int> order = {0, 1, 2, 3, 4, 5, 6, 7};
        std::sort(order.begin(), order.end(), [=](int l, int r){
            return node->children[l]->pythagoreandistancetopoint(p) < node->children[r]->pythagoreandistancetopoint(p);
        });
        
        for(int i : order)
        {
            if(node->children[i]->pythagoreandistancetopoint(p) > norm2(best-p))
                break;
            best = _findNearest(node->children[i], p, best);
        }
    }

    return best;
}

Vec3 Octree::findNearestneighbor(const Vec3& p) const
{
    Vec3 best(1000000, 1000000, 1000000);
    return _findNearest(root, p, best);
}

//find points in AABB
static void _findpointsinradius(OctNode* node, const Vec3& min, const Vec3& max, std::vector<Vec3>& out)
{
    if(node->isLeaf)
    {
        for(const Vec3& p : node->data)
        {
            if(p[0] > min[0] && p[0] < max[0] &&
               p[1] > min[1] && p[1] < max[1] &&
               p[2] > min[2] && p[2] < max[2])
            {
                out.push_back(p);
            }
        }
    }
    else
    {
        for(int i=0; i<8; i++)
        {
            if(node->children[i]->intersectradius(min, max))
                _findpointsinradius(node->children[i], min, max, out);
        }
    }
}

std::vector<Vec3> Octree::findpointsinradius(const Vec3& minimum_dimensions, const Vec3& maximum_dimensions) const
{
    std::vector<Vec3> out;
    _findpointsinradius(root, minimum_dimensions, maximum_dimensions, out);
    return out;
}

//debugging
static void _clear_memory(OctNode* node, int depth, const Vec3& ref)
{
    if(node->isLeaf)
    {
        if(node->data.size())
        {
            printf("%s[leaf] ", std::string(depth*4, ' ').c_str());
            for(const Vec3& v : node->data)
                printf("(%.0f, %.0f, %.0f) %.1f   ", v[0], v[1], v[2], norm(v-ref));
            printf("\n");
        }
    }
    else
    {
        printf("%s[-], (%.0f, %.0f, %.0f)\n", std::string(depth*4, ' ').c_str(), 
            node->center[0], node->center[1], node->center[2]
        );

        for(int i=0; i<8; i++)
        {
            _clear_memory(node->children[i], depth+1, ref);
        }
    }
}

void Octree::clear_memory(const Vec3& ref) const 
{
    _clear_memory(this->root, 0, ref); 
}