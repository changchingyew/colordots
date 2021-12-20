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

void splitter(rgb_t *input, int width, int height, int channels, rgb_t *output[NUM_CROP_INFO])
{
    // Create a command queue using the device selector and request profiling
    auto prop_list = property_list{property::queue::enable_profiling()};
#if 1
    gpu_selector sel;
#else
    cpu_selector sel;
#endif
    queue q(sel, dpc_common::exception_handler, prop_list);

    crop_info_t crop_info[]=
    {
        {.left = 300, .top=300},
        {.left = 600, .top=300},
        {.left = 300, .top=600},
        {.left = 600, .top=600}
    };

    rgb_t *input_buf = malloc_device<rgb_t>(width * height * sizeof(rgb_t), q);
    rgb_t **output_buf[NUM_CROP_INFO];
    int i;

    crop_info_t *crop_info_buf = malloc_device<crop_info_t>(NUM_CROP_INFO * sizeof(crop_info_t), q);
    for(i=0; i<NUM_CROP_INFO; i++) {
        output_buf[i] = malloc_device<rgb_t *>(WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t), q);
    }

    event e;

    try
    {
        e = q.submit([&](handler &h)
                    {
                        h.memcpy(input_buf, input, width * height * sizeof(rgb_t)); 
                    });
        q.wait_and_throw();

        cl_ulong time_start =
            e.get_profiling_info<info::event_profiling::command_start>();

        e = q.submit([&](handler &h)
                    {
                        h.memcpy(crop_info_buf, crop_info, NUM_CROP_INFO * sizeof(crop_info_t)); 
                    });
        q.wait_and_throw();

        e = q.submit([&](handler& h)
                    {
                        h.parallel_for(range<3> {NUM_CROP_INFO, WIDTH_CROP, HEIGHT_CROP}, [=](id<3> idx)
                                    {
                                        int crop_id = idx[0];
                                        int row = idx[1];
                                        int col = idx[2];

                                        output_buf[crop_id][row][col] = input_buf[(row + crop_info_buf[crop_id].top ) * width + (col + crop_info_buf[crop_id].left)];
                                    });
                    });
        q.wait_and_throw();

        e = q.submit([&](handler &h)
                    {
                        h.memcpy(output[0], output_buf[0], WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t));
                    });
        q.wait_and_throw();

        e = q.submit([&](handler &h)
                    {
                        h.memcpy(output[1], output_buf[1], WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t));
                    });
        q.wait_and_throw();

        e = q.submit([&](handler &h)
                    {
                        h.memcpy(output[2], output_buf[2], WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t));
                    });
        q.wait_and_throw();

        e = q.submit([&](handler &h)
                    {
                        h.memcpy(output[3], output_buf[3], WIDTH_CROP * HEIGHT_CROP * sizeof(rgb_t));
                    });
        q.wait_and_throw();

        cl_ulong time_end =
            e.get_profiling_info<info::event_profiling::command_end>();

        double elapsed = (time_end - time_start) / 1e3;
        cout << "kernel time:" << elapsed << " useconds\n";
    }
    catch (sycl::exception e)
    {
        cout << "SYCL exception caught: " << e.what() << "\n";
    }
}
