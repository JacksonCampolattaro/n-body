//
// Created by jackcamp on 5/20/19.
//

#ifndef N_BODY_RECORDER_H
#define N_BODY_RECORDER_H


#include "View.h"

class Recorder {

public:

    /**
     * Constructor for the recorder
     * @param view The viewport to be recorded
     * @param outputPath The location of the output file
     */
    Recorder(View *view, std::string outputPath);

    /**
     * Renders the most recently drawn frame and adds it to the output
     */
    void renderFrame();

    /**
     * Closes the encoder, completing the video
     */
    void completeVideo();

private:

    glm::ivec2 dimensions;
    FILE *ffmpeg;



};


#endif //N_BODY_RECORDER_H
