//
// Created by jackcamp on 4/10/19.
//

#ifndef N_BODY_BARNESHUTSOLVER_H
#define N_BODY_BARNESHUTSOLVER_H


#include "../Solver.h"


class BarnesHutSolver : public Solver {

public:

    void solve(std::vector<Body *> bodies, PhysicsContext *phys) override;

    /**
     * Sets the accuracy of the calculation (lower is more accurate, 0 degenerates to naive)
     * @param theta Ratio defining how far any group of bodies must be before they can be grouped together
     * @return This solver, for use in chaining named parameters.
     */
    BarnesHutSolver *setTheta(float theta);

private:

    /*Constant determining the accuracy of the traversal*/
    float theta = 1;

    /*Size of the top level node*/
    float sideLength = 100000;

    /*Best position for the center of the octree, starts at <0, 0, 0>, but changes each cycle*/
    glm::vec3 idealTreeCenterLocation = glm::vec3(0, 0, 0); /*Placed to divide bodies evenly between its subnodes*/

};


#endif //N_BODY_BARNESHUTSOLVER_H
