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

void splitter(rgb_t *input, size_t width, size_t height, int channels, size_t crop_width, size_t crop_height, vector<crop_info_t> crop_info, vector<rgb_t*> &output)
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

    int input_size = width * height * channels;
    rgb_t* kernel_input = malloc_host<rgb_t>(input_size, q);
    memcpy(kernel_input, input, input_size);

    const size_t num_crop = crop_info.size();
    crop_info_t* kernel_crop_info = malloc_shared<crop_info_t>(num_crop, q);
    memcpy(kernel_crop_info, crop_info.data(), num_crop * sizeof(crop_info_t));

    for(int i = 0; i < num_crop; i++)
        output.push_back(malloc_shared<rgb_t>(crop_width * crop_height, q));

    rgb_t **kernel_output = malloc_host<rgb_t*>(num_crop * sizeof(rgb_t*), q);
    memcpy(kernel_output, output.data(), num_crop * sizeof(rgb_t*));

    const range<3> splitter_range{num_crop, crop_height, crop_width};

    try
    {
        e = q.submit([&](handler& h)
                    {
                        h.parallel_for(splitter_range, [=](id<3> idx)
                                    {
                                        int crop_id = idx[0];
                                        int row = idx[1];
                                        int col = idx[2];

                                        rgb_t *kernel_output_ptr = kernel_output[crop_id];
                                        kernel_output_ptr[row * crop_width + col] = kernel_input[(row + kernel_crop_info[crop_id].top ) * width + (col + kernel_crop_info[crop_id].left)];
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