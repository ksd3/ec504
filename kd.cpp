#include "kd.h"

//contructing the tree
//every time the depth increases the axis is split
//the axis is split around the middle
static KDNode* _build_kdnode(int depth, KDAxis axis, std::vector<Vec3>& test) {
    KDNode* node = new KDNode();
    if(depth == 0 || test.size() < 3) {
        node->isLeaf = true;
        node->data.insert(node->data.end(), test.begin(), test.end());
    }
    else {
        std::sort(test.begin(), test.end(), [=](const Vec3& l, const Vec3& r) {
            return l[axis] < r[axis];
        });

        int splitlengthforpartition = test.size() / 2;
        std::vector<Vec3> left(test.begin(), test.begin()+splitlengthforpartition);
        std::vector<Vec3> right(test.begin()+splitlengthforpartition, test.end());

        node->axis = axis;
        node->split = test[splitlengthforpartition][axis];
        node->left = _build_kdnode(depth-1, (KDAxis)((axis+1) % KDAxis::N), left);
        node->right = _build_kdnode(depth-1, (KDAxis)((axis+1) % KDAxis::N), right);
    }
    return node;
}

void KDTree::build_kdtree(int maxDepth, const std::vector<Vec3>& points) {
    std::vector<Vec3> recursivetree = points;
    root = _build_kdnode(maxDepth, KDAxis::X, recursivetree);
}

// nearest neighbor search
static Vec3 _findNearest(KDNode* node, const Vec3& p, Vec3 best) {
    if(node->isLeaf) {
        if(node->data.size() == 0)
            return best;

        std::sort(node->data.begin(), node->data.end(), [=](const Vec3& l, const Vec3& r) {
            return norm2(l - p) < norm2(r - p);
        });
        return norm2(best - p) < norm2(node->data[0]-p) ? best : node->data[0];
    }
    else {
        KDNode* first_node = p[node->axis] < node->split ? node->left : node->right;
        best = _findNearest(first_node, p, best);
        if(std::abs(node->split - p[node->axis]) <= norm(best - p)) {
            KDNode* other = node->left == first_node ? node->right : node->left;
            best = _findNearest(other, p, best);
        }

        return best;
    }
}

Vec3 KDTree::findNearestneighbor(const Vec3& p) const
{
    Vec3 best(1000000, 1000000, 1000000);
    return _findNearest(root, p, best);
}

//find points in the tree
static void _findpointsinradius(KDNode* node, const Vec3& minimum1, const Vec3& maximum1, std::vector<Vec3>& outtree)
{
    if(node->isLeaf)
    {
        for(const Vec3& test : node->data) {
            if(test[0] > minimum1[0] && test[0] < maximum1[0] &&
               test[1] > minimum1[1] && test[1] < maximum1[1] &&
               test[2] > minimum1[2] && test[2] < maximum1[2])
            {
                outtree.push_back(test);
            }
        }
    }
    else {
        if(minimum1[node->axis] < node->split)
            _findpointsinradius(node->left, minimum1, maximum1, outtree);
        if(maximum1[node->axis] > node->split)
            _findpointsinradius(node->right, minimum1, maximum1, outtree);
    }
}

std::vector<Vec3> KDTree::findpointsinradius(const Vec3& minimum_dimensions, const Vec3& maximum_dimensions) const {
    std::vector<Vec3> outtree;
    _findpointsinradius(root, minimum_dimensions, maximum_dimensions, outtree);
    return outtree;
}

//debugging
static void _clear_memory(KDNode* node, int level, const Vec3& ref) {
    if(node->isLeaf) {
        printf("%s[leaf] ", std::string(level*4, ' ').c_str());
        for(const Vec3& v : node->data)
            printf("(%.0f, %.0f, %.0f) %.1f   ", v[0], v[1], v[2], norm(v-ref));
        printf("\n");
    }
    else {
        char axismap[] = {'x', 'y', 'z'};
        printf("%s[-] %c %.0f\n", std::string(level*4, ' ').c_str(), axismap[node->axis], node->split);
        _clear_memory(node->left, level+1, ref);
        _clear_memory(node->right, level+1, ref);
    }
}

void KDTree::clear_memory(const Vec3& ref) const {
    _clear_memory(root, 0, ref);
}

