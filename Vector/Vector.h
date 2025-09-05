#pragma once

#include <stdexcept> 
#include <utility>

namespace pSTL {
	template <typename T>
	class Vector {
	public:
		/********************** Constructors **********************/

		Vector() noexcept
			: m_size(0), m_capacity(0), m_arr(nullptr) {
		}

		explicit Vector(size_t size)
			: m_size(size), m_capacity(size ? size * 3 / 2 : 0), m_arr(m_capacity ? new T[m_capacity]{} : nullptr) {
		}

		Vector(size_t size, T initValue)
			: m_size(size), m_capacity(size * 3 / 2), m_arr(m_capacity ? new T[m_capacity] : nullptr) {

			// one compile time check and then runtime for unknown variables
			static_assert(size > 0, "Vector(size, initValue): size must be > 0");
			if (size == 0) throw std::invalid_argument("Vector(size, initValue): size must be > 0");
			
			for (int i = 0; i < size; i++) {
				m_arr[i] = initValue;
			}
		}

		Vector(const Vector& other) noexcept
			: m_size(other.m_size),
			m_capacity(other.m_capacity),
			m_arr(other.m_capacity ? new T[other.m_capacity] : nullptr) {
			for (int i = 0; i < m_size; i++) {
				m_arr[i] = other.m_arr[i];
			}
		}

		Vector(Vector&& other) noexcept
			: m_size(other.m_size), m_capacity(other.m_capacity), m_arr(other.m_arr) {
			other.m_size = 0;
			other.m_capacity = 0;
			other.m_arr = nullptr;
		}

		Vector& operator=(const Vector& other) noexcept {
			Vector temp = other;
			std::swap(*this, temp);

			return *this;
		}

		Vector& operator=(Vector&& other) noexcept {
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
			std::swap(m_arr, other.m_arr);

			return *this;
		}

		~Vector() {
			delete[] m_arr;
		}


		/********************** Getters **********************/

		T& at(size_t index) {
			if (index >= m_size) throw std::out_of_range("Out of bound access!");
			return m_arr[index];
		}
		const T& at(size_t index) const {
			if (index >= m_size) throw std::out_of_range("Out of bound access!");
			return m_arr[index];
		}

		T& operator[](size_t index) {
			return m_arr[index];
		}
		const T& operator[](size_t index) const {
			return m_arr[index];
		}

		T& front() {
			if (m_size == 0) throw std::out_of_range("Array is empty!");
			return m_arr[0];
		}
		const T& front() const {
			if (m_size == 0) throw std::out_of_range("Array is empty!");
			return m_arr[0];
		}

		T& back() {
			if (m_size == 0) throw std::out_of_range("Array is empty!");
			return m_arr[m_size - 1];
		}
		const T& back() const {
			if (m_size == 0) throw std::out_of_range("Array is empty!");
			return m_arr[m_size - 1];
		}

		size_t size() const { return m_size; }
		size_t capacity() const { return m_capacity; }
		bool empty() const noexcept { return m_size == 0; }


		/********************** Getters **********************/

		T* begin() noexcept { return m_arr; }
		T* end()   noexcept { return m_arr + m_size; }
		const T* begin() const noexcept { return m_arr; }
		const T* end()   const noexcept { return m_arr + m_size; }
		const T* cbegin() const noexcept { return m_arr; }
		const T* cend()   const noexcept { return m_arr + m_size; }


		/********************** Setters **********************/

		void reserve(size_t newCapacity) {
			if (newCapacity <= m_capacity) return;

			T* tempArr = new T[newCapacity];
			for (size_t i = 0; i < m_size; i++) {
				tempArr[i] = std::move(m_arr[i]);
			}

			m_capacity = newCapacity;
			std::swap(m_arr, tempArr);
			delete[] tempArr;
		}

		void push_back(const T& val) {
			if (m_size == m_capacity) {
				size_t newCapacity = m_capacity * 3 / 2;
				if (newCapacity == 0) {
					newCapacity = 1;
				}

				reserve(newCapacity);
			}
			m_arr[m_size++] = val;
		}
		void push_back(T&& val) {
			if (m_size == m_capacity) {
				size_t newCapacity = m_capacity * 3 / 2 + 1;
				if (newCapacity == 0) {
					newCapacity = 1;
				}

				reserve(newCapacity);
			}
			m_arr[m_size++] = std::move(val);
		}

		void insert_at(size_t index, const T& val) {
			if (index > m_size) throw std::out_of_range("insert_at: index out of range");

			if (m_size == m_capacity) {
				size_t newCap = m_capacity ? (m_capacity + m_capacity / 2) : 1;
				reserve(newCap);
			}

			for (size_t i = m_size; i > index; i--) {
				m_arr[i] = std::move(m_arr[i - 1]);
			}

			m_arr[index] = val;                  
			m_size++;
		}

		void pop_back() {
			if (m_size == 0) throw std::out_of_range("pop_back on empty array");

			if constexpr (std::is_pointer<T>::value) {
				delete m_arr[m_size - 1];
				
			} 
			m_size--;
		}


		/********************** Utility **********************/

		static constexpr size_t npos = static_cast<size_t>(-1);
		size_t find(const T& val) const {
			for (size_t i = 0; i < m_size; ++i) if (m_arr[i] == val) return i;
			return npos;
		}

		void swap(Vector& other) noexcept {
			std::swap(m_size, other.m_size);
			std::swap(m_capacity, other.m_capacity);
			std::swap(m_arr, other.m_arr);
		}

	private:
		size_t m_size;
		size_t m_capacity;
		T* m_arr;
	};
}