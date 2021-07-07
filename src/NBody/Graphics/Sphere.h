//
// Created by jackcamp on 7/6/21.
//

#ifndef N_BODY_SPHERE_H
#define N_BODY_SPHERE_H

#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Trade/MeshData.h>

#include <nlohmann/json.hpp>

using namespace Magnum;
using nlohmann::json;

namespace NBody::Graphics {

    class Sphere {
    private:

        float _radius = 1.0f;

    public:

        [[nodiscard]] const float &radius() const { return _radius; }

        float &radius() { return _radius; }

        static GL::Mesh mesh();

        friend void to_json(json &j, const Sphere &s);

        friend void from_json(const json &j, Sphere &s);

    };

    void to_json(json &j, const Sphere &s);

    void from_json(const json &j, Sphere &s);

}

#endif //N_BODY_SPHERE_H
