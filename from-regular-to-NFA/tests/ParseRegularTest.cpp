#include <gtest/gtest.h>
#include "../functions.h"

TEST(ParseRegularTest, NoThrow) {
  EXPECT_NO_THROW(ParseRegular("ab+c.aba.*.bac.+.+*"));
  EXPECT_NO_THROW(ParseRegular("aaa+.*"));
}

TEST(ParseRegularTest, Throw) {
  EXPECT_THROW(ParseRegular("aaa"), std::runtime_error);
  EXPECT_THROW(ParseRegular("+a"), std::runtime_error);
  EXPECT_THROW(ParseRegular("a-"), std::runtime_error);
}
