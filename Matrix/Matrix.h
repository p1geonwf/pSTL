#pragma once

#include <iostream>
#include <vector>
#include <stdexcept>

namespace pSTL {
	template <typename T>
	class Matrix {
	public:
		Matrix() = default;
		explicit Matrix(size_t rows, size_t cols) : m_matrix(rows, std::vector<T>(cols, T{})) {}
		explicit Matrix(size_t rows, size_t cols, T val) : m_matrix(rows, std::vector<T>(cols, val)) {}
		Matrix(std::initializer_list<std::initializer_list<T>> init) {
			for (auto& row : init) {
				m_matrix.emplace_back(row);
			}
		}

		// No need for big 5 explicit declarations, did it just for practice
		~Matrix() = default;
		Matrix(const Matrix& other) noexcept : m_matrix(other.m_matrix) {}
		Matrix(Matrix&& other) noexcept : m_matrix(std::move(other.m_matrix)) {}
		Matrix& operator=(const Matrix& other) noexcept {
			if (this != &other) {
				m_matrix = other.m_matrix;
			}
			return *this;
		}
		Matrix& operator=(Matrix&& other) noexcept {
			if (this != &other) {
				m_matrix = std::move(other.m_matrix);
			}
			return *this;
		}

		size_t rows() const {
			return m_matrix.size();
		}

		size_t cols() const {
			return rows() ? m_matrix[0].size() : 0;
		}

		void printMatrix() const {
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					std::cout << m_matrix[i][j] << " ";
				}
				std::cout << std::endl;
			}
		}

		const std::vector<T>& operator[](size_t row) const { // Accessor
			if (row >= rows()) {
				throw std::out_of_range("Index out of range!");
			}
			return m_matrix[row];
		}
		std::vector<T>& operator[](size_t row) { // Mutator
			if (row >= rows()) {
				throw std::out_of_range("Index out of range!");
			}
			return m_matrix[row];
		}

		Matrix operator+(const Matrix& other) const {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for addition!");
			}

			Matrix temp(rows(), cols());
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					temp[i][j] = m_matrix[i][j] + other[i][j];
				}
			}
			return temp;
		}

		Matrix operator-(const Matrix& other) const {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for subtraction!");
			}

			Matrix temp(rows(), cols());
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					temp[i][j] = m_matrix[i][j] - other[i][j];
				}
			}
			return temp;
		}

		Matrix operator*(const Matrix& other) const {
			if (cols() != other.rows()) {
				throw std::invalid_argument("Matrix dimensions must be compatible for multiplication!");
			}

			Matrix temp(rows(), other.cols(), T{});

			for (size_t i = 0; i < temp.rows(); i++) {
				for (size_t j = 0; j < temp.cols(); j++) {
					for (size_t k = 0; k < cols(); k++) {
						temp[i][j] += m_matrix[i][k] * other.m_matrix[k][j];
					}
				}
			}

			return temp;
		}

		Matrix& operator+=(const Matrix& other) {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for addition!");
			}

			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					m_matrix[i][j] += other[i][j];
				}
			}
			return *this;
		}

		Matrix& operator-=(const Matrix& other) {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for subtraction!");
			}

			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					m_matrix[i][j] -= other[i][j];
				}
			}
			return *this;
		}

		Matrix& operator*=(const T& scalar) {
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					m_matrix[i][j] *= scalar;
				}
			}
			return *this;
		}

		Matrix operator*(const T& scalar) const {
			Matrix result(*this);
			result *= scalar;
			return result;
		}

		friend Matrix operator*(const T& scalar, Matrix& m) {
			return m * scalar;
		}

		Matrix cwiseMul(const Matrix& other) {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for multiplication!");
			}
			Matrix temp(rows(), cols());

			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					temp[i][j] = m_matrix[i][j] * other[i][j];
				}
			}
			return temp;
		}

		Matrix& operator/=(const T& scalar) {
			if (scalar == T{}) {
				throw std::invalid_argument("Can't divide by zero");
			}
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					m_matrix[i][j] /= scalar;
				}
			}
			return *this;
		}

		Matrix operator/(const T& scalar) const {
			Matrix result(*this);
			result /= scalar;
			return result;
		}

		Matrix cwiseDiv(const Matrix& other) {
			if (rows() != other.rows() || cols() != other.cols()) {
				throw std::invalid_argument("Matrix dimensions must match for division!");
			}
			Matrix temp(rows(), cols());

			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					if (other[i][j] == T{}) {
						throw std::invalid_argument("Can't divide by zero");
					}
					temp[i][j] = m_matrix[i][j] / other[i][j];
				}
			}
			return temp;
		}

		void transpose() {
			Matrix temp(cols(), rows());
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					temp[j][i] = m_matrix[i][j];
				}
			}
			m_matrix = std::move(temp.m_matrix);
		}

		Matrix transposed() const {
			Matrix temp(cols(), rows());
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < cols(); j++) {
					temp[j][i] = m_matrix[i][j];
				}
			}
			return temp;
		}

		T determinant() const {
			if (rows() != cols()) {
				throw std::logic_error("Determinant is only defined for square matrices.");
			}

			if (rows() == 1) {
				return m_matrix[0][0];
			}

			if (rows() == 2) {
				return m_matrix[0][0] * m_matrix[1][1] - m_matrix[0][1] * m_matrix[1][0];
			}

			T det = {};
			for (size_t j = 0; j < rows(); j++) {
				Matrix subMatrix(rows() - 1, rows() - 1);
				for (size_t i = 1; i < rows(); i++) {
					size_t subCol = 0;
					for (size_t k = 0; k < rows(); k++) {
						if (k == j)
							continue;
						subMatrix[i - 1][subCol] = m_matrix[i][k];
						subCol++;
					}
				}
				T sign = (j % 2 == 0) ? T{ 1 } : T{ -1 };
				det += sign * m_matrix[0][j] * subMatrix.determinant();
			}
			return det;
		}

		Matrix inverse() const {
			if (rows() != cols()) {
				throw std::logic_error("Matrix must be square!");
			}

			T det = determinant();
			if (det == T{}) {
				throw std::logic_error("Determinant must be non zero to invert matrix!");
			}

			Matrix cofactor(rows(), rows());
			for (size_t i = 0; i < rows(); i++) {
				for (size_t j = 0; j < rows(); j++) {
					Matrix minor(rows() - 1, rows() - 1);
					for (size_t r = 0, mi = 0; r < rows(); r++) {
						if (r == i)
							continue;
						for (size_t c = 0, mj = 0; c < rows(); c++) {
							if (c == j)
								continue;
							minor[mi][mj] = m_matrix[r][c];
							mj++;
						}
						mi++;
					}
					T sign = ((i + j) % 2 == 0) ? T{ 1 } : T{ -1 };
					cofactor[i][j] = sign * minor.determinant();
				}
			}

			Matrix adjugate = cofactor.transposed();

			Matrix inv(rows(), rows());
			for (size_t i = 0; i < rows(); ++i) {
				for (size_t j = 0; j < rows(); ++j) {
					inv[i][j] = adjugate[i][j] / det;
				}
			}

			return inv;
		}

	private:
		std::vector<std::vector<T>> m_matrix;
	};
}