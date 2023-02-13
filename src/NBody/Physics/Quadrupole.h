//
// Created by Jackson Campolattaro on 2/13/23.
//

#ifndef N_BODY_QUADRUPOLE_H
#define N_BODY_QUADRUPOLE_H

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


}

#endif //N_BODY_QUADRUPOLE_H
