//
// Created by Jackson Campolattaro on 8/28/22.
//

#ifndef N_BODY_VIDEOOUTPUTSTREAM_H
#define N_BODY_VIDEOOUTPUTSTREAM_H
/*
 * Copyright (c) 2003 Fabrice Bellard
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * libavformat API example.
 *
 * Output a media file in any supported libavformat format. The default
 * codecs are used.
 * @example muxing.c
 */

#include <string>

#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <cmath>
#include <cassert>
#include <utility>

#include <Magnum/Magnum.h>
#include <Magnum/Math/Vector2.h>
#include <Magnum/Math/Color.h>
#include <Magnum/Image.h>
#include <Corrade/Containers/StridedArrayView.h>

#include <spdlog/spdlog.h>

extern "C" {
#include <libavutil/avassert.h>
#include <libavutil/channel_layout.h>
#include <libavutil/opt.h>
#include <libavutil/mathematics.h>
#include <libavutil/timestamp.h>
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

#define STREAM_PIX_FMT    AV_PIX_FMT_YUV420P /* default pix_fmt */

#define SCALE_FLAGS SWS_BICUBIC

class VideoOutputStream {
public:

    std::string _filePath;

    AVFormatContext *formatContext = nullptr;

    AVStream *stream = nullptr;
    AVCodecContext *enc = nullptr;
    const AVCodec *codec = nullptr;

    AVFrame *frame = nullptr;
    AVFrame *tmp_frame = nullptr;
    AVFrame *rgb_frame = nullptr;

    AVPacket *tmp_pkt = nullptr;

    float t = 0.0f;

    struct SwsContext *sws_ctx = nullptr;
    struct SwrContext *swr_ctx = nullptr;

public:

    VideoOutputStream(std::string filePath, const Magnum::Vector2i &resolution, int frameRate);

    ~VideoOutputStream();

    void open();

    void close();

    bool writeFrame(const Magnum::Image2D &image) {
        assert(image.size().x() == rgb_frame->width);
        assert(image.size().y() == rgb_frame->height);

        auto pixels = image.pixels<Magnum::Color3ub>();
        uint8_t *data = rgb_frame->data[0];
        for (int y = 0; y < rgb_frame->height; y++) {
            for (int x = 0; x < rgb_frame->width; x++) {
                int offset = ((rgb_frame->height - y - 1) * rgb_frame->linesize[0]) + (x * 3);
                data[offset + 0] = pixels[y][x].r();
                data[offset + 1] = pixels[y][x].g();
                data[offset + 2] = pixels[y][x].b();
            }
        }

        // Convert the rgb frame data to the output format
        sws_scale(sws_ctx, (const uint8_t *const *) rgb_frame->data,
                  rgb_frame->linesize, 0, enc->height, frame->data,
                  frame->linesize);

        return writeFrame(frame);
    }

private:

    static AVFrame *allocFrame(enum AVPixelFormat pix_fmt, int width, int height) {
        AVFrame *picture;
        int ret;

        picture = av_frame_alloc();
        if (!picture)
            return NULL;

        picture->format = pix_fmt;
        picture->width = width;
        picture->height = height;
        picture->pts = 0;

        /* allocate the buffers for the frame data */
        ret = av_frame_get_buffer(picture, 0);
        if (ret < 0) {
            fprintf(stderr, "Could not allocate frame data.\n");
            exit(1);
        }

        return picture;
    }

    bool writeFrame(AVFrame *f) const {

        int ret;

        // send the frame to the encoder
        ret = avcodec_send_frame(enc, f);
        if (ret < 0) {
            fprintf(stderr, "Error sending a frame to the encoder: %s\n",
                    av_err2str(ret));
            exit(1);
        }

        while (ret >= 0) {
            ret = avcodec_receive_packet(enc, tmp_pkt);
            if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF)
                break;
            else if (ret < 0) {
                fprintf(stderr, "Error encoding a frame: %s\n", av_err2str(ret));
                exit(1);
            }

            /* rescale output packet timestamp values from codec to stream timebase */
            av_packet_rescale_ts(tmp_pkt, enc->time_base, stream->time_base);
            tmp_pkt->stream_index = stream->index;

            /* Write the compressed frame to the media file. */
            ret = av_interleaved_write_frame(formatContext, tmp_pkt);
            /* pkt is now blank (av_interleaved_write_frame() takes ownership of
             * its contents and resets pkt), so that no unreferencing is necessary.
             * This would be different if one used av_write_frame(). */
            if (ret < 0) {
                fprintf(stderr, "Error while writing output packet: %s\n", av_err2str(ret));
                exit(1);
            }
        }

        // Prepare the frame object for the next write
        if (av_frame_make_writable(frame) < 0)
            exit(1);

        // Update the frame's timestamp
        frame->pts++;

        return ret == AVERROR_EOF;
    }

};

#endif //N_BODY_VIDEOOUTPUTSTREAM_H
