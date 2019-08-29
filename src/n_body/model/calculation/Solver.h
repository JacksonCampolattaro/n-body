//
// Created by jackcamp on 4/5/19.
//

#ifndef N_BODY_SOLVER_H
#define N_BODY_SOLVER_H

#include "../Body.h" // The Solver contains bodies
#include "../PhysicsContext.h" // The solver affects the bodies according to the physicsContext context it's given

#include <vector> // Used to handle the list of bodies in the Simulation
#include <csignal>

class Solver {

public:

    /**
     * Used to enable multithreaded workloads via openmp
     * @param enabled whether or not multithreading should be allowed
     * @return This solver, for use in chaining named parameters.
     */
    Solver *enableThreading(bool enabled = true);

    /**
     * Calculates forces between bodies and applies them to each body
     * @param bodies the list of bodies to perform calculations on
     * @param phys the physics rules used by the simulation
     */
    virtual void solve(std::vector<Body> *bodies, PhysicsContext *phys) = 0;

    bool isThreadingEnabled() const;

    /**
     * Signal for the preparation of the solver
     *//*
    typedef sigc::signal<void> type_signal_preparing_solver;
    type_signal_preparing_solver signal_preparing_solver(); // Accessor for the signal

    *//**
     * Signal for the preparation of the solver
     *//*
    typedef sigc::signal<void> type_signal_solving;
    type_signal_solving signal_solving(); // Accessor for the signal

    *//**
     * Signal for the shift of body buffers
     *//*
    typedef sigc::signal<void> type_signal_shifting_buffers;
    type_signal_shifting_buffers signal_shifting_buffers(); // Accessor for the signal

    *//**
     * Signal for the completion of calculations
     *//*
    typedef sigc::signal<void> type_signal_complete;
    type_signal_complete signal_complete(); // Accessor for the signal*/

protected:

    /*type_signal_preparing_solver m_signal_preparing_solver;
    type_signal_solving m_signal_solving;
    type_signal_shifting_buffers m_signal_shifting_buffers;
    type_signal_complete m_signal_complete;*/

    /*Flag enabling multithreading of workloads*/
    bool threadingEnabled = false;

};


#endif //N_BODY_SOLVER_H
