CXX := g++
CXXFLAGS := -g -std=c++14
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})

jked : ${OBJECTS}
	$(CXX) $(CXXFLAGS) -o jked $(OBJECTS)

%.o : %.c
	$(CXX) -c $(CXXFLAGS) $< -o $@
