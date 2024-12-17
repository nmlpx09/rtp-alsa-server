CC = clang++-19
CCFLAGS = -O3 -std=c++2c -pedantic -Wall -W -Werror -Wextra -c -I.
LDFLAGS = -lasound
OBJ = bin/main.o rtp/rtp.o rcv/socket.o send/alsa.o
RUN = bin/stream

all: $(OBJ)
	$(CC) $^ -o $(RUN) $(LDFLAGS)

%.o: %.cpp
	$(CC) $(CCFLAGS) $< -o $@

install:
	cp $(RUN) /usr/bin/

clean:
	rm -f $(OBJ) $(RUN)
