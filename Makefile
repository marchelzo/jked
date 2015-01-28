CXX := clang++
CXXFLAGS := -g -std=c++1y
LDFLAGS := -lSDL2 -lSDL2_ttf
SOURCES := $(wildcard src/*.cpp)
OBJECTS := $(patsubst %.cpp,%.o,${SOURCES})

jked : ${OBJECTS}
	$(CXX) $(CXXFLAGS) -o jked $(OBJECTS) $(LDFLAGS)

%.o : %.c
	$(CXX) -c $(CXXFLAGS) $< -o $@
