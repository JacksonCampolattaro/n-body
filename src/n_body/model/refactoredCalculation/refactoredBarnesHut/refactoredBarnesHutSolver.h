//
// Created by jackcamp on 5/30/19.
//

#ifndef N_BODY_REFACTOREDBARNESHUTSOLVER_H
#define N_BODY_REFACTOREDBARNESHUTSOLVER_H


#include "../refactoredSolver.h"
#include "../../calculation/BarnesHut/Octant.h"

class refactoredBarnesHutSolver : public refactoredSolver {

public:

    refactoredBarnesHutSolver() = default;


    void build(std::vector<Body *> bodies, PhysicsContext *physicsContext) override;


    void kick(Body *subjectBody) override;


    /**
     * Sets the accuracy of the calculation (lower is more accurate, 0 degenerates to naive)
     * @param theta Ratio defining how far any group of bodies must be before they can be grouped together
     * @return This solver, for use in chaining named parameters.
     */
    refactoredBarnesHutSolver *setTheta(float theta);


private:

    /*The tree itself is contained in a smart pointer*/
    std::unique_ptr<Octant> octree;

    /*Constant determining the accuracy of the traversal*/
    float theta = 1;

    /*Size of the top level node*/
    float sideLength = 100000;

    /*Best position for the center of the octree, starts at <0, 0, 0>, but changes each cycle*/
    glm::vec3 idealTreeCenterLocation = glm::vec3(0, 0, 0); /*Placed to divide bodies evenly between its subnodes*/


    std::unique_ptr<Octant> generateOctree();

};


#endif //N_BODY_REFACTOREDBARNESHUTSOLVER_H
