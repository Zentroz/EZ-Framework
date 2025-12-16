#pragma once

#include<optional>

template <typename T>
using Ref = std::optional<std::reference_wrapper<T>>;