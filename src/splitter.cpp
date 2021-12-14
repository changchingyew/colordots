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
    int crop_offset_x = 600;
    int crop_offset_y = 600;

    range input_range(width*height);
    range<2> output_range(WIDTH_CROP, HEIGHT_CROP);

    buffer<rgb, 1> input_buf(input, input_range);
    buffer<rgb, 2> output_buf(output, output_range);

#if 1
    gpu_selector sel;
#else
    cpu_selector sel;
#endif
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

                         h.parallel_for(output_range, [=](id<2> idx)
                                        {
                                            int row = idx[0];
                                            int col = idx[1];

                                            output_acc[row][col] = input_acc[(row + crop_offset_y) * width + (col + crop_offset_x)];
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
