#include "config.h"

#include <common/utils.h>
#include <rcv/interface.h>
#include <rcv/socket.h>
#include <rtp/rtp.h>
#include <send/interface.h>
#include <send/alsa.h>

#include <condition_variable>
#include <deque>
#include <memory>
#include <mutex>
#include <iostream>
#include <thread>

struct TContext {
    std::deque<NRtp::TRtp> queue;
    std::mutex mutex;
    std::condition_variable cv;
};

int main() {
    NRcv::TRcvPtr rcv = std::make_unique<NRcv::TSocket>(IP, PORT);
    NSend::TSendPtr send = std::make_unique<NSend::TAlsa>(DEVICE, FORMAT, CHANNELS, RATE, DATASIZE);
    
    if (auto ec = send->Init(); ec) {
        std::cerr << ec.message() << std::endl;
        return 1;
    }

    if (auto ec = rcv->Init(); ec) {
        std::cerr << ec.message() << std::endl;
        return 1;
    }

    auto ctx = std::make_shared<TContext>();

    auto recvWorker = [rcv = std::move(rcv), ctx] noexcept {
        while(true) {
            if (auto result = rcv->Rcv(DATASIZE); result) {
                if (auto rtp = NRtp::TRtp{std::move(result).value()}; rtp) {
                    std::unique_lock<std::mutex> ulock{ctx->mutex};
                    ctx->queue.emplace_back(std::move(rtp));
                }
            } else {
                std::cerr << result.error().message() << std::endl;
            }
            ctx->cv.notify_one();
        }
    };

    auto sendWorker = [send = std::move(send), ctx] noexcept {
        bool dropMode = false;
        while(true) {
            std::unique_lock<std::mutex> ulock{ctx->mutex};
            ctx->cv.wait(ulock, [ctx] { return !ctx->queue.empty(); });

            if (dropMode) {
                if (ctx->queue.size() == 1) {
                    dropMode = false;
                } else {
                    ctx->queue.pop_front();
                }
            } else if (ctx->queue.size() > MAXQUEUESIZE) {
                dropMode = true;
            }

            auto rtp = std::move(ctx->queue.front());
            ctx->queue.pop_front();
            ulock.unlock();

            auto payload = rtp.GetPayload();

            if (NUtils::isZero(payload)) {
                continue;
            }

            if (auto ec = send->Send(std::move(payload)); ec) {
                std::cerr << ec.message() << std::endl;
            }   
        }
    };

    std::thread tSend(std::move(sendWorker));
    std::thread tRecv(std::move(recvWorker));

    tRecv.join();
    tSend.join();
    return 0;
}
