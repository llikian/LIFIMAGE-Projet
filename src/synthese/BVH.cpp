/***************************************************************************************************
 * @file  BVH.cpp
 * @brief Implementation of the BVH class
 **************************************************************************************************/

#include "synthese/BVH.hpp"

bool BVH::Node::intersect(const Ray& ray) const {
    float tx1 = (pmin.x - ray.origin.x) / ray.direction.x, tx2 = (pmax.x - ray.origin.x) / ray.direction.x;
    float ty1 = (pmin.y - ray.origin.y) / ray.direction.y, ty2 = (pmax.y - ray.origin.y) / ray.direction.y;
    float tz1 = (pmin.z - ray.origin.z) / ray.direction.z, tz2 = (pmax.z - ray.origin.z) / ray.direction.z;

    float tmin = std::min(tx1, tx2), tmax = std::max(tx1, tx2);
    tmin = std::max(tmin, std::min(ty1, ty2)), tmax = std::min(tmax, std::max(ty1, ty2));
    tmin = std::max(tmin, std::min(tz1, tz2)), tmax = std::min(tmax, std::max(tz1, tz2));

    return tmax >= tmin && tmin < infinity && tmax > 0;
}

BVH::BVH(std::vector<const Object*>& objects): objects(objects), usedNodes(1), rootIndex(0) { }

void BVH::initialize() {
    objectIndices.clear();
    nodes.clear();
    usedNodes = 1;
    rootIndex = 0;

    if(objects.empty()) { return; }

    for(uint i = 0 ; i < objects.size() ; ++i) { objectIndices.push_back(i); }

    nodes.reserve(objects.size() * 2 - 1);

    Node& root = nodes[rootIndex];
    root.left = 0;
    root.firstObjectIndex = 0;
    root.objectCount = objects.size();
    updateBounds(rootIndex);
    subdivide(rootIndex);
}

Hit BVH::intersect(const Ray& ray) const {
    if(objects.empty()) { return Hit(); }

    return intersect(ray, rootIndex);
}

Hit BVH::intersect(const Ray& ray, uint nodeIndex) const {
    const Node& node = nodes[nodeIndex];
    if(!node.intersect(ray)) { return Hit(); }

    if(node.isLeaf()) {
        Hit closest;

        for(uint i = 0 ; i < node.objectCount ; i++) {
            const Object* object = objects.at(objectIndices.at(node.firstObjectIndex + i));
            Hit hit = object->intersect(ray);

            if(hit.intersection != infinity && (closest.object == nullptr || hit.intersection < closest.intersection)) {
                closest.intersection = hit.intersection;
                closest.normal = hit.normal;
                closest.object = object;
            }
        }

        return closest;
    } else {
        Hit hit1 = intersect(ray, node.left);
        Hit hit2 = intersect(ray, node.left + 1);

        return hit1.intersection < hit2.intersection ? hit1 : hit2;
    }
}

void BVH::updateBounds(uint nodeIndex) {
    Node& node = nodes[nodeIndex];
    node.pmin.x = node.pmin.y = node.pmin.z = infinity;
    node.pmax.x = node.pmax.y = node.pmax.z = -infinity;
    for(uint i = 0 ; i < node.objectCount ; ++i) {
        objects.at(objectIndices.at(node.firstObjectIndex + i))->compareBoundingBox(node.pmin, node.pmax);
    }
}

void BVH::subdivide(uint nodeIndex) {
    Node& node = nodes[nodeIndex];
    if(node.objectCount <= 2) { return; }

    Vector extent = node.pmax - node.pmin;

    int axis = 0;
    if(extent.y > extent(axis)) { axis = 1; }
    if(extent.z > extent(axis)) { axis = 2; }

    float bboxCenter = node.pmin(axis) + extent(axis) * 0.5f;

    int i = node.firstObjectIndex;
    int j = i + node.objectCount - 1;
    while(i <= j) {
        if(objects.at(objectIndices.at(i))->getCentroid()(axis) < bboxCenter) {
            ++i;
        } else {
            std::swap(objectIndices[i], objectIndices[j--]);
        }
    }

    int leftCount = i - node.firstObjectIndex;
    if(leftCount == 0 || leftCount == node.objectCount) { return; }

    int leftIndex = usedNodes++;
    int rightIndex = usedNodes++;

    nodes[leftIndex].firstObjectIndex = node.firstObjectIndex;
    nodes[leftIndex].objectCount = leftCount;
    nodes[rightIndex].firstObjectIndex = i;
    nodes[rightIndex].objectCount = node.objectCount - leftCount;
    node.left = leftIndex;
    node.objectCount = 0; // Shows this node isn't a leaf anymore

    updateBounds(leftIndex);
    updateBounds(rightIndex);

    subdivide(leftIndex);
    subdivide(rightIndex);
}
