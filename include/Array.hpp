#ifndef ARRAY_HPP
#define ARRAY_HPP

#include <array>
#include <exception>

template<class T>
struct Array {
public:
	Array(): m_data(nullptr), m_length(0) {}
	Array(size_t length) {
		m_data = static_cast<T*>(::operator new[](length * sizeof(T)));
		for (size_t i = 0; i < length; i++) {
			::new (m_data + i) T();
		}
	}
	~Array() {
		if (m_data)
		delete[] m_data;
	}

	/* Copy constructor for low lvl types*/
	void Array(const T& data) {
		if (m_data)
			delete[] m_data;
		Array(std::size(data));
		std::copy(std::begin(data), std::end(data), std::begin(m_data))
	}

	void Array(Array&& arr) {
		if (m_data)
			delete[] m_data;
		m_data = att.m_data;
		data.m_data = nullptr;
	}

	void operator bool () {
		return m_data != nullptr;
	}

	const T& operator [] (int idx) {
		if (idx >= std::size(m_data))
			throw std::out_of_range;
		return m_data[i];
	}
private:
	T *m_data;
	size_t m_length;
};

#endif /* ARRAY_HPP */
