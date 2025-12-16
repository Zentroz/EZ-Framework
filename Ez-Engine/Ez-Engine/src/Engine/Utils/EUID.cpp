#include"Engine/Utils/EUID.h"

EUID::EUID(bool null) {
    if (null) return;
    std::random_device rd;
    uint64_t high = ((uint64_t)rd() << 32) | rd();
    uint64_t low = ((uint64_t)rd() << 32) | rd();

    std::stringstream ss;
    ss << std::hex << std::setfill('0')
        << std::setw(16) << high
        << std::setw(16) << low;
    m_UID = ss.str();
}

EUID::EUID(std::string id) : m_UID(id) {}

std::string EUID::Get() const {
    return m_UID;
}

bool EUID::isNull() {
    return m_UID.empty();
}