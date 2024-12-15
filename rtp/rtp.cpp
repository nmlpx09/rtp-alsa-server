#include "rtp.h"

namespace NRtp {

namespace {

std::uint32_t Swap32(std::uint32_t value) {
    return ((value << 24) | ((value & 0x0000FF00) << 8) | ((value & 0x00FF0000) >> 8) | (value >> 24));
}

std::uint16_t Swap16(std::uint16_t value) {
    return (value >> 8) | ((value & 0x00FF) << 8);
}

}

TRtp::TRtp(TData&& data) noexcept 
  : Data(std::move(data))
{
    if (Data.size() < sizeof(TRtpHeaders)) {
        return;
    }

    const auto* headers = (const TRtpHeaders*)Data.data();

    V = headers->v;
    P = headers->p;
    X = headers->x;
    Cc = headers->cc;
    M = headers->m;
    Pt = headers->pt;
    Sn = Swap16(headers->sn);
    Ts = Swap32(headers->ts);
    Ssrc = Swap32(headers->ssrc);

    auto payloadBegin = sizeof(TRtpHeaders) + sizeof(std::uint32_t) * headers->cc;

    Payload = TPayload{Data}.subspan(payloadBegin);
}

TPayload TRtp::GetPayload() const noexcept {
    return Payload;
}

TRtp::operator bool() const noexcept {
    return Payload.size() > 0;
}

}
