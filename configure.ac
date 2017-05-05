#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.
AC_PREREQ([2.69])
AC_INIT(gltest, 0.1)
AM_INIT_AUTOMAKE([foreign])
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADERS([config.h])

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Check for libraries
AC_PROG_RANLIB

PKG_CHECK_MODULES(glfw3, glfw3)
PKG_CHECK_MODULES(glew, glew)
PKG_CHECK_MODULES(glu, glu)
PKG_CHECK_MODULES(gl, gl)
PKG_CHECK_MODULES(xrandr, xrandr)
PKG_CHECK_MODULES(xxf86vm, xxf86vm)
PKG_CHECK_MODULES(xi, xi)
PKG_CHECK_MODULES(xinerama, xinerama)
PKG_CHECK_MODULES(x11, x11)

PKG_CHECK_MODULES([DEPS], [glfw3 glew glu gl xrandr xxf86vm xi xinerama x11])

AC_CONFIG_FILES([Makefile])
AC_OUTPUT
