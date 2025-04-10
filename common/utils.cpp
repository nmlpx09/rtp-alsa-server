#include "utils.h"

#include <cstring>

namespace NUtils {

bool isZero(const TPayload& payload) {
    return payload.front() == 0 && !std::memcmp(payload.data(), payload.data() + 1, payload.size() - 1);
}

}
