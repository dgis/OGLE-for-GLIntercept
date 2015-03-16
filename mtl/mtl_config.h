/* mtl/mtl_config.h.  Generated automatically by configure.  */
/* mtl/mtl_config.h.in.  Generated automatically from configure.in by autoheader.  */
#ifndef MTL_CONFIG_H
#define MTL_CONFIG_H


/* #undef HAVE_COPYSIGN */
/* #undef HAVE_STD_ABS */
#define MTL_EXCEPTIONS 0
/* #undef USE_BLAIS */
#define MTL_CMPLR_HAS_LIMITS 1

/* Define if you have the daxpy function.  */
/* #undef HAVE_DAXPY */

/* Define if you have the dgetrf function.  */
/* #undef HAVE_DGETRF */

/* Name of package */
#define PACKAGE "mtl"

/* Version number of package */
#define VERSION "2.1.2-20"


#if defined(_MSC_VER) && !defined(__MWERKS__)
#define _MSVCPP_ _MSC_VER
#endif

/* Assuming that configure is not run for Visual C++ and
   Metrowerks Codewarrior, so the above macros are not
   yet defined.
  */


#if !defined ( _MSVCPP_ )
#define STD_REVERSE_ITER 1
#else
#define STD_REVERSE_ITER 0
#endif


#if defined(__sgi) && !defined(__GNUC__) && (_COMPILER_VERSION < 730)
#  define MTL_NO_TEMPLATE
#elif defined(__GNUC__) && (__GNUC__ < 9)
#  define MTL_NO_TEMPLATE
#elif defined(__KCC_VERSION) && (__KCC_VERSION < 3400)
#  define MTL_NO_TEMPLATE
#elif defined(__MWERKS__)
#  define MTL_NO_TEMPLATE
#endif


#ifdef MTL_NO_TEMPLATE
#define MTL_TEMPLATE
#else
#define MTL_TEMPLATE template
#endif

#if defined ( _MSVCPP_ )
#define MTL_MAX(X,Y) (X > Y ? X : Y)
#define MTL_MIN(X,Y) (X < Y ? X : Y)
#else
#define MTL_MIN(X,Y) std::min(X,Y)
#define MTL_MAX(X,Y) std::max(X,Y)
#endif

#define MTL_CONJ std::conj
#define MTL_ABS std::abs


#ifdef _MSVCPP_
#define MTL_PARTIAL_SPEC 0
#else
#define MTL_PARTIAL_SPEC 1
#endif


#if defined(__GNUC__) || defined(_MSVCPP_) || defined( __MWERKS__)
#define MTL_DISABLE_BLOCKING
#endif

#if defined(MTL_FORTRAN_SYMBOLS_WITH_DOUBLE_TRAILING_UNDERSCORES)
#define MTL_FSYMB(X) X##__
#elif defined(MTL_FORTRAN_SYMBOLS_WITH_TRAILING_UNDERSCORES)
#define MTL_FSYMB(X) X##_
#else
#define MTL_FSYMB(X) X
#endif


#define PACKAGE "mtl"
#define VERSION "2.1.2-20"

#endif

