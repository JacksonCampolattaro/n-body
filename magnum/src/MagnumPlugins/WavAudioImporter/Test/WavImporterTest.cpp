/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019
              Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2016 Alice Margatroid <loveoverwhelming@gmail.com>

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
#include <Corrade/Containers/Array.h>
#include <Corrade/TestSuite/Tester.h>
#include <Corrade/TestSuite/Compare/Container.h>
#include <Corrade/Utility/DebugStl.h>
#include <Corrade/Utility/Directory.h>

#include "Magnum/Audio/AbstractImporter.h"

#include "configure.h"

namespace Magnum { namespace Audio { namespace Test { namespace {

struct WavImporterTest: TestSuite::Tester {
    explicit WavImporterTest();

    void empty();
    void wrongSignature();
    void unsupportedFormat();
    void unsupportedChannelCount();

    void invalidPadding();
    void invalidLength();
    void invalidDataChunk();
    void invalidFactChunk();

    void zeroSamples();

    void mono4();
    void mono8();
    void mono8junk();
    void mono8ALaw();
    void mono8MuLaw();
    void mono16();

    void stereo4();
    void stereo8();
    void stereo8ALaw();
    void stereo8MuLaw();
    void stereo12();
    void stereo16();
    void stereo24();
    void stereo32();

    void mono32f();
    void stereo32f();
    void stereo64f();

    void surround51Channel16();
    void surround71Channel24();

    /* Explicitly forbid system-wide plugin dependencies */
    PluginManager::Manager<AbstractImporter> _manager{"nonexistent"};
};

WavImporterTest::WavImporterTest() {
    addTests({&WavImporterTest::empty,
              &WavImporterTest::wrongSignature,
              &WavImporterTest::unsupportedFormat,
              &WavImporterTest::unsupportedChannelCount,

              &WavImporterTest::invalidPadding,
              &WavImporterTest::invalidLength,
              &WavImporterTest::invalidDataChunk,
              &WavImporterTest::invalidFactChunk,

              &WavImporterTest::zeroSamples,

              &WavImporterTest::mono4,
              &WavImporterTest::mono8,
              &WavImporterTest::mono8junk,
              &WavImporterTest::mono8ALaw,
              &WavImporterTest::mono8MuLaw,
              &WavImporterTest::mono16,

              &WavImporterTest::stereo4,
              &WavImporterTest::stereo8,
              &WavImporterTest::stereo8ALaw,
              &WavImporterTest::stereo8MuLaw,
              &WavImporterTest::stereo12,
              &WavImporterTest::stereo16,
              &WavImporterTest::stereo24,
              &WavImporterTest::stereo32,

              &WavImporterTest::mono32f,
              &WavImporterTest::stereo32f,
              &WavImporterTest::stereo64f,

              &WavImporterTest::surround51Channel16,
              &WavImporterTest::surround71Channel24});

    /* Load the plugin directly from the build tree. Otherwise it's static and
       already loaded. */
    #ifdef WAVAUDIOIMPORTER_PLUGIN_FILENAME
    CORRADE_INTERNAL_ASSERT(_manager.load(WAVAUDIOIMPORTER_PLUGIN_FILENAME) & PluginManager::LoadState::Loaded);
    #endif
}

void WavImporterTest::empty() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");

    std::ostringstream out;
    Error redirectError{&out};
    char a{};
    /* Explicitly checking non-null but empty view */
    CORRADE_VERIFY(!importer->openData({&a, 0}));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): the file is too short: 0 bytes\n");
}

void WavImporterTest::wrongSignature() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "wrongSignature.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): the file signature is invalid\n");
}

void WavImporterTest::unsupportedFormat() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "unsupportedFormat.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): unsupported format Audio::WavAudioFormat::AdPcm\n");
}

void WavImporterTest::unsupportedChannelCount() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "unsupportedChannelCount.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): PCM with unsupported channel count 6 with 8 bits per sample\n");
}

void WavImporterTest::invalidPadding() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "invalidPadding.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): the file has improper size, expected 66 but got 73\n");
}

void WavImporterTest::invalidLength() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "invalidLength.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): the file has improper size, expected 160844 but got 80444\n");
}

void WavImporterTest::invalidDataChunk() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "invalidDataChunk.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): the file contains no data chunk\n");
}

void WavImporterTest::invalidFactChunk() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "invalidFactChunk.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Mono16);
    CORRADE_COMPARE(importer->frequency(), 22050);

    CORRADE_COMPARE(importer->data().size(), 3724);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            -27, -11, -1, -9, 24, -6, 127, -5}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::zeroSamples() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");

    /* No error should happen, it should just give an empty buffer back */
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "zeroSamples.wav")));
    CORRADE_COMPARE(importer->format(), BufferFormat::Mono16);
    CORRADE_COMPARE(importer->frequency(), 22050);
    CORRADE_VERIFY(importer->data().empty());
}

void WavImporterTest::mono4() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono4.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): unsupported format Audio::WavAudioFormat::AdPcm\n");
}

