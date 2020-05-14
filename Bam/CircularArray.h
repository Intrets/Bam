#pragma once
#include <vector>

template <class T>
class CircularArray
{
public:

	int32_t index;
	int32_t size;
	//double* arr;
	std::vector<T> arr;
	T total;

	CircularArray(T, int32_t);

	void insert(T);
	T sum(T);
	T average(T initial);

	CircularArray() = delete;
};

template<class T>
CircularArray<T>::CircularArray(T value, int32_t s) {
	size = s;
	index = 0;
	total = s*value;
	arr.assign(s, value);
}

template<class T>
void CircularArray<T>::insert(T value) {
	index = (index + 1) % size;
	total -= arr[index];
	arr[index] = value;
	total += arr[index];
}

template<class T>
inline T CircularArray<T>::sum(T) {
	return total;
}

template<class T>
inline T CircularArray<T>::average(T initial) {
	return sum(initial)/(static_cast<T>(arr.size()));
}

