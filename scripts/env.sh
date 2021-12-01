#!/bin/bash
set -x
export DISPLAY=:0
export MIPS_ROOT=/media-usb/mips
export MIPS_APP=${MIPS_ROOT}/MIPS
#export FFMPEG_PATH=${MIPS_ROOT}/ffmpeg
export FFMPEG_PATH=/media-usb/libraries.media.middleware.cartwheel.ffmpeg/ffmpeg
#export FFMPEG_PATH=${MIPS_ROOT}/mips_install/bin

export STREAM_PATH=${MIPS_APP}/content/
export STREAM_FILE=Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.mkv

export OUTPUT_PATH=/tmp/
export OUTPUT_FILE=out.yuv
#export OUTPUT_PATH=/dev/
#export OUTPUT_FILE=null

export RTSPSERVER_PATH=${MIPS_APP}/tools/rtsp-server/

export MIPS_INSTALL=${MIPS_ROOT}/mips_install
export PKG_CONFIG_PATH=${MIPS_INSTALL}/lib/pkgconfig
export LD_LIBRARY_PATH=${MIPS_INSTALL}/lib

export MEDIA_PATH=/media-nvme

#MEDIA=/media-usb/Puppies_3840x2160_20mbps_60fps_High_at_L5.2.h264
MEDIA=/media-nvme/Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.265
#MEDIA=/media-nvme/Puppies_3840x2160_20mbps_60fps_High_at_L5.2_short.mp4
#MEDIA=/media-usb/MIPS/content/FPS_test_1080p60_L4.2_100frames.mkv

#CODEC=h264_qsv
CODEC=hevc_qsv
#CODEC=VP9_qsv
