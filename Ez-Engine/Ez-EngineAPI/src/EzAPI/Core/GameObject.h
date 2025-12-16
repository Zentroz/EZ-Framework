#pragma once

#include<string>

namespace EZ {
	class GameObject {
	public:
		std::string GetID() const;

		void AddComponentRaw(uint8_t componentType) {

		}

		template <typename T>
		T& AddComponent();
		template <typename T>
		T& GetComponent();
		template <typename T>
		bool HasComponent();
	private:
		std::string m_ID;
	};
}