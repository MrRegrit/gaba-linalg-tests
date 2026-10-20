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

TEST(MatrixState, SizeAndCapacity)
{
    linalg::Matrix m(3, 4);
    EXPECT_EQ(m.rows(), 3u);
    EXPECT_EQ(m.columns(), 4u);
    EXPECT_EQ(m.size(), 12u);
    EXPECT_EQ(m.capacity(), 12u);
    EXPECT_FALSE(m.empty());
}

TEST(MatrixState, BeginEndRangeBasedLoop)
{
    linalg::Matrix m = { {1, 2, 3}, {4, 5, 6} };
    double expected[] = {1, 2, 3, 4, 5, 6};
    std::size_t i = 0;
    for (double v : m)
    {
        EXPECT_DOUBLE_EQ(v, expected[i]);
        ++i;
    }
    EXPECT_EQ(i, 6u);
}

// ==================== ПУНКТ 3: конструкторы ====================

TEST(MatrixConstructors, Default)
{
    linalg::Matrix m0;
    EXPECT_TRUE(m0.empty());
}

TEST(MatrixConstructors, SingleParam)
{
    linalg::Matrix m1(4);
    EXPECT_EQ(m1.rows(), 4u);
    EXPECT_EQ(m1.columns(), 1u);
    for (double v : m1)
        EXPECT_DOUBLE_EQ(v, 0.0);
}

TEST(MatrixConstructors, TwoParams)
{
    linalg::Matrix m2(4, 6);
    EXPECT_EQ(m2.rows(), 4u);
    EXPECT_EQ(m2.columns(), 6u);
    EXPECT_EQ(m2.size(), 24u);
    for (double v : m2)
        EXPECT_DOUBLE_EQ(v, 0.0);
}

TEST(MatrixConstructors, Copy)
{
    linalg::Matrix m1 = { {1, 2, 3}, {4, 5, 6} };
    linalg::Matrix m3(m1);
    EXPECT_EQ(m3, m1);
    m3(0, 0) = 100.0;
    EXPECT_NE(m3, m1);
}

TEST(MatrixConstructors, Move)
{
    linalg::Matrix m2 = { {1, 2, 3}, {4, 5, 6} };
    linalg::Matrix m4(std::move(m2));
    EXPECT_EQ(m4.rows(), 2u);
    EXPECT_EQ(m4.columns(), 3u);
    EXPECT_TRUE(m2.empty());
}

TEST(MatrixConstructors, InitializerListMultiRow)
{
    linalg::Matrix m5 = { {1, 2, 3}, {4, 5, 6} };
    EXPECT_EQ(m5.rows(), 2u);
    EXPECT_EQ(m5.columns(), 3u);
    EXPECT_DOUBLE_EQ(m5(1, 2), 6.0);
}

TEST(MatrixConstructors, InitializerListSingleRow)
{
    linalg::Matrix m6 = { {1, 2, 3, 4, 5, 6} };
    EXPECT_EQ(m6.rows(), 1u);
    EXPECT_EQ(m6.columns(), 6u);
}

TEST(MatrixConstructors, InitializerListColumn)
{
    linalg::Matrix m7 = { 1, 2, 3, 4, 5, 6 };
    EXPECT_EQ(m7.rows(), 6u);
    EXPECT_EQ(m7.columns(), 1u);
    EXPECT_DOUBLE_EQ(m7(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(m7(5, 0), 6.0);
}

TEST(MatrixConstructors, CopyAssignment)
{
    linalg::Matrix m2 = { {1, 2, 3}, {4, 5, 6} };
    linalg::Matrix m1(4, 6);
    m1 = m2;
    EXPECT_EQ(m1, m2);
}

TEST(MatrixConstructors, MoveAssignment)
{
    linalg::Matrix m2 = { {1, 2, 3}, {4, 5, 6} };
    linalg::Matrix m1;
    m1 = std::move(m2);
    EXPECT_EQ(m1.rows(), 2u);
    EXPECT_EQ(m1.columns(), 3u);
    EXPECT_TRUE(m2.empty());
}

TEST(MatrixConstructors, CopyAssignmentReusesCapacity)
{
    linalg::Matrix m1(10, 10);
    std::size_t cap_before = m1.capacity();
    linalg::Matrix m2 = { {1, 2}, {3, 4} };
    m1 = m2;
    EXPECT_EQ(m1.capacity(), cap_before);
    EXPECT_EQ(m1, m2);
}

// ==================== ПУНКТ 4: модификация ====================

TEST(MatrixModification, Reshape)
{
    linalg::Matrix m(2, 3);
    m.reshape(6, 1);
    EXPECT_EQ(m.rows(), 6u);
    EXPECT_EQ(m.columns(), 1u);
    EXPECT_EQ(m.capacity(), 6u);
}

TEST(MatrixModification, Reserve)
{
    linalg::Matrix m(2, 2);
    m.reserve(100);
    EXPECT_GE(m.capacity(), 100u);
    // Данные сохраняются
    m(0, 0) = 5.0;
    m.reserve(1000);
    EXPECT_DOUBLE_EQ(m(0, 0), 5.0);
    // Размер не меняется
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.columns(), 2u);
}

TEST(MatrixModification, ClearKeepsCapacity)
{
    linalg::Matrix m(3, 3);
    std::size_t cap = m.capacity();
    m.clear();
    EXPECT_TRUE(m.empty());
    EXPECT_EQ(m.capacity(), cap);
}

TEST(MatrixModification, ShrinkToFit)
{
    linalg::Matrix m(2, 2);
    m.reserve(100);
    m.shrink_to_fit();
    EXPECT_EQ(m.capacity(), 4u);
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.columns(), 2u);
}

