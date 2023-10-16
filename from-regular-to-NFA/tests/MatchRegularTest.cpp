#include <gtest/gtest.h>
#include "../functions.h"

TEST(MatchRegularTest, Negative) {
  EXPECT_FALSE(MatchRegular("ab+c.aba.*.bac.+.+*", 'a', 4));
}

TEST(MatchRegularTest, Positive) {
  EXPECT_TRUE(MatchRegular("acb..bab.c..ab.ba.+.+*a.", 'b', 2));
}
