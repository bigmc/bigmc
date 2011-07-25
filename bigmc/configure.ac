AC_INIT([BigMC], [0.1], [gdpe@itu.dk], [bigmc], [http://itu.dk/~gdpe/bigmc/])
AC_PREREQ([2.59])
AM_INIT_AUTOMAKE([1.10 no-define])
AC_CONFIG_HEADERS([config.h])

AC_PROG_CXX

AC_PROG_LIBTOOL

AC_CONFIG_MACRO_DIR([m4])

AC_PREFIX_DEFAULT(/usr/local/bigmc)

AC_CONFIG_FILES([Makefile
src/Makefile
src/predicates/Makefile
doc/Makefile
])

AC_PROG_LEX
AC_PROG_YACC
AC_CHECK_LIB([readline], [readline])
AC_CHECK_LIB([pthread], [pthread_create])
AC_CHECK_LIB([tcmalloc], [malloc])
AC_HEADER_ASSERT

AC_CHECK_HEADER([readline/readline.h],[],AC_MSG_ERROR([Missing readline/readline.h... is GNU Readline installed correctly?]))
AC_CHECK_HEADER([readline/history.h],[],AC_MSG_ERROR([Missing readline/history.h... is GNU Readline installed correctly?]))

AX_PTHREAD

AC_OUTPUT

