/*
 * Copyright(c) 2018 Intel Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "gtest/gtest.h"
#include <boost/log/sources/logger.hpp>
namespace src = boost::log::sources;

#include "commands_aggregation/commands_aggregation.hpp"
#include "framework/framework.hpp"

TEST(CommandsAggregationApplication, RunsWithoutThrowingExceptions) {
  compute_samples::Framework framework;
  const char *argv[] = {"compute_samples", "commands_aggregation", nullptr};
  int argc = sizeof(argv) / sizeof(argv[0]) - 1;
  EXPECT_NO_THROW(framework.run(argc, argv));
}

TEST(CommandsAggregationApplication, ComputesExpectedResultsUsingSingleQueue) {
  compute_samples::CommandsAggregationApplication app;
  const size_t global_work_size = 128;
  src::logger logger;

  const std::vector<uint32_t> output =
      app.run_workloads_in_order(global_work_size, logger);

  const size_t number_of_kernels = 10;
  const size_t number_of_iterations = 1000000;
  const std::vector<uint32_t> reference(number_of_kernels * global_work_size,
                                        number_of_iterations);
  EXPECT_EQ(output, reference);
}

TEST(CommandsAggregationApplication,
     ComputesExpectedResultsUsingMultipleQueues) {
  compute_samples::CommandsAggregationApplication app;
  const size_t global_work_size = 128;
  src::logger logger;

  const std::vector<uint32_t> output =
      app.run_workloads_out_of_order(global_work_size, logger);

  const size_t number_of_kernels = 10;
  const size_t number_of_iterations = 1000000;
  const std::vector<uint32_t> reference(number_of_kernels * global_work_size,
                                        number_of_iterations);
  EXPECT_EQ(output, reference);
}
