CFLAGS=-Wall -O3 -g
CXXFLAGS=$(CFLAGS)
LDFLAGS+=-lrgbmatrix -loscpack -lrt -lm -lpthread

all : bin/clock

bin/clock : obj/Main.o obj/ClockDisplay.o obj/OscServer.o
	mkdir -p bin
	$(CXX) $^ -o $@ $(LDFLAGS)

obj/%.o : %.cc
	mkdir -p obj
	$(CXX) -I/usr/local/include/rpi-rgb-led-matrix -I/usr/local/include/oscpack $(CXXFLAGS) -c -o $@ $<

clean:
	rm -rf obj bin
