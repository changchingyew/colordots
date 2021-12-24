//==============================================================
// Copyright © 2020, Intel Corporation. All rights reserved.
//
// SPDX-License-Identifier: MIT
// =============================================================

// located in $ONEAPI_ROOT/compiler/latest/linux/include/sycl/
#include <CL/sycl.hpp>
#include <cstdlib>
#include <iostream>
#include "dpc_common.hpp"
#include "splitter.hpp"

using namespace std;
using namespace sycl;

static void kernel_time(const string &msg, event e)
{
    cl_ulong time_start =
        e.get_profiling_info<info::event_profiling::command_start>();

    cl_ulong time_end =
        e.get_profiling_info<info::event_profiling::command_end>();

    double elapsed = (time_end - time_start) / 1e3;
    cout << msg << elapsed << " useconds\n";
}

void splitter(rgb_t *input, int width, int height, int channels, vector<rgb_t*> &output)
{
    // Create a command queue using the device selector and request profiling
    auto prop_list = property_list{property::queue::enable_profiling()};
#if 1
    gpu_selector sel;
#else
    cpu_selector sel;
#endif
    queue q(sel, dpc_common::exception_handler, prop_list);
    event e;

    crop_info_t crop_info[]=
    {
        {.left = 0, .top = 0},
        {.left = 300, .top = 0},
        {.left = 600, .top = 0},
        {.left = 900, .top = 0},
        {.left = 1200, .top = 0},
        {.left = 1500, .top = 0},
        {.left = 1800, .top = 0},
        {.left = 2100, .top = 0},
        {.left = 2400, .top = 0},
        {.left = 2700, .top = 0},
        {.left = 0, .top = 300},
        {.left = 300, .top = 300},
        {.left = 600, .top = 300},
        {.left = 900, .top = 300},
        {.left = 1200, .top = 300},
        {.left = 1500, .top = 300},
        {.left = 1800, .top = 300},
        {.left = 2100, .top = 300},
        {.left = 2400, .top = 300},
        {.left = 2700, .top = 300},
        {.left = 0, .top = 600},
        {.left = 300, .top = 600},
        {.left = 600, .top = 600},
        {.left = 900, .top = 600},
        {.left = 1200, .top = 600},
        {.left = 1500, .top = 600},
        {.left = 1800, .top = 600},
        {.left = 2100, .top = 600},
        {.left = 2400, .top = 600},
        {.left = 2700, .top = 600},
        {.left = 0, .top = 900},
        {.left = 300, .top = 900},
        {.left = 600, .top = 900},
        {.left = 900, .top = 900},
        {.left = 1200, .top = 900},
        {.left = 1500, .top = 900},
        {.left = 1800, .top = 900},
        {.left = 2100, .top = 900},
        {.left = 2400, .top = 900},
        {.left = 2700, .top = 900},
        {.left = 0, .top = 1200},
        {.left = 300, .top = 1200},
        {.left = 600, .top = 1200},
        {.left = 900, .top = 1200},
        {.left = 1200, .top = 1200},
        {.left = 1500, .top = 1200},
        {.left = 1800, .top = 1200},
        {.left = 2100, .top = 1200},
        {.left = 2400, .top = 1200},
        {.left = 2700, .top = 1200},
        {.left = 0, .top = 1500},
        {.left = 300, .top = 1500},
        {.left = 600, .top = 1500},
        {.left = 900, .top = 1500},
        {.left = 1200, .top = 1500},
        {.left = 1500, .top = 1500},
        {.left = 1800, .top = 1500},
        {.left = 2100, .top = 1500},
        {.left = 2400, .top = 1500},
        {.left = 2700, .top = 1500},
        {.left = 0, .top = 1800},
        {.left = 300, .top = 1800},
        {.left = 600, .top = 1800},
        {.left = 900, .top = 1800},
        {.left = 1200, .top = 1800},
        {.left = 1500, .top = 1800},
        {.left = 1800, .top = 1800},
        {.left = 2100, .top = 1800},
        {.left = 2400, .top = 1800},
        {.left = 2700, .top = 1800},
        {.left = 0, .top = 2100},
        {.left = 300, .top = 2100},
        {.left = 600, .top = 2100},
        {.left = 900, .top = 2100},
        {.left = 1200, .top = 2100},
        {.left = 1500, .top = 2100},
        {.left = 1800, .top = 2100},
        {.left = 2100, .top = 2100},
        {.left = 2400, .top = 2100},
        {.left = 2700, .top = 2100}
    };

    int input_size = width * height * channels;
    rgb_t* kernel_input = malloc_host<rgb_t>(input_size, q);
    memcpy(kernel_input, input, input_size);

    for(int i = 0; i < NUM_CROP_INFO; i++)
        output.push_back(malloc_shared<rgb_t>(CROP_SIZE, q));

    rgb_t **kernel_output = malloc_host<rgb_t*>(NUM_CROP_INFO * sizeof(rgb_t*), q);
    memcpy(kernel_output, output.data(), NUM_CROP_INFO * sizeof(rgb_t*));

    try
    {
        e = q.submit([&](handler& h)
                    {
                        h.parallel_for(range<3> {NUM_CROP_INFO, HEIGHT_CROP, WIDTH_CROP}, [=](id<3> idx)
                                    {
                                        int crop_id = idx[0];
                                        int row = idx[1];
                                        int col = idx[2];

                                        rgb_t *kernel_output_ptr = kernel_output[crop_id];
                                        kernel_output_ptr[row * WIDTH_CROP + col] = kernel_input[(row + crop_info[crop_id].top ) * width + (col + crop_info[crop_id].left)];
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