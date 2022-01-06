#ifndef _SPLITTER_HPP
#define _SPLITTER_HPP

#include <vector>
#include <CL/sycl.hpp>
#include "dpc_common.hpp"

using namespace std;
using namespace sycl;

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

static void kernel_time(const string &msg, event e)
{
    cl_ulong time_start =
        e.get_profiling_info<info::event_profiling::command_start>();

    cl_ulong time_end =
        e.get_profiling_info<info::event_profiling::command_end>();

    double elapsed = (time_end - time_start) / 1e3;
    cout << msg << elapsed << " useconds\n";
}

template <typename T>
void splitter(T *input, size_t width, size_t height, int channels, size_t crop_width, size_t crop_height, vector<crop_info_t> crop_info, vector<T*> &output)
{
#define MULTI_PIXEL_NUM 4
typedef struct {
    T pixel[MULTI_PIXEL_NUM];
} multi_pixel_t;

    // Create a command queue using the device selector and request profiling
    auto prop_list = property_list{property::queue::enable_profiling()};
#if 1
    gpu_selector sel;
#else
    cpu_selector sel;
#endif
    queue q(sel, dpc_common::exception_handler, prop_list);
    event e;

    int input_size = width * height * channels;
#if 1
    T* kernel_input = malloc_host<T>(input_size, q);
    memcpy(kernel_input, input, input_size);
#else
    multi_pixel_t* kernel_input = malloc_host<multi_pixel_t>(input_size/MULTI_PIXEL_NUM, q);
    memcpy(kernel_input, input, input_size);
#endif

    const size_t num_crop = crop_info.size();
    crop_info_t* kernel_crop_info = malloc_shared<crop_info_t>(num_crop, q);
    memcpy(kernel_crop_info, crop_info.data(), num_crop * sizeof(crop_info_t));

    for(int i = 0; i < num_crop; i++)
        output.push_back(malloc_shared<T>(crop_width * crop_height, q));

#if 1
    T **kernel_output = malloc_host<T*>(num_crop * sizeof(T*), q);
    memcpy(kernel_output, output.data(), num_crop * sizeof(T*));

    const range<3> splitter_range{num_crop, crop_height, crop_width};
#else
    multi_pixel_t **kernel_output = malloc_host<multi_pixel_t*>(num_crop * sizeof(multi_pixel_t*), q);
    memcpy(kernel_output, output.data(), num_crop * sizeof(multi_pixel_t*));

    const range<3> splitter_range{num_crop, crop_height, crop_width/MULTI_PIXEL_NUM};
#endif

    try
    {
        e = q.submit([&](handler& h)
                    {
                        h.parallel_for(splitter_range, [=](id<3> idx)
                                    {
                                        int crop_id = idx[0];
                                        int row = idx[1];
                                        int col = idx[2];

#if 1
                                        T *kernel_output_ptr = kernel_output[crop_id];
                                        kernel_output_ptr[row * crop_width + col] = kernel_input[(row + kernel_crop_info[crop_id].top ) * width + (col + kernel_crop_info[crop_id].left)];
#else
                                        multi_pixel_t *kernel_output_ptr = kernel_output[crop_id];
                                        kernel_output_ptr[row * crop_width/MULTI_PIXEL_NUM + col] = kernel_input[(row + kernel_crop_info[crop_id].top ) * width/MULTI_PIXEL_NUM + (col + kernel_crop_info[crop_id].left)];
#endif
                                    });
                    });
        q.wait_and_throw();

        kernel_time("kernel time: ", e);
    }
    catch (sycl::exception e)
    {
        cout << "SYCL exception caught: " << e.what() << "\n";
    }
}

#endif