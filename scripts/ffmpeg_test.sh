#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

DEBUG="-loglevel verbose"

OUTPUT="-frames:v 10 -vf crop=500:500:0:0 -y crop_0.yuv\
        -frames:v 10 -vf crop=500:500:500:0 -y crop_1.yuv"
# OUTPUT="-frames:v 10 -y out.yuv"

${FFMPEG_PATH}/ffmpeg_g -c:v ${CODEC} ${DEBUG} -i ${MEDIA} ${OUTPUT}