AC_INIT(libproj, latest, dewey@fishandwhistle.net)
AC_CONFIG_SRCDIR(src/libproj-config.c)

# query R home and flags
: ${R_HOME="`R RHOME`"}
if test -z "${R_HOME}"; then
  AC_MSG_ERROR([Can't find R_HOME. Did you use R CMD INSTALL?])
fi
RBIN="${R_HOME}/bin/R"

: ${CC=`"${RBIN}" CMD config CC`}
: ${CFLAGS=`"${RBIN}" CMD config CFLAGS`}
: ${CPPFLAGS=`"${RBIN}" CMD config CPPFLAGS`}
: ${LDFLAGS=`"${RBIN}" CMD config LDFLAGS`}
: ${CPP="$CC -E"}

# apply PKG_XX overrides
LIBS="${LIBS} ${PKG_LIBS}"
CPPFLAGS="${CPPFLAGS} ${PKG_CPPFLAGS}"

# pkg-config modules for libtiff and libcurl
: ${PKG_CONFIG_MODULE_TIFF=libtiff-4}
: ${PKG_CONFIG_MODULE_CURL=libcurl}

AC_PATH_PROG([PKG_CONFIG], [pkg-config])
AC_LANG(C)

AC_CHECK_LIB([tiff],[TIFFClientOpen],,[
    AC_MSG_NOTICE([Using libtiff directly doesn't work, falling back to pkg-config.])
    AC_MSG_NOTICE([Note that you can provide correct PKG_LIBS to allow installation without pkg-config.])
    AC_MSG_CHECKING([for pkg-config])
    AS_IF(["$PKG_CONFIG" --version], , [AC_MSG_RESULT([no])
       AC_MSG_ERROR([PKG_LIBS flags don't work and pkg-config is not present.])])
    AC_MSG_CHECKING([pkg-config can find $PKG_CONFIG_MODULE_TIFF])
    AS_IF([$PKG_CONFIG $PKG_CONFIG_MODULE_TIFF], [AC_MSG_RESULT(yes)], [AC_MSG_RESULT([no])
       AC_MSG_ERROR([pkg-config failed. Install libtiff-dev (deb), libtiff-devel (rpm), libtiff (brew), or equivalent])])
    TIFF_CPPFLAGS="`$PKG_CONFIG --cflags $PKG_CONFIG_MODULE_TIFF`"
    TIFF_LIBS="`$PKG_CONFIG --libs $PKG_CONFIG_MODULE_TIFF`"
    LIBS0="$LIBS"
    LIBS="$LIBS0 ${TIFF_LIBS}"
    CPPFLAGS="$CPPFLAGS ${TIFF_CPPFLAGS}"
    AC_MSG_CHECKING([whether ${TIFF_CPPFLAGS} ${TIFF_LIBS} works])
    AC_LINK_IFELSE([AC_LANG_CALL([], [TIFFClientOpen])], AC_MSG_RESULT([yes]), [
        AC_MSG_RESULT([no])
        AC_MSG_CHECKING([whether pkg-config with --static works])
        TIFF_LIBS="`$PKG_CONFIG --static --libs $PKG_CONFIG_MODULE_TIFF`"
        LIBS="$LIBS0 ${TIFF_LIBS}"
        AC_LINK_IFELSE([AC_LANG_CALL([], [TIFFClientOpen])], AC_MSG_RESULT([yes]),,[
          AC_MSG_RESULT([no])
          AC_MSG_ERROR([Config failed for libtiff, check config.log for details.])
        ])
    ])
])

AC_CHECK_LIB([curl],[curl_easy_setopt],,[
    AC_MSG_NOTICE([Using libcurl directly doesn't work, falling back to pkg-config.])
    AC_MSG_NOTICE([Note that you can provide correct PKG_LIBS to allow installation without pkg-config.])
    AC_MSG_CHECKING([for pkg-config])
    AS_IF(["$PKG_CONFIG" --version], , [AC_MSG_RESULT([no])
       AC_MSG_ERROR([PKG_LIBS flags don't work and pkg-config is not present.])])
    AC_MSG_CHECKING([pkg-config can find $PKG_CONFIG_MODULE_CURL])
    AS_IF([$PKG_CONFIG $PKG_CONFIG_MODULE_CURL], [AC_MSG_RESULT(yes)], [AC_MSG_RESULT([no])
       AC_MSG_ERROR([pkg-config failed. Install libcurl4-openssl-dev (deb), libcurl-devel (rpm), or equivalent])])
    CURL_CPPFLAGS="`$PKG_CONFIG --cflags $PKG_CONFIG_MODULE_CURL`"
    CURL_LIBS="`$PKG_CONFIG --libs $PKG_CONFIG_MODULE_CURL`"
    LIBS0="$LIBS"
    LIBS="$LIBS0 ${CURL_LIBS}"
    CPPFLAGS="$CPPFLAGS ${CURL_CPPFLAGS}"
    AC_MSG_CHECKING([whether ${CURL_CPPFLAGS} ${CURL_LIBS} works])
    AC_LINK_IFELSE([AC_LANG_CALL([], [curl_easy_setopt])], AC_MSG_RESULT([yes]), [
        AC_MSG_RESULT([no])
        AC_MSG_CHECKING([whether pkg-config with --static works])
        CURL_LIBS="`$PKG_CONFIG --static --libs $PKG_CONFIG_MODULE_CURL`"
        LIBS="$LIBS0 ${CURL_LIBS}"
        AC_LINK_IFELSE([AC_LANG_CALL([], [curl_easy_setopt])], AC_MSG_RESULT([yes]),,[
          AC_MSG_RESULT([no])
          AC_MSG_ERROR([Config failed for libcurl, check config.log for details.])
        ])
    ])
])

AC_MSG_NOTICE([ PKG_CPPFLAGS: $CPPFLAGS])
AC_MSG_NOTICE([ PKG_LIBS    : $LIBS])

# check headers
AC_CHECK_HEADERS([tiff.h tiffio.h],, [AC_MSG_ERROR([Compilation with libtiff headers failed])])
AC_CHECK_HEADERS([curl/curl.h],, [AC_MSG_ERROR([Compilation with libcurl headers failed])])

AC_ARG_VAR([PKG_CPPFLAGS],[custom C preprocessor flags for package compilation])
AC_ARG_VAR([PKG_LIBS],[custom libraries for package compilation])
AC_ARG_VAR([PKG_CONFIG],[path to the pkg-config executable (pkg-config)])
AC_ARG_VAR([PKG_CONFIG_MODULE_TIFF],[name of the libtiff pkg-config module (libtiff-4)])
AC_ARG_VAR([PKG_CONFIG_MODULE_CURL],[name of the curl pkg-config module (libcurl)])

AC_CONFIG_FILES(src/Makevars)
AC_OUTPUT