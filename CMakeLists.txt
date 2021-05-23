# Copyright 2019 Google LLC
# 
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
# 
#     https://www.apache.org/licenses/LICENSE-2.0
# 
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
cmake_minimum_required(VERSION 3.13)

enable_testing()
find_package(GTest MODULE REQUIRED)

# Include directories accessible from here.
include_directories(.)

# Require C++17 with no extensions.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add the test to the std::expected implementation.
add_executable(expected_test netlib/expected_test.cc)
target_link_libraries(expected_test PRIVATE GTest::GTest GTest::Main)
add_test(expected_test expected_test)
