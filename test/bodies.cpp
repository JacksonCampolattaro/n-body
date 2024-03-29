
#include <NBody/Simulation/Simulation.h>
#include <fstream>
#include <filesystem>

using namespace NBody;

void cubicGrid(Simulation &simulation,
               Physics::Position corner, glm::ivec3 size, float spacing,
               Physics::Velocity velocity, float mass) {

    glm::vec3 fsize = size;

    for (int x = 0; x < size.x; ++x) {
        for (int y = 0; y < size.y; ++y) {
            for (int z = 0; z < size.z; ++z) {

                Physics::Position position{
                        corner +
                        (Physics::Position{static_cast<float>(x), static_cast<float>(y), static_cast<float>(z)}
                         * spacing)
                };

                Graphics::Color color{
                        y / fsize.y + x / fsize.x,
                        x / fsize.x,
                        1.0f - y / fsize.y + x / fsize.x
                };

                simulation.newParticle()
                        .setPosition(position)
                        .setVelocity(velocity)
                        .setMass({mass})
                        .setColor(color)
                        .setSphere({2.0f});
            }
        }
    }
}

void threeBody(Simulation &simulation) {

    simulation.newParticle()
            .setPosition({0.0, 5.0, 5.0})
            .setVelocity({0, -0.1, 0})
            .setColor({0.8, 0.8, 0.8})
            .setSphere({1.0})
            .setMass({0.5});
    simulation.newParticle()
            .setPosition({-5, 0, 0})
            .setVelocity({0, 0.15, 0})
            .setColor({0.8, 0.8, 0.0})
            .setSphere({1.0})
            .setMass({0.5});
    simulation.newParticle()
            .setPosition({5, 0, 0})
            .setVelocity({0, -0.05, 0})
            .setColor({0.8, 0.0, 0.0})
            .setSphere({1.0})
            .setMass({0.5});
}

void galaxy(Simulation &simulation) {

//    simulation._rule._gravitationalConstant = 0.01;
//    simulation._rule._timeIncrement = 0.001;

    cubicGrid(simulation,
              {-50, -150, -400},
              {5, 5, 500}, 10,
              {60, 20, 0}, 5000);

    simulation.newParticle()
            .setPosition({0, 0, -500})
            .setVelocity({0, 0, 0})
            .setColor({0.9, 0.9, 0.9})
            .setSphere({16.0})
            .setMass(190000000.0f);
}

void blender(Simulation &simulation) {

//    simulation._rule._gravitationalConstant = 0.01;
//    simulation._rule._timeIncrement = 0.05;

    cubicGrid(simulation,
              {-50, -100, -450},
              {5, 5, 100},10,
              {0.8, 0.25, 0}, 5000);

    simulation.newParticle()
            .setPosition({-60, 0, -500})
            .setVelocity({0, -90, 0})
            .setColor({0.9, 0.9, 0.9})
            .setSphere({16.0})
            .setMass({180000000});
    simulation.newParticle()
            .setPosition({60, 0, -500})
            .setVelocity({0, 90, 0})
            .setColor({0.9, 0.9, 0.9})
            .setSphere({16.0})
            .setMass({180000000});
}

int main(int argc, char **argv) {

    // Set the directory to place the files in
    std::filesystem::path directory;
    if (argc > 1) directory.assign(argv[1]);

    // Generate a cubic grid scenario
    {
        std::ofstream file((directory / "cubicgrid.json").native());
        if (!file.is_open()) return EXIT_FAILURE;
        Simulation simulation;
        cubicGrid(simulation, {0, 0, 0}, {3, 3, 3}, 10.0, {0, 0, 0}, 1.0);
        file << simulation << std::endl;
        file.close();
    }

    // Generate a three body scenario
    {
        std::ofstream file((directory / "threebody.json").native());
        if (!file.is_open()) return EXIT_FAILURE;
        Simulation simulation;
        threeBody(simulation);
        file << simulation << std::endl;
        file.close();
    }

    // Generate a galaxy scenario
    {
        std::ofstream file((directory / "galaxy.json").native());
        if (!file.is_open()) return EXIT_FAILURE;
        Simulation simulation;
        galaxy(simulation);
        file << simulation << std::endl;
        file.close();
    }

    // Generate a blender scenario
    {
        std::ofstream file((directory / "blender.json").native());
        if (!file.is_open()) return EXIT_FAILURE;
        Simulation simulation;
        blender(simulation);
        file << simulation << std::endl;
        file.close();
    }
}