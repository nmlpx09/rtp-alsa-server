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
    if (Sockfd = socket(AF_INET, SOCK_DGRAM, 0); Sockfd < 0 ) {
        return make_error_code(EErrorCode::SocketOpen);
    }

    auto servaddr = sockaddr_in {
        .sin_family = AF_INET,
        .sin_port = htons(Port),
        .sin_addr = {
            .s_addr = inet_addr(Ip.c_str())
        },
        .sin_zero = 0
    };

    if (bind(Sockfd, (const sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        return make_error_code(EErrorCode::SocketBind);
    }

    return {};
}

std::pair<std::error_code, TData> TSocket::Rcv(std::int32_t dataSize) const noexcept {
    if (Sockfd < 0) {
        return std::make_pair(make_error_code(EErrorCode::SocketInit), TData{});
    }

    if (dataSize < 1) {
        return std::make_pair(make_error_code(EErrorCode::Ok), TData{});
    }

    TData buffer(dataSize);

    auto n = recv(Sockfd, buffer.data(), dataSize, MSG_WAITALL);

    if (n < 0) {
        return std::make_pair(make_error_code(EErrorCode::Recv), TData{});
    }

    if (n < dataSize) {
        return std::make_pair(make_error_code(EErrorCode::Ok), TData{});
    }

    return std::make_pair(make_error_code(EErrorCode::Ok), std::move(buffer));
}

}
