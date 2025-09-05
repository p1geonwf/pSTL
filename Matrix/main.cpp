#include <cassert>
#include <cmath>
#include <iostream>
#include "Matrix.h"

template <typename T>
bool matricesEqual(const Matrix<T>& A, const Matrix<T>& B) {
    if (A.rows() != B.rows() || A.cols() != B.cols())
        return false;
    for (size_t i = 0; i < A.rows(); i++)
        for (size_t j = 0; j < A.cols(); j++)
            if (A[i][j] != B[i][j])
                return false;
    return true;
}

bool matricesEqualDouble(const Matrix<double>& A, const Matrix<double>& B, const double tol = 1e-6) {
    if (A.rows() != B.rows() || A.cols() != B.cols())
        return false;
    for (size_t i = 0; i < A.rows(); i++)
        for (size_t j = 0; j < A.cols(); j++)
            if (std::fabs(A[i][j] - B[i][j]) > tol)
                return false;
    return true;
}

int main() {
    // Construction
    Matrix<int> A{ {1, 2}, {3, 4} };
    assert(A.rows() == 2 && A.cols() == 2);
    assert(A[0][0] == 1 && A[1][1] == 4);

    // Addition and Subtraction
    Matrix<int> B{ {5, 6}, {7, 8} };
    Matrix<int> C = A + B;
    Matrix<int> expectedAdd{ {6, 8}, {10, 12} };
    assert(matricesEqual(C, expectedAdd));

    Matrix<int> D = B - A;
    Matrix<int> expectedSub{ {4, 4}, {4, 4} };
    assert(matricesEqual(D, expectedSub));

    // Matrix Multiplication (Standard)
    Matrix<int> E = A * B;
    Matrix<int> expectedMult{ {19, 22}, {43, 50} };
    assert(matricesEqual(E, expectedMult));

    // Element-wise Multiplication and Division
    Matrix<int> F = A.cwiseMul(B);
    Matrix<int> expectedElemMult{ {5, 12}, {21, 32} };
    assert(matricesEqual(F, expectedElemMult));

    Matrix<int> G{ {10, 20}, {30, 40} };
    Matrix<int> H{ {2, 4}, {5, 8} };
    Matrix<int> I = G.cwiseDiv(H);
    Matrix<int> expectedElemDiv{ {5, 5}, {6, 5} };
    assert(matricesEqual(I, expectedElemDiv));

    // Scalar Multiplication and Division (using overloaded operators)
    Matrix<double> M{ {1.0, 2.0}, {3.0, 4.0} };

    // non‑mutating multiply
    Matrix<double> M2 = M * 2.0;
    Matrix<double> expectedScalarMult{ {2.0, 4.0}, {6.0, 8.0} };
    assert(matricesEqualDouble(M2, expectedScalarMult));

    // commutative friend
    M2 = 2.0 * M;
    assert(matricesEqualDouble(M2, expectedScalarMult));

    // non‑mutating divide
    Matrix<double> M3 = M / 2.0;
    Matrix<double> expectedScalarDiv{ {0.5, 1.0}, {1.5, 2.0} };
    assert(matricesEqualDouble(M3, expectedScalarDiv));

    // mutating operators
    M2 = M;
    M2 *= 2.0;
    assert(matricesEqualDouble(M2, expectedScalarMult));

    M3 = M;
    M3 /= 2.0;
    assert(matricesEqualDouble(M3, expectedScalarDiv));

    // Transpose (In-place)
    Matrix<int> T{ {1, 2, 3}, {4, 5, 6} };
    T.transpose();
    Matrix<int> expectedTranspose{ {1, 4}, {2, 5}, {3, 6} };
    assert(matricesEqual(T, expectedTranspose));

    // 2x2 determinant
    int detA = A.determinant();
    assert(detA == -2);

    // 3x3 determinant
    Matrix<int> threeByThree{ {1, 2, 3}, {0, 4, 5}, {1, 0, 6} };
    int det3 = threeByThree.determinant();
    assert(det3 == 22);

    // Inverse (2x2 example)
    Matrix<double> mat2x2{ {4.0, 7.0}, {2.0, 6.0} };
    Matrix<double> inv2x2 = mat2x2.inverse();
    Matrix<double> expectedInv2x2{ {0.6, -0.7}, {-0.2, 0.4} };
    assert(matricesEqualDouble(inv2x2, expectedInv2x2));

    // Identity check
    Matrix<double> identity = mat2x2 * inv2x2;
    Matrix<double> expectedIdentity{ {1.0, 0.0}, {0.0, 1.0} };
    assert(matricesEqualDouble(identity, expectedIdentity, 1e-6));

    // Test Exception Cases
    bool caught = false;
    try {
        Matrix<int> X{ {1, 2}, {3, 4} };
        Matrix<int> Y{ {1, 2, 3}, {4, 5, 6} };
        Matrix<int> Z = X + Y;
    }
    catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);

    std::cout << "All tests passed!" << std::endl;
    return 0;
}
