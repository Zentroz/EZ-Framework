#ifndef SPARSESET_CLASS_H
#define SPARSESET_CLASS_H

#include<vector>
#include"ErrorHandler.h"

template<typename T>
class SparseSet {
public:
    unsigned int size;

    SparseSet(int capacity);

	T* get(unsigned int key) {
		if (sparse[key] == -1) {
			return nullptr;
		}

		return dense[sparse[key]];
	}

	void insert(unsigned int key, T* value) {
		if (sparse[key] != -1) {
			EXCEPTION(("Value already exists. Key: " + std::to_string(key)).c_str());
			return;
		}

		dense.push_back(value);
		sparse[key] = size;
		size++;
	}

	void remove(unsigned int key) {
		if (sparse[key] == -1) {
			EXCEPTION("Value does not exist.");
			return;
		}

		size_t backIndex = 0;

		for (size_t i = 0; i < sizeof(sparse); i++) {
			if (sparse[i] == dense.size() - 1) {
				backIndex = i;
				break;
			}
		}

		std::swap(dense[sparse[key]], dense.back());
		dense.pop_back();

		sparse[backIndex] = sparse[key];
		sparse[key] = -1;
		size--;
	}

	std::vector<T*> getAll() {
		return dense;
	}

	void clear() {
		for (unsigned int i = 0; i < capacity; i++) {
			sparse[i] = -1;
		}
		dense.clear();
		size = 0;
	}

private:
	unsigned int* sparse;
	std::vector<T*> dense;
    unsigned int capacity;
};

template<typename T>
SparseSet<T>::SparseSet(int capacity) {
	sparse = new unsigned int[capacity]();

	for (unsigned int i = 0; i < capacity; i++) {
		sparse[i] = -1;
	}
	
	this->capacity = capacity;
	size = 0;
}

#endif