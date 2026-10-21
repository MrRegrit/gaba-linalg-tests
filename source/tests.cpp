#include <iostream>
#include <matrix.h>

#include <iostream>
#include <sstream>

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
    linalg::Matrix a = { {0.3, 0.3} };
    linalg::Matrix sum = a * 3;
    linalg::Matrix nine = { {0.9, 0.9} };
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

TEST(MatrixOutput, PrintsAlignedRows)
{
    linalg::Matrix m = { {123, 2, 3}, {4, 5, 6} };
    std::ostringstream ss;
    ss << m;
    std::string out = ss.str();
    EXPECT_EQ(out, "|123   2   3|\n|  4   5   6|\n");
}

TEST(MatrixComputations, Norm)
{
    linalg::Matrix m = { {3, 4} };
    EXPECT_DOUBLE_EQ(m.norm(), 5.0);
}

TEST(MatrixComputations, Trace)
{
    linalg::Matrix m = { {1, 2}, {3, 4} };
    EXPECT_DOUBLE_EQ(m.trace(), 5.0);
}

TEST(MatrixComputations, TraceNonSquareThrows)
{
    linalg::Matrix m(2, 3);
    EXPECT_THROW(m.trace(), std::runtime_error);
}

TEST(MatrixComputations, Determinant)
{
    linalg::Matrix m = { {1, 2}, {3, 4} };
    EXPECT_DOUBLE_EQ(m.det(), -2.0);
}

TEST(MatrixComputations, DeterminantSingular)
{
    linalg::Matrix m = { {1, 2}, {2, 4} }; // вырожденная
    EXPECT_DOUBLE_EQ(m.det(), 0.0);
}

TEST(MatrixComputations, Rank)
{
    linalg::Matrix m = { {1, 2, 3}, {2, 4, 6}, {0, 1, 1} };
    EXPECT_EQ(m.rank(), 2u);
}

TEST(MatrixComputations, GaussForwardDoesNotChangeArgumentInDet)
{
    // det и rank не должны менять исходную матрицу
    linalg::Matrix m = { {1, 2}, {3, 4} };
    linalg::Matrix original = m;
    m.det();
    m.rank();
    EXPECT_EQ(m, original);
}

TEST(MatrixComputations, Concatenate)
{
    linalg::Matrix a = { {1, 2}, {3, 4} };
    linalg::Matrix b = { {5}, {6} };
    linalg::Matrix r = concatenate(a, b);
    EXPECT_EQ(r.rows(), 2u);
    EXPECT_EQ(r.columns(), 3u);
    EXPECT_DOUBLE_EQ(r(0, 2), 5.0);
    EXPECT_DOUBLE_EQ(r(1, 2), 6.0);
}

TEST(MatrixComputations, Transpose)
{
    linalg::Matrix m = { {1, 2, 3}, {4, 5, 6} };
    linalg::Matrix t = transpose(m);
    EXPECT_EQ(t.rows(), 3u);
    EXPECT_EQ(t.columns(), 2u);
    EXPECT_DOUBLE_EQ(t(2, 1), 6.0);
    // Исходная матрица не меняется
    EXPECT_EQ(m.rows(), 2u);
    EXPECT_EQ(m.columns(), 3u);
}

TEST(MatrixComputations, Invert)
{
    linalg::Matrix m = { {4, 7}, {2, 6} };
    linalg::Matrix inv = invert(m);
    // m * inv = E
    linalg::Matrix prod = m * inv;
    EXPECT_NEAR(prod(0, 0), 1.0, 1e-9);
    EXPECT_NEAR(prod(0, 1), 0.0, 1e-9);
    EXPECT_NEAR(prod(1, 0), 0.0, 1e-9);
    EXPECT_NEAR(prod(1, 1), 1.0, 1e-9);
    // Исходная матрица не меняется
    EXPECT_EQ(m(0, 0), 4.0);
}

TEST(MatrixComputations, InvertSingularThrows)
{
    linalg::Matrix m = { {1, 2}, {2, 4} };
    EXPECT_THROW(invert(m), std::runtime_error);
}

TEST(MatrixComputations, PowerZeroIdentity)
{
    // A^0 = E
    linalg::Matrix m = { {1, 2}, {3, 4} };
    linalg::Matrix p = power(m, 0);
    EXPECT_DOUBLE_EQ(p(0, 0), 1.0);
    EXPECT_DOUBLE_EQ(p(0, 1), 0.0);
    EXPECT_DOUBLE_EQ(p(1, 0), 0.0);
    EXPECT_DOUBLE_EQ(p(1, 1), 1.0);
}

TEST(MatrixComputations, PowerPositive)
{
    linalg::Matrix m = { {1, 1}, {1, 0} };
    linalg::Matrix p = power(m, 2);
    // m^2 = [[2,1],[1,1]]
    EXPECT_DOUBLE_EQ(p(0, 0), 2.0);
    EXPECT_DOUBLE_EQ(p(0, 1), 1.0);
    EXPECT_DOUBLE_EQ(p(1, 0), 1.0);
    EXPECT_DOUBLE_EQ(p(1, 1), 1.0);
}

