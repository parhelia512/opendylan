AC_INIT([Open Dylan], [2025.2pre])
AC_PREREQ(2.50)

# Directory for config.guess etc.
AC_CONFIG_AUX_DIR(config/build-aux)

SUPPORT_HARP=
SUPPORT_LLVM=
BDW_GC_STATIC=
BDW_GC_MODULE=bdw-gc
ADDITIONAL_RUNTIME_TARGET_PLATFORMS=
MACOS_SDK=

AC_CANONICAL_TARGET

# Shared library filename extension
shrext=so
MULTIARCH=$host
case $host in
  i*86-*-linux*)
      OPEN_DYLAN_TARGET_PLATFORM=x86-linux;
      MULTIARCH=i386-linux-gnu
      SUPPORT_LLVM=yes
    ;;
  i*86-*-freebsd*)
      OPEN_DYLAN_TARGET_PLATFORM=x86-freebsd;
      BDW_GC_MODULE=bdw-gc-threaded
      SUPPORT_HARP=yes
      SUPPORT_LLVM=yes
    ;;
  x86_64-*-darwin*)
      OPEN_DYLAN_TARGET_PLATFORM="x86_64-darwin";
      MACOS_SDK=`xcrun --show-sdk-path`
      SUPPORT_LLVM=yes
      shrext=dylib
    ;;
  amd64-*-freebsd* | x86_64-*-freebsd*)
      OPEN_DYLAN_TARGET_PLATFORM=x86_64-freebsd;
      BDW_GC_MODULE=bdw-gc-threaded
      SUPPORT_LLVM=yes
    ;;
  x86_64-*-linux*)
      OPEN_DYLAN_TARGET_PLATFORM=x86_64-linux;
      MULTIARCH=x86_64-linux-gnu
      SUPPORT_LLVM=yes
    ;;
  arm-*-linux*)
      OPEN_DYLAN_TARGET_PLATFORM=arm-linux;
    ;;
  aarch64-*-linux*)
      OPEN_DYLAN_TARGET_PLATFORM=aarch64-linux;
      SUPPORT_LLVM=yes
    ;;
  riscv64-*-linux*)
      OPEN_DYLAN_TARGET_PLATFORM=riscv64-linux;
      SUPPORT_LLVM=yes
    ;;
  i*86-*-netbsd*)
      OPEN_DYLAN_TARGET_PLATFORM=x86-netbsd;
    ;;
  amd64-*-netbsd* | x86_64-*-netbsd*)
      OPEN_DYLAN_TARGET_PLATFORM=x86_64-netbsd;
    ;;
  *)
      AC_MSG_ERROR([The $host platform is not supported.])
    ;;
esac
AC_SUBST(OPEN_DYLAN_TARGET_PLATFORM)
AC_SUBST(ADDITIONAL_RUNTIME_TARGET_PLATFORMS)

AC_SUBST(MACOS_SDK)
AC_SUBST(shrext)

AM_INIT_AUTOMAKE

AC_PROG_INSTALL
AC_PROG_LN_S

AC_PATH_PROG(PERL, perl, no)
test "$PERL" = no && AC_MSG_ERROR([
perl is required to build Open Dylan])
AC_SUBST(PERL)

PERL_MODULES="File::Spec Getopt::Long Time::HiRes"
for module in $PERL_MODULES; do
  AC_MSG_CHECKING(for Perl module $module)
  $PERL -M$module -e 'exit 0' || AC_MSG_ERROR([
$module is required to build Open Dylan])
  AC_MSG_RESULT(yes)
done

