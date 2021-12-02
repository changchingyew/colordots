#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

DEBUG="-loglevel verbose"

# for i in `seq 1 20`;
# do
# 	./test_driver.pl -s beh_avce_nummbperslice -p u18.04_kmb_64 --gold no -i $i
# done

INPUT="-fflags +genpts"
# OUTPUT="-frames:v 10 -y out.yuv"
# OUTPUT="-frames:v 30 -f rtp -sdp_file test.sdp rtp://127.0.0.1:10001"
#OUTPUT="-f rtp -sdp_file test.sdp rtp://127.0.0.1:10001"
# OUTPUT="-frames:v 10 -vf crop=500:500:0:0 -y crop_0.yuv\
#         -frames:v 10 -vf crop=500:500:500:0 -y crop_1.yuv"
# OUTPUT="-frames:v 10 -vf crop=500:500:0:0 -f rtp rtp://127.0.0.1:2001\
#         -frames:v 10 -vf crop=500:500:500:0 -f rtp rtp://127.0.0.1:2002"
#OUTPUT="-vf crop=500:500:0:0 -f rtp rtp://127.0.0.1:10001\
#        -vf crop=500:500:500:0 -f rtp rtp://127.0.0.1:10002"
OUTPUT="-vf crop=500:500:0:0 -c:v hevc -f rtp rtp://127.0.0.1:10001\
       -vf crop=500:500:500:0 -c:v hevc -f rtp -sdp_file test.sdp rtp://127.0.0.1:10003"

${FFMPEG_PATH}/ffmpeg_g -c:v ${CODEC} ${DEBUG} ${INPUT} -i ${MEDIA} ${OUTPUT}
