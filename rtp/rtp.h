#pragma once

#include <common/types.h>

#include <cstdint>

namespace NRtp {

struct TRtpHeaders {
    std::uint32_t cc : 4  = 0;
    std::uint32_t x  : 1  = 0;
    std::uint32_t p  : 1  = 0;
    std::uint32_t v  : 2  = 0;
    std::uint32_t pt : 7  = 0;
    std::uint32_t m  : 1  = 0;
    std::uint32_t sn : 16 = 0;
    std::uint32_t ts      = 0;
    std::uint32_t ssrc    = 0; 
};

struct TRtp {
public:
    TRtp() = delete;
    TRtp(TData&& buffer) noexcept;
    TRtp(const TRtp&) = delete;
    TRtp(TRtp&&) noexcept = default;
    TRtp& operator=(const TRtp&) = delete;
    TRtp& operator=(TRtp&&) = delete;

    TPayload GetPayload() const noexcept;
    operator bool() const noexcept;

private:
    std::uint8_t V = 0;
    std::uint8_t P = 0;
    std::uint8_t X = 0;
    std::uint8_t Cc = 0;
    std::uint8_t M = 0;
    std::uint8_t Pt = 0;
    std::uint16_t Sn = 0;
    std::uint32_t Ts = 0;
    std::uint32_t Ssrc = 0;

    TData Data;
    TPayload Payload;
};

}
