//
// Created by Jackson Campolattaro on 8/28/22.
//

#include "VideoOutputStream.h"

VideoOutputStream::VideoOutputStream(std::string filePath, const Magnum::Vector2i &resolution, int frameRate) :
        _filePath(std::move(filePath)) {

    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, _filePath.c_str());
    assert(formatContext);
    assert(formatContext->oformat->video_codec != AV_CODEC_ID_NONE);

    /* find the encoder */
    codec = avcodec_find_encoder(formatContext->oformat->video_codec);
    if (!codec) {
        fprintf(stderr, "Could not find encoder for '%s'\n",
                avcodec_get_name(formatContext->oformat->video_codec));
        exit(1);
    }

    tmp_pkt = av_packet_alloc();
    if (!tmp_pkt) {
        fprintf(stderr, "Could not allocate AVPacket\n");
        exit(1);
    }

    stream = avformat_new_stream(formatContext, NULL);
    if (!stream) {
        fprintf(stderr, "Could not allocate stream\n");
        exit(1);
    }
    stream->id = formatContext->nb_streams - 1;
    enc = avcodec_alloc_context3(codec);
    if (!enc) {
        fprintf(stderr, "Could not alloc an encoding context\n");
        exit(1);
    }

    enc->codec_id = formatContext->oformat->video_codec;

    // Heuristically select a bitrate for the given resolution (leaning towards the high side)
    enc->bit_rate = frameRate * resolution.product() / 10;

    /* Resolution must be a multiple of two. */
    enc->width = resolution.x();
    enc->height = resolution.y();

    /* timebase: This is the fundamental unit of time (in seconds) in terms
     * of which frame timestamps are represented. For fixed-fps content,
     * timebase should be 1/framerate and timestamp increments should be
     * identical to 1. */
    stream->time_base = (AVRational) {1, frameRate};
    enc->time_base = stream->time_base;

    enc->gop_size = 12; /* emit one intra frame every twelve frames at most */
    enc->pix_fmt = STREAM_PIX_FMT;
    if (enc->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        /* just for testing, we also add B-frames */
        enc->max_b_frames = 2;
    }
    if (enc->codec_id == AV_CODEC_ID_MPEG1VIDEO) {
        /* Needed to avoid using macroblocks in which some coeffs overflow.
         * This does not happen with normal video, it just happens here as
         * the motion of the chroma plane does not match the luma plane. */
        enc->mb_decision = 2;
    }

    /* Some formats want stream headers to be separate. */
    if (formatContext->oformat->flags & AVFMT_GLOBALHEADER)
        enc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

    sws_ctx = sws_getContext(enc->width, enc->height,
                             AV_PIX_FMT_RGB24,
                             enc->width, enc->height,
                             enc->pix_fmt,
                             SCALE_FLAGS, NULL, NULL, NULL);
    if (!sws_ctx) {
        fprintf(stderr,
                "Could not initialize the conversion context\n");
        exit(1);
    }

    open();
}

VideoOutputStream::~VideoOutputStream() {

    close();

    avformat_free_context(formatContext);
    avcodec_free_context(&enc);
    av_frame_free(&frame);
    av_frame_free(&tmp_frame);
    av_packet_free(&tmp_pkt);
    sws_freeContext(sws_ctx);
    swr_free(&swr_ctx);
}

void VideoOutputStream::open() {

    int ret;
    AVCodecContext *c = enc;

    /* open the codec */
    ret = avcodec_open2(c, codec, nullptr);
    if (ret < 0) {
        fprintf(stderr, "Could not open video codec: %s\n", av_err2str(ret));
        exit(1);
    }

    /* allocate and init a re-usable frame */
    frame = allocFrame(c->pix_fmt, c->width, c->height);
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    rgb_frame = allocFrame(AV_PIX_FMT_RGB24, c->width, c->height);
    if (!rgb_frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }

    /* If the output format is not YUV420P, then a temporary YUV420P
     * picture is needed too. It is then converted to the required
     * output format. */
    tmp_frame = NULL;
    if (c->pix_fmt != AV_PIX_FMT_YUV420P) {
        tmp_frame = allocFrame(AV_PIX_FMT_YUV420P, c->width, c->height);
        if (!tmp_frame) {
            fprintf(stderr, "Could not allocate temporary picture\n");
            exit(1);
        }
    }

    /* copy the stream parameters to the muxer */
    ret = avcodec_parameters_from_context(stream->codecpar, c);
    if (ret < 0) {
        fprintf(stderr, "Could not copy the stream parameters\n");
        exit(1);
    }

    /* open the output file */
    ret = avio_open(&formatContext->pb, _filePath.c_str(), AVIO_FLAG_WRITE);
    if (ret < 0) {
        fprintf(stderr, "Could not open '%s': %s\n", _filePath.c_str(),
                av_err2str(ret));
        exit(1);
    }

    // Write the stream header
    ret = avformat_write_header(formatContext, nullptr);
    if (ret < 0) {
        fprintf(stderr, "Error occurred when opening output file: %s\n",
                av_err2str(ret));
        exit(1);
    }
}

void VideoOutputStream::close() {

    // Write empty frames until encoding is done
    bool doneEncoding = false;
    while (!doneEncoding)
        doneEncoding = writeFrame(nullptr);

    // Write the stream trailer
    av_write_trailer(formatContext);

    // Close the output file.
    avio_closep(&formatContext->pb);
}
