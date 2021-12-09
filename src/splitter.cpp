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

void splitter(rgb *input, int width, int height, int channels, rgb *output)
{
    size_t num_pixels = width * height;
    size_t img_size = width * height * sizeof(rgb);

    size_t crop_size = WIDTH_CROP * HEIGHT_CROP * sizeof(rgb);

    int crop_x = 1;
    int crop_y = 1;

    buffer<rgb, 2> input_buf(input, range(width, height));
    buffer<rgb, 2> output_buf(output, range(WIDTH_CROP, HEIGHT_CROP));

    gpu_selector sel;
    event e;

    try
    {
        // Create a command queue using the device selector and request profiling
        auto prop_list = property_list{property::queue::enable_profiling()};
        queue q(sel, dpc_common::exception_handler, prop_list);

        e = q.submit([&](auto &h)
                     {
                         accessor input_acc(input_buf, h, read_only);
                         accessor output_acc(output_buf, h, write_only);

                         int crop_x_offset = crop_x * WIDTH_CROP;
                         int crop_y_offset = crop_y * HEIGHT_CROP;

                         h.parallel_for(range<2>(WIDTH_CROP, HEIGHT_CROP), [=](id<2> idx)
                                        {
                                            int j = idx[0];
                                            int i = idx[1];

                                            output_acc[j][i] = input_acc[crop_x_offset + j][crop_y_offset + i];
                                        });
                     });
        q.wait_and_throw();
    }
    catch (sycl::exception e)
    {
        cout << "SYCL exception caught: " << e.what() << "\n";
    }

    kernel_time("kernel time: ", e);
}
