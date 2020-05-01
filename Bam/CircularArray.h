#pragma once
#include <vector>

template <class T>
class CircularArray
{
public:

	int index;
	int size;
	//double* arr;
	std::vector<T> arr;
	T total;

	CircularArray(T, int);

	void insert(T);
	T sum(T);
	T average(T initial);

	CircularArray() = delete;
};

template<class T>
CircularArray<T>::CircularArray(T value, int s) {
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

