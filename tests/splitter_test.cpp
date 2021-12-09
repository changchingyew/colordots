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
    uint8_t *input = stbi_load("../tests/silverfalls1.png", &img_width, &img_height, &channels, 0);
    uint8_t *output;
    if (input == NULL)
    {
        cout << "Error in loading the image\n";
        exit(1);
    }
    cout << "Loaded image with a width of " << img_width << ", a height of "
         << img_height << " and " << channels << " channels\n";


    splitter(input, img_width, img_height, channels, output);
    return 0;
}