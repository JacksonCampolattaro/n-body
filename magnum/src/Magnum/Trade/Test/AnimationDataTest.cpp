/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include <sstream>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/Utility/DebugStl.h>

#include "Magnum/Math/Quaternion.h"
#include "Magnum/Trade/AnimationData.h"

namespace Magnum { namespace Trade { namespace Test { namespace {

struct AnimationDataTest: TestSuite::Tester {
    explicit AnimationDataTest();

    void construct();
    void constructImplicitDuration();
    void constructImplicitDurationEmpty();
    void constructCopy();
    void constructMove();

    void constructTrackDataDefault();

    void trackCustomResultType();

    void trackWrongIndex();
    void trackWrongType();
    void trackWrongResultType();

    void debugAnimationTrackType();
    void debugAnimationTrackTargetType();
};

AnimationDataTest::AnimationDataTest() {
    addTests({&AnimationDataTest::construct,
              &AnimationDataTest::constructImplicitDuration,
              &AnimationDataTest::constructImplicitDurationEmpty,
              &AnimationDataTest::constructCopy,
              &AnimationDataTest::constructMove,

              &AnimationDataTest::constructTrackDataDefault,

              &AnimationDataTest::trackCustomResultType,

              &AnimationDataTest::trackWrongIndex,
              &AnimationDataTest::trackWrongType,
              &AnimationDataTest::trackWrongResultType,

              &AnimationDataTest::debugAnimationTrackType,
              &AnimationDataTest::debugAnimationTrackTargetType});
}

using namespace Math::Literals;

void AnimationDataTest::construct() {
    /* Ain't the prettiest, but trust me: you won't do it like this in the
       plugins anyway */
    struct Data {
        Float time;
        Vector3 position;
        Quaternion rotation;
    };
    Containers::Array<char> buffer{sizeof(Data)*3};
    auto view = Containers::arrayCast<Data>(buffer);
    view[0] = {0.0f, {3.0f, 1.0f, 0.1f}, Quaternion::rotation(45.0_degf, Vector3::yAxis())};
    view[1] = {5.0f, {0.3f, 0.6f, 1.0f}, Quaternion::rotation(20.0_degf, Vector3::yAxis())};
    view[2] = {7.5f, {1.0f, 0.3f, 2.1f}, Quaternion{}};

    const int state = 5;
    AnimationData data{std::move(buffer), Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Vector3,
         AnimationTrackTargetType::Translation3D, 42,
         Animation::TrackView<Float, Vector3>{
            {view, &view[0].time, view.size(), sizeof(Data)},
            {view, &view[0].position, view.size(), sizeof(Data)},
            Animation::Interpolation::Constant,
            animationInterpolatorFor<Vector3>(Animation::Interpolation::Constant)}},
        {AnimationTrackType::Quaternion,
         AnimationTrackTargetType::Rotation3D, 1337,
         Animation::TrackView<Float, Quaternion>{
            {view, &view[0].time, view.size(), sizeof(Data)},
            {view, &view[0].rotation, view.size(), sizeof(Data)},
            Animation::Interpolation::Linear,
            animationInterpolatorFor<Quaternion>(Animation::Interpolation::Linear)}}
        }}, {-1.0f, 7.0f}, &state};

    CORRADE_COMPARE(data.duration(), (Range1D{-1.0f, 7.0f}));
    CORRADE_COMPARE(data.data().size(), sizeof(Data)*3);
    CORRADE_COMPARE(data.trackCount(), 2);
    CORRADE_COMPARE(data.importerState(), &state);

