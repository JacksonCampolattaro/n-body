//
// Created by jackcamp on 5/20/19.
//

#include <string>

#include "Recorder.h"

Recorder::Recorder(View *view, std::string outputPath) {
}

void Recorder::renderFrame() {
}

void Recorder::completeVideo() {

    pclose(ffmpeg);
}
