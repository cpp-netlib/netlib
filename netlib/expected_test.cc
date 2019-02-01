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

using ::testing::Eq;

using error_code = int;

enum errors : error_code { undefined };

expected<bool, error_code> f(bool b) {
  if (b) return true;
  return unexpected(errors::undefined);
}

// For testing support for larger types.
struct test_data {
  int64_t first;
  int64_t second;
};

expected<test_data, error_code> g(bool b, int64_t first, int64_t second) {
  if (b) return unexpected{errors::undefined};
  return test_data{first, second};
}

TEST(ExpectedTest, Construction) { expected<bool, error_code> e; }

TEST(ExpectedTest, Unexpected) {
  auto e1 = f(true);
  ASSERT_TRUE(e1);

  auto e2 = f(false);
  ASSERT_FALSE(e2);
  ASSERT_THROW(*e2, bad_expected_access<error_code>);

  auto e3 = g(true, 1, 2);
  ASSERT_FALSE(e3);
  EXPECT_THAT(e3.error(), Eq(errors::undefined));
  ASSERT_THROW(*e3, bad_expected_access<error_code>);

  e3 = g(false, 2, 3);
  ASSERT_TRUE(e3);
  ASSERT_THAT(e3->first, Eq(2));
  ASSERT_THAT(e3->second, Eq(3));
}

}  // namespace
}  // namespace cppnetlib
