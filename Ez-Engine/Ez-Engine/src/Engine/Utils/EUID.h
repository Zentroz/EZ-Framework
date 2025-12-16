#ifndef UUID_GENERATOR_H
#define UUID_GENERATOR_H

#include <random>
#include <string>
#include <sstream>
#include <iomanip>

class EUID {
public:
	EUID(bool null = false);
	EUID(std::string id);
	std::string Get() const;

	bool operator==(const EUID& other) const {
		return (m_UID == other.Get());
	}
	/*
	bool operator!=(const EUID& other) const {
		return (m_UID != other.Get());
	}
	*/

	bool isNull();

private:
	std::string m_UID;
};

namespace std {
	template <>
	struct hash<EUID> {
		size_t operator()(const EUID& k) const {
			// Combine hashes of member variables
			return std::hash<std::string>()(k.Get());
		}
	};
}

#endif
