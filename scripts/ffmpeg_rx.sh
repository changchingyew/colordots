#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

${FFMPEG_PATH}/ffmpeg -protocol_whitelist file,data,rtp,udp -i test.sdp -f null -
#${FFMPEG_PATH}/ffmpeg -protocol_whitelist file,data,rtp,udp -i test.sdp -frames:v 10 -y out.yuv
# ${FFMPEG_PATH}/ffmpeg -protocol_whitelist file,data,rtp,udp -i test.sdp -y out.yuv