AC_PATH_PROGS(DYLANCOMPILER, dylan-compiler opendylan minimal-console-compiler, no)
test "$DYLANCOMPILER" = no && AC_MSG_ERROR([
dylan-compiler, opendylan or minimal-console-compiler is required to bootstrap Open Dylan. Please download and install a build from http://opendylan.org/download/index.html])
# In 2014.1 and later we need -verbose so fdmake.pl gets warning count summaries.
AC_MSG_CHECKING(if $DYLANCOMPILER supports the -verbose flag)
if $DYLANCOMPILER -version -verbose >/dev/null 2>&1 ; then
  DYLANCOMPILER="$DYLANCOMPILER -verbose -build"
  AC_MSG_RESULT(yes)
else
  test "$DYLANCOMPILER" = dylan-compiler && DYLANCOMPILER="$DYLANCOMPILER -build"
  test "$DYLANCOMPILER" = opendylan && DYLANCOMPILER="$DYLANCOMPILER -build"
  test "$DYLANCOMPILER" = minimal-console-compiler && DYLANCOMPILER="$DYLANCOMPILER -build"
  AC_MSG_RESULT(no)
fi
AC_MSG_CHECKING(if $DYLANCOMPILER supports the -jobs flag)
if $DYLANCOMPILER -version -jobs 1 >/dev/null 2>&1 ; then
  DYLANCOMPILER="$DYLANCOMPILER -jobs \$(DYLAN_JOBS)"
  AC_MSG_RESULT(yes)
else
  AC_MSG_RESULT(no)
fi

AC_SUBST(DYLANCOMPILER)

# TODO: figure out the bootstrap type given the fdcompile version
#       as we may be able to just do a 1-stage-bootstrap. But for now,
#       default to a 3-stage-bootstrap to be safe.
bootstrap_target=3-stage-bootstrap
AC_SUBST(bootstrap_target)

AC_PROG_CC([clang-20 clang-19 clang-18 clang-17 clang-16 clang-15 clang-14 clang14 clang-12 clang12 clang-11 clang11 clang-10 clang10 clang-9 clang90 clang-8 clang80 clang gcc cc])
AC_PROG_CXX([clang++-20 clang++-19 clang++-18 clang++-17 clang++-16 clang++-15 clang++-14 clang++14 clang++-12 clang++12 clang++-11 clang++11 clang++-10 clang++10 clang++-9 clang++90 clang++-8 clang++80 clang++ g++ c++])

AS_IF([test -n "$SUPPORT_LLVM"],
      [AC_MSG_CHECKING([if LLVM bitcode can be compiled])
       cp $srcdir/config/llvm/$OPEN_DYLAN_TARGET_PLATFORM.bc conftest.bc
       AS_IF([$CC -c $CFLAGS conftest.bc >&AS_MESSAGE_LOG_FD 2>&1 &&
              { test -f conftest.o || test -f conftest.obj; }],
             [AC_MSG_RESULT([yes])],
             [AC_MSG_RESULT([no])
              SUPPORT_LLVM=])
       rm -f conftest.bc conftest.o conftest.obj])
AC_SUBST(SUPPORT_LLVM)

DEFAULT_COMPILER_BACK_END=c
SUPPORTED_COMPILER_BACK_ENDS=c
AS_IF([test -n "$SUPPORT_HARP"],
      [DEFAULT_COMPILER_BACK_END=harp
       SUPPORTED_COMPILER_BACK_ENDS="harp $SUPPORTED_COMPILER_BACK_ENDS"])
AS_IF([test -n "$SUPPORT_LLVM"],
      [DEFAULT_COMPILER_BACK_END=llvm
       SUPPORTED_COMPILER_BACK_ENDS="llvm $SUPPORTED_COMPILER_BACK_ENDS"])
AC_SUBST(SUPPORTED_COMPILER_BACK_ENDS)

AC_CHECK_PROGS([MAKE],[gnumake gmake make],[AC_MSG_ERROR([Unable to find a make program.])])
if ! $MAKE --version | grep -q "GNU" 2> /dev/null; then
  AC_MSG_ERROR([OpenDylan requires GNU make to build.])
fi

AC_DEFUN([CANONICALIZE_COLLECTOR_NAME],
[AS_CASE(${$1},
[mps|MPS], [$1=MPS],
[gc|boehm|Boehm|BDW|BDW_GC], [$1=BDW_GC],
[malloc], [$1=MALLOC],
AC_MSG_ERROR([Collector name `${$1}' is unrecognized.]))])

AC_ARG_WITH(harp-collector,
            AS_HELP_STRING([--with-harp-collector=mps|boehm|malloc],
                           [Which GC to use with the HARP back-end]),
            [HARP_COLLECTOR=$withval],
            [HARP_COLLECTOR=mps])
AS_IF([test -n "$SUPPORT_HARP"],
      [AC_MSG_CHECKING([which GC to use with the HARP back-end])
       AC_MSG_RESULT($HARP_COLLECTOR)])
CANONICALIZE_COLLECTOR_NAME(HARP_COLLECTOR)
AC_SUBST(HARP_COLLECTOR)

AC_ARG_WITH(c-collector,
            AS_HELP_STRING([--with-c-collector=boehm|malloc],
                           [Which GC to use with the C back-end]),
            [C_COLLECTOR=$withval],
            [C_COLLECTOR=boehm])
AC_MSG_CHECKING([which GC to use with the C back-end ])
AC_MSG_RESULT($C_COLLECTOR)
CANONICALIZE_COLLECTOR_NAME(C_COLLECTOR)
AC_SUBST(C_COLLECTOR)

AC_ARG_WITH(llvm-collector,
            AS_HELP_STRING([--with-llvm-collector=boehm|malloc],
                           [Which GC to use with the LLVM back-end]),
            [LLVM_COLLECTOR=$withval],
            [LLVM_COLLECTOR=boehm])
AS_IF([test -n "$SUPPORT_LLVM"],
      [AC_MSG_CHECKING([which GC to use with the LLVM back-end ])
       AC_MSG_RESULT($LLVM_COLLECTOR)])
CANONICALIZE_COLLECTOR_NAME(LLVM_COLLECTOR)
AC_SUBST(LLVM_COLLECTOR)

AC_ARG_WITH(mps,
           AS_HELP_STRING([--with-mps=DIR],
                          [Memory Pool System (MPS) source directory]))

AS_IF([test -n "$SUPPORT_HARP" && test x$HARP_COLLECTOR = xMPS],
      [VALIDATE_MPS=yes])

AS_IF([test -n "$SUPPORT_LLVM" && test x$LLVM_COLLECTOR = xMPS],
      [VALIDATE_MPS=yes])

AS_IF([test -n "$VALIDATE_MPS"],
      [MPS_CFLAGS="-I${with_mps}/code"
       MPS_LIBS=
       save_CPPFLAGS="$CPPFLAGS"
       CPPFLAGS="$CPPFLAGS $MPS_CFLAGS"
       AC_CHECK_HEADER([mps.h], [],
                       [AC_MSG_ERROR([Incorrect path to MPS specified. Please make sure you specified an absolute path to the root MPS directory using --with-mps=DIR])])
       CPPFLAGS="$save_CPPFLAGS"
       # This is to help catch people who haven't updated to the new MPS yet.
       AS_IF([test ! -f ${with_mps}/code/mps.c],
             [AC_MSG_ERROR([You have an old version of MPS and need to update.])])])
AC_SUBST(MPS_CFLAGS)
AC_SUBST(MPS_LIBS)

AC_ARG_WITH(gc,
            AS_HELP_STRING([--with-gc=DIR],
                           [Directory prefix where the Boehm-Demers-Weiser Garbage Collector is installed]),
            [BDW_GC_CFLAGS="-I${with_gc}/include"
             BDW_GC_LIBS="-L${with_gc}/lib -lgc"
             save_CPPFLAGS="$CPPFLAGS"
             save_LIBS="$LIBS"
             LIBS="$BDW_GC_LIBS $LIBS"
             AC_CHECK_LIB(gc, [GC_malloc],
                          [],
                          [AC_MSG_ERROR([Could not find the Boehm-Demers-Weiser Garbage Collector. Please specify the prefix directory using --with-gc=DIR, or omit --with-gc to search using pkg-config.])])
             CPPFLAGS="$save_CPPFLAGS"
             LIBS="$save_LIBS"],
            [PKG_CHECK_MODULES(BDW_GC, $BDW_GC_MODULE,
                               [AS_IF([test -n "$BDW_GC_STATIC"],
                                      [AC_MSG_CHECKING(if static gc library is present)
                                       BDW_GC_LIBDIR=`$PKG_CONFIG --variable=libdir $BDW_GC_MODULE`
                                       BDW_GC_STATIC_LIB=$BDW_GC_LIBDIR/libgc.a
                                       AS_IF([test -f "$BDW_GC_STATIC_LIB"],
                                             [AC_MSG_RESULT(yes)],
                                             [AC_MSG_RESULT(no)
                                              BDW_GC_STATIC=
                                              BDW_GC_STATIC_LIB=])])],
                               [AC_CHECK_LIB(gc, [GC_malloc],
                                             [],
                                             [AC_MSG_ERROR([Could not find the Boehm-Demers-Weiser Garbage Collector. Please specify the prefix directory using --with-gc=DIR, or omit --with-gc to search using pkg-config.])])
                                BDW_GC_LIBS=-lgc])])
AC_SUBST(BDW_GC_STATIC)
AC_SUBST(BDW_GC_STATIC_LIB)

# We don't want to build generated code with warnings enabled usually.
# The exception to this is if the person running configure has passed
# in CFLAGS with -W options ...
AC_MSG_CHECKING(if we should disable C compiler warnings for generated code)
AS_IF([echo "$CFLAGS" | grep -q -- "-W" 2> /dev/null],
      [
        DISABLE_WARNINGS_CFLAGS=
        AC_MSG_RESULT(no)
      ],[
        DISABLE_WARNINGS_CFLAGS=-w
        AC_MSG_RESULT(yes)
      ]
)
AC_SUBST(DISABLE_WARNINGS_CFLAGS)

AC_CHECK_FUNC(sigaction,
              AC_CHECK_TYPE(siginfo_t,
                            AC_DEFINE(HAVE_SIGINFO_T, 1,
                                      [Define if you have 'siginfo_t']),
              [],
              [#include <signal.h>]))

# libm
AC_SEARCH_LIBS([atan], [m])

# libunwind
AC_CHECK_HEADERS([libunwind.h], [backtrace=yes], [backtrace=no])

AC_MSG_CHECKING(for libunwind library)
AC_LANG_CONFTEST([AC_LANG_PROGRAM([#define UNW_LOCAL_ONLY
                                   #include <stddef.h>
                                   #include <libunwind.h>],
                                  [unw_step(NULL)])])
AC_LINK_IFELSE([], [AC_MSG_RESULT(none needed)],
               [save_LIBS="$LIBS"
                LIBS="$LIBS -lunwind"
                AC_LINK_IFELSE([], [AC_MSG_RESULT(-lunwind)],
                               [AC_MSG_RESULT(not found)
                                LIBS="$save_LIBS"])])

AS_ECHO(["
Targeting '${target}' ('${OPEN_DYLAN_TARGET_PLATFORM}')

  Default compiler back-end                               ${DEFAULT_COMPILER_BACK_END}
  LLVM support                                            ${SUPPORT_LLVM:-no}"])
AS_IF([test -n "$SUPPORT_LLVM"],
      [AS_ECHO(["  LLVM back-end collector                                 ${LLVM_COLLECTOR}"])])
AS_ECHO(["  HARP support                                            ${SUPPORT_HARP:-no}"])
AS_IF([test -n "$SUPPORT_HARP"],
      [AS_ECHO(["  HARP back-end collector                                 ${HARP_COLLECTOR}"])])
AS_ECHO(["  C back-end collector                                    ${C_COLLECTOR}
  Backtrace support using libunwind                       ${backtrace}
"])

AC_CONFIG_FILES(Makefile
                sources/jamfiles/Makefile
                sources/jamfiles/config.jam
                sources/lib/run-time/Makefile)

AC_OUTPUT
