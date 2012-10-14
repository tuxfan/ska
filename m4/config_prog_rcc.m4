dnl ------------------------------------------------------------------------ dnl
dnl Copyright (c) 2012 Los Alamos National Security, LLC
dnl All rights reserved.
dnl ------------------------------------------------------------------------ dnl

dnl ------------------------------------------------------------------------ dnl
dnl Look for Qt rcc program
dnl ------------------------------------------------------------------------ dnl

AC_DEFUN([CONFIG_PROG_RCC], [
	AC_ARG_VAR([QT_RCC], [User specified path to the rcc executable])

	if test -n "$QT_RCC" ; then
		AC_PATH_PROG(RCC, $QT_RCC)
	else
		AC_PATH_PROG(RCC, rcc)
	fi

	if test -n "$RCC" ; then
		AC_SUBST(HAS_RCC, [yes])
	else
		AC_SUBST(HAS_RCC, [no])
	fi
])
