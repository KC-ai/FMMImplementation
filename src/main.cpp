#include "Particle2D.h"
#include "QuadtreeNode.h"
#include <vector>
#include <omp.h>

// Function prototypes
void buildQuadtree(QuadtreeNode* node, int depth);
void computeMultipole(QuadtreeNode* node);
void evaluateInteractions(QuadtreeNode* node);

int main() {
    // Initialize particles
    std::vector<Particle2D> particles = {
        {0.1, 0.2, 1.0},
        {0.3, 0.4, 2.0},
        // Add more particles as needed
    };

    // Create root node and assign particles
    QuadtreeNode* root = new QuadtreeNode();
    root->particles = particles;
    root->centerX = 0.5; // Set center of the root node (example value)
    root->centerY = 0.5;

    // Build quadtree in parallel
    #pragma omp parallel
    {
        #pragma omp single
        buildQuadtree(root, 10); // Example depth
    }

    // Compute multipole expansions in parallel
    #pragma omp parallel
    {
        #pragma omp single
        computeMultipole(root);
    }

    // Evaluate interactions in parallel
    #pragma omp parallel
    {
        #pragma omp single
        evaluateInteractions(root);
    }

    // Clean up and exit
    delete root;
    return 0;
}
