dnl ------------------------------------------------------------------------ dnl
dnl Copyright (c) 2012 Los Alamos National Security, LLC
dnl All rights reserved.
dnl ------------------------------------------------------------------------ dnl

dnl ------------------------------------------------------------------------ dnl
dnl Look for llvm-config
dnl ------------------------------------------------------------------------ dnl

AC_DEFUN([CONFIG_PROG_LLVM_CONFIG], [
    AC_ARG_VAR([LLVM_CONFIG],
        [User specified path to the llvm-config executable])

    AC_PATH_PROG(LLVM_CONFIG, llvm-config)

    if test -n "$LLVM_CONFIG" ; then
        AC_SUBST(HAS_LLVM_CONFIG, [yes])
    else
        AC_SUBST(HAS_LLVM_CONFIG, [no])
    fi
])
