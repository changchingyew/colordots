#ifndef _SPLITTER_HPP
#define _SPLITTER_HPP

#include <vector>
using namespace std;

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

void splitter(rgb_t *input, size_t img_width, size_t img_height, int channels, size_t crop_width, size_t crop_height, vector<crop_info_t> crop_info, vector<rgb_t*> &output);

#endif