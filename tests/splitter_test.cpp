#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#include "splitter.hpp"

using namespace std;

void gen_crop_info(int img_width, int img_height, int crop_width, int crop_height, vector<crop_info_t> &crop_info)
{
    crop_info_t crop_info_entry;
    int i, j;

    for ( j = 0; j <= img_height - crop_height; j += crop_height)
        for ( i = 0; i <= img_width - crop_width; i += crop_width)
        {
            crop_info_entry.left = i;
            crop_info_entry.top = j;
            crop_info.push_back(crop_info_entry);
        }
}

void write_output(string filename_prefix, int width, int height, int channels, vector<crop_info_t> crop_info, vector<rgb_t*> output)
{

    string filename;
    int i = 0;
    for(auto it : output) {
        filename = filename_prefix + to_string(crop_info.at(i).left) + "x" + to_string(crop_info.at(i).top) + ".bmp";
        stbi_write_bmp(filename.c_str(), width, height, sizeof(rgb_t), it);
        i++;
    }
}

void write_output(string filename_prefix, int width, int height, int channels, vector<crop_info_t> crop_info, vector<rgbe_t*> output)
{
    string filename;
    int i = 0;
    for(auto it : output) {
        filename = filename_prefix + to_string(crop_info.at(i).left) + "x" + to_string(crop_info.at(i).top) + ".hdr";
        const float *outputf {(float *)it};
        stbi_write_hdr(filename.c_str(), width, height, channels, outputf);
        i++;
    }
}

int main(int argc, char **argv)
{
    int img_width, img_height, channels;

    rgb_t *input = (rgb_t *)stbi_load("../tests/silverfalls.bmp", &img_width, &img_height, &channels, STBI_rgb);
    if (input == NULL)
    {
        cout << "Error in loading the image\n";
        exit(1);
    }

    cout << "Loaded RGB image with a width of " << img_width << ", a height of "
         << img_height << " and " << channels << " channels\n";

    size_t crop_width = 300, crop_height = 300;
    vector<crop_info_t> crop_info;
    vector<rgb_t*> output;

    gen_crop_info(img_width, img_height, crop_width, crop_height, crop_info);

    splitter<rgb_t>(input, img_width, img_height, channels, crop_width, crop_height, crop_info, output);

    write_output("silverfallscrop", crop_width, crop_height, channels, crop_info, output);

    stbi_image_free(input);

    rgbe_t *input2 = (rgbe_t *)stbi_loadf("../tests/seymour_park.hdr", &img_width, &img_height, &channels, 0);
    if (input2 == NULL)
    {
        cout << "Error in loading the image\n";
        exit(1);
    }

    cout << "Loaded HDR image with a width of " << img_width << ", a height of "
         << img_height << " and " << channels << " channels\n";

    crop_info.clear();
    gen_crop_info(img_width, img_height, crop_width, crop_height, crop_info);

    vector<rgbe_t*> output2;
    splitter<rgbe_t>(input2, img_width, img_height, channels, crop_width, crop_height, crop_info, output2);

    write_output("seymour_park_crop", crop_width, crop_height, channels, crop_info, output2);

    stbi_image_free(input2);

    return 0;
}