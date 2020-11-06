# Copyright 2018 Google LLC
#
# Licensed under the Apache License, Version 2.0 (the "License"); you may not
# use this file except in compliance with the License. You may obtain a copy of
# the License at
#
# https://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
# WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
# License for the specific language governing permissions and limitations under
# the License.
cmake_minimum_required(VERSION 3.1.0)
project(astc-codec)

option(OPTION_ASTC_TESTS "Build all the unit tests." OFF)

# TODO add support for the fuzzer, it has some additional dependencies we are not
# yet bringing in.
option(OPTION_BUILD_FUZZER "Build the fuzzer tests." OFF)

set (CMAKE_CXX_STANDARD 11)
set (OPTION_ASTC_TESTS 0)

add_subdirectory(src/base)
add_subdirectory(src/decoder)
