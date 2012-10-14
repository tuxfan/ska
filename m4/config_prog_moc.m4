dnl ------------------------------------------------------------------------ dnl
dnl Copyright (c) 2012 Los Alamos National Security, LLC
dnl All rights reserved.
dnl ------------------------------------------------------------------------ dnl

dnl ------------------------------------------------------------------------ dnl
dnl Look for Qt moc program
dnl ------------------------------------------------------------------------ dnl

AC_DEFUN([CONFIG_PROG_MOC], [
	AC_ARG_VAR([QT_MOC], [User specified path to the moc executable])

	
	if test -n "$QT_MOC" ; then
		AC_PATH_PROG(MOC, $QT_MOC)
	else
		AC_PATH_PROG(MOC, moc)
	fi

	if test -n "$MOC" ; then
		AC_SUBST(HAS_MOC, [yes])
	else
		AC_SUBST(HAS_MOC, [no])
	fi
])
