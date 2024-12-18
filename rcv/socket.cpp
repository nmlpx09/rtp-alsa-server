#include "errors.h"
#include "socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

namespace NRcv {

TSocket::TSocket(
    std::string ip,
    std::uint16_t port
) noexcept
: Ip(std::move(ip))
, Port(port) {
}

TSocket::~TSocket() {
    close(Sockfd);
}

std::error_code TSocket::Init() noexcept {
    if (Sockfd = socket(AF_INET, SOCK_DGRAM, 0); Sockfd < 0) {
        return make_error_code(EErrorCode::SocketOpen);
    }

    auto servaddr = sockaddr_in {
        .sin_family = AF_INET,
        .sin_port = htons(Port),
        .sin_addr = {
            .s_addr = inet_addr(Ip.c_str())
        },
        .sin_zero = {0}
    };

    if (bind(Sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        return make_error_code(EErrorCode::SocketBind);
    }

    return {};
}

std::expected<TData, std::error_code> TSocket::Rcv(std::int32_t dataSize) const noexcept {
    if (Sockfd < 0) {
        return std::unexpected(EErrorCode::SocketInit);
    }

    if (dataSize < 1) {
        return TData{};
    }

    TData buffer(dataSize);

    auto n = recv(Sockfd, buffer.data(), dataSize, MSG_WAITALL);

    if (n < 0) {
         return std::unexpected(EErrorCode::Recv);
    }

    if (n < dataSize) {
        return TData{};
    }

    return buffer;
}

}
