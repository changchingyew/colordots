#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "splitter.hpp"

using namespace std;

int main(int argc, char **argv)
{
    int img_width, img_height, channels;

    rgb_t *input = (rgb_t *)stbi_load("../tests/silverfalls.bmp", &img_width, &img_height, &channels, STBI_rgb);
    if (input == NULL)
    {
        cout << "Error in loading the image\n";
        exit(1);
    }

    if (channels != sizeof(rgb_t))
    {
        cout << "Error in number of channels:" << channels << "\n";
        exit(1);
    }

    cout << "Loaded image with a width of " << img_width << ", a height of "
         << img_height << " and " << channels << " channels\n";

    size_t crop_width = 300, crop_height = 300;
    vector<crop_info_t> crop_info;
    crop_info_t crop_info_entry;
    vector<rgb_t*> output;
    int i, j;

    for ( j = 0; j <= img_height - crop_height; j += crop_height)
        for ( i = 0; i <= img_width - crop_width; i += crop_width)
        {
            crop_info_entry.left = i;
            crop_info_entry.top = j;
            crop_info.push_back(crop_info_entry);
        }

    splitter(input, img_width, img_height, channels, crop_width, crop_height, crop_info, output);

    string filename;
    i = 0;
    for(auto it : output) {
        filename = "silverfallscrop" + to_string(i) + ".bmp";
        stbi_write_bmp(filename.c_str(), crop_width, crop_height, sizeof(rgb_t), it);
        i++;
    }

    stbi_image_free(input);

    return 0;
}