CC = g++
CCFLAGS = -Ofast -std=c++20 -pedantic -Wall -W -Werror -Wextra -c -I.
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
