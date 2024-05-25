#ifndef QUADTREENODE_H
#define QUADTREENODE_H

#include <vector>
#include "Particle2D.h"

struct MultipoleExpansion {
    double mass;
    double centerX, centerY;
};

struct QuadtreeNode {
    std::vector<Particle2D> particles;
    QuadtreeNode* children[4] = {nullptr}; // Quadtree structure
    MultipoleExpansion multipole;
    double centerX, centerY; // Center of the node for partitioning
};

#endif // QUADTREENODE_H
