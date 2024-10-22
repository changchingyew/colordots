#!/bin/bash

SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

set +x
DEBUG="-loglevel verbose"

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
# OUTPUT="-vf crop=500:500:0:0 -c:v hevc -f rtp rtp://127.0.0.1:10001\
#        -vf crop=500:500:500:0 -c:v hevc -f rtp -sdp_file test.sdp rtp://127.0.0.1:10003"

WCROP=300
HCROP=300
WIDTH=1920
HEIGHT=1080
ENC="-c:v hevc"
PORT=10000
for (( Y = 0; Y <= HEIGHT; Y+=HCROP ))
do
    for (( X = 0; X <= WIDTH; X+=WCROP ))
    do
        RTP="-sdp_file test.sdp -f rtp rtp://127.0.0.1:$PORT"
        PORT=$((PORT + 2))
        OUTPUT="${OUTPUT} -vf crop=$WCROP:$HCROP:$X:$Y $ENCODE $RTP"
        # OUTPUT="${OUTPUT} -vf crop=$WCROP:$HCROP:$X:$Y $ENCODE -f null -"
    done
    # OUTPUT="${OUTPUT}\n"
done

# echo -e $OUTPUT
set -x
${FFMPEG_PATH}/ffmpeg_g -c:v ${CODEC} ${DEBUG} ${INPUT} -i ${MEDIA} ${OUTPUT}
