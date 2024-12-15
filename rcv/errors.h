#pragma once

#include <system_error>

namespace NRcv {

enum EErrorCode {
    Ok,
    SocketOpen,
    SocketBind,
    SocketInit,
    Recv
};

}

namespace std {

template<> struct is_error_code_enum<NRcv::EErrorCode> : std::true_type{};

}

namespace NRcv {

class TErrorCategory final : public std::error_category {
public:
    const char* name() const noexcept override {
        return "rcv error";
    }

    std::string message(int value) const override {
        switch (value) {
            case Ok:
                return "ok";
            case SocketOpen:
                return "socket open error";
            case SocketBind:
                return "socket bind error";
            case SocketInit:
                return "socket init error";
            case Recv:
                return "recv ended with error";
        }

        return "unknown error code: " + std::to_string(value);
    }

    static const TErrorCategory& instance() {
        static const TErrorCategory errorCategory;
        return errorCategory;
    }
};

inline std::error_code make_error_code(EErrorCode errorCode) {
    return {errorCode, TErrorCategory::instance()};
}

}
