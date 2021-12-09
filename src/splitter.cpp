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

// declare the kernel name as a global to reduce name mangling
class SplitterKernel;

static void kernel_time(const string &msg, event e) {
  cl_ulong time_start =
      e.get_profiling_info<info::event_profiling::command_start>();

  cl_ulong time_end =
      e.get_profiling_info<info::event_profiling::command_end>();

  double elapsed = (time_end - time_start) / 1e3;
  cout << msg << elapsed << " useconds\n";
}

void splitter(uint8_t *input, int width, int height, int channels, uint8_t *output)
{
    size_t num_pixels = width * height;
    size_t img_size = width * height * channels;

    size_t col_crop = 4;
    size_t row_crop = 4;
    size_t width_crop = 300;
    size_t height_crop = 300;
    size_t crop_row_size = width_crop * channels;
    size_t crop_size = width_crop * height_crop * channels;

    buffer input_buf(input, range(img_size));
    buffer output_buf(output, range(img_size));

    gpu_selector sel;
    event e;

  try {
    // Create a command queue using the device selector and request profiling
    auto prop_list = property_list{property::queue::enable_profiling()};
    queue q(sel, dpc_common::exception_handler, prop_list);

    e = q.submit([&](auto &h) {
        accessor input_acc(input_buf, h, read_only);
        accessor output_acc(output_buf, h, write_only);

        h.parallel_for(range<2>(col_crop, row_crop), [=](id<2> idx) {
            int j = idx[0];
            int i = idx[1];

            // memcpy(&output_acc, &input_acc, crop_row_size);
        });
    });
    q.wait_and_throw();
  } catch (sycl::exception e) {
    cout << "SYCL exception caught: " << e.what() << "\n";
  }

  kernel_time("kernel time: ", e);
}

