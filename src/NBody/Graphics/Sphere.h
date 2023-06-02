//
// Created by jackcamp on 7/6/21.
//

#ifndef N_BODY_SPHERE_H
#define N_BODY_SPHERE_H

#include <NBody/Graphics/Color.h>

#include <Magnum/GL/Mesh.h>
#include <Magnum/MeshTools/Compile.h>
#include <Magnum/Primitives/Icosphere.h>
#include <Magnum/Trade/MeshData.h>

#include <nlohmann/json.hpp>
#include <Magnum/Math/Matrix4.h>

using namespace Magnum;
using nlohmann::json;

namespace NBody::Graphics {

    class Sphere {
    private:

        float _radius = 1.0f;

    public:

        Sphere(float radius = 1.0f) : _radius(radius) {}

        [[nodiscard]] const float &radius() const { return _radius; }

        float &radius() { return _radius; }

        static GL::Mesh mesh();

        friend void to_json(json &j, const Sphere &s);

        friend void from_json(const json &j, Sphere &s);

        friend auto operator<=>(const Sphere &lhs, const Sphere &rhs) = default;

    public:

        struct InstanceData {
            Matrix4 transformationMatrix;
            Matrix3x3 normalMatrix;
            Color color;
        };

    };

    void to_json(json &j, const Sphere &s);

    void from_json(const json &j, Sphere &s);

}

#endif //N_BODY_SPHERE_H
