#pragma once

#include <common/types.h>

#include <cstdint>
#include <memory>
#include <utility>
#include <system_error>

namespace NRcv {

struct TInterface {
public:
    virtual std::error_code Init() noexcept = 0;
    virtual std::pair<std::error_code, TData> Rcv(std::int32_t dataSize) const noexcept = 0;
    virtual ~TInterface() {};
};

using TRcvPtr = std::unique_ptr<TInterface>;

}
