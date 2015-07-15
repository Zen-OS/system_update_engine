// Copyright (c) 2009 The Chromium OS Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "update_engine/omaha_hash_calculator.h"

#include <math.h>
#include <unistd.h>

#include <string>
#include <vector>

#include <chromeos/secure_blob.h>
#include <glib.h>
#include <gtest/gtest.h>

#include "update_engine/libcurl_http_fetcher.h"
#include "update_engine/utils.h"

using std::string;
using std::vector;

namespace chromeos_update_engine {

// Generated by running this on a linux shell:
// $ echo -n hi | openssl dgst -sha256 -binary | openssl base64
static const char kExpectedHash[] =
    "j0NDRmSPa5bfid2pAcUXaxCm2Dlh3TwayItZstwyeqQ=";
static const uint8_t kExpectedRawHash[] = {
  0x8f, 0x43, 0x43, 0x46, 0x64, 0x8f, 0x6b, 0x96,
  0xdf, 0x89, 0xdd, 0xa9, 0x01, 0xc5, 0x17, 0x6b,
  0x10, 0xa6, 0xd8, 0x39, 0x61, 0xdd, 0x3c, 0x1a,
  0xc8, 0x8b, 0x59, 0xb2, 0xdc, 0x32, 0x7a, 0xa4
};

class OmahaHashCalculatorTest : public ::testing::Test {
 public:
  OmahaHashCalculatorTest() {}
};

TEST_F(OmahaHashCalculatorTest, SimpleTest) {
  OmahaHashCalculator calc;
  calc.Update("hi", 2);
  calc.Finalize();
  EXPECT_EQ(kExpectedHash, calc.hash());
  chromeos::Blob raw_hash(std::begin(kExpectedRawHash),
                          std::end(kExpectedRawHash));
  EXPECT_TRUE(raw_hash == calc.raw_hash());
}

TEST_F(OmahaHashCalculatorTest, MultiUpdateTest) {
  OmahaHashCalculator calc;
  calc.Update("h", 1);
  calc.Update("i", 1);
  calc.Finalize();
  EXPECT_EQ(kExpectedHash, calc.hash());
  chromeos::Blob raw_hash(std::begin(kExpectedRawHash),
                          std::end(kExpectedRawHash));
  EXPECT_TRUE(raw_hash == calc.raw_hash());
}

TEST_F(OmahaHashCalculatorTest, ContextTest) {
  OmahaHashCalculator calc;
  calc.Update("h", 1);
  string calc_context = calc.GetContext();
  calc.Finalize();
  OmahaHashCalculator calc_next;
  calc_next.SetContext(calc_context);
  calc_next.Update("i", 1);
  calc_next.Finalize();
  EXPECT_EQ(kExpectedHash, calc_next.hash());
  chromeos::Blob raw_hash(std::begin(kExpectedRawHash),
                          std::end(kExpectedRawHash));
  EXPECT_TRUE(raw_hash == calc_next.raw_hash());
}

TEST_F(OmahaHashCalculatorTest, BigTest) {
  OmahaHashCalculator calc;

  int digit_count = 1;
  int next_overflow = 10;
  for (int i = 0; i < 1000000; i++) {
    char buf[8];
    if (i == next_overflow) {
      next_overflow *= 10;
      digit_count++;
    }
    ASSERT_EQ(digit_count, snprintf(buf, sizeof(buf), "%d", i)) << " i = " << i;
    calc.Update(buf, strlen(buf));
  }
  calc.Finalize();

  // Hash constant generated by running this on a linux shell:
  // $ C=0
  // $ while [ $C -lt 1000000 ]; do
  //     echo -n $C
  //     let C=C+1
  //   done | openssl dgst -sha256 -binary | openssl base64
  EXPECT_EQ("NZf8k6SPBkYMvhaX8YgzuMgbkLP1XZ+neM8K5wcSsf8=", calc.hash());
}

TEST_F(OmahaHashCalculatorTest, UpdateFileSimpleTest) {
  string data_path;
  ASSERT_TRUE(
      utils::MakeTempFile("data.XXXXXX", &data_path, nullptr));
  ScopedPathUnlinker data_path_unlinker(data_path);
  ASSERT_TRUE(utils::WriteFile(data_path.c_str(), "hi", 2));

  static const int kLengths[] = { -1, 2, 10 };
  for (size_t i = 0; i < arraysize(kLengths); i++) {
    OmahaHashCalculator calc;
    EXPECT_EQ(2, calc.UpdateFile(data_path, kLengths[i]));
    EXPECT_TRUE(calc.Finalize());
    EXPECT_EQ(kExpectedHash, calc.hash());
    chromeos::Blob raw_hash(std::begin(kExpectedRawHash),
                            std::end(kExpectedRawHash));
    EXPECT_TRUE(raw_hash == calc.raw_hash());
  }

  OmahaHashCalculator calc;
  EXPECT_EQ(0, calc.UpdateFile(data_path, 0));
  EXPECT_EQ(1, calc.UpdateFile(data_path, 1));
  EXPECT_TRUE(calc.Finalize());
  // echo -n h | openssl dgst -sha256 -binary | openssl base64
  EXPECT_EQ("qqlAJmTxpB9A67xSyZk+tmrrNmYClY/fqig7ceZNsSM=", calc.hash());
}

TEST_F(OmahaHashCalculatorTest, RawHashOfFileSimpleTest) {
  string data_path;
  ASSERT_TRUE(
      utils::MakeTempFile("data.XXXXXX", &data_path, nullptr));
  ScopedPathUnlinker data_path_unlinker(data_path);
  ASSERT_TRUE(utils::WriteFile(data_path.c_str(), "hi", 2));

  static const int kLengths[] = { -1, 2, 10 };
  for (size_t i = 0; i < arraysize(kLengths); i++) {
    chromeos::Blob exp_raw_hash(std::begin(kExpectedRawHash),
                                std::end(kExpectedRawHash));
    chromeos::Blob raw_hash;
    EXPECT_EQ(2, OmahaHashCalculator::RawHashOfFile(data_path,
                                                    kLengths[i],
                                                    &raw_hash));
    EXPECT_TRUE(exp_raw_hash == raw_hash);
  }
}

TEST_F(OmahaHashCalculatorTest, UpdateFileNonexistentTest) {
  OmahaHashCalculator calc;
  EXPECT_EQ(-1, calc.UpdateFile("/some/non-existent/file", -1));
}

TEST_F(OmahaHashCalculatorTest, AbortTest) {
  // Just make sure we don't crash and valgrind doesn't detect memory leaks
  {
    OmahaHashCalculator calc;
  }
  {
    OmahaHashCalculator calc;
    calc.Update("h", 1);
  }
}

}  // namespace chromeos_update_engine