    {
        CORRADE_COMPARE(data.trackType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(data.trackResultType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(data.trackTargetType(0), AnimationTrackTargetType::Translation3D);
        CORRADE_COMPARE(data.trackTarget(0), 42);

        Animation::TrackView<Float, Vector3> track = data.track<Vector3>(0);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Constant);
        CORRADE_COMPARE(track.at(2.5f), (Vector3{3.0f, 1.0f, 0.1f}));
    } {
        CORRADE_COMPARE(data.trackType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(data.trackResultType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(data.trackTargetType(1), AnimationTrackTargetType::Rotation3D);
        CORRADE_COMPARE(data.trackTarget(1), 1337);

        Animation::TrackView<Float, Quaternion> track = data.track<Quaternion>(1);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Linear);
        CORRADE_COMPARE(track.at(2.5f), Quaternion::rotation(32.5_degf, Vector3::yAxis()));
    }
}

void AnimationDataTest::constructImplicitDuration() {
    /* Ain't the prettiest, but trust me: you won't do it like this in the
       plugins anyway */
    struct Data {
        Float time;
        bool value;
    };
    Containers::Array<char> buffer{sizeof(Data)*4};
    auto view = Containers::arrayCast<Data>(buffer);
    view[0] = {1.0f, true};
    view[1] = {5.0f, false};
    view[2] = {3.0f, true};
    view[3] = {7.0f, false};

    const int state = 5;
    AnimationData data{std::move(buffer), Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Bool,
         AnimationTrackTargetType(129), 0,
         Animation::TrackView<Float, bool>{
            {view, &view[0].time, 2, sizeof(Data)},
            {view, &view[0].value, 2, sizeof(Data)},
            Animation::Interpolation::Constant}},
        {AnimationTrackType::Bool,
         AnimationTrackTargetType(130), 1,
         Animation::TrackView<Float, bool>{
            {view, &view[2].time, 2, sizeof(Data)},
            {view, &view[2].value, 2, sizeof(Data)},
            Animation::Interpolation::Linear}}
        }}, &state};

    CORRADE_COMPARE(data.duration(), (Range1D{1.0f, 7.0f}));
    CORRADE_COMPARE(data.trackCount(), 2);
    CORRADE_COMPARE(data.importerState(), &state);
    {
        CORRADE_COMPARE(data.trackType(0), AnimationTrackType::Bool);
        CORRADE_COMPARE(data.trackResultType(0), AnimationTrackType::Bool);
        CORRADE_COMPARE(data.trackTargetType(0), AnimationTrackTargetType(129));
        CORRADE_COMPARE(data.trackTarget(0), 0);

        Animation::TrackView<Float, bool> track = data.track<bool>(0);
        CORRADE_COMPARE(track.duration(), (Range1D{1.0f, 5.0f}));
        CORRADE_COMPARE(track.keys().size(), 2);
        CORRADE_COMPARE(track.values().size(), 2);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Constant);
        CORRADE_COMPARE(track.at(6.0f), false);
    } {
        CORRADE_COMPARE(data.trackType(1), AnimationTrackType::Bool);
        CORRADE_COMPARE(data.trackResultType(1), AnimationTrackType::Bool);
        CORRADE_COMPARE(data.trackTargetType(1), AnimationTrackTargetType(130));
        CORRADE_COMPARE(data.trackTarget(1), 1);

        Animation::TrackView<Float, bool> track = data.track<bool>(1);
        CORRADE_COMPARE(track.duration(), (Range1D{3.0f, 7.0f}));
        CORRADE_COMPARE(track.keys().size(), 2);
        CORRADE_COMPARE(track.values().size(), 2);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Linear);
        CORRADE_COMPARE(track.at(4.5f), true);
    }
}

void AnimationDataTest::constructImplicitDurationEmpty() {
    AnimationData data{nullptr, nullptr};
    CORRADE_COMPARE(data.duration(), Range1D{});
}

void AnimationDataTest::constructCopy() {
    CORRADE_VERIFY(!(std::is_constructible<AnimationData, const AnimationData&>{}));
    CORRADE_VERIFY(!(std::is_assignable<AnimationData, const AnimationData&>{}));
}

