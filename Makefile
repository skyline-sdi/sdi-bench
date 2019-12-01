CXX = c++
CXXFLAGS = -O3 -m64 -std=c++11

all: sdi

sdi:
	$(CXX) $(CXXFLAGS) -o $@ *.cpp

clean:
	rm -f sdi
