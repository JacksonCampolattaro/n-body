//
// Created by Jackson Campolattaro on 2/13/23.
//

#ifndef N_BODY_QUADRUPOLE_H
#define N_BODY_QUADRUPOLE_H

#include <glm/glm.hpp>

namespace NBody {

    // todo: there must be some way to generate this automatically, right?
    class Quadrupole {
    private:

        float _xx = 0.0f;
        float _xy = 0.0f;
        float _xz = 0.0f;
        float _yy = 0.0f;
        float _yz = 0.0f;
        float _zz = 0.0f;

    public:

        Quadrupole() = default;

        Quadrupole(const float &xx,
                   const float &xy,
                   const float &xz,
                   const float &yy,
                   const float &yz,
                   const float &zz) : _xx(xx), _xy(xy), _xz(xz), _yy(yy), _yz(yz), _zz(zz) {}

        Quadrupole(const glm::mat3 &outerProduct) {
            xx() = outerProduct[0].x;
            xy() = outerProduct[0].y;
            xz() = outerProduct[0].z;
            yy() = outerProduct[1].y;
            yz() = outerProduct[1].z;
            zz() = outerProduct[2].z;
        }

        Quadrupole &operator+=(const Quadrupole &right) {
            xx() += right.xx();
            xy() += right.xy();
            xz() += right.xz();
            yy() += right.yy();
            yz() += right.yz();
            zz() += right.zz();
            return *this;
        }

        float &xx() { return _xx; }

        float &xy() { return _xy; }

        float &xz() { return _xz; }

        float &yx() { return _xy; }

        float &yy() { return _yy; }

        float &yz() { return _yz; }

        float &zx() { return _xz; }

        float &zy() { return _yz; }

        float &zz() { return _zz; }

        [[nodiscard]] const float &xx() const { return _xx; }

        [[nodiscard]] const float &xy() const { return _xy; }

        [[nodiscard]] const float &xz() const { return _xz; }

        [[nodiscard]] const float &yx() const { return _xy; }

        [[nodiscard]] const float &yy() const { return _yy; }

        [[nodiscard]] const float &yz() const { return _yz; }

        [[nodiscard]] const float &zx() const { return _xz; }

        [[nodiscard]] const float &zy() const { return _yz; }

        [[nodiscard]] const float &zz() const { return _zz; }
    };

    inline Quadrupole operator*(const Quadrupole &q, const float &v) {
        // Equivalent to vector-matrix multiplication
        return {
                q.xx() * v,
                q.xy() * v,
                q.xz() * v,
                q.yy() * v,
                q.yz() * v,
                q.zz() * v
        };
    }

    inline glm::vec3 operator*(const Quadrupole &q, const glm::vec3 &v) {
        // Equivalent to vector-matrix multiplication
        return {
                q.xx() * v.x + q.xy() * v.y + q.xz() * v.z,
                q.yx() * v.x + q.yy() * v.y + q.yz() * v.z,
                q.zx() * v.x + q.zy() * v.y + q.zz() * v.z
        };
    }

}

#endif //N_BODY_QUADRUPOLE_H