void AnimationDataTest::constructMove() {
    /* Ugh and here we go again! */
    struct Data {
        Float time;
        Vector3 position;
        Quaternion rotation;
    };
    Containers::Array<char> buffer{sizeof(Data)*3};
    auto view = Containers::arrayCast<Data>(buffer);
    view[0] = {0.0f, {3.0f, 1.0f, 0.1f}, Quaternion::rotation(45.0_degf, Vector3::yAxis())};
    view[1] = {5.0f, {0.3f, 0.6f, 1.0f}, Quaternion::rotation(20.0_degf, Vector3::yAxis())};
    view[2] = {7.5f, {1.0f, 0.3f, 2.1f}, Quaternion{}};

    const int state = 5;
    AnimationData a{std::move(buffer), Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Vector3,
         AnimationTrackTargetType::Translation3D, 42,
         Animation::TrackView<Float, Vector3>{
            {view, &view[0].time, view.size(), sizeof(Data)},
            {view, &view[0].position, view.size(), sizeof(Data)},
            Animation::Interpolation::Constant,
            animationInterpolatorFor<Vector3>(Animation::Interpolation::Constant)}},
        {AnimationTrackType::Quaternion,
         AnimationTrackTargetType::Rotation3D, 1337,
         Animation::TrackView<Float, Quaternion>{
            {view, &view[0].time, view.size(), sizeof(Data)},
            {view, &view[0].rotation, view.size(), sizeof(Data)},
            Animation::Interpolation::Linear,
            animationInterpolatorFor<Quaternion>(Animation::Interpolation::Linear)}}
        }}, {-1.0f, 7.0f}, &state};

    AnimationData b{std::move(a)};

    CORRADE_COMPARE(b.duration(), (Range1D{-1.0f, 7.0f}));
    CORRADE_COMPARE(b.data().size(), sizeof(Data)*3);
    CORRADE_COMPARE(b.trackCount(), 2);
    CORRADE_COMPARE(b.importerState(), &state);

    {
        CORRADE_COMPARE(b.trackType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(b.trackResultType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(b.trackTargetType(0), AnimationTrackTargetType::Translation3D);
        CORRADE_COMPARE(b.trackTarget(0), 42);

        Animation::TrackView<Float, Vector3> track = b.track<Vector3>(0);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Constant);
        CORRADE_COMPARE(track.at(2.5f), (Vector3{3.0f, 1.0f, 0.1f}));
    } {
        CORRADE_COMPARE(b.trackType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(b.trackResultType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(b.trackTargetType(1), AnimationTrackTargetType::Rotation3D);
        CORRADE_COMPARE(b.trackTarget(1), 1337);

        Animation::TrackView<Float, Quaternion> track = b.track<Quaternion>(1);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Linear);
        CORRADE_COMPARE(track.at(2.5f), Quaternion::rotation(32.5_degf, Vector3::yAxis()));
    }

    int other;
    AnimationData c{nullptr, nullptr, &other};
    c = std::move(b);

    CORRADE_COMPARE(c.duration(), (Range1D{-1.0f, 7.0f}));
    CORRADE_COMPARE(c.data().size(), sizeof(Data)*3);
    CORRADE_COMPARE(c.trackCount(), 2);
    CORRADE_COMPARE(c.importerState(), &state);

    {
        CORRADE_COMPARE(c.trackType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(c.trackResultType(0), AnimationTrackType::Vector3);
        CORRADE_COMPARE(c.trackTargetType(0), AnimationTrackTargetType::Translation3D);
        CORRADE_COMPARE(c.trackTarget(0), 42);

        Animation::TrackView<Float, Vector3> track = c.track<Vector3>(0);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Constant);
        CORRADE_COMPARE(track.at(2.5f), (Vector3{3.0f, 1.0f, 0.1f}));
    } {
        CORRADE_COMPARE(c.trackType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(c.trackResultType(1), AnimationTrackType::Quaternion);
        CORRADE_COMPARE(c.trackTargetType(1), AnimationTrackTargetType::Rotation3D);
        CORRADE_COMPARE(c.trackTarget(1), 1337);

        Animation::TrackView<Float, Quaternion> track = c.track<Quaternion>(1);
        CORRADE_COMPARE(track.keys().size(), 3);
        CORRADE_COMPARE(track.values().size(), 3);
        CORRADE_COMPARE(track.interpolation(), Animation::Interpolation::Linear);
        CORRADE_COMPARE(track.at(2.5f), Quaternion::rotation(32.5_degf, Vector3::yAxis()));
    }

    CORRADE_VERIFY(std::is_nothrow_move_constructible<AnimationData>::value);
    CORRADE_VERIFY(std::is_nothrow_move_assignable<AnimationData>::value);
}

void AnimationDataTest::constructTrackDataDefault() {
    AnimationTrackData data;
    CORRADE_VERIFY(true); /* no public accessors here, so nothing to check */
}

void AnimationDataTest::trackCustomResultType() {
    using namespace Math::Literals;

    struct Data {
        Float time;
        Vector3i position;
    };
    Containers::Array<char> buffer{sizeof(Data)*3};
    auto view = Containers::arrayCast<Data>(buffer);
    view[0] = {0.0f, {300, 100, 10}};
    view[1] = {5.0f, {30, 60, 100}};

    AnimationData data{std::move(buffer), Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Vector3i,
         AnimationTrackType::Vector3,
         AnimationTrackTargetType::Scaling3D, 0,
         Animation::TrackView<Float, Vector3i, Vector3>{
             {view, &view[0].time, view.size(), sizeof(Data)},
             {view, &view[0].position, view.size(), sizeof(Data)},
             [](const Vector3i& a, const Vector3i& b, Float t) -> Vector3 {
                 return Math::lerp(Vector3{a}*0.01f, Vector3{b}*0.01f, t);
             }}}}
    }};

    CORRADE_COMPARE((data.track<Vector3i, Vector3>(0).at(2.5f)), (Vector3{1.65f, 0.8f, 0.55f}));
}

