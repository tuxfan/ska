dnl ------------------------------------------------------------------------ dnl
dnl Copyright (c) 2012 Los Alamos National Security, LLC
dnl All rights reserved.
dnl ------------------------------------------------------------------------ dnl

dnl ------------------------------------------------------------------------ dnl
dnl
dnl ------------------------------------------------------------------------ dnl

AC_DEFUN([CONFIG_PROG_MOC], [
    AC_ARG_VAR([MOC],
        [User specified path to the moc executable])

    AC_PATH_PROG(MOC, moc)

    if test -n "$MOC" ; then
        AC_SUBST(HAS_MOC, [yes])
    else
        AC_SUBST(HAS_MOC, [no])
    fi
])
