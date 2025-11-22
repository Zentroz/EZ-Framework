#ifndef SPARSESET_CLASS_H
#define SPARSESET_CLASS_H

#include<vector>
#include<unordered_map>
#include"ErrorHandler.h"

template<typename T>
class SparseSet {
public:
	uint32_t size;

    SparseSet(uint32_t capacity, uint8_t paginationSize);

	T& get(uint32_t key) {
		uint32_t elementIndex = key % paginationSize;
		uint32_t pageIndex = key / paginationSize;

		if (!pages.contains(pageIndex) || pages[pageIndex][elementIndex] >= capacity) {
			EXCEPTION(("There is no element for Key: " + std::to_string(key)).c_str());
		}

		return dense[pages[pageIndex][elementIndex]];
	}

	void insert(uint32_t key, T value) {
		uint32_t elementIndex = key % paginationSize;
		uint32_t pageIndex = key / paginationSize;

		if (pages.contains(pageIndex)) {
			auto insetPoint = pages[pageIndex].begin() + elementIndex;
			pages[pageIndex][elementIndex] = size;
		}
		else {
			pages[pageIndex] = std::vector<uint32_t>(paginationSize, capacity + 2);
			pages[pageIndex][elementIndex] = size;
		}

		dense.push_back(value);
		size++;
	}

	void remove(uint32_t key) {
		uint32_t elementIndex = key % paginationSize;
		uint32_t pageIndex = key / paginationSize;

		if (!pages.contains(pageIndex)) {
			return;
		}

		uint32_t backPage = 0;
		uint32_t backIndex = 0;

		for (uint32_t i = 0; i < capacity / paginationSize; i++) {
			bool found = false;
			for (uint32_t j = 0; j < paginationSize; j++) {
				if (pages[i][j] == dense.size() - 1) {
					backPage = i;
					backIndex = j;
					found = true;
					break;
				}
			}

			if (found) break;
		}

		std::swap(dense[pages[pageIndex][elementIndex]], dense.back());
		dense.pop_back();

		pages[backPage][backIndex] = pages[pageIndex][elementIndex];
		pages[pageIndex][elementIndex] = capacity + 2;
		size--;
	}

	std::vector<T> getAll() const {
		return dense;
	}

	std::vector<uint32_t> getAssignedIndex() {
		std::vector<uint32_t> assignedIndex{};

		for (auto& pair : pages) {
			for (uint32_t i : pair.second) {
				if (i < capacity) assignedIndex.push_back(i);
			}
		}

		return assignedIndex;
	}

	bool has(uint32_t key) {
		uint32_t elementIndex = key % paginationSize;
		uint32_t pageIndex = key / paginationSize;

		if (!pages.contains(pageIndex) || pages[pageIndex].size() == 0) return false;

		return pages[pageIndex][elementIndex] < capacity;
	}

	void clear() {
		pages.clear();
		dense.clear();
		size = 0;
	}

private:
	std::unordered_map<uint32_t, std::vector<uint32_t>> pages;
	std::vector<T> dense;
    unsigned int capacity;
	uint8_t paginationSize;
};

template<typename T>
SparseSet<T>::SparseSet(uint32_t capacity, uint8_t paginationSize) : capacity(capacity), paginationSize(paginationSize) {	
	size = 0;
}

#endif