CXX=g++
CXXFLAGS+=-std=c++20
#CXXFLAGS+=-std=c++2a
#CXXFLAGS+=-std=c++17
CXXFLAGS+=-fsanitize=address
#CXXFLAGS+=-fcoroutines
#CXXFLAGS+=-pthread
CXXFLAGS+=-O3
CXXFLAGS+=-Wall
CXXFLAGS+=-Wextra
CXXFLAGS+=-Werror=vla
CXXFLAGS+=-pedantic
CXXFLAGS+=-pedantic-errors

#LDFLAGS+=-lm
#LDFLAGS+=-latomic
#LDFLAGS+=-lstdc++fs # GCC requires prior to 9.1
#LDFLAGS+=-lc++fs # LLVM requires prior to 9.0

all: preclude
