#ifndef _SPLITTER_HPP
#define _SPLITTER_HPP

#define WIDTH_CROP 300
#define HEIGHT_CROP 300

#pragma pack(push, 1)

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} rgb;

#pragma pack(pop)

void splitter(rgb *input, int img_width, int img_height, int channels, rgb *output);

#endif