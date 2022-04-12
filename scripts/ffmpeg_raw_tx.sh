SCRIPT_DIR="$( cd "$(dirname "${BASH_SOURCE[0]:-$0}")" >/dev/null 2>&1 ; pwd -P )"
. "${SCRIPT_DIR}/env.sh" 

DEBUG="-loglevel verbose"

#ffmpeg -i ~/Downloads/Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.mkv -c:v hevc_qsv -vcodec rawvideo -video_size 1920x1080 -loglevel debug -pixel_format rgb24 -y output.yuv
#ffmpeg -i ~/Downloads/Puppies_1920x1080_38mbps_30fps_Main_at_L4.1.mkv -c:v hevc_qsv -vcodec rawvideo -video_size 1920x1080 -loglevel debug -pixel_format rgb24 -sdp_file rawvid.sdp -f rtp rtp://127.0.0.1:10003

set -x
#OUTPUT="-c:v hevc_qsv -vcodec rawvideo -video_size 1920x1080 -pixel_format rgb24 -y output.yuv"
OUTPUT="-c:v hevc_qsv -vcodec rawvideo -video_size 1920x1080 -loglevel debug -pixel_format rgb24 -sdp_file rawvid.sdp -f rtp rtp://127.0.0.1:10003"

${FFMPEG_PATH}/ffmpeg_g  ${DEBUG} ${INPUT} -i ${MEDIA} ${OUTPUT}
