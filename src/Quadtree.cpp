#include "QuadtreeNode.h"
#include <omp.h>
#include <cmath>

// Function prototypes
void buildQuadtree(QuadtreeNode* node, int depth);
void computeMultipole(QuadtreeNode* node);
void evaluateInteractions(QuadtreeNode* node);
MultipoleExpansion computeLeafMultipole(const std::vector<Particle2D>& particles);
void aggregateMultipoles(QuadtreeNode* parent, QuadtreeNode* child);
void evaluateFarFieldInteractions(QuadtreeNode* node);
void evaluateNearFieldInteraction(const Particle2D& p1, const Particle2D& p2);

// Function definitions

// Recursively builds the quadtree
void buildQuadtree(QuadtreeNode* node, int depth) {
    if (depth == 0 || node->particles.size() <= 1) return;

    // Create the children nodes
    for (int i = 0; i < 4; ++i) {
        node->children[i] = new QuadtreeNode();
    }

    // Assign particles to corresponding child nodes
    for (auto& particle : node->particles) {
        // Determine which child the particle belongs to and add it to that child
        int childIndex = 0;
        if (particle.x > node->centerX) childIndex += 1;
        if (particle.y > node->centerY) childIndex += 2;
        node->children[childIndex]->particles.push_back(particle);
    }

    // Clear particles from the current node as they are now in children
    node->particles.clear();

    // Recursively build the quadtree for each child node in parallel
    #pragma omp parallel for
    for (int i = 0; i < 4; ++i) {
        buildQuadtree(node->children[i], depth - 1);
    }
}

// Recursively computes multipole expansions for the nodes
void computeMultipole(QuadtreeNode* node) {
    if (node->children[0] != nullptr) {
        // Recursively compute multipole expansions for child nodes in parallel
        #pragma omp parallel for
        for (int i = 0; i < 4; ++i) {
            computeMultipole(node->children[i]);
        }

        // Aggregate child's multipole expansions
        for (int i = 0; i < 4; ++i) {
            aggregateMultipoles(node, node->children[i]);
        }
    } else {
        // Compute multipole expansion for leaf node
        node->multipole = computeLeafMultipole(node->particles);
    }
}

// Evaluates interactions between particles using the quadtree
void evaluateInteractions(QuadtreeNode* node) {
    if (node->children[0] != nullptr) {
        #pragma omp parallel for
        for (int i = 0; i < 4; ++i) {
            evaluateInteractions(node->children[i]);
        }

        // Evaluate far-field interactions using multipole expansions
        evaluateFarFieldInteractions(node);
    }

    // Evaluate near-field interactions directly
    for (size_t i = 0; i < node->particles.size(); ++i) {
        for (size_t j = i + 1; j < node->particles.size(); ++j) {
            evaluateNearFieldInteraction(node->particles[i], node->particles[j]);
        }
    }
}

// Computes multipole expansion for a leaf node
MultipoleExpansion computeLeafMultipole(const std::vector<Particle2D>& particles) {
    MultipoleExpansion multipole;
    multipole.mass = 0;
    multipole.centerX = 0;
    multipole.centerY = 0;

    // Simple center of mass calculation
    for (const auto& particle : particles) {
        multipole.mass += particle.mass;
        multipole.centerX += particle.mass * particle.x;
        multipole.centerY += particle.mass * particle.y;
    }

    if (multipole.mass != 0) {
        multipole.centerX /= multipole.mass;
        multipole.centerY /= multipole.mass;
    }

    return multipole;
}

void aggregateMultipoles(QuadtreeNode* parent, QuadtreeNode* child) {
    parent->multipole.mass += child->multipole.mass;
    parent->multipole.centerX += child->multipole.mass * child->multipole.centerX;
    parent->multipole.centerY += child->multipole.mass * child->multipole.centerY;

    if (parent->multipole.mass != 0) {
        parent->multipole.centerX /= parent->multipole.mass;
        parent->multipole.centerY /= parent->multipole.mass;
    }
}

// Evaluates far-field interactions using multipole expansions
void evaluateFarFieldInteractions(QuadtreeNode* node) {
    // Placeholder logic for evaluating far-field interactions
    // For simplicity, this is left as a placeholder. In a real application,
    // you would use the multipole expansion terms to approximate interactions.
}

// Evaluates near-field interactions directly between two particles
void evaluateNearFieldInteraction(const Particle2D& p1, const Particle2D& p2) {
    // Example force calculation between two particles
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    double distance = std::sqrt(dx * dx + dy * dy);

    if (distance > 0) {
        double force = (p1.mass * p2.mass) / (distance * distance);
        // Apply the force to the particles (this is a simple example)
    }
}
