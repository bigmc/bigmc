AC_INIT([BigMC], [0.1], [gdpe@itu.dk], [bigmc], [http://itu.dk/~gdpe/bigmc/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])
AC_PROG_CC
AC_CONFIG_FILES([Makefile])
AC_PROG_LEX
if test "x$LEX" != xflex; then
	LEX="$SHELL $missing_dir/missing flex"
	AC_SUBST([LEX_OUTPUT_ROOT], [lex.yy])
	AC_SUBST([LEXLIB], [''])
fi
AC_OUTPUT