TEST(MatrixComputations, TransposeInverseCommutativity)
{
    // (A^T)^-1 = (A^-1)^T
    linalg::Matrix m = { {4, 7}, {2, 6} };
    linalg::Matrix lhs = invert(transpose(m));
    linalg::Matrix rhs = transpose(invert(m));
    EXPECT_EQ(lhs, rhs);
}

TEST(MatrixComputations, InverseOfProduct)
{
    // (A1 * A2)^-1 = A2^-1 * A1^-1
    linalg::Matrix a1 = { {4, 7}, {2, 6} };
    linalg::Matrix a2 = { {3, 1}, {5, 2} };
    linalg::Matrix lhs = invert(a1 * a2);
    linalg::Matrix rhs = invert(a2) * invert(a1);
    EXPECT_EQ(lhs, rhs);
}

TEST(MatrixComputations, InversePower)
{
    // (A^-1)^6 = (A^-2)^3
    linalg::Matrix m = { {4, 7}, {2, 6} };
    linalg::Matrix lhs = power(invert(m), 6);
    linalg::Matrix rhs = power(power(invert(m), 2), 3);
    EXPECT_EQ(lhs, rhs);
}

TEST(MatrixComputations, Solve)
{
        // Решаем систему A*x = f
    linalg::Matrix a = { {3, 1}, {1, 2} };
    linalg::Matrix f = { {9}, {8} };
    linalg::Matrix x = solve(a, f);
        // Решение x = [2, 3]
    EXPECT_NEAR(x(0, 0), 2.0, 1e-9);
    EXPECT_NEAR(x(1, 0), 3.0, 1e-9);
    // A и f не меняются
    EXPECT_DOUBLE_EQ(a(0, 0), 3.0);
    EXPECT_DOUBLE_EQ(f(0, 0), 9.0);
}

// ==================== ДОПы ====================

TEST(MatrixAccess, SquareBracketRead)
{
    linalg::Matrix m = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} };
    EXPECT_DOUBLE_EQ(m[0][1], 2.0);   // 1-я строка, 2-й столбец
    EXPECT_DOUBLE_EQ(m[1][2], 6.0);
    EXPECT_DOUBLE_EQ(m[1][0], 4.0);
}

TEST(MatrixAccess, SquareBracketWrite)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    m[0][1] = 10.0;
    m[1][1] = -7.0;
    EXPECT_DOUBLE_EQ(m(0, 1), 10.0);
    EXPECT_DOUBLE_EQ(m(1, 1), -7.0);
}

TEST(MatrixAccess, SquareBracketMatchesCallOperator)
{
    linalg::Matrix m = { {1.0, 2.0, 3.0}, {4.0, 5.0, 6.0} };
    EXPECT_DOUBLE_EQ(m[0][0], m(0, 0));
    EXPECT_DOUBLE_EQ(m[1][2], m(1, 2));
    m[0][2] = 100.0;
    EXPECT_DOUBLE_EQ(m(0, 2), 100.0);
}

TEST(MatrixAccess, SquareBracketConst)
{
    const linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_DOUBLE_EQ(m[1][0], 3.0);
    EXPECT_DOUBLE_EQ(m[0][1], 2.0);
}

TEST(MatrixAccess, SquareBracketOutOfRangeRowThrows)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(m[5][0], std::runtime_error);
    const linalg::Matrix cm = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(cm[5][0], std::runtime_error);
}

TEST(MatrixAccess, SquareBracketOutOfRangeColumnThrows)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(m[0][5], std::runtime_error);
    const linalg::Matrix cm = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(cm[0][5], std::runtime_error);
}

TEST(MatrixExtraMath, Minor2x2)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    // minor = определитель подматрицы без строки и столбца
    EXPECT_DOUBLE_EQ(m.minor(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(m.minor(0, 1), 3.0);
    EXPECT_DOUBLE_EQ(m.minor(1, 0), 2.0);
    EXPECT_DOUBLE_EQ(m.minor(1, 1), 1.0);
}

TEST(MatrixExtraMath, MinorThrowsOnNonSquareOrSize1)
{
    linalg::Matrix non_square = { {1.0, 2.0, 3.0} };
    EXPECT_THROW(non_square.minor(0, 0), std::runtime_error);

    linalg::Matrix one = { {5.0} };
    EXPECT_THROW(one.minor(0, 0), std::runtime_error);
}

TEST(MatrixExtraMath, MinorOutOfRangeThrows)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(m.minor(5, 0), std::runtime_error);
    EXPECT_THROW(m.minor(0, 5), std::runtime_error);
}

TEST(MatrixExtraMath, Cofactor2x2)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    // cofactor = (-1)^(r+c) * minor
    EXPECT_DOUBLE_EQ(m.cofactor(0, 0), 4.0);
    EXPECT_DOUBLE_EQ(m.cofactor(0, 1), -3.0);
    EXPECT_DOUBLE_EQ(m.cofactor(1, 0), -2.0);
    EXPECT_DOUBLE_EQ(m.cofactor(1, 1), 1.0);
}

