
#include <gtest/gtest.h>

#include "project/math.hpp"

TEST(Math, Add)
{
    EXPECT_EQ(project::add(2, 3), 5);
}
