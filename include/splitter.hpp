#ifndef _SPLITTER_HPP
#define _SPLITTER_HPP

#include <vector>
using namespace std;

#define NUM_CROP_INFO 80

#define WIDTH_CROP 300
#define HEIGHT_CROP 300
#define CROP_SIZE ( WIDTH_CROP * HEIGHT_CROP )

#pragma pack(push, 1)

typedef struct
{
    uint8_t blue;
    uint8_t green;
    uint8_t red;
} rgb_t;

typedef struct 
{
    uint32_t left;
    uint32_t top;
} crop_info_t;

#pragma pack(pop)

void splitter(rgb_t *input, int img_width, int img_height, int channels, vector<rgb_t*> &output);

#endif