TEST(MatrixExtraMath, AdjugateRelationWithDetAndInverse)
{
    linalg::Matrix m = { {4.0, 7.0}, {2.0, 6.0} };
    // adj(A) = det(A) * A^-1
    linalg::Matrix adj = m.adjugate();
    linalg::Matrix expected = m.det() * invert(m);
    EXPECT_EQ(adj, expected);
}

TEST(MatrixExtraMath, AdjugateProduct)
{
    // A * adj(A) = det(A) * E
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    linalg::Matrix prod = m * m.adjugate();
    EXPECT_NEAR(prod(0, 0), m.det(), 1e-9);
    EXPECT_NEAR(prod(0, 1), 0.0, 1e-9);
    EXPECT_NEAR(prod(1, 0), 0.0, 1e-9);
    EXPECT_NEAR(prod(1, 1), m.det(), 1e-9);
}

TEST(MatrixExtraMath, AdjugateNonSquareThrows)
{
    linalg::Matrix m = { {1.0, 2.0, 3.0} };
    EXPECT_THROW(m.adjugate(), std::runtime_error);
}

TEST(MatrixExtraMath, Sum)
{
    linalg::Matrix m = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_DOUBLE_EQ(m.sum(), 10.0);
}

TEST(MatrixExtraMath, SumSingleElement)
{
    linalg::Matrix m = { {5.0} };
    EXPECT_DOUBLE_EQ(m.sum(), 5.0);
}

TEST(MatrixExtraMath, Min)
{
    linalg::Matrix m = { {4.0, -2.0, 7.0}, {1.0, 9.0, -3.0} };
    EXPECT_DOUBLE_EQ(m.min(), -3.0);
}

TEST(MatrixExtraMath, MinEmptyThrows)
{
    linalg::Matrix m;
    EXPECT_THROW(m.min(), std::runtime_error);
}

TEST(MatrixExtraMath, Max)
{
    linalg::Matrix m = { {4.0, -2.0, 7.0}, {1.0, 9.0, -3.0} };
    EXPECT_DOUBLE_EQ(m.max(), 9.0);
}

TEST(MatrixExtraMath, MaxEmptyThrows)
{
    linalg::Matrix m;
    EXPECT_THROW(m.max(), std::runtime_error);
}

TEST(MatrixExtraMath, Dot)
{
    linalg::Matrix a = { {1.0, 2.0}, {3.0, 4.0} };
    linalg::Matrix b = { {1.0, 0.0}, {0.0, 1.0} };
    // 1*1 + 2*0 + 3*0 + 4*1 = 5
    EXPECT_DOUBLE_EQ(a.dot(b), 5.0);
}

TEST(MatrixExtraMath, DotWithItselfEqualsSquaredNorm)
{
    linalg::Matrix m = { {3.0, 4.0} };
    double n = m.norm();
    EXPECT_DOUBLE_EQ(m.dot(m), n * n);
}

TEST(MatrixExtraMath, DotDifferentSizeThrows)
{
    linalg::Matrix a = { {1.0, 2.0} };
    linalg::Matrix b = { {1.0, 2.0}, {3.0, 4.0} };
    EXPECT_THROW(a.dot(b), std::runtime_error);
}

TEST(MatrixProperties, IsSquare)
{
    EXPECT_TRUE(linalg::Matrix({ {1.0, 2.0}, {3.0, 4.0} }).is_square());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 2.0, 3.0} }).is_square());
    EXPECT_FALSE(linalg::Matrix(2, 3).is_square());
    EXPECT_TRUE(linalg::Matrix(0, 0).is_square());
}

TEST(MatrixProperties, IsSymmetric)
{
    EXPECT_TRUE(linalg::Matrix({ {1.0, 2.0}, {2.0, 1.0} }).is_symmetric());
    EXPECT_TRUE(linalg::Matrix({ {5.0} }).is_symmetric());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 2.0}, {3.0, 4.0} }).is_symmetric());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 2.0, 3.0} }).is_symmetric());
}

TEST(MatrixProperties, IsIdentity)
{
    EXPECT_TRUE(linalg::Matrix({ {1.0, 0.0}, {0.0, 1.0} }).is_identity());
    EXPECT_TRUE(linalg::Matrix({ {1.0} }).is_identity());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 1.0}, {0.0, 1.0} }).is_identity());
    EXPECT_FALSE(linalg::Matrix({ {2.0, 0.0}, {0.0, 1.0} }).is_identity());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 2.0, 3.0} }).is_identity());
}

TEST(MatrixProperties, IsOrthogonal)
{
    // матрица поворота на 90°: A * A^T = E
    EXPECT_TRUE(linalg::Matrix({ {0.0, 1.0}, {-1.0, 0.0} }).is_orthogonal());
    EXPECT_TRUE(linalg::Matrix({ {1.0, 0.0}, {0.0, 1.0} }).is_orthogonal());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 1.0}, {0.0, 1.0} }).is_orthogonal());
    EXPECT_FALSE(linalg::Matrix({ {1.0, 2.0, 3.0} }).is_orthogonal());
}
