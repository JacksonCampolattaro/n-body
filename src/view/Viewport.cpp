//
// Created by jackcamp on 12/26/19.
//

#include "Viewport.h"

using namespace Magnum;

Viewport::Viewport(const Arguments &arguments) :
        Platform::Application(
                arguments,
                Configuration{}.setTitle("MINI 2020")
        ) {


    // Configuring CLI Input
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Setting the name and description
    CLI::App CLIApplication("Modular Integrator for N-body Interaction");

    // Getting the headless option
    bool headless = false;
    CLIApplication.add_flag("--headless",
                            headless,
                            "Runs the program without an interface or graphics"
    );

    // Getting the verbose option
    bool verbose = false;
    CLIApplication.add_flag("-v,--verbose",
                            verbose,
                            "Enables more thorough logging, at the cost of performance"
    );

    // Getting the debug option
    bool debug = false;
    CLIApplication.add_flag("-d,--debug",
                            debug,
                            "Enables logging of information that minimally impacts performance"
    );

    // Getting the quiet option
    bool quiet = false;
    CLIApplication.add_flag("-q,--quiet",
                            quiet,
                            "Disables printing debug information to the console"
    );

    // Getting the quiet option
    bool silent = false;
    CLIApplication.add_flag("-s,--silent",
                            quiet,
                            "Disables printing any information to the console"
    );

    // Getting the logfile path, with a default value
    string logPath = "";
    CLIApplication.add_option("-l,--logfile",
                              logPath,
                              "Sets the path to write log files to",
                              true
    );

    // Getting the body archive path, with a default value
    string bodyArchivePath = "../../../scenarios/blender/blender.bod";
    CLIApplication.add_option("-b,--bodies",
                              bodyArchivePath,
                              "Sets the file path to read bodies from",
                              true
    )->check(CLI::ExistingFile);

    // Getting the physics archive path, with a default value
    string physicsArchivePath = "../../../scenarios/blender/blender.phys";
    CLIApplication.add_option("-p,--physics",
                              physicsArchivePath,
                              "Sets the file path to read physics from",
                              true
    )->check(CLI::ExistingFile);

    // Setting the number of cycles to calculate
    unsigned int cycles = 100;
    CLIApplication.add_option("-c,--cycles",
                              cycles,
                              "Sets the number of calculation cycles to perform",
                              true
    );

    // Interpreting the input using the CLI macro
    CLIApplication.parse(arguments.argc, arguments.argv);


    // Configuring logging
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Create the log distributor, which will pass output to different sinks
    auto logDistributor = make_shared<spdlog::sinks::dist_sink_st>();

    // Create a logger using the distributor as the sink
    auto logger = make_shared<spdlog::logger>("log", logDistributor);
    spdlog::register_logger(logger);

    // Set the default level of the logger to debug
    logger->set_level(spdlog::level::info);

    // Setting verbosity level
    if (debug)
        logger->set_level(spdlog::level::debug);
    if (verbose)
        logger->set_level(spdlog::level::trace);
    if (quiet)
        logger->set_level(spdlog::level::warn);

    // Attaching the console as a sink
    if (!silent) {
        logDistributor->add_sink(make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logger->log(spdlog::level::debug, "Sink console:stdout attached to logger");
    }

    // Attaching the logfile as another sink
    if (!logPath.empty()) {
        logDistributor->add_sink(make_shared<spdlog::sinks::basic_file_sink_mt>(logPath));
        logger->log(spdlog::level::debug, "Sink file:\"{}\" attached to logger", logPath);
    }

    // Loading from XML files
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Loading Bodies
    std::ifstream bodiesInputStream(bodyArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load body file at path \"{}\"", bodyArchivePath);
    }
    cereal::XMLInputArchive bodiesInputArchive(bodiesInputStream);
    auto bodies = BodyList();
    bodiesInputArchive(bodies);

    // Loading Physics
    std::ifstream physicsInputStream(physicsArchivePath);
    if (!bodiesInputStream.is_open()) {
        logger->error("Failed to load physics file at path \"{}\"", physicsArchivePath);
    }
    cereal::XMLInputArchive physicsInputArchive(physicsInputStream);
    auto physics = PhysicsContext();
    physicsInputArchive(physics);



    // Adding a sphere to the view
    ////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    GL::Renderer::enable(GL::Renderer::Feature::DepthTest);
    GL::Renderer::enable(GL::Renderer::Feature::FaceCulling);

    const Trade::MeshData3D sphere = Primitives::uvSphereSolid(50, 50);

    GL::Buffer vertices;
    vertices.setData(MeshTools::interleave(sphere.positions(0), sphere.normals(0)));

    Containers::Array<char> indexData;
    MeshIndexType indexType;
    UnsignedInt indexStart, indexEnd;
    std::tie(indexData, indexType, indexStart, indexEnd) =
            MeshTools::compressIndices(sphere.indices());
    GL::Buffer indices;
    indices.setData(indexData);

    _mesh.setPrimitive(sphere.primitive())
            .setCount(sphere.indices().size())
            .addVertexBuffer(std::move(vertices), 0, Shaders::Phong::Position{},
                             Shaders::Phong::Normal{})
            .setIndexBuffer(std::move(indices), 0, indexType, indexStart, indexEnd);

    _transformation =
            Matrix4::rotationX(Math::Deg<float>(30.0)) *
            Matrix4::rotationY(Math::Deg<float>(40.0));

    _projection =
            Matrix4::perspectiveProjection(
                    Math::Deg<float>(35.0), Vector2{windowSize()}.aspectRatio(), 0.01f, 100.0f) *
            Matrix4::translation(Vector3::zAxis(-10.0f));

    _color = Color3::fromHsv({Math::Deg<float>(35.0), 1.0f, 1.0f});
}

void Viewport::drawEvent() {

    GL::defaultFramebuffer.clear(
            GL::FramebufferClear::Color | GL::FramebufferClear::Depth);

    _shader.setLightPosition({7.0f, 5.0f, 2.5f})
            .setLightColor(Color3{1.0f})
            .setDiffuseColor(_color)
            .setAmbientColor(Color3::fromHsv({_color.hue(), 1.0f, 0.3f}))
            .setTransformationMatrix(_transformation)
            .setNormalMatrix(_transformation.rotationNormalized())
            .setProjectionMatrix(_projection);

    _mesh.draw(_shader);

    swapBuffers();
}
