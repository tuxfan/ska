dnl ------------------------------------------------------------------------ dnl
dnl Copyright (c) 2012 Los Alamos National Security, LLC
dnl All rights reserved.
dnl ------------------------------------------------------------------------ dnl

dnl ------------------------------------------------------------------------ dnl
dnl Look for Qt rcc program
dnl ------------------------------------------------------------------------ dnl

AC_DEFUN([CONFIG_PROG_RCC], [
    AC_ARG_VAR([RCC],
        [User specified path to the rcc executable])

    AC_PATH_PROG(RCC, rcc)

    if test -n "$RCC" ; then
        AC_SUBST(HAS_RCC, [yes])
    else
        AC_SUBST(HAS_RCC, [no])
    fi
])
