# about

simple rtp and alsa server

# requirement

1. socket
3. c++20
4. make
5. libasound2-dev

# build

make

# install

make install

# configure PulseAudio host

in file default.pa `/etc/pulse/default.pa` add:

`
load-module module-null-sink sink_name=rtp rate=48000 format=s16be channels=2 sink_properties=device.description=<name>
`
`
load-module module-rtp-send source=rtp.monitor destination_ip=<ip server> port=<port server> mtu=1932
`
