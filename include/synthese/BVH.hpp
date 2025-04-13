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
    /**
     * @struct BVH::Node
     * @brief A node in the BVH's tree, represents an AABB (Axis-Aligned Bounding Box).
     */
    struct Node {
        Point pmin; ///< The lower bound of the bounding box.
        Point pmax; ///< The higher bound of the bounding box.
        uint left; ///< The index of the left child. The right child isn't necessary since it's always left + 1.
        uint firstObjectIndex; ///< The index of the first object encompassed by the bounding box.
        uint objectCount; ///< The amount of objects encompassed by the bounding box.

        /**
         * @brief Checks if the node is a leaf.
         * @return Whether the node is a leaf or not.
         */
        bool isLeaf() const { return objectCount > 0; }

        /**
         * @brief Checks if a ray intersects with a bounding box.
         * @param ray The ray to check the intersection with.
         * @return Whether a ray intersects with the bounding box.
         */
        bool intersect(const Ray& ray) const;
    };

    /**
     * Constructor. Initializes all values but doesn't initialize the BVH yet. Call BVH::initialize() once all objects
     * have been added to the scene.
     * @param objects A reference to the objects in a scene.
     */
    explicit BVH(std::vector<const Object*>& objects);

    /**
     * @brief Initializes the BVH. Creates the root, calculates its bounds and calls the subdivide method on it.
     */
    void initialize();

    /**
     * @brief Recursively calculates the intersection between a ray and the BVH starting at the root.
     * @param ray The ray to check the intersection with.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray) const;

private:
    /**
     * @brief Recursively calculates the intersection between a ray and the BVH starting at a specific node.
     * @param ray The ray to check the intersection with.
     * @param nodeIndex The index of the node.
     * @return The information on the hit object. If no object is hit, the intersection will be set to infinity.
     */
    Hit intersect(const Ray& ray, uint nodeIndex) const;

    /**
     * @brief Updates the bounds of a given node. Iterates through all the objects encompassed by the node to calculate
     * its lower and higher bounds.
     * @param nodeIndex The index of the node.
     */
    void updateBounds(uint nodeIndex);

    /**
     * @brief Recursively subdivides a given node
     * @param nodeIndex The index of the node.
     */
    void subdivide(uint nodeIndex);

    const std::vector<const Object*>& objects; ///< A reference to the objects in a scene.
    std::vector<uint> objectIndices;           ///< The indices of the objects. Used to avoid copies of bigger objects.
    std::vector<Node> nodes;                   ///< The BVH's nodes.
    uint usedNodes;                            ///< The amount of nodes currently in the BHV.
    uint rootIndex;                            ///< The index of the root, usually 0.
};
