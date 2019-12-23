CXX = c++
CXXFLAGS = -O3 -m64 -std=c++11

all: sdi sdi-nsl

bin:
	mkdir -p bin

sdi: bin
	$(CXX) $(CXXFLAGS) -o bin/$@ *.cpp

sdi-nsl: bin
	$(CXX) $(CXXFLAGS) -o bin/$@ *.cpp -DWITHOUT_STOPLINE

clean:
	rm -rf bin
