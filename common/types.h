#pragma once

#include <cstdint>
#include <span>
#include <vector>

using TData = std::vector<std::uint8_t>;
using TPayload = std::span<TData::value_type>;
