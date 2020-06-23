#include "mask.h"

#include <gtest/gtest.h>
#include <sstream>
#include <iostream>

using namespace testing;
using namespace Otus;

TEST(mask, main) { 
  ASSERT_TRUE(Mask("a").Valid("a"));
  ASSERT_FALSE(Mask("a").Valid("b"));
  ASSERT_TRUE(Mask("a*").Valid("a"));
  ASSERT_FALSE(Mask("a?").Valid("a"));
  ASSERT_TRUE(Mask("a?").Valid("ab"));
  ASSERT_TRUE(Mask("*b").Valid("ab"));
  ASSERT_TRUE(Mask("a*b").Valid("acb"));
  ASSERT_FALSE(Mask("a*b").Valid("abc"));
  ASSERT_TRUE(Mask("a.b").Valid("a.b"));
  ASSERT_TRUE(Mask(".b").Valid(".b"));
  ASSERT_TRUE(Mask("a.").Valid("a."));
  ASSERT_TRUE(Mask("*.*").Valid("blabla.bla"));
}

TEST(mask, case_insensitive) { 
  ASSERT_TRUE(Mask("aBcDe").Valid("AbCdE"));
  ASSERT_TRUE(Mask("a*b").Valid("AwErTyb"));
  ASSERT_TRUE(Mask("a?").Valid("AB"));
}