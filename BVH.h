#ifndef BVH_H
#define BVH_H

#include <limits>
#include <algorithm>

// Defining the structure of Axis-Aligned-Bounding-Box

struct AABB {
    float minX, minY, maxX, maxY;

    AABB() : minX(0), minY(0), maxX(0), maxY(0) {}

    AABB(float minX, float minY, float maxX, float maxY)
        : minX(minX), minY(minY), maxX(maxX), maxY(maxY) {}

    // Expands this AABB to include another AABB
    void expand(const AABB& other) {
        minX = std::min(minX, other.minX);
        minY = std::min(minY, other.minY);
        maxX = std::max(maxX, other.maxX);
        maxY = std::max(maxY, other.maxY);
    }

    // Checks if this AABB overlaps with another AABB
    bool overlaps(const AABB& other) const {
        return !(minX > other.maxX || maxX < other.minX ||
                 minY > other.maxY || maxY < other.minY);
    }

    // Computes the area of the AABB
    float area() const {
        return (maxX - minX) * (maxY - minY);
    }
};


// Defining the structure of BVH Node

struct BVHNode {
    AABB bounds;
    BVHNode* left;
    BVHNode* right;
    int particleIndex; // Index of the particle, -1 if not a leaf

    BVHNode() : left(nullptr), right(nullptr), particleIndex(-1) {}

    bool isLeaf() const {
        return particleIndex != -1;
    }
};

// Defining the BHV class

class BVH {
public:
    BVHNode* root;

    BVH() : root(nullptr) {}

    ~BVH() {
        deleteTree(root);
    }

    void build(std::vector<Particle>& particles) {
        std::vector<AABB> particleBounds;
        for (auto& particle : particles) {
            particleBounds.push_back(AABB(
                particle.getPosition().x - 0.1f, particle.getPosition().y - 0.1f,
                particle.getPosition().x + 0.1f, particle.getPosition().y + 0.1f
            ));
        }

        root = buildRecursive(particles, particleBounds, 0, particles.size());
    }

    std::vector<int> query(const AABB& queryBounds) {
        std::vector<int> result;
        queryRecursive(root, queryBounds, result);
        return result;
    }

    void updateParticles(std::vector<Particle>& particles, float deltaTime) {
        for (auto& particle : particles) {
            particle.update(deltaTime);
        }
        build(particles); // Rebuild BVH after updating particles
    }

private:
    BVHNode* buildRecursive(std::vector<Particle>& particles, std::vector<AABB>& particleBounds, size_t start, size_t end) {
        BVHNode* node = new BVHNode();

        // Compute the bounding box of the current set of particles
        for (size_t i = start; i < end; ++i) {
            node->bounds.expand(particleBounds[i]);
        }

        size_t count = end - start;

        if (count == 1) {
            // Leaf node
            node->particleIndex = start;
            return node;
        }

        // Split along the largest axis
        float extentX = node->bounds.maxX - node->bounds.minX;
        float extentY = node->bounds.maxY - node->bounds.minY;
        int axis = (extentX > extentY) ? 0 : 1;

        std::sort(particles.begin() + start, particles.begin() + end,
                  [axis](const Particle& a, const Particle& b) {
                      if (axis == 0) {
                          return a.getPosition().x < b.getPosition().x;
                      } else {
                          return a.getPosition().y < b.getPosition().y;
                      }
                  });

        size_t mid = start + count / 2;

        node->left = buildRecursive(particles, particleBounds, start, mid);
        node->right = buildRecursive(particles, particleBounds, mid, end);

        return node;
    }

    void queryRecursive(BVHNode* node, const AABB& queryBounds, std::vector<int>& result) {
        if (!node || !node->bounds.overlaps(queryBounds)) {
            return;
        }

        if (node->isLeaf()) {
            result.push_back(node->particleIndex);
        } else {
            queryRecursive(node->left, queryBounds, result);
            queryRecursive(node->right, queryBounds, result);
        }
    }

    void deleteTree(BVHNode* node) {
        if (!node) return;
        deleteTree(node->left);
        deleteTree(node->right);
        delete node;
    }
};


#endif