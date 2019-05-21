//
// Created by jackcamp on 5/20/19.
//

#include <string>

#include "Recorder.h"

Recorder::Recorder(View *view, std::string outputPath) {

    this->dimensions = view->getDimensions();

    // Rendering settings
    int fps = 60;
    std::string resolution = std::to_string(dimensions.x) + "x" + std::to_string(dimensions.y);
    std::string qualityPreset = "slow";
    std::string encodingLevel = "18"; // Lower is better, compression should be visually unnoticable under 18

    // Creating the command to open
    std::string command =
            "ffmpeg -r " + std::to_string(fps) + " -f rawvideo -pix_fmt rgba -s " + resolution +
            " -i - -threads 0 -preset " + qualityPreset + " -y -pix_fmt yuv420p -crf " + encodingLevel + " -vf vflip " +
            outputPath;

    // Opening the ffmpeg instance
    ffmpeg = popen(command.c_str(), "w");
}

void Recorder::renderFrame() {

    int *buffer = new int[dimensions.x * dimensions.y];
    glReadPixels(0, 0, dimensions.x, dimensions.y, GL_RGBA, GL_UNSIGNED_BYTE, buffer);
    fwrite(buffer, sizeof(int)*dimensions.x*dimensions.y, 1, ffmpeg);
    delete buffer;
}

void Recorder::completeVideo() {

    pclose(ffmpeg);
}
