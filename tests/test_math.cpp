
#include "project/math.hpp"

#include <gtest/gtest.h>

TEST(Math, Add)
{
    EXPECT_EQ(project::add(2, 3), 5);
}
