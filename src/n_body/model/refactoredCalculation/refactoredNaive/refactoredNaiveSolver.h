//
// Created by jackcamp on 5/30/19.
//

#ifndef N_BODY_REFACTOREDNAIVESOLVER_H
#define N_BODY_REFACTOREDNAIVESOLVER_H


#include "../refactoredSolver.h"

class refactoredNaiveSolver : public refactoredSolver {

public:

    refactoredNaiveSolver() = default;


    void build(std::vector<Body *> bodies, PhysicsContext *physicsContext) override;


    void kick(Body *subjectBody) override;


};


#endif //N_BODY_REFACTOREDNAIVESOLVER_H
