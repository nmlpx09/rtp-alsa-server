#pragma once

#include <common/types.h>

#include <cstdint>
#include <expected>
#include <memory>
#include <system_error>

namespace NRcv {

struct TInterface {
public:
    virtual std::error_code Init() noexcept = 0;
    virtual std::expected<TData, std::error_code> Rcv(std::int32_t dataSize) const noexcept = 0;
    virtual ~TInterface() {};
};

using TRcvPtr = std::unique_ptr<TInterface>;

}
