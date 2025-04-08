/***************************************************************************************************
 * @file  BVH.hpp
 * @brief Declaration of the BVH class
 **************************************************************************************************/

#pragma once

#include <vector>
#include "Object.hpp"
#include "vec.h"

/**
 * @class BVH
 * @brief A Bounding Volume Hierarchy implementation. Written with the help of this article:
 * https://jacco.ompf2.com/2022/04/13/how-to-build-a-bvh-part-1-basics/
 */
class BVH {
public:
    struct Node {
        Point pmin, pmax;
        uint left;
        uint firstPrimitiveIndex;
        uint primitiveCount;

        bool isLeaf() const { return primitiveCount > 0; }

        bool intersect(const Ray& ray) const;
    };

    explicit BVH(std::vector<const Object*>& objects);
    ~BVH();

    void initialize();

    void updateBounds(uint nodeIndex) const;
    void subdivide(uint nodeIndex);

    Hit intersect(const Ray& ray) const;
    const Object* getClosestObject(const Ray& ray) const;

private:
    Hit intersect(const Ray& ray, uint nodeIndex) const;
    const Object* getClosestObject(const Ray& ray, uint nodeIndex) const;

    const std::vector<const Object*>& objects;
    std::vector<uint> objectIndices;
    Node* nodes;
    uint usedNodes;
    uint rootIndex;
};
