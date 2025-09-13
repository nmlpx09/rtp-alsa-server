# about

a simple rtp to alsa server

# requirement

1. clang-20
2. make
3. libasound2-dev

# build

make

# install

make install

# configure PulseAudio host

in file default.pa `/etc/pulse/default.pa` add:

`
load-module module-null-sink sink_name=rtp sink_properties=device.description=Player
load-module module-rtp-send source=rtp.monitor format=s16le rate=48000 channels=2 destination_ip=<ip> port=<port> mtu=1932
`
