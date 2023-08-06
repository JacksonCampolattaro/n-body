//
// Created by Jackson Campolattaro on 8/6/23.
//

#include <spdlog/spdlog.h>
#include <spdlog/spdlog.h>

int main(int argc, char *argv[]) {
    spdlog::set_level(spdlog::level::info);

    std::ofstream out{argc > 1 ? std::string{argv[1]} : "benchmarks/all-solvers-agora-data.csv"};
    out << "Solver,N,Theta,Time\n";

    //runFastTests<QuadrupoleImplicitReverseBarnesHutSolver<Gravity>>("RBH",  out);
    runFastTests<QuadrupoleImplicitMVDRSolver<Gravity>>("MVDR-2",  out);
    runFastTests<OctupoleImplicitMVDRSolver<Gravity>>("MVDR-3", out);
    runFastTests<HexadecupoleImplicitMVDRSolver<Gravity>>("MVDR-4", out);
    runFastTests<QuadrupoleImplicitFMMSolver<Gravity>>("FMM-2",  out);
    runFastTests<OctupoleImplicitFMMSolver<Gravity>>("FMM-3",  out);
    runFastTests<QuadrupoleLinearBVHSolver<Gravity>>("LBVH-BH",  out);
    runFastTests<QuadrupoleBarnesHutSolver<Gravity>>("BH",  out);
    //runFastTests<QuadrupoleImplicitLinearBVHFMMSolver<Gravity>>("LBVH-FMM",  out);

}
