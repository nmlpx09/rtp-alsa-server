#include "utils.h"

#include <cstring>

namespace NUtils {

bool isZero(const TPayload& payload) {
    const auto* pointer = reinterpret_cast<const std::uint8_t*>(payload.data());
    const auto size = payload.size() * sizeof(TData::value_type);
    return payload.front() == 0 && !std::memcmp(pointer, pointer + 1, size - 1);
}

}
