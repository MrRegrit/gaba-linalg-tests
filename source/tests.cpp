#include <iostream>
#include <matrix.h>

#include <gtest/gtest.h>


TEST(MatrixState, DefaultIsEmpty)
{
    linalg::Matrix m;
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.rows(), 0u);
    EXPECT_EQ(m.columns(), 0u);
    EXPECT_EQ(m.size(), 0u);
    EXPECT_EQ(m.capacity(), 0u);
}
