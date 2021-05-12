CXXFLAGS+=-std=c++20
CXXFLAGS+=-fsanitize=address
#CXXFLAGS+=-fcoroutines
#CXXFLAGS+=-pthread
CXXFLAGS+=-O3
CXXFLAGS+=-Wall
CXXFLAGS+=-Wextra
CXXFLAGS+=-Werror=vla
CXXFLAGS+=-pedantic
CXXFLAGS+=-pedantic-errors

LDFLAGS+=-lm
LDFLAGS+=-latomic
LDFLAGS+=-lstdc++fs

all: preclude