void WavImporterTest::mono8() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono8.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Mono8);
    CORRADE_COMPARE(importer->frequency(), 22050);

    CORRADE_COMPARE(importer->data().size(), 2136);
    CORRADE_COMPARE_AS(importer->data().prefix(4),
        (Containers::Array<char>{Containers::InPlaceInit, {127, 127, 127, 127}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::mono8junk() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono8junk.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Mono8);
    CORRADE_COMPARE(importer->frequency(), 22050);

    CORRADE_COMPARE_AS(importer->data().prefix(4),
        (Containers::Array<char>{Containers::InPlaceInit, {127, 127, 127, 127}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::mono8ALaw() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono8ALaw.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::MonoALaw);
    CORRADE_COMPARE(importer->frequency(), 8000);

    CORRADE_COMPARE(importer->data().size(), 4096);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            87, 84, 85, 85, 85, -43, -43, -43}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::mono8MuLaw() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono8MuLaw.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::MonoMuLaw);
    CORRADE_COMPARE(importer->frequency(), 8000);

    CORRADE_COMPARE(importer->data().size(), 4096);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            -5, -3, -1, -2, -1, 127, 127, 126}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::mono16() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono16.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Mono16);
    CORRADE_COMPARE(importer->frequency(), 44000);

    CORRADE_COMPARE_AS(importer->data(),
        (Containers::Array<char>{Containers::InPlaceInit, {
            '\x1d', '\x10', '\x71', '\xc5'}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo4() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo4.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): unsupported format Audio::WavAudioFormat::AdPcm\n");
}

void WavImporterTest::stereo8() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo8.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Stereo8);
    CORRADE_COMPARE(importer->frequency(), 96000);

    CORRADE_COMPARE_AS(importer->data(),
        (Containers::Array<char>{Containers::InPlaceInit, {
            '\xde', '\xfe', '\xca', '\x7e'}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo8ALaw() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo8ALaw.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::StereoALaw);
    CORRADE_COMPARE(importer->frequency(), 8000);

    CORRADE_COMPARE(importer->data().size(), 4096);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            -43, -43, -43, -43, -43, -43, 85, -43}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo8MuLaw() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo8MuLaw.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::StereoMuLaw);
    CORRADE_COMPARE(importer->frequency(), 8000);

    CORRADE_COMPARE(importer->data().size(), 4096);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            -1, -1, -1, -1, -1, -1, 127, -1}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo12() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo12.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): PCM with unsupported channel count 2 with 12 bits per sample\n");
}

void WavImporterTest::stereo16() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo16.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::Stereo16);
    CORRADE_COMPARE(importer->frequency(), 44100);

    CORRADE_COMPARE_AS(importer->data(),
        (Containers::Array<char>{Containers::InPlaceInit, {39, 79, 39, 79}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo24() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo24.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): PCM with unsupported channel count 2 with 24 bits per sample\n");
}

void WavImporterTest::stereo32() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo32.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): PCM with unsupported channel count 2 with 32 bits per sample\n");
}

void WavImporterTest::mono32f() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "mono32f.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::MonoFloat);
    CORRADE_COMPARE(importer->frequency(), 48000);

    CORRADE_COMPARE(importer->data().size(), 3920);
    CORRADE_COMPARE_AS(importer->data().prefix(16),
        (Containers::Array<char>{Containers::InPlaceInit, {
            0, 0, 0, 0, 108, 57, -103, 59, 3, 63, 42, 60, -33, -81, -120, 60}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo32f() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo32f.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::StereoFloat);
    CORRADE_COMPARE(importer->frequency(), 44100);

    CORRADE_COMPARE(importer->data().size(), 1352);
    CORRADE_COMPARE_AS(importer->data().prefix(8),
        (Containers::Array<char>{Containers::InPlaceInit, {
            17, -77, -103, 56, 5, 50, 72, 56}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::stereo64f() {
    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "stereo64f.wav")));

    CORRADE_COMPARE(importer->format(), BufferFormat::StereoDouble);
    CORRADE_COMPARE(importer->frequency(), 8000);

    CORRADE_COMPARE(importer->data().size(), 375888);
    CORRADE_COMPARE_AS(importer->data().prefix(64),
        (Containers::Array<char>{Containers::InPlaceInit, {
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 63, 0, 0, 0, 0, 0, 0, 16, 63,
            0, 0, 0, 0, 0, 0, 24, -65, 0, 0, 0, 0, 0, 0, 0, 0}}),
        TestSuite::Compare::Container<Containers::ArrayView<const char>>);
}

void WavImporterTest::surround51Channel16() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "surround51Channel16.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): unsupported format Audio::WavAudioFormat::Extensible\n");
}

void WavImporterTest::surround71Channel24() {
    std::ostringstream out;
    Error redirectError{&out};

    Containers::Pointer<AbstractImporter> importer = _manager.instantiate("WavAudioImporter");
    CORRADE_VERIFY(!importer->openFile(Utility::Directory::join(WAVAUDIOIMPORTER_TEST_DIR, "surround71Channel24.wav")));
    CORRADE_COMPARE(out.str(), "Audio::WavImporter::openData(): unsupported format Audio::WavAudioFormat::Extensible\n");
}

}}}}

CORRADE_TEST_MAIN(Magnum::Audio::Test::WavImporterTest)
