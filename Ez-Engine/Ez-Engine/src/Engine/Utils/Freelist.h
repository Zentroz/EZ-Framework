#ifndef FREELIST_H
#define FREELIST_H

#include<queue>

namespace ENGINE {
	namespace UTILITY {
		template <typename T>
		class FreeList {
		public:
			FreeList() = default;

			void push(T item) {
				m_FreeQueue.push(item);
			}

			T pop() {
				T data = m_FreeQueue.front();
				m_FreeQueue.pop();
				return data;
			}

			size_t Size() { return m_FreeQueue.size(); }
		private:
			std::queue<T> m_FreeQueue;
		};
	}
}

#endif