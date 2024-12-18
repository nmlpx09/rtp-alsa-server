#include "alsa.h"
#include "errors.h"

namespace NSend {

TAlsa::TAlsa(
    std::string device,
    std::string format,
    std::size_t channels,
    std::size_t rate,
    std::size_t bufferSize
) noexcept
: Device(std::move(device))
, Format(std::move(format))
, Channels(channels)
, Rate(rate)
, BufferSize(bufferSize) {
}

TAlsa::TAlsa(TAlsa&& alsa) noexcept {
    std::swap(Device, alsa.Device);
    std::swap(Format, alsa.Format);
    std::swap(Channels, alsa.Channels);
    std::swap(Rate, alsa.Rate);
    std::swap(BufferSize, alsa.BufferSize);
    std::swap(SoundDevice, alsa.SoundDevice);
    std::swap(HwParams, alsa.HwParams);
    std::swap(FrameSize, alsa.FrameSize);
}

TAlsa::~TAlsa() {
    snd_pcm_drain(SoundDevice);
    snd_pcm_close(SoundDevice);
}

std::error_code TAlsa::Init() noexcept {
    if (auto err = snd_pcm_open(&SoundDevice, Device.c_str(), SND_PCM_STREAM_PLAYBACK, 0); err < 0) {
        return make_error_code(EErrorCode::DeviceOpen);
    }

    snd_pcm_hw_params_alloca(&HwParams);
 
    if (auto err = snd_pcm_hw_params_any(SoundDevice, HwParams); err < 0) {
        return make_error_code(EErrorCode::InitializeParameter);
    }

    if (auto err = snd_pcm_hw_params_set_rate_resample(SoundDevice, HwParams, 1); err < 0) {
        return make_error_code(EErrorCode::Resampling);
    }

    if (auto err = snd_pcm_hw_params_set_access(SoundDevice, HwParams, SND_PCM_ACCESS_RW_INTERLEAVED); err < 0) {
        return make_error_code(EErrorCode::SetAccess);
    }

    if (Channels > 2) {
        return make_error_code(EErrorCode::Channels);
    }

    if (auto err = snd_pcm_hw_params_set_channels(SoundDevice, HwParams, Channels); err < 0) {
        return make_error_code(EErrorCode::SetChannels);
    }

    snd_pcm_format_t format;
    if (Format == "s16be") {
        format = SND_PCM_FORMAT_S16_BE;
        FrameSize = sizeof(std::int16_t) * Channels;
    } else {
        return make_error_code(EErrorCode::Format);
    }

    if (auto err = snd_pcm_hw_params_set_format(SoundDevice, HwParams, format); err < 0) {
        return make_error_code(EErrorCode::SetFormat);
    }

    if (Rate != 48000 && Rate != 44100) {
        return make_error_code(EErrorCode::Rate);
    }

    std::uint32_t rate = Rate;
    if (auto err = snd_pcm_hw_params_set_rate_near(SoundDevice, HwParams, &rate, 0); err < 0) {
        return make_error_code(EErrorCode::SetRate);
    }

    snd_pcm_uframes_t bufferSize = BufferSize;
    if (auto err = snd_pcm_hw_params_set_buffer_size_near(SoundDevice, HwParams, &bufferSize); err < 0) {
        return make_error_code(EErrorCode::SetBuffer);
    }

    if (auto err = snd_pcm_hw_params(SoundDevice, HwParams); err < 0) {
        return make_error_code(EErrorCode::SetHwParams);
    }

    if (auto err = snd_pcm_prepare(SoundDevice); err < 0) {
        return make_error_code(EErrorCode::Prepare);
    }

    return {};
}

std::error_code TAlsa::Send(TPayload&& payload) noexcept {
    if (SoundDevice == nullptr) {
        return make_error_code(EErrorCode::DeviceInit);
    }
    auto frames = payload.size() / FrameSize;

    auto err = snd_pcm_writei(SoundDevice, payload.data(), frames);

    if (err == -ENODEV) {
        return Init();
    } else if (err == -EPIPE) {
        snd_pcm_prepare(SoundDevice);
        snd_pcm_writei(SoundDevice, payload.data(), frames);
    }
    return {};
}

}
