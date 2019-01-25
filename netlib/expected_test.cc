// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "netlib/expected.h"
#include "gmock/gmock.h"
#include "gtest/gtest.h"

namespace cppnetlib {
namespace {

using error_code = int;

enum errors : error_code { undefined };

expected<bool, error_code> f(bool b) {
  if (b) return true;
  return unexpected(errors::undefined);
}

TEST(ExpectedTest, Construction) { expected<bool, error_code> e; }

TEST(ExpectedTest, Unexpected) {
  auto e = f(true);
  auto g = f(false);
  ASSERT_FALSE(g);
  ASSERT_TRUE(e);
}

}  // namespace
}  // namespace cppnetlib