TEST(MatrixModification, SwapMethod)
{
    linalg::Matrix a = { {1, 2} };
    linalg::Matrix b = { {3, 4}, {5, 6} };
    linalg::Matrix old_a = a;
    linalg::Matrix old_b = b;
    a.swap(b);
    EXPECT_EQ(a, old_b);
    EXPECT_EQ(b, old_a);
}

TEST(MatrixModification, SwapFreeFunction)
{
    linalg::Matrix a = { {1, 2} };
    linalg::Matrix b = { {3, 4}, {5, 6} };
    linalg::Matrix old_a = a;
    linalg::Matrix old_b = b;
    linalg::swap(a, b);
    EXPECT_EQ(a, old_b);
    EXPECT_EQ(b, old_a);
}

TEST(MatrixModification, CallOperatorReadWrite)
{
    linalg::Matrix m = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} };
    double val = m(0, 2);
    EXPECT_DOUBLE_EQ(val, 3.0);
    m(0, 2) = 7.0;
    EXPECT_DOUBLE_EQ(m(0, 2), 7.0);
}

TEST(MatrixModification, CallOperatorConst)
{
    const linalg::Matrix m = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} };
    double val = m(0, 2);
    EXPECT_DOUBLE_EQ(val, 3.0);
}

TEST(MatrixModification, OutOfRangeThrows)
{
    linalg::Matrix m(2, 2);
    EXPECT_THROW(m(2, 0), std::runtime_error);
    EXPECT_THROW(m(0, 5), std::runtime_error);
    const linalg::Matrix cm(2, 2);
    EXPECT_THROW(cm(2, 0), std::runtime_error);
}


TEST(MatrixArithmetic, UnaryPlus)
{
    linalg::Matrix m = { {1, 2} };
    linalg::Matrix r = +m;
    EXPECT_EQ(r, m);
}

TEST(MatrixArithmetic, UnaryMinus)
{
    linalg::Matrix m = { {1, -2} };
    linalg::Matrix r = -m;
    EXPECT_DOUBLE_EQ(r(0, 0), -1.0);
    EXPECT_DOUBLE_EQ(r(0, 1), 2.0);
}

TEST(MatrixArithmetic, Add)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {5, 6}, {7, 8} };
    linalg::Matrix r = a + b;
    EXPECT_DOUBLE_EQ(r(0, 0), 6.0);
    EXPECT_DOUBLE_EQ(r(1, 1), 12.0);
}

TEST(MatrixArithmetic, AddAssign)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {5, 6}, {7, 8} };
    linalg::Matrix& ref = (a += b);
    EXPECT_EQ(&ref, &a);
    EXPECT_DOUBLE_EQ(a(1, 1), 12.0);
}

TEST(MatrixArithmetic, Subtract)
{
    linalg::Matrix a = { {5, 6}, {7, 8} };
    linalg::Matrix b = { {1, 2}, {3, 4} };
    linalg::Matrix r = a - b;
    EXPECT_DOUBLE_EQ(r(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(r(1, 1), 4.0);
}

TEST(MatrixArithmetic, SubtractAssign)
{
    linalg::Matrix a = { {5, 6}, {7, 8} };
    linalg::Matrix b = { {1, 2}, {3, 4} };
    linalg::Matrix& ref = (a -= b);
    EXPECT_EQ(&ref, &a);
    EXPECT_DOUBLE_EQ(a(0, 0), 4.0);
}

TEST(MatrixArithmetic, ScalarMultiplyBothSides)
{
    linalg::Matrix m = { {1, 2}, {3, 4} };
    linalg::Matrix r1 = 2.0 * m;
    linalg::Matrix r2 = m * 2.0;
    EXPECT_DOUBLE_EQ(r1(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(r1(1, 1), 8.0);
    EXPECT_EQ(r1, r2);
}

TEST(MatrixArithmetic, ScalarMultiplyAssign)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix& ref = (a *= 3.0);
    EXPECT_EQ(&ref, &a);
    EXPECT_DOUBLE_EQ(a(1, 1), 12.0);
}

TEST(MatrixArithmetic, MatrixMultiply)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {5, 6}, {7, 8} };
    linalg::Matrix r = a * b;
    EXPECT_DOUBLE_EQ(r(0, 0), 19.0);
    EXPECT_DOUBLE_EQ(r(0, 1), 22.0);
    EXPECT_DOUBLE_EQ(r(1, 0), 43.0);
    EXPECT_DOUBLE_EQ(r(1, 1), 50.0);
}

TEST(MatrixArithmetic, MatrixMultiplyAssign)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {5, 6}, {7, 8} };
    linalg::Matrix& ref = (a *= b);
    EXPECT_EQ(&ref, &a);
    EXPECT_DOUBLE_EQ(a(0, 0), 19.0);
}

TEST(MatrixArithmetic, EqualNotEqual)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {1, 2}, {3, 4} };
    linalg::Matrix c = { {1, 2}, {3, 5} };
    EXPECT_TRUE(a == b);
    EXPECT_TRUE(a != c);
    EXPECT_FALSE(a == c);
}

TEST(MatrixArithmetic, ApproximateEquality)
{
    linalg::Matrix a = { {0.3} };
    linalg::Matrix sum = a + a + a;
    linalg::Matrix nine = { {0.9} };
    EXPECT_EQ(sum, nine);
}

TEST(MatrixArithmetic, IncompatibleSizeThrows)
{
    linalg::Matrix a(2, 3);
    linalg::Matrix b(2, 2);
    EXPECT_THROW(a + b, std::runtime_error);
    EXPECT_THROW(a * b, std::runtime_error);
    EXPECT_THROW(a += b, std::runtime_error);
}