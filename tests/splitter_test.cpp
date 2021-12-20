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
    int i;

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

    rgb_t *output[NUM_CROP_INFO];
    for(i = 0; i < NUM_CROP_INFO; i++)
        output[i] = (rgb_t *)malloc(WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t));

    splitter(input, img_width, img_height, channels, output);

    string filename;
    for(i = 0; i < NUM_CROP_INFO; i++) {
        filename = "silverfallscrop" + to_string(i) + ".bmp";
        stbi_write_bmp(filename.c_str(), WIDTH_CROP, HEIGHT_CROP, sizeof(rgb_t), output[i]);
        free(output[i]);
    }

    stbi_image_free(input);

    return 0;
}