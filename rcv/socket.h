#pragma once

#include "interface.h"

#include <string>

namespace NRcv {

struct TSocket: TInterface {
public:
    TSocket(std::string ip, std::uint16_t port) noexcept;
    TSocket(const TSocket&) = delete;
    TSocket(TSocket&&) noexcept = default;
    ~TSocket();
    TSocket& operator=(const TSocket&) = delete;
    TSocket& operator=(TSocket&&) = delete;

    std::error_code Init() noexcept override;
    std::pair<std::error_code, TData> Rcv(std::int32_t dataSize) const noexcept override;

private:
    std::string Ip;
    std::uint16_t Port = 0;
    std::int32_t Sockfd = -1;
};

}
