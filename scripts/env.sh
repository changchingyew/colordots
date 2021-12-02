#!/bin/bash
set -x
SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"

export DISPLAY=:0
export WORK_DIR=${SCRIPT_DIR}/../..

export STREAM_FILE=Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.mkv

export OUTPUT_PATH=/tmp/
export OUTPUT_FILE=out.yuv

export MEDIA_PATH=${WORK_DIR}

export FFMPEG_PATH=${WORK_DIR}/libraries.media.middleware.cartwheel.ffmpeg/ffmpeg
export LOCAL_INSTALL=${WORK_DIR}/local_install
export LD_LIBRARY_PATH=${LOCAL_INSTALL}/lib:${LD_LIBRARY_PAT}

#MEDIA=/media-usb/Puppies_3840x2160_20mbps_60fps_High_at_L5.2.h264
MEDIA=${MEDIA_PATH}/Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.265
#MEDIA=/media-nvme/Puppies_3840x2160_20mbps_60fps_High_at_L5.2_short.mp4
#MEDIA=/media-usb/MIPS/content/FPS_test_1080p60_L4.2_100frames.mkv

#CODEC=h264_qsv
CODEC=hevc_qsv
#CODEC=VP9_qsv