void AnimationDataTest::trackWrongIndex() {
    std::ostringstream out;
    Error redirectError{&out};

    AnimationData data{nullptr, nullptr};
    data.trackType(0);
    data.trackResultType(0);
    data.trackTargetType(0);
    data.trackTarget(0);
    data.track(0);

    CORRADE_COMPARE(out.str(),
        "Trade::AnimationData::trackType(): index out of range\n"
        "Trade::AnimationData::trackResultType(): index out of range\n"
        "Trade::AnimationData::trackTargetType(): index out of range\n"
        "Trade::AnimationData::trackTarget(): index out of range\n"
        "Trade::AnimationData::track(): index out of range\n");
}

void AnimationDataTest::trackWrongType() {
    std::ostringstream out;
    Error redirectError{&out};

    AnimationData data{nullptr, Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Vector3i,
         AnimationTrackType::Vector3,
         AnimationTrackTargetType::Scaling3D, 0, {}}
    }}};

    data.track<Vector3>(0);

    CORRADE_COMPARE(out.str(), "Trade::AnimationData::track(): improper type requested for Trade::AnimationTrackType::Vector3i\n");
}

void AnimationDataTest::trackWrongResultType() {
    std::ostringstream out;
    Error redirectError{&out};

    AnimationData data{nullptr, Containers::Array<AnimationTrackData>{Containers::InPlaceInit, {
        {AnimationTrackType::Vector3i,
         AnimationTrackType::Vector3,
         AnimationTrackTargetType::Scaling3D, 0, {}}
    }}};

    data.track<Vector3i, Vector2>(0);

    CORRADE_COMPARE(out.str(), "Trade::AnimationData::track(): improper result type requested for Trade::AnimationTrackType::Vector3\n");
}

void AnimationDataTest::debugAnimationTrackType() {
    std::ostringstream out;

    Debug{&out} << AnimationTrackType::DualQuaternion << AnimationTrackType(0xde);
    CORRADE_COMPARE(out.str(), "Trade::AnimationTrackType::DualQuaternion Trade::AnimationTrackType(0xde)\n");
}

void AnimationDataTest::debugAnimationTrackTargetType() {
    std::ostringstream out;

    Debug{&out} << AnimationTrackTargetType::Rotation3D << AnimationTrackTargetType(135) << AnimationTrackTargetType(0x42);
    CORRADE_COMPARE(out.str(), "Trade::AnimationTrackTargetType::Rotation3D Trade::AnimationTrackTargetType::Custom(135) Trade::AnimationTrackTargetType(0x42)\n");
}

}}}}

CORRADE_TEST_MAIN(Magnum::Trade::Test::AnimationDataTest)
