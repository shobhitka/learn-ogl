# what flags you want to pass to the C compiler & linker
AM_CPPFLAGS = -Wall -g -std=gnu++0x $(DEPS_CFLAGS) -Werror
LDADD = $(DEPS_LIBS) -lSOIL

bin_PROGRAMS = gltest
gltest_SOURCES = gltest.cpp shader.cpp moving_triangle.cpp	\
				 triangle.cpp colored.cpp uniform.cpp	\
				 texture_basic.cpp
