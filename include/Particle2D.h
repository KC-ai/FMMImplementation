#ifndef PARTICLE2D_H
#define PARTICLE2D_H

struct Particle2D {
    double x, y;
    double charge;
};

#endif 

//here, we're initializing our particle, since the FMM covers coulombic charge interactions, we're using a 2D particle first with the x and y vectors and a charge 
