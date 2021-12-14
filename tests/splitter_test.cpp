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
    rgb *input = (rgb *)stbi_load("../tests/silverfalls.bmp", &img_width, &img_height, &channels, STBI_rgb);
    if (input == NULL)
    {
        cout << "Error in loading the image\n";
        exit(1);
    }

    if (channels != sizeof(rgb))
    {
        cout << "Error in number of channels:" << channels << "\n";
        exit(1);
    }

    cout << "Loaded image with a width of " << img_width << ", a height of "
         << img_height << " and " << channels << " channels\n";

    rgb *output = (rgb *)malloc(WIDTH_CROP * HEIGHT_CROP * sizeof(rgb));

    splitter(input, img_width, img_height, channels, output);

    stbi_write_bmp("silverfallscrop.bmp", WIDTH_CROP, HEIGHT_CROP, sizeof(rgb), output);

    stbi_image_free(input);
    free(output);

    return 0;
}