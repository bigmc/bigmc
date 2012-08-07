AC_INIT([BigMC], [20120807], [gdpe@itu.dk], [bigmc], [http://bigraph.org/bigmc/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX
AC_PROG_CC
AM_PROG_CC_C_O

AC_PROG_LIBTOOL

AC_CONFIG_MACRO_DIR([m4])

AC_PREFIX_DEFAULT(/usr/local/bigmc)

AC_CONFIG_FILES([Makefile
src/Makefile
tests/Makefile
doc/Makefile
])

AC_PROG_LEX
AC_PROG_YACC
#AC_CHECK_LIB([pthread], [pthread_create])
#AC_CHECK_LIB([tcmalloc], [malloc])

AC_ARG_WITH(
	[readline],
	[AS_HELP_STRING([--with-readline], [enable support for readline])],
	[], [with_readline=check])

LIBREADLINE=
AS_IF([test "x$with_readline" != xno],
	[
		AC_CHECK_HEADER(
			[readline/readline.h],
			[
				AC_SEARCH_LIBS(
					[readline],
					[readline],
					[
						AC_SUBST([LIBREADLINE], ["-lreadline"])
						AC_DEFINE(HAVE_LIBREADLINE, [1], [Define if you have libreadline])
					], [
						if test "x$with_readline" != xcheck; then
							AC_MSG_FAILURE([readline test failed (--without-readline to disable)])
						fi
					]
				)
			], [
				if test "x$with_readline" != xcheck; then
					AC_MSG_FAILURE([readline test failed (--without-readline to disable)])
				fi
			]
		)
	]
)

AC_HEADER_ASSERT

#AX_PTHREAD

AC_OUTPUT

