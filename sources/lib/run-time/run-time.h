#ifndef OPENDYLAN_CRT_RUNTIME_H
#define OPENDYLAN_CRT_RUNTIME_H

#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <setjmp.h>

/* For ssize_t definition (POSIX extension) */
#ifndef OPEN_DYLAN_PLATFORM_WINDOWS
#include <sys/types.h>
#endif

#ifdef __GNUC__
#  define OPEN_DYLAN_COMPILER_GCC_LIKE
#  if defined(__clang__)
#    define OPEN_DYLAN_COMPILER_CLANG
#  else
#    define OPEN_DYLAN_COMPILER_GCC
#  endif
#else
#  warning Unknown compiler
#endif

#ifdef __has_builtin
#    define OPEN_DYLAN_COMPILER_HAS_BUILTIN(x) __has_builtin(x)
#else
#    define OPEN_DYLAN_COMPILER_HAS_BUILTIN(x) 0
#endif

/* This check is needed to bootstrap from the 2011.1 release. */
#if !defined(OPEN_DYLAN_PLATFORM_DARWIN) && defined(__APPLE__)
#  define OPEN_DYLAN_PLATFORM_DARWIN 1
#endif

#ifdef OPEN_DYLAN_PLATFORM_DARWIN
#  include "AvailabilityMacros.h"
#  if !defined(OPEN_DYLAN_COMPILER_CLANG) || \
      (__clang_major__ < 3) || \
      (MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_7)
#    define USE_PTHREAD_TLS 1
#  endif
#endif

#define OPTIONAL_ARGUMENT_CHECK(fn, req, count)
#define REQUIRED_ARGUMENT_CHECK(fn, req, count)

/* CONCRETE RAW TYPES */

typedef signed char             INT8;
typedef unsigned char           UINT8;
typedef signed short            INT16;
typedef unsigned short          UINT16;
typedef signed long             INT32;
typedef unsigned long           UINT32;
#ifdef OPEN_DYLAN_PLATFORM_WINDOWS
typedef _int64                  INT64;
typedef unsigned _int64         UINT64;
#else
typedef signed long long        INT64;
typedef unsigned long long      UINT64;
#endif
typedef float                   FLT;
typedef double                  DFLT;
#ifdef OPEN_DYLAN_PLATFORM_WINDOWS
typedef double                  EFLT;
#else
typedef long double             EFLT;
#endif

/* C RAW TYPES */

typedef INT64                   long_long;
typedef UINT64                  unsigned_long_long;
typedef EFLT                    long_double;

/* DYLAN RAW TYPES */

typedef unsigned long           DBOOL;
typedef unsigned char           DBCHR;
typedef unsigned char           DBYTE;
typedef UINT16                  DDBYTE;
typedef UINT16                  DUCHR;
typedef long                    DSINT;
typedef long                    DMINT;
typedef DMINT                   DWORD;
typedef unsigned long           DUMINT;
typedef INT64                   DLMINT;
typedef DLMINT                  DDWORD;
typedef UINT64                  DULMINT;
typedef INT64                   DBINT;
typedef float                   DSFLT;
typedef double                  DDFLT;
typedef long double             DEFLT;
typedef unsigned long           DADDR;
typedef char*                   DBSTR;
typedef const char*             DCBSTR;
typedef void*                   dylan_value;
typedef void*                   D;

/* COMPILER-SPECIFIC INTRINSICS */


#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define OPEN_DYLAN_NO_INLINE __attribute__((noinline))
#else
#  define OPEN_DYLAN_NO_INLINE
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define NORETURN_FUNCTION __attribute__((noreturn))
#else
#  define NORETURN_FUNCTION
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define PURE_FUNCTION __attribute__((pure))
#else
#  warning missing attribute PURE_FUNCTION - performance degraded
#  define PURE_FUNCTION
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define CONDITIONAL_UPDATE(var, new_val, old_val) \
     (__sync_bool_compare_and_swap(&var, old_val, new_val) ? DTRUE : DFALSE)
#else
#  warning missing primitive CONDITIONAL_UPDATE - thread safety compromised
#  define CONDITIONAL_UPDATE(var, new_val, old_val) \
     ((old_val) == (var) ? (var = (new_val), DTRUE) : DFALSE)
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define SYNCHRONIZE_SIDE_EFFECTS() __sync_synchronize()
#else
#  warning missing primitive SYNCHRONIZE_SIDE_EFFECTS - thread safety compromised
#  define SYNCHRONIZE_SIDE_EFFECTS()
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define SEQUENCE_POINT() __asm__ __volatile__ ("" ::: "memory")
#else
#  warning missing primitive SEQUENCE_POINT - thread safety compromised
#  define SEQUENCE_POINT()
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
#  define TLS_VARIABLE __thread
#else
#  define TLS_VARIABLE
#endif

#ifdef OPEN_DYLAN_COMPILER_GCC
#  define TLS_INITIAL_EXEC __attribute__((tls_model("initial-exec")))
#endif

#if defined(OPEN_DYLAN_COMPILER_CLANG) && defined(__has_attribute)
#  if __has_attribute(tls_model)
#    define TLS_INITIAL_EXEC __attribute__((tls_model("initial-exec")))
#  endif
#endif

#ifndef TLS_INITIAL_EXEC
#  define TLS_INITIAL_EXEC
#endif

static inline long atomic_increment(long *var) {
#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
  return __sync_add_and_fetch(var, 1);
#else
#warning missing primitive atomic_increment - thread safety compromised
  *var = *var + 1;
  return *var;
#endif
}

static inline long atomic_decrement(long *var) {
#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
  return __sync_sub_and_fetch(var, 1);
#else
#warning missing primitive atomic_decrement - thread safety compromised
  *var = *var - 1;
  return *var;
#endif
}

static inline long atomic_cas(long *destination, long exchange, long compare) {
#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
  return __sync_val_compare_and_swap(destination, compare, exchange);
#else
#warning missing primitive atomic_cas - thread safety compromised
  int old = *destination;
  if (old == compare) {
     *destination = exchange;
  }
  return old;
#endif
}

/* DYLAN TAGGING */

#define TAG_BITS(x) (((unsigned long) x)&3)

#define ITAG 1
#define CTAG 2
#define UTAG 3

#define I(n) ((dylan_value)((((unsigned long)(n))<<2)|ITAG))
#define C(n) ((dylan_value)((((unsigned long)(n))<<2)|CTAG))
#define U(n) ((dylan_value)((((unsigned long)(n))<<2)|UTAG))
#define R(n) ((long)(n)>>2)

#define FI(n) ((dylan_value)(((unsigned long)(n))|ITAG))
#define FC(n) ((dylan_value)(((unsigned long)(n))|CTAG))
#define FU(n) ((dylan_value)(((unsigned long)(n))|UTAG))

#define TAGGEDQ(n) (TAG_BITS(n) != 0)

#define DTRUE  &KPtrueVKi
#define DFALSE &KPfalseVKi

/* BASIC DYLAN TYPES */

typedef dylan_value (*DFN)(dylan_value,int,...);
typedef dylan_value (*DLFN)();

struct _dylan_implementation_class;
struct _dylan_type;
struct _dylan_class;

/* This corresponds to <mm-wrapper> defined in
 * dfmc/modeling/objects.dylan.
 */
typedef struct _Wrapper {
  struct _Wrapper *                     wrapper_wrapper;
  struct _dylan_implementation_class  * iclass;
  dylan_value                           subtype_mask;
  DMINT                                 fixed_part;
  DMINT                                 variable_part;
  dylan_value                           number_patterns;
  DMINT                                 patterns[1]; /* REPEATED */
} Wrapper;

typedef struct _dylan_object {
  Wrapper*      mm_wrapper;
  dylan_value   slots[1];
} dylan_object;

typedef struct _dylan_single_float {
  Wrapper * mm_wrapper;
  DSFLT     data;
} dylan_single_float;

typedef struct _dylan_double_float {
  Wrapper * mm_wrapper;
  DDFLT     data;
} dylan_double_float;

typedef struct _dylan_machine_word {
  Wrapper * mm_wrapper;
  DMINT     data;
} dylan_machine_word;

typedef struct _dylan_unsigned_machine_word {
  Wrapper * mm_wrapper;
  DUMINT    data;
} dylan_unsigned_machine_word;

typedef struct _dylan_double_integer {
  Wrapper * mm_wrapper;
  DUMINT    low;
  DMINT     high;
} dylan_double_integer;

/* This is the implementation class and corresponds
 * to the <implementation-class> defined in
 * dfmc/modeling/objects.dylan. Note that this
 * struct declaration is not the full struct.
 *
 * For a breakdown of the class properties, see
 * the packed-slots definition for ^class-properties
 * in dfmc/modeling/objects.dylan or the copy of it
 * in dylan/class.dylan.
 */
typedef struct _dylan_implementation_class {
  Wrapper *             my_wrapper;
  dylan_value           the_class_properties;
  struct _dylan_class * the_class;
  dylan_value           the_wrapper;
} dylan_implementation_class;

/* This corresponds to <type> defined in
 * dfmc/modeling/objects.dylan.
 */
typedef struct _dylan_type {
  Wrapper * mm_wrapper;
  DLFN      instancep_function;
} dylan_type;

/* This corresponds to <class> defined in
 * dfmc/modeling/objects.dylan.
 */
typedef struct _dylan_class {
  Wrapper *                     my_wrapper;
  DLFN                          instancep_function;
  dylan_value                   debug_name;
  dylan_implementation_class  * the_iclass;
  dylan_value                   subtype_bit;
} dylan_class;

#define OBJECT_WRAPPER(x) \
    (((dylan_object*)(x))->mm_wrapper)

#define OBJECT_ICLASS(x) \
    (((Wrapper*)(OBJECT_WRAPPER(x)))->iclass)

#define OBJECT_CLASS(x) \
    (((dylan_implementation_class*)(OBJECT_ICLASS(x)))->the_class)

#define CLASS_WRAPPER(x) \
    (((dylan_class*)(x))->the_wrapper)

#define CLASS_ICLASS(x) \
    (((dylan_class*)(x))->the_iclass)

#define ICLASS_WRAPPER(x) \
    (((dylan_implementation_class*)(x))->the_wrapper)

#define ICLASS_CLASS(x) \
    (((dylan_implementation_class*)(x))->the_class)

#define WRAPPER_ICLASS(x) \
    (((Wrapper*)(x))->iclass)

#define WRAPPER_CLASS(x) \
    (ICLASS_CLASS(WRAPPER_ICLASS(x)))

typedef struct _dylan_simple_object_vector {
  dylan_value class;
  dylan_value size;
  dylan_value data[1];
} dylan_simple_object_vector;

static inline int vector_size (dylan_simple_object_vector* vector) {
  return(R(vector->size));
}

static inline int vector_size_setter (int new_size, dylan_simple_object_vector* vector) {
  vector->size = I(new_size);
  return(new_size);
}

static inline dylan_value* vector_data(dylan_simple_object_vector* vector) {
  return(vector->data);
}

static inline dylan_value vector_ref(dylan_simple_object_vector* vector, int offset) {
  return(vector_data((dylan_simple_object_vector*)vector)[offset]);
}

typedef struct _dylan_byte_string {
  dylan_value class;
  dylan_value size;
  char data[1];
} dylan_byte_string;

typedef struct _dylan_symbol {
  dylan_value   class;
  dylan_value   name;
} dylan_symbol;

typedef struct _dylan_simple_method {
  dylan_value   class;
  DFN           xep;
  dylan_value   signature;
  DLFN          mep;
} dylan_simple_method;

typedef struct _dylan_simple_closure_method {
  dylan_value   class;
  DFN           xep;
  dylan_value   signature;
  DLFN          mep;
  dylan_value   size;
  dylan_value   environment[0];
} dylan_simple_closure_method;

typedef struct _dylan_keyword_method {
  dylan_value   class;
  DFN           xep;
  dylan_value   signature;
  DLFN          mep;
  DLFN          iep;
  dylan_value   keyword_specifiers;
} dylan_keyword_method;

typedef struct _dylan_keyword_closure_method {
  dylan_value   class;
  DFN           xep;
  dylan_value   signature;
  DLFN          mep;
  DLFN          iep;
  dylan_value   keyword_specifiers;
  dylan_value   size;
  dylan_value   environment[0];
} dylan_keyword_closure_method;

typedef struct _dylan_accessor_method {
  dylan_value   header;
  DFN           xep;
  dylan_value   slotd;
} dylan_accessor_method;

typedef struct _dylan_signature {
  dylan_value   class;
  dylan_value   properties;
  dylan_simple_object_vector* required;
  dylan_simple_object_vector* values;
  dylan_value   rest_value;
} dylan_signature;

typedef struct _engine {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
} ENGINE;

typedef struct _monomorphic_discriminator {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  DWORD         key;
  dylan_value   nextnode;
} MONOMORPHICDISCRIMINATOR;

typedef struct _if_type_discriminator {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  dylan_value   type;
  dylan_value   thennode;
  dylan_value   elsenode;
} IFTYPEDISCRIMINATOR;

typedef struct _typecheck_discriminator {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  dylan_value   type;
  dylan_value   nextnode;
} TYPECHECKDISCRIMINATOR;


typedef struct _single_method_engine_node {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  dylan_value   meth;
  dylan_value   data;
  dylan_value   keywords;                /* Not in all. */
} SINGLEMETHODENGINE;

typedef struct _cache_header_engine_node {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  dylan_value   nextnode;
  dylan_value   parent;
} CACHEHEADERENGINE;

typedef struct _profiling_cache_header_engine_node {
  dylan_value   class;
  dylan_value   properties;
  DLFN          callback;
  DLFN          entry_point;
  dylan_value   nextnode;
  dylan_value   parent;
  DSINT         count1;
  DSINT         count2;
} PROFILINGCACHEHEADERENGINE;


typedef struct _dylan_generic_function {
  dylan_value   class;
  DFN           xep;
  dylan_value   signature;
  dylan_value   cache;
  dylan_value   debug_name;
  dylan_value   methods;
  ENGINE*       engine;
} dylan_generic_function;

#define DEFUN(name, xep, iep)   dylan_value name[] = {I(0),I(0),I(0),(dylan_value)xep,(dylan_value)iep,I(0),I(0),I(0),I(0)}

extern dylan_value primitive_set_generic_function_entrypoints(dylan_value fn); /* !@#$ FIX UP NAME */

extern dylan_value primitive_runtime_module_handle();

/* MULTIPLE VALUES */

#define VALUES_MAX 64           /* maximum number of multiple values */

typedef struct _mv {
  int           count;
  dylan_value   value[VALUES_MAX];
} MV;

#define MV_GET_ELT(n) \
  (get_teb()->return_values.count > (n) ? get_teb()->return_values.value[n] : DFALSE)
#define MV_SET_ELT(n, t)        (get_teb()->return_values.value[n] = (t))
#define MV_SET_COUNT(n)         (get_teb()->return_values.count = (n))

extern dylan_value MV_SPILL (dylan_value first_value);
extern dylan_value MV_UNSPILL (dylan_value spill_t);
extern dylan_value MV_GET_REST_AT (dylan_value first_value, DSINT first);
extern dylan_value MV_SET_REST_AT (dylan_value v, DSINT first);
extern dylan_value MV_CHECK_TYPE_REST (dylan_value first_value, dylan_value rest_type, int n, ...);

#define MV_CHECK_TYPE_PROLOGUE(fv) \
  MV *spill;                       \
  spill = (MV*)MV_SPILL(fv)

#define MV_CHECK_TYPE_EPILOGUE()   \
  MV_UNSPILL((dylan_value)spill)

extern DMINT _unused_arg;
extern DMINT* P_unused_arg;

/* NON-LOCAL CONTROL FLOW FRAMES */

//#define VERIFY_NLX

typedef struct _bind_exit_frame {
  jmp_buf                       destination;
  MV                            return_values;
  struct _unwind_protect_frame* present_unwind_protect_frame;
#ifdef VERIFY_NLX
  struct _teb*                  verify_teb;
#endif
} Bind_exit_frame;

typedef struct _unwind_protect_frame {
  jmp_buf                       destination;
  MV                            return_values;
  struct _bind_exit_frame*      ultimate_destination;
  struct _unwind_protect_frame* previous_unwind_protect_frame;
#ifdef VERIFY_NLX
  struct _teb*                  verify_teb;
#endif
} Unwind_protect_frame;

extern dylan_value SETUP_EXIT_FRAME (dylan_value);
extern dylan_value SETUP_UNWIND_FRAME (dylan_value);
extern dylan_value FRAME_DEST (dylan_value);
extern dylan_value FRAME_RETVAL (dylan_value);
extern dylan_value FALL_THROUGH_UNWIND (dylan_value);
extern dylan_value CONTINUE_UNWIND ();
extern dylan_value NLX (Bind_exit_frame*, dylan_value);

#define ENTER_EXIT_FRAME(destvar) \
  Bind_exit_frame bef_ ## destvar; \
  destvar = SETUP_EXIT_FRAME(& bef_ ## destvar)

#define ENTER_UNWIND_FRAME(destvar) \
  Unwind_protect_frame uwp_ ## destvar; \
  destvar = SETUP_UNWIND_FRAME(& uwp_ ## destvar)

#define nlx_longjmp(env, val)_longjmp(env, val);
#define nlx_setjmp(env) _setjmp(env)

/* PER-THREAD CONTEXT */

#define MAX_ARGUMENTS 256

typedef struct _teb {
        /* dispatch context (used together, keep close) */
        dylan_simple_method* function;
        int argument_count;
        dylan_value   next_methods;

        /* return values (for multiple values) */
        MV  return_values;

        /* unwinding state */
        Unwind_protect_frame* uwp_frame;
        Unwind_protect_frame  top_uwp_frame;

        /* thread state */
        void *thread;
        void *thread_handle;
        void *tlv_vector;

        /* argument buffers (used in dispatch, primitives...) */
        dylan_value arguments[MAX_ARGUMENTS];
        dylan_value new_arguments[MAX_ARGUMENTS];
        dylan_value a[MAX_ARGUMENTS];
        dylan_value iep_a[MAX_ARGUMENTS];
        dylan_value apply_buffer[MAX_ARGUMENTS];
        dylan_value buffer[MAX_ARGUMENTS];
} TEB;

#ifdef USE_PTHREAD_TLS
extern PURE_FUNCTION TEB* get_teb(void);
#else
extern TLS_VARIABLE TLS_INITIAL_EXEC TEB* dylan_teb;
PURE_FUNCTION static inline TEB* get_teb()
{
  return dylan_teb;
}
#endif

/* CALLING CONVENTION ENTRY POINTS */

extern dylan_value xep_0 (dylan_simple_method*,int);
extern dylan_value xep_1 (dylan_simple_method*,int,dylan_value);
extern dylan_value xep_2 (dylan_simple_method*,int,dylan_value,dylan_value);
extern dylan_value xep_3 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_4 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_5 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_6 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_7 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_8 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep_9 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value);
extern dylan_value xep   (dylan_simple_method*,int,...);

extern dylan_value rest_xep_0 (dylan_simple_method*,int,...);
extern dylan_value rest_xep_1 (dylan_simple_method*,int,dylan_value,...);
extern dylan_value rest_xep_2 (dylan_simple_method*,int,dylan_value,dylan_value,...);
extern dylan_value rest_xep_3 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_4 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_5 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_6 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_7 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_8 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep_9 (dylan_simple_method*,int,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,dylan_value,...);
extern dylan_value rest_xep   (dylan_simple_method*,int,...);

extern dylan_value rest_key_xep_1 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_2 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_3 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_4 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_5 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_6 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_7 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_8 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep_9 (dylan_simple_method*,int,...);
extern dylan_value rest_key_xep   (dylan_simple_method*,int,...);

extern dylan_value key_mep_1 (dylan_value a1, ...);
extern dylan_value key_mep_2 (dylan_value a1, ...);
extern dylan_value key_mep_3 (dylan_value a1, ...);
extern dylan_value key_mep_4 (dylan_value a1, ...);
extern dylan_value key_mep_5 (dylan_value a1, ...);
extern dylan_value key_mep_6 (dylan_value a1, ...);
extern dylan_value key_mep_7 (dylan_value a1, ...);
extern dylan_value key_mep_8 (dylan_value a1, ...);
extern dylan_value key_mep_9 (dylan_value a1, ...);
extern dylan_value key_mep (dylan_value a1, ...);

extern dylan_value gf_xep_0 (dylan_simple_method* fn, int n);
extern dylan_value gf_xep_1 (dylan_simple_method* fn, int n, dylan_value a1);
extern dylan_value gf_xep_2 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2);
extern dylan_value gf_xep_3 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2, dylan_value a3);
extern dylan_value gf_xep_4 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4);
extern dylan_value gf_xep_5 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5);
extern dylan_value gf_xep_6 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6);
extern dylan_value gf_xep_7 (dylan_simple_method* fn, int n, dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6, dylan_value a7);
extern dylan_value gf_xep   (dylan_simple_method* fn, int n, ...);

extern dylan_value gf_optional_xep_0 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_1 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_2 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_3 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_4 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_5 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep_6 (dylan_simple_method* fn, int n, ...);
extern dylan_value gf_optional_xep   (dylan_simple_method* fn, int n, ...);

extern dylan_value gf_iep_0 ();
extern dylan_value gf_iep_1 (dylan_value a1);
extern dylan_value gf_iep_2 (dylan_value a1, dylan_value a2);
extern dylan_value gf_iep_3 (dylan_value a1, dylan_value a2, dylan_value a3);
extern dylan_value gf_iep_4 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4);
extern dylan_value gf_iep_5 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5);
extern dylan_value gf_iep_6 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6);
extern dylan_value gf_iep_7 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6, dylan_value a7);
extern dylan_value gf_iep   (dylan_value argvec);

extern dylan_value slotacc_single_q_instance_getter_xep (dylan_accessor_method* am, int n, dylan_value a1);
extern dylan_value slotacc_single_q_instance_setter_xep (dylan_accessor_method* am, int n, dylan_value a1, dylan_value a2);
extern dylan_value slotacc_single_q_class_getter_xep (dylan_accessor_method* am, int n, dylan_value a1);
extern dylan_value slotacc_single_q_class_setter_xep (dylan_accessor_method* am, int n, dylan_value a1, dylan_value a2);
extern dylan_value slotacc_repeated_instance_getter_xep (dylan_accessor_method* am, int n, dylan_value a1, dylan_value a2);
extern dylan_value slotacc_repeated_instance_setter_xep (dylan_accessor_method* am, int n, dylan_value a1, dylan_value a2, dylan_value a3);
extern dylan_value primitive_set_accessor_method_xep (dylan_value am, dylan_value what);

#define CALLN(fn,n) (dylan_value)((((dylan_simple_method*)fn)->xep)(((dylan_simple_method*)(fn)),n
#define CALL0(fn) CALLN(fn,0)))
#define CALL1(fn,a1) CALLN(fn,1),(a1)))
#define CALL2(fn,a1,a2) CALLN(fn,2),(a1),(a2)))
#define CALL3(fn,a1,a2,a3) CALLN(fn,3),(a1),(a2),(a3)))
#define CALL4(fn,a1,a2,a3,a4) CALLN(fn,4),(a1),(a2),(a3),(a4)))
#define CALL5(fn,a1,a2,a3,a4,a5) CALLN(fn,5),(a1),(a2),(a3),(a4),(a5)))
#define CALL6(fn,a1,a2,a3,a4,a5,a6) CALLN(fn,6),(a1),(a2),(a3),(a4),(a5),(a6)))
#define CALL7(fn,a1,a2,a3,a4,a5,a6,a7) CALLN(fn,7),(a1),(a2),(a3),(a4),(a5),(a6),(a7)))
#define CALL8(fn,a1,a2,a3,a4,a5,a6,a7,a8) CALLN(fn,8),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)))
#define CALL9(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9) CALLN(fn,9),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)))
#define CALL10(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) CALLN(fn,10),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10)))

#define MEP_CALL_PROLOG(fn,nm,ac) \
  {\
    TEB* mcp_teb = get_teb(); \
    mcp_teb->function = (dylan_simple_method*)(fn); \
    mcp_teb->next_methods = (nm); \
    mcp_teb->argument_count = (ac); \
  }
#define MEP_CALLN(fn) (dylan_value)((((dylan_simple_method*)fn)->mep)(
#define MEP_CALL0(fn) MEP_CALLN(fn)))
#define MEP_CALL1(fn,a1) MEP_CALLN(fn)(a1)))
#define MEP_CALL2(fn,a1,a2) MEP_CALLN(fn)(a1),(a2)))
#define MEP_CALL3(fn,a1,a2,a3) MEP_CALLN(fn)(a1),(a2),(a3)))
#define MEP_CALL4(fn,a1,a2,a3,a4) MEP_CALLN(fn)(a1),(a2),(a3),(a4)))
#define MEP_CALL5(fn,a1,a2,a3,a4,a5) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5)))
#define MEP_CALL6(fn,a1,a2,a3,a4,a5,a6) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6)))
#define MEP_CALL7(fn,a1,a2,a3,a4,a5,a6,a7) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7)))
#define MEP_CALL8(fn,a1,a2,a3,a4,a5,a6,a7,a8) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)))
#define MEP_CALL9(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)))
#define MEP_CALL10(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) MEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10)))

#define MIEP_CALL_PROLOG(nm) { get_teb()->next_methods = (nm); }

#define ENGINE_NODE_CALL_PROLOG(fn,eng,ac) \
  { \
    TEB* encp_teb = get_teb(); \
    encp_teb->next_methods = (fn); \
    encp_teb->function = (dylan_simple_method*)(eng); \
    encp_teb->argument_count =(ac); \
  }
#define ENGINE_NODE_CALL(fn,eng,ac) \
  { \
    TEB* enc_teb = get_teb(); \
    enc_teb->next_methods = (fn); \
    enc_teb->function = (dylan_simple_method*)(eng); \
  }

#define ENGINE_NODE_CALL0(eng) \
    ((((ENGINE*)eng)->entry_point)())
#define ENGINE_NODE_CALL1(eng,a1) \
    ((((ENGINE*)eng)->entry_point)((a1)))
#define ENGINE_NODE_CALL2(eng,a1,a2) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2)))
#define ENGINE_NODE_CALL3(eng,a1,a2,a3) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2),(a3)))
#define ENGINE_NODE_CALL4(eng,a1,a2,a3,a4) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2),(a3),(a4)))
#define ENGINE_NODE_CALL5(eng,a1,a2,a3,a4,a5) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2),(a3),(a4),(a5)))
#define ENGINE_NODE_CALL6(eng,a1,a2,a3,a4,a5,a6) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2),(a3),(a4),(a5),(a6)))
#define ENGINE_NODE_CALL7(eng,a1,a2,a3,a4,a5,a6,a7) \
    ((((ENGINE*)eng)->entry_point)((a1),(a2),(a3),(a4),(a5),(a6),(a7)))

extern dylan_value inline_invoke_engine_node(ENGINE*, int, ...);

#define ENGINE_NODE_CALLN(ac,eng) \
    (inline_invoke_engine_node((ENGINE*)(eng),(ac)

#define CONGRUENT_CALL_PROLOG(fn,ac) \
  { \
    TEB *ccp_teb = get_teb(); \
    ccp_teb->next_methods = (fn); \
    ccp_teb->function = (dylan_simple_method*)(((dylan_generic_function*)fn)->engine); \
    ccp_teb->argument_count =(ac); \
  }
#define CONGRUENT_CALL(fn,ac) \
  { \
    TEB *cc_teb = get_teb(); \
    cc_teb->next_methods = (fn); \
    cc_teb->function = (dylan_simple_method*)(((dylan_generic_function*)fn)->engine); \
  }

#define CONGRUENT_CALL0() \
    ((((ENGINE*)get_teb()->function)->entry_point)())
#define CONGRUENT_CALL1(a1) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1)))
#define CONGRUENT_CALL2(a1,a2) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2)))
#define CONGRUENT_CALL3(a1,a2,a3) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2),(a3)))
#define CONGRUENT_CALL4(a1,a2,a3,a4) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2),(a3),(a4)))
#define CONGRUENT_CALL5(a1,a2,a3,a4,a5) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2),(a3),(a4),(a5)))
#define CONGRUENT_CALL6(a1,a2,a3,a4,a5,a6) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2),(a3),(a4),(a5),(a6)))
#define CONGRUENT_CALL7(a1,a2,a3,a4,a5,a6,a7) \
    ((((ENGINE*)get_teb()->function)->entry_point)((a1),(a2),(a3),(a4),(a5),(a6),(a7)))

#define CONGRUENT_CALLN(ac) \
    (inline_invoke_engine_node((ENGINE*)(get_teb()->function),(ac)


#define IEP_CALLN(fn) (dylan_value)((((dylan_simple_method*)fn)->iep)(
#define IEP_CALL0(fn) IEP_CALLN(fn)))
#define IEP_CALL1(fn,a1) IEP_CALLN(fn)(a1)))
#define IEP_CALL2(fn,a1,a2) IEP_CALLN(fn)(a1),(a2)))
#define IEP_CALL3(fn,a1,a2,a3) IEP_CALLN(fn)(a1),(a2),(a3)))
#define IEP_CALL4(fn,a1,a2,a3,a4) IEP_CALLN(fn)(a1),(a2),(a3),(a4)))
#define IEP_CALL5(fn,a1,a2,a3,a4,a5) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5)))
#define IEP_CALL6(fn,a1,a2,a3,a4,a5,a6) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6)))
#define IEP_CALL7(fn,a1,a2,a3,a4,a5,a6,a7) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7)))
#define IEP_CALL8(fn,a1,a2,a3,a4,a5,a6,a7,a8) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)))
#define IEP_CALL9(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)))
#define IEP_CALL10(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) IEP_CALLN(fn)(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10)))

/* CLOSURE SUPPORT */

extern void INIT_CLOSURE(dylan_value, int, ...);
extern dylan_value MAKE_CLOSURE_INITD(dylan_value, int, ...);
extern dylan_value MAKE_CLOSURE_INITD_SIG(dylan_value, dylan_value, int, ...);
extern dylan_value MAKE_CLOSURE(dylan_value, int);
extern dylan_value MAKE_CLOSURE_SIG(dylan_value, dylan_value, int);
extern dylan_value MAKE_METHOD_SIG(dylan_value, dylan_value);
extern dylan_value SET_METHOD_SIG(dylan_value, dylan_value);
#define CAPTURE_ENVIRONMENT dylan_simple_closure_method* _fn = ((dylan_simple_closure_method*)get_teb()->function);
extern void INIT_KEYWORD_CLOSURE(dylan_value, int, ...);
extern dylan_value MAKE_KEYWORD_CLOSURE_INITD(dylan_value, int, ...);
extern dylan_value MAKE_KEYWORD_CLOSURE_INITD_SIG(dylan_value, dylan_value, int, ...);
extern dylan_value MAKE_KEYWORD_CLOSURE(dylan_value, int);
extern dylan_value MAKE_KEYWORD_CLOSURE_SIG(dylan_value, dylan_value, int);
extern dylan_value MAKE_KEYWORD_METHOD_SIG(dylan_value, dylan_value);
extern dylan_value SET_KEYWORD_METHOD_SIG(dylan_value, dylan_value);
#define CAPTURE_KEYWORD_ENVIRONMENT dylan_keyword_closure_method* _fn = ((dylan_keyword_closure_method*)get_teb()->function);
#define CREF(n) (_fn->environment[(n)])
#define MREF    (_fn)

/*
 * PRIMITIVES
 */

/* COMPARISON PRIMITIVES */

#define RAWASBOOL(x)                    ((dylan_value)((x) ? DTRUE : DFALSE))
#define primitive_raw_as_boolean(x)     RAWASBOOL(x)
#define primitive_as_boolean(x)         (((dylan_value)(x)) == DFALSE ? DFALSE : DTRUE)
#define BOOLASRAW(x)                    (((dylan_value)(x)) != DFALSE)
#define primitive_boolean_as_raw(x)     BOOLASRAW(x)
#define primitive_not(x)                (((dylan_value)(x)) == DFALSE ? DTRUE : DFALSE)
#define primitive_idQ(x, y)             (RAWASBOOL((x) == (y)))
#define primitive_not_idQ(x, y)         (RAWASBOOL((x) != (y)))

extern dylan_value primitive_compare_bytes(dylan_value base1, DSINT offset1,
                                           dylan_value base2, DSINT offset2, DSINT size);
extern dylan_value primitive_compare_words(dylan_value base1, DSINT offset1,
                                           dylan_value base2, DSINT offset2, DSINT size);

/* TYPE CONSTRAINTS */
#define primitive_the(t, x)             (x)

/* COMPARISON PRIMITIVES */

#define primitive_instanceQ(x, y)       ((((dylan_type*)(y))->instancep_function)((x),(y)))
#define primitive_range_check(x, l, h)  (RAWASBOOL(((x) >= (l)) & ((x) < (h))))
extern dylan_value primitive_type_check(dylan_value x, dylan_value t);


/* OBJECT REPRESENTATION PRIMITIVES AND SUPPORT */

extern void primitive_break (void);
extern void primitive_invoke_debugger (dylan_value format_string, dylan_value arguments);
extern dylan_value primitive_inside_debuggerQ (void);
extern void primitive_debug_message (dylan_value format_string, dylan_value arguments);

#if defined(__alpha) || defined(__x86_64__)
#define primitive_word_size() 8
#else
#define primitive_word_size() 4
#endif

#define primitive_header_size() primitive_word_size ()

#if OPEN_DYLAN_COMPILER_HAS_BUILTIN(__builtin_readcyclecounter)
#  define primitive_read_cycle_counter() __builtin_readcyclecounter()
#elif defined(OPEN_DYLAN_COMPILER_GCC_LIKE) && \
      (defined(OPEN_DYLAN_ARCH_X86) || defined(OPEN_DYLAN_ARCH_X86_64))
static inline uint64_t primitive_read_cycle_counter(void)
{
   uint32_t hi, lo;
   __asm__ __volatile__ ("rdtsc" : "=a"(lo), "=d"(hi));
   return ((uint64_t)lo) | (((uint64_t)hi) << 32);
}
#else
#  define primitive_read_cycle_counter() 0
#endif

#if defined(OPEN_DYLAN_COMPILER_GCC_LIKE)
#  define primitive_read_return_address() __builtin_return_address(0)
#else
#  define primitive_read_return_address() 0
#endif



/* Well this wasn't right
#define primitive_object_class(object) (((dylan_object*)object)->class)
*/

#define primitive_initialized_slot_value(object, position) \
  ((((dylan_object*)object)->slots)[position])
#define primitive_slot_value_setter(new_value, object, position) \
  ((((dylan_object*)object)->slots)[position] = (new_value))

extern dylan_value primitive_slot_value(dylan_value object, DSINT position);

#define primitive_repeated_slot_value(object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + position])
#define primitive_repeated_slot_value_setter(new_value, object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + position] = (new_value))
#define primitive_repeated_instance_size(object, base_position) \
  (R((((dylan_object*)object)->slots)[base_position - 1]))

#define primitive_byte_element(object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[index])
#define primitive_byte_element_setter(new_value, object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define SLOT_VALUE_INITD(object, position) \
  ((((dylan_object*)object)->slots)[position])
#define SLOT_VALUE_SETTER(new_value, object, position) \
  ((((dylan_object*)object)->slots)[position] = (new_value))
#define RAW_SLOT_VALUE_SETTER(new_value, object, position) \
  ((((dylan_object*)object)->slots)[position] = (dylan_value)(new_value))

extern dylan_value SLOT_VALUE(dylan_value object, DSINT position);

#define REPEATED_DYLAN_VALUE_SLOT_VALUE(object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + position])
#define REPEATED_DYLAN_VALUE_SLOT_VALUE_SETTER(new_value, object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + position] = (new_value))

#define REPEATED_DBCHR_SLOT_VALUE(object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DBCHR_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DUCHR_SLOT_VALUE(object, position, index) \
        (((DUCHR*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DUCHR_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DUCHR*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DBYTE_SLOT_VALUE(object, position, index) \
        (((DBYTE*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DBYTE_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DBYTE*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DDBYTE_SLOT_VALUE(object, position, index) \
        (((DDBYTE*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DDBYTE_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DDBYTE*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DDFLT_SLOT_VALUE(object, position, index) \
        (((DDFLT*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DDFLT_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DDFLT*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DDWORD_SLOT_VALUE(object, position, index) \
        (((DDWORD*)&((((dylan_object*)object)->slots)[position]))[index])
#define REPEATED_DDWORD_SLOT_VALUE_SETTER(new_value, object, position, index) \
        (((DDWORD*)&((((dylan_object*)object)->slots)[position]))[index] = \
           (new_value))

#define REPEATED_DYLAN_VALUE_SLOT_VALUE_TAGGED(object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + R(position)])
#define REPEATED_DYLAN_VALUE_SLOT_VALUE_TAGGED_SETTER(new_value, object, base_position, position) \
  ((((dylan_object*)object)->slots)[base_position + R(position)] = (new_value))

#define REPEATED_DWORD_SLOT_VALUE_TAGGED(object, position, index)       \
        (((DWORD*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DWORD_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DWORD*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

#define REPEATED_DSFLT_SLOT_VALUE_TAGGED(object, position, index) \
        (((DSFLT*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DSFLT_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DSFLT*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

#define REPEATED_DDFLT_SLOT_VALUE_TAGGED(object, position, index)       \
        (((DDFLT*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DDFLT_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DDFLT*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

/* SHOULD REMOVE BELOW */

#define REPEATED_DBCHR_SLOT_VALUE_TAGGED(object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DBCHR_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DBCHR*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

#define REPEATED_DUCHR_SLOT_VALUE_TAGGED(object, position, index) \
        (((DUCHR*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DUCHR_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DUCHR*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

#define REPEATED_DBYTE_SLOT_VALUE_TAGGED(object, position, index) \
        (((DBYTE*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DBYTE_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DBYTE*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

#define REPEATED_DDBYTE_SLOT_VALUE_TAGGED(object, position, index) \
        (((DDBYTE*)&((((dylan_object*)object)->slots)[position]))[R(index)])
#define REPEATED_DDBYTE_SLOT_VALUE_TAGGED_SETTER(new_value, object, position, index) \
        (((DDBYTE*)&((((dylan_object*)object)->slots)[position]))[R(index)] = \
           (new_value))

extern DSINT primitive_repeated_slot_offset(dylan_value x);
extern dylan_value     primitive_repeated_slot_as_raw(dylan_value x, DSINT offset);

extern void primitive_fillX
  (dylan_value dst, int base_offset, int offset, int size, dylan_value value);

extern void primitive_fill_bytesX
  (dylan_value dst, int base_offset, int offset, int size, DSINT value);

extern void primitive_replace_bytesX
  (dylan_value dst, DSINT dst_base_offset, DSINT dst_offset,
   dylan_value src, DSINT src_base_offset, DSINT src_offset, DSINT size);

extern void primitive_replaceX
  (dylan_value dst, DSINT dst_base_offset, DSINT dst_offset,
   dylan_value src, DSINT src_base_offset, DSINT src_offset, DSINT size);

/* LOW-LEVEL ACCESSOR PRIMITIVES */

#define AT(t,x,o,b)             (((t*)(((DADDR)(x))+(int)(b)))[(int)(o)])
#define AT_SETTER(t,e,x,o,b)    (AT(t,x,o,b) = (e))

#define primitive_c_pointer_at(x, o, b) AT(void *,x,o,b)
#define primitive_c_pointer_at_setter(e, x, o, b)  AT_SETTER(void*,e,x,o,b)

#define primitive_c_signed_int_at(x, o, b) \
        AT(signed int,x,o,b)
#define primitive_c_signed_int_at_setter(e, x, o, b) \
        AT_SETTER(signed int,e,x,o,b)
#define primitive_c_unsigned_int_at(x, o, b) \
        AT(unsigned int,x,o,b)
#define primitive_c_unsigned_int_at_setter(e, x, o, b) \
        AT_SETTER(unsigned int,e,x,o,b)
#define primitive_c_signed_char_at(x, o, b) \
        AT(signed char,x,o,b)
#define primitive_c_signed_char_at_setter(e, x, o, b) \
        AT_SETTER(signed char,e,x,o,b)
#define primitive_c_unsigned_char_at(x, o, b) \
        AT(unsigned char,x,o,b)
#define primitive_c_unsigned_char_at_setter(e, x, o, b) \
        AT_SETTER(unsigned char,e,x,o,b)
#define primitive_c_signed_short_at(x, o, b) \
        AT(signed short,x,o,b)
#define primitive_c_signed_short_at_setter(e, x, o, b) \
        AT_SETTER(signed short,e,x,o,b)
#define primitive_c_unsigned_short_at(x, o, b) \
        AT(unsigned short,x,o,b)
#define primitive_c_unsigned_short_at_setter(e, x, o, b) \
        AT_SETTER(unsigned short,e,x,o,b)
#define primitive_c_signed_long_at(x, o, b) \
        AT(signed long,x,o,b)
#define primitive_c_signed_long_at_setter(e, x, o, b) \
        AT_SETTER(signed long,e,x,o,b)
#define primitive_c_unsigned_long_at(x, o, b) \
        AT(unsigned long,x,o,b)
#define primitive_c_unsigned_long_at_setter(e, x, o, b) \
        AT_SETTER(unsigned long,e,x,o,b)
#define primitive_c_signed_long_long_at(x, o, b) \
        AT(signed_long_long,x,o,b)
#define primitive_c_signed_long_long_at_setter(e, x, o, b) \
        AT_SETTER(signed_long_long,e,x,o,b)
#define primitive_c_unsigned_long_long_at(x, o, b) \
        AT(unsigned_long_long,x,o,b)
#define primitive_c_unsigned_long_long_at_setter(e, x, o, b) \
        AT_SETTER(unsigned_long_long,e,x,o,b)
#define primitive_c_size_t_at(x, o, b) \
        AT(size_t,x,o,b)
#define primitive_c_size_t_at_setter(e, x, o, b) \
        AT_SETTER(size_t,e,x,o,b)
#define primitive_c_ssize_t_at(x, o, b) \
        AT(ssize_t,x,o,b)
#define primitive_c_ssize_t_at_setter(e, x, o, b) \
        AT_SETTER(ssize_t,e,x,o,b)
#define primitive_c_float_at(x, o, b) \
        AT(float,x,o,b)
#define primitive_c_float_at_setter(e, x, o, b) \
        AT_SETTER(float,e,x,o,b)
#define primitive_c_double_at(x, o, b) \
        AT(double,x,o,b)
#define primitive_c_double_at_setter(e, x, o, b) \
        AT_SETTER(double,e,x,o,b)
#define primitive_c_long_double_at(x, o, b) \
        AT(long_double,x,o,b)
#define primitive_c_long_double_at_setter(e, x, o, b) \
        AT_SETTER(long_double,e,x,o,b)

/*
 * bit-size and bit-offset are Dylan constants so a good C compiler
 * should be able to optimize these.
 */
#define MAKE_MASK(t, bit_offset, bit_size) \
  ((~(((t)(-1l)) << (bit_size))) << (bit_offset))


#define AT_FIELD(t, x, b, bit_offset, bit_size) \
  ((t)(((*((unsigned long *)(((DADDR)(x))+(b)))) >> (bit_offset)) \
       & MAKE_MASK(t, 0, bit_size)))


#define AT_FIELD_SETTER(t, n, x, b, bit_offset, bit_size) \
  (*((t *)(((DADDR)(x))+(b))) = \
   ((((n) & MAKE_MASK(t, 0, bit_size)) << (bit_offset)) \
     | ( (*((t *)(((DADDR)(x))+(b)))) \
         & ((t)~MAKE_MASK(t, bit_offset, bit_size)))))

#define primitive_c_unsigned_field(pointer, byte_offset, bit_offset, \
                                    bit_size) \
  AT_FIELD(unsigned long, pointer, byte_offset, bit_offset, bit_size)

#define primitive_c_signed_field(pointer, byte_offset, bit_offset, \
                                  bit_size) \
  AT_FIELD(signed long, pointer, byte_offset, bit_offset, bit_size)

#define primitive_c_int_field(pointer, byte_offset, bit_offset, \
                               bit_size) \
  AT_FIELD(unsigned int, pointer, byte_offset, bit_offset, bit_size)

#define primitive_c_unsigned_field_setter(new, pointer, byte_offset, \
                                           bit_offset, bit_size) \
  AT_FIELD_SETTER(unsigned long, new, pointer, byte_offset, bit_offset, \
                  bit_size)

#define primitive_c_signed_field_setter(new, pointer, byte_offset, \
                                           bit_offset, bit_size) \
  AT_FIELD_SETTER(signed long, new, pointer, byte_offset, bit_offset, \
                  bit_size)

#define primitive_c_int_field_setter(new, pointer, byte_offset, \
                                          bit_offset, bit_size) \
  AT_FIELD_SETTER(unsigned int, new, pointer, byte_offset, bit_offset, \
                  bit_size)

#define primitive_element(x, o, b) \
        AT(dylan_value,x,o,b)
#define primitive_element_setter(e, x, o, b) \
        AT_SETTER(dylan_value,e,x,o,b)

/*
#define primitive_boolean_at(x, o, b) \
        AT(DBOOL,x,o,b)
#define primitive_boolean_at_setter(e, x, o, b) \
        AT_SETTER(DBOOL,e,x,o,b)
#define primitive_byte_character_at(x, o, b) \
        AT(DBCHR,x,o,b)
#define primitive_byte_character_at_setter(e, x, o, b) \
        AT_SETTER(DBCHR,e,x,o,b)
#define primitive_small_integer_at(x, o, b) \
        AT(DSINT,x,o,b)
#define primitive_small_integer_at_setter(e, x, o, b) \
        AT_SETTER(DSINT,e,x,o,b)
#define primitive_machine_integer_at(x, o, b) \
        AT(DMINT,x,o,b)
#define primitive_machine_integer_at_setter(e, x, o, b) \
        AT_SETTER(DMINT,e,x,o,b)
#define primitive_unsigned_machine_integer_at(x, o, b) \
        AT(DUMINT,x,o,b)
#define primitive_unsigned_machine_integer_at_setter(e, x, o, b) \
        AT_SETTER(DUMINT,e,x,o,b)
#define primitive_long_machine_integer_at(x, o, b) \
        AT(DMINT,x,o,b)
#define primitive_long_machine_integer_at_setter(e, x, o, b) \
        AT_SETTER(DMINT,e,x,o,b)
#define primitive_unsigned_long_machine_integer_at(x, o, b) \
        AT(DUMINT,x,o,b)
#define primitive_unsigned_long_machine_integer_at_setter(e, x, o, b) \
        AT_SETTER(DUMINT,e,x,o,b)
#define primitive_single_float_at(x, o, b) \
        AT(float,x,o,b)
#define primitive_single_float_at_setter(e, x, o, b) \
        AT_SETTER(float,e,x,o,b)
#define primitive_double_float_at(x, o, b) \
        AT(double,x,o,b)
#define primitive_double_float_at_setter(e, x, o, b) \
        AT_SETTER(double,e,x,o,b)
#define primitive_extended_float_at(x, o, b) \
        AT(long_double,x,o,b)
#define primitive_extended_float_at_setter(e, x, o, b) \
        AT_SETTER(long_double,e,x,o,b)
#define primitive_pointer_at(x, o, b) \
        AT(dylan_value,x,o,b)
#define primitive_pointer_at_setter(e, x, o, b) \
        AT_SETTER(dylan_value,e,x,o,b)
#define primitive_address_at(x, o, b) \
        AT(DADDR,x,o,b)
#define primitive_address_at_setter(e, x, o, b) \
        AT_SETTER(DADDR,e,x,o,b)
*/

/* Allocate General Purpose Space
 *
 * MMAllocMisc and MMFreeMisc provide general-purpose memory for
 * internal use by the Dylan run-time system in a manner similar
 * to ANSI malloc and free.  The memory will not be relocated, and must
 * be freed explicitly.
 */

extern void *MMAllocMisc(size_t size);
extern void MMFreeMisc(void *p, size_t size);
extern void *dylan__malloc__misc(size_t size);
extern void *dylan__malloc__ambig(size_t size);
extern void *mps__malloc(size_t size);
extern void mps__free(size_t *old);

/* ALLOCATION PRIMITIVES */

extern dylan_value primitive_allocate(DSINT);
extern dylan_value primitive_untraced_allocate(DSINT);
extern dylan_value primitive_manual_allocate(dylan_value);
extern void primitive_manual_free(dylan_value);
extern dylan_value primitive_allocate_wrapper(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT);
extern dylan_value primitive_byte_allocate_filled_terminated(DSINT, DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT);
extern dylan_value primitive_byte_allocate_leaf_filled_terminated(DSINT, DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT);
extern dylan_value primitive_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT);
extern dylan_value primitive_object_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, dylan_value);
extern dylan_value primitive_byte_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DBYTE);
extern dylan_value primitive_double_byte_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DDBYTE);
extern dylan_value primitive_word_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DWORD);
extern dylan_value primitive_double_word_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DDWORD);
extern dylan_value primitive_single_float_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DSFLT);
extern dylan_value primitive_double_float_allocate_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DDFLT);
extern dylan_value primitive_byte_allocate_leaf_filled(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, DBYTE);
extern dylan_value primitive_allocate_in_awl_pool(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, dylan_value);
extern dylan_value primitive_allocate_weak_in_awl_pool(DSINT, dylan_value, DSINT, dylan_value, DSINT, DSINT, dylan_value);

/* lower level allocation API */
void *primitive_alloc(size_t size,
                      void *wrapper);
void *primitive_alloc_s1(size_t size,
                          void *wrapper,
                          void *data1);
void *primitive_alloc_s2(size_t size,
                          void *wrapper,
                          void *data1,
                          void *data2);
void *primitive_alloc_s(size_t size,
                        void *wrapper,
                        int no_to_fill,
                        void *fill);
void *primitive_alloc_r(size_t size,
                        void *wrapper,
                        size_t rep_size,
                        size_t rep_size_slot);
void *primitive_alloc_rf(size_t size,
                         void *wrapper,
                         size_t rep_size,
                         size_t rep_size_slot,
                         dylan_value fill);
void *primitive_alloc_s_r(size_t size,
                          void *wrapper,
                          int no_to_fill,
                          void *fill,
                          size_t rep_size,
                          size_t rep_size_slot);
void *primitive_alloc_s_rf(size_t size,
                           void *wrapper,
                           int no_to_fill,
                           void *fill,
                           size_t rep_size,
                           size_t rep_size_slot,
                           dylan_value rep_fill);
void *primitive_alloc_s_rhf(size_t size,
                            void *wrapper,
                            int no_to_fill,
                            void *fill,
                            size_t rep_size,
                            size_t rep_size_slot,
                            DDBYTE rep_fill);
void *primitive_alloc_s_rsff(size_t size,
                             void *wrapper,
                             int no_to_fill,
                             void *fill,
                             size_t rep_size,
                             size_t rep_size_slot,
                             DSFLT rep_fill);
void *primitive_alloc_s_rdff(size_t size,
                             void *wrapper,
                             int no_to_fill,
                             void *fill,
                             size_t rep_size,
                             size_t rep_size_slot,
                             DDFLT rep_fill);
void *primitive_alloc_s_rdwf(size_t size,
                             void *wrapper,
                             int no_to_fill,
                             void *fill,
                             size_t rep_size,
                             size_t rep_size_slot,
                             DDWORD rep_fill);
void *primitive_alloc_s_rbf(size_t size,
                            void *wrapper,
                            int no_to_fill,
                            void *fill,
                            size_t rep_size,
                            size_t rep_size_slot,
                            int byte_fill);
void *primitive_alloc_s_rbfz(size_t size,
                             void *wrapper,
                             int no_to_fill,
                             void *fill,
                             size_t rep_size,
                             size_t rep_size_slot,
                             int byte_fill);
void *primitive_alloc_rbfz(size_t size,
                           void *wrapper,
                           size_t rep_size,
                           size_t rep_size_slot,
                           int byte_fill);
void *primitive_alloc_s_rb(size_t size,
                           void *wrapper,
                           int no_to_fill,
                           void *fill,
                           size_t rep_size,
                           size_t rep_size_slot);
void *primitive_alloc_leaf(size_t size,
                           void *wrapper);
void *primitive_alloc_leaf_s_r(size_t size,
                               void *wrapper,
                               int no_to_fill,
                               void *fill,
                               size_t rep_size,
                               size_t rep_size_slot);
void *primitive_alloc_leaf_s1(size_t size,
                              void *wrapper,
                              void *data1);
void *primitive_alloc_leaf_s2(size_t size,
                              void *wrapper,
                              void *data1,
                              void *data2);
void *primitive_alloc_leaf_s(size_t size,
                             void *wrapper,
                             int no_to_fill,
                             void *fill);
void *primitive_alloc_leaf_r(size_t size,
                             void *wrapper,
                             size_t rep_size,
                             size_t rep_size_slot);
void *primitive_alloc_leaf_s_rbf(size_t size,
                                 void *wrapper,
                                 int no_to_fill,
                                 void *fill,
                                 size_t rep_size,
                                 size_t rep_size_slot,
                                 int byte_fill);
void *primitive_alloc_leaf_rf(size_t size,
                              void *wrapper,
                              size_t rep_size,
                              size_t rep_size_slot,
                              dylan_value rep_fill);
void *primitive_alloc_leaf_rbf(size_t size,
                               void *wrapper,
                               size_t rep_size,
                               size_t rep_size_slot,
                               DBYTE rep_fill);
void *primitive_alloc_leaf_rhf(size_t size,
                               void *wrapper,
                               size_t rep_size,
                               size_t rep_size_slot,
                               DDBYTE rep_fill);
void *primitive_alloc_leaf_rsff(size_t size,
                                void *wrapper,
                                size_t rep_size,
                                size_t rep_size_slot,
                                DSFLT rep_fill);
void *primitive_alloc_leaf_rdff(size_t size,
                                void *wrapper,
                                size_t rep_size,
                                size_t rep_size_slot,
                                DDFLT rep_fill);
void *primitive_alloc_leaf_rdwf(size_t size,
                                void *wrapper,
                                size_t rep_size,
                                size_t rep_size_slot,
                                DDWORD rep_fill);
void *primitive_alloc_leaf_s_rbfz(size_t size,
                                  void *wrapper,
                                  int no_to_fill,
                                  void *fill,
                                  size_t rep_size,
                                  size_t rep_size_slot,
                                  int byte_fill);
void *primitive_alloc_leaf_rbfz(size_t size,
                                void *wrapper,
                                size_t rep_size,
                                size_t rep_size_slot,
                                int byte_fill);
void *primitive_alloc_leaf_s_rb(size_t size,
                                void *wrapper,
                                int no_to_fill,
                                void *fill,
                                size_t rep_size,
                                size_t rep_size_slot);
void *primitive_alloc_exact_awl_s_r(size_t size,
                                    void *wrapper,
                                    void *assoc,
                                    int no_to_fill,
                                    void *fill,
                                    size_t rep_size,
                                    size_t rep_size_slot);
void *primitive_alloc_weak_awl_s_r(size_t size,
                                   void *wrapper,
                                   void *assoc,
                                   int no_to_fill,
                                   void *fill,
                                   size_t rep_size,
                                   size_t rep_size_slot);
void *primitive_alloc_exact_awl_rf(size_t size,
                                   void *wrapper,
                                   void *assoc,
                                   size_t rep_size,
                                   size_t rep_size_slot,
                                   void *fill);
void *primitive_alloc_weak_awl_rf(size_t size,
                                  void *wrapper,
                                  void *assoc,
                                  size_t rep_size,
                                  size_t rep_size_slot,
                                  void *fill);
void *primitive_alloc_wrapper_s_r(size_t size,
                                  void *wrapper,
                                  int no_to_fill,
                                  void *fill,
                                  size_t rep_size,
                                  size_t rep_size_slot);
void *primitive_alloc_rt(size_t size,
                         void *wrapper,
                         size_t rep_size,
                         size_t rep_size_slot,
                         void *template);

/* stack allocation */

#define primitive_byte_stack_allocate_filled(size, class_wrapper, number_slots, fill_value, repeated_size, repeated_size_offset, repeated_fill_value) \
  initialize_byte_stack_allocate_filled((dylan_value*)primitive_byte_stack_allocate(size, repeated_size + 1), class_wrapper, number_slots, fill_value, repeated_size, repeated_size_offset, repeated_fill_value)

extern dylan_value initialize_byte_stack_allocate_filled
    (dylan_value ptr, dylan_value class_wrapper, DSINT number_slots,
     dylan_value fill_value, DSINT repeated_size, DSINT repeated_size_offset,
     DBYTE repeated_fill_value);


#define primitive_object_stack_allocate_filled(size, class_wrapper, number_slots, fill_value, repeated_size, repeated_size_offset, repeated_fill_value) \
  initialize_object_stack_allocate_filled(primitive_byte_stack_allocate(size, repeated_size * sizeof(dylan_value)), class_wrapper, number_slots, fill_value, repeated_size, repeated_size_offset, repeated_fill_value)

extern dylan_value initialize_object_stack_allocate_filled
      (dylan_value ptr, dylan_value class_wrapper, DSINT number_slots, dylan_value fill_value,
       DSINT repeated_size, DSINT repeated_size_offset,
       dylan_value repeated_fill_value);


/* allocation counting stubs     (gts,98sep10) */

#define primitive_initialize_allocation_count()
#define primitive_allocation_count() 0

#define primitive_begin_heap_alloc_stats()
#define primitive_end_heap_alloc_stats(x) 0

/* CLASS BREAKPOINT PRIMITIVES */

#define primitive_display_class_breakpoints(x) 0
#define primitive_clear_class_breakpoint(x)
#define primitive_set_class_breakpoint(x,y)

/* GC PRIMITIVES */

#define primitive_pin_object(x) (x)
extern void primitive_unpin_object(dylan_value);
extern void primitive_mps_finalize(dylan_value);
extern dylan_value primitive_mps_finalization_queue_first(void);
extern void primitive_mps_park(void);
extern void primitive_mps_clamp(void);
extern void primitive_mps_release(void);
extern void primitive_mps_collect(DBOOL);
extern DBOOL primitive_mps_collection_stats(dylan_value);
extern void primitive_mps_enable_gc_messages(void);
extern DSINT primitive_mps_committed(void);
extern void primitive_mps_begin_ramp_alloc(void);
extern void primitive_mps_end_ramp_alloc(void);
extern void primitive_mps_begin_ramp_alloc_all(void);
extern void primitive_mps_end_ramp_alloc_all(void);
extern void primitive_mps_ld_reset(void *hs);
extern void primitive_mps_ld_add(void *hs, void *o);
extern void primitive_mps_ld_merge(void *hs1, void *hs2);
extern int primitive_mps_ld_isstale(void *hs);

/* KEYBOARD BREAK HANDLING */

#define primitive_keyboard_interrupt_signaled() ((DBOOL)0)
#define primitive_keyboard_interrupt_signaled_setter(x)
#define primitive_keyboard_interrupt_polling() ((DBOOL)0)
#define primitive_keyboard_interrupt_polling_setter(x)
#define primitive_keyboard_interrupt_polling_thread(x) ((DBOOL)0)
#define primitive_keyboard_interrupt_polling_thread_setter(y, x)

/* BYTE CHARACTER PRIMITIVES */

#define primitive_byte_character_as_raw(x) \
   (R(x))
#define primitive_raw_as_byte_character(x) \
   (C(x))

/* UNICODE CHARACTER PRIMITIVES */

#define primitive_unicode_character_as_raw(x) \
   (R(x))
#define primitive_raw_as_unicode_character(x) \
   (U(x))

/* INTEGER PRIMITIVES */

/* SMALL-INTEGER PRIMITIVES */

#define primitive_raw_as_integer(x) \
   (I(x))

/* BIG-INTEGER PRIMITIVES */

/* MACHINE-INTEGER PRIMITIVES */

/* UNSIGNED-MACHINE-INTEGER PRIMITIVES */

/* ADDRESS PRIMITIVES */

/* POINTER PRIMITIVES */

#define primitive_cast_pointer_as_raw(x)  ((DADDR)x)
#define primitive_cast_raw_as_pointer(x)  ((dylan_value)x)

/* MACHINE-WORD PRIMITIVES */

#define primitive_integerQ(x)                             RAWASBOOL(TAG_BITS(x) == 1)

#define primitive_machine_word_equalQ(x, y)               RAWASBOOL((DMINT)(x) == (DMINT)(y))
#define primitive_machine_word_not_equalQ(x, y)           RAWASBOOL((DMINT)(x) != (DMINT)(y))
#define primitive_machine_word_less_thanQ(x, y)           RAWASBOOL((DMINT)(x) < (DMINT)(y))
#define primitive_machine_word_not_less_thanQ(x, y)       RAWASBOOL((DMINT)(x) >= (DMINT)(y))
#define primitive_machine_word_greater_thanQ(x, y)        RAWASBOOL((DMINT)(x) > (DMINT)(y))
#define primitive_machine_word_not_greater_thanQ(x, y)    RAWASBOOL((DMINT)(x) <= (DMINT)(y))

extern dylan_value primitive_wrap_machine_word(DMINT);

#define primitive_unwrap_machine_word(x) \
  (((dylan_machine_word*)(x))->data)

extern dylan_value primitive_wrap_c_pointer(dylan_value, DMINT);
/*   primitive_unwrap_c_pointer
 * assumes that instances of <c-pointer> are implemented as
 * struct {dylan_value class; dylan_value pointer_address; ...}
 * ..and that the address is stored as a raw pointer.
 */
#define primitive_unwrap_c_pointer(c_ptr) \
  ((void*)(*(((dylan_value*)(c_ptr))+1)))

#define primitive_cast_integer_as_raw(x)                  ((DMINT)(x))
#define primitive_cast_raw_as_integer(x)                  ((dylan_value)(x))

extern dylan_value primitive_wrap_abstract_integer(DMINT);
extern dylan_value primitive_wrap_unsigned_abstract_integer(DMINT);
extern DMINT primitive_unwrap_abstract_integer(dylan_value);
#define primitive_box_integer(x)                          (I(x))
#define primitive_unbox_integer(x)                        (R(x))

#define primitive_machine_word_logand(x, y)               ((x) & (y))
#define primitive_machine_word_logior(x, y)               ((x) | (y))
#define primitive_machine_word_logxor(x, y)               ((x) ^ (y))

#define primitive_machine_word_lognot(x)                  (~(x))

#define primitive_machine_word_logbitQ(i, x)              RAWASBOOL((1UL << (i)) & ((DUMINT)(x)))
#define primitive_machine_word_logbit_set(i, x)           ((1UL << (i)) | ((DUMINT)(x)))
#define primitive_machine_word_logbit_clear(i, x)         (~(1UL << (i)) & ((DUMINT)(x)))

#define primitive_machine_word_bit_field_deposit(f, o, s, x)      (((x) & ~(((1 << (s)) - 1) << (o))) | ((f) << (o)))
#define primitive_machine_word_bit_field_extract(o, s, x)         (((x) & (((1 << (s)) - 1) << (o))) >> (o))

#ifdef OPEN_DYLAN_COMPILER_GCC_LIKE
static inline DMINT primitive_machine_word_count_low_zeros(DMINT x) {
  return x ? __builtin_ctzl(x) : (8 * sizeof(x));
}
static inline DMINT primitive_machine_word_count_high_zeros(DMINT x) {
  return x ? __builtin_clzl(x) : (8 * sizeof(x));
}
static inline DMINT primitive_machine_word_count_ones(DMINT x) {
  return __builtin_popcountl(x);
}
#else
extern DMINT primitive_machine_word_count_low_zeros(DMINT);
extern DMINT primitive_machine_word_count_high_zeros(DMINT);
extern DMINT primitive_machine_word_count_ones(DMINT);
#endif

#define primitive_machine_word_add(x, y)                  ((x) + (y))

extern DMINT primitive_machine_word_add_with_overflow(DMINT, DMINT);

#define primitive_machine_word_subtract(x, y)             ((x) - (y))

extern DMINT primitive_machine_word_subtract_with_overflow(DMINT, DMINT);

#define primitive_machine_word_multiply_low(x, y)         ((x) * (y))

extern DMINT primitive_machine_word_multiply_high(DMINT, DMINT);
extern DMINT primitive_machine_word_multiply_lowShigh(DMINT, DMINT);
extern DMINT primitive_machine_word_multiply_low_with_overflow(DMINT, DMINT);
extern DMINT primitive_machine_word_multiply_with_overflow(DMINT, DMINT);

#define primitive_machine_word_negative(x)                (-(DMINT)(x))
#define primitive_machine_word_abs(x)                     ((x)<0?-(x):(x))

extern DMINT primitive_machine_word_negative_with_overflow(DMINT);
extern DMINT primitive_machine_word_abs_with_overflow(DMINT);

extern DMINT primitive_machine_word_floorS_quotient(DMINT, DMINT);
extern DMINT primitive_machine_word_floorS_remainder(DMINT, DMINT);
extern DMINT primitive_machine_word_floorS(DMINT, DMINT);
extern DMINT primitive_machine_word_ceilingS_quotient(DMINT, DMINT);
extern DMINT primitive_machine_word_ceilingS_remainder(DMINT, DMINT);
extern DMINT primitive_machine_word_ceilingS(DMINT, DMINT);
extern DMINT primitive_machine_word_roundS_quotient(DMINT, DMINT);
extern DMINT primitive_machine_word_roundS_remainder(DMINT, DMINT);
extern DMINT primitive_machine_word_roundS(DMINT, DMINT);
extern DMINT primitive_machine_word_truncateS_quotient(DMINT, DMINT);
extern DMINT primitive_machine_word_truncateS_remainder(DMINT, DMINT);
extern DMINT primitive_machine_word_truncateS(DMINT, DMINT);

#define primitive_machine_word_divide_quotient(x, y)      ((x) / (y))
#define primitive_machine_word_divide_remainder(x, y)     ((x) % (y))

extern DMINT primitive_machine_word_divide(DMINT, DMINT);

#define primitive_machine_word_shift_left_low(x, i)       ((DMINT)(x) << (i))

extern DMINT primitive_machine_word_shift_left_high(DMINT, DMINT);
extern DMINT primitive_machine_word_shift_left_lowShigh(DMINT, DMINT);
extern DMINT primitive_machine_word_shift_left_low_with_overflow(DMINT, DMINT);
extern DMINT primitive_machine_word_shift_left_with_overflow(DMINT, DMINT);

#define primitive_machine_word_shift_right(x, i)          ((DMINT)(x) >> (i))

/* The C run-time does no overflow checks ... */

#define primitive_machine_word_add_signal_overflow(x, y) \
  primitive_machine_word_add(x, y)
#define primitive_machine_word_subtract_signal_overflow(x, y) \
  primitive_machine_word_subtract(x, y)
#define primitive_machine_word_multiply_signal_overflow(x, y) \
  primitive_machine_word_multiply_low(x, y)
#define primitive_machine_word_negative_signal_overflow(x) \
  primitive_machine_word_negative(x)
#define primitive_machine_word_abs_signal_overflow(x) \
  primitive_machine_word_abs(x)
#define primitive_machine_word_shift_left_signal_overflow(x, y) \
  primitive_machine_word_shift_left_low(x, y)

extern DMINT primitive_machine_word_double_floorS_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_floorS_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_floorS(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_ceilingS_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_ceilingS_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_ceilingS(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_roundS_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_roundS_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_roundS(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_truncateS_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_truncateS_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_truncateS(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_divide_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_divide_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_double_divide(DMINT, DMINT, DMINT);

#define primitive_machine_word_unsigned_less_thanQ(x, y) \
  RAWASBOOL(((DUMINT)(x)) < ((DUMINT)(y)))

#define primitive_machine_word_unsigned_not_less_thanQ(x, y) \
  RAWASBOOL(((DUMINT)(x)) >= ((DUMINT)(y)))

#define primitive_machine_word_unsigned_greater_thanQ(x, y) \
  RAWASBOOL(((DUMINT)(x)) > ((DUMINT)(y)))

#define primitive_machine_word_unsigned_not_greater_thanQ(x, y) \
  RAWASBOOL(((DUMINT)(x)) <= ((DUMINT)(y)))

extern DMINT primitive_machine_word_unsigned_add_with_carry(DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_subtract_with_borrow(DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_multiply_high(DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_multiply(DMINT, DMINT);

#define primitive_machine_word_unsigned_divide_quotient(x, y) \
  ((DMINT)(((DUMINT)(x)) / ((DUMINT)(y))))

#define primitive_machine_word_unsigned_divide_remainder(x, y) \
  ((DMINT)(((DUMINT)(x)) % ((DUMINT)(y))))

extern DMINT primitive_machine_word_unsigned_divide(DMINT, DMINT);

extern DMINT primitive_machine_word_unsigned_rotate_left(DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_rotate_right(DMINT, DMINT);

#define primitive_machine_word_unsigned_shift_right(x, i) \
  ((DMINT)(((DUMINT)(x)) >> (i)))

extern DMINT primitive_machine_word_unsigned_double_divide_quotient(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_divide_remainder(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_divide(DMINT, DMINT, DMINT);

extern DMINT primitive_machine_word_unsigned_shift_left_high(DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_shift_left_high(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_shift_left(DMINT, DMINT, DMINT);

extern DMINT primitive_machine_word_unsigned_double_shift_right_low(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_shift_right_high(DMINT, DMINT, DMINT);
extern DMINT primitive_machine_word_unsigned_double_shift_right(DMINT, DMINT, DMINT);

/* FLOATS */
#include <math.h>

/* Win32 only defines the single precision functions for C++ (Huh?) */
#ifdef OPEN_DYLAN_PLATFORM_WINDOWS
#define sqrtf(x)  (DSFLT)sqrt((DDFLT)x)
#define logf(x)   (DSFLT)log((DDFLT)x)
#define expf(x)   (DSFLT)exp((DDFLT)x)
#define powf(b,p) (DSFLT)pow((DDFLT)b,(DDFLT)p)
#define sinf(x)   (DSFLT)sin((DDFLT)x)
#define cosf(x)   (DSFLT)cos((DDFLT)x)
#define tanf(x)   (DSFLT)tan((DDFLT)x)
#define asinf(x)  (DSFLT)asin((DDFLT)x)
#define acosf(x)  (DSFLT)acos((DDFLT)x)
#define atanf(x)  (DSFLT)atan((DDFLT)x)
#endif

#define primitive_double_float_as_single(x) ((DSFLT)x)
#define primitive_single_float_as_double(x) ((DDFLT)x)

/* SINGLE-FLOAT PRIMITIVES */

#define primitive_single_float_as_raw(x) \
    (((dylan_single_float*)x)->data)
extern dylan_value primitive_raw_as_single_float(DSFLT x);
#define primitive_single_float_as_integer(x) \
    ((DSINT)(x))
#define primitive_integer_as_single_float(x) \
    ((DSFLT)(x))
extern DMINT primitive_single_float_as_double_integer(DSFLT);
extern DSFLT primitive_double_integer_as_single_float(DMINT, DMINT);
extern DUMINT primitive_cast_single_float_as_machine_word(DSFLT);
extern DSFLT primitive_cast_machine_word_as_single_float(DUMINT);
#define primitive_single_float_negate(x) \
    (-(x))
#define primitive_single_float_add(x, y) \
    ((x) + (y))
#define primitive_single_float_subtract(x, y) \
    ((x) - (y))
#define primitive_single_float_multiply(x, y) \
    ((x) * (y))
#define primitive_single_float_divide(x, y) \
    ((x) / (y))
#define primitive_single_float_equalsQ(x, y) \
    RAWASBOOL((x) == (y))
#define primitive_single_float_less_thanQ(x, y) \
    RAWASBOOL((x) < (y))
#define primitive_single_float_sqrt(x) \
    (sqrtf(x))
#define primitive_single_float_log(x) \
    (logf(x))
#define primitive_single_float_exp(x) \
    (expf(x))
#define primitive_single_float_expt(base, power) \
    (powf(base, power))
#define primitive_single_float_sin(x) \
    (sinf(x))
#define primitive_single_float_cos(x) \
    (cosf(x))
#define primitive_single_float_tan(x) \
    (tanf(x))
#define primitive_single_float_asin(x) \
    (asinf(x))
#define primitive_single_float_acos(x) \
    (acosf(x))
#define primitive_single_float_atan(x) \
    (atanf(x))

/* DOUBLE-FLOAT PRIMITIVES */

#define primitive_double_float_as_raw(x) \
    (((dylan_double_float*)x)->data)
extern dylan_value primitive_raw_as_double_float(DDFLT x);
#define primitive_double_float_as_integer(x) \
    ((DSINT)(x))
#define primitive_integer_as_double_float(x) \
    ((DDFLT)(x))
extern DMINT primitive_double_float_as_double_integer(DDFLT);
extern DDFLT primitive_double_integer_as_double_float(DMINT, DMINT);
extern DUMINT primitive_cast_double_float_as_machine_words(DDFLT);
extern DDFLT primitive_cast_machine_words_as_double_float(DUMINT, DUMINT);
#define primitive_double_float_negate(x) \
    (-(x))
#define primitive_double_float_add(x, y) \
    ((x) + (y))
#define primitive_double_float_subtract(x, y) \
    ((x) - (y))
#define primitive_double_float_multiply(x, y) \
    ((x) * (y))
#define primitive_double_float_divide(x, y) \
    ((x) / (y))
#define primitive_double_float_equalsQ(x, y) \
    RAWASBOOL((x) == (y))
#define primitive_double_float_less_thanQ(x, y) \
    RAWASBOOL((x) < (y))
#define primitive_double_float_sqrt(x) \
    (sqrt(x))
#define primitive_double_float_log(x) \
    (log(x))
#define primitive_double_float_exp(x) \
    (exp(x))
#define primitive_double_float_expt(base, power) \
    (pow(base, power))
#define primitive_double_float_sin(x) \
    (sin(x))
#define primitive_double_float_cos(x) \
    (cos(x))
#define primitive_double_float_tan(x) \
    (tan(x))
#define primitive_double_float_asin(x) \
    (asin(x))
#define primitive_double_float_acos(x) \
    (acos(x))
#define primitive_double_float_atan(x) \
    (atan(x))

/* VECTOR PRIMITIVES */

#define primitive_vector_element(v, i)           ((((dylan_simple_object_vector*)v)->data)[R(i)])
#define primitive_vector_element_setter(e, v, i) ((((dylan_simple_object_vector*)v)->data)[R(i)] = (e))
#define primitive_vector_size(v)                 (((dylan_simple_object_vector*)v)->size)
#define primitive_vector_as_raw(v)               (((dylan_simple_object_vector*)v)->data)

extern dylan_value primitive_vector(dylan_value dn, ...);
extern dylan_value primitive_raw_as_vector(dylan_value a, dylan_value n);
extern dylan_value primitive_make_vector(int size);
extern dylan_value primitive_copy_vector(dylan_value vector);
extern dylan_value VECTOR_REF_OR_F(dylan_value v, int offset);

/* STRING PRIMITIVES */

#define primitive_strlen(s)                     (strlen((DBSTR)s))
#define primitive_string_as_raw(v)              (((dylan_byte_string*)v)->data)
extern dylan_value primitive_raw_as_string(DBSTR buffer);

/* CALLING CONVENTION PRIMITIVES */

#define primitive_function_parameter()                  ((dylan_value)(get_teb()->function))
#define primitive_next_methods_parameter()              ((dylan_value)(get_teb()->next_methods))
#define primitive_callable_as_engine_nodeQ(m)           ((dylan_value)DTRUE)

/* APPLY PRIMITIVES */

extern dylan_value primitive_apply (dylan_value fn, dylan_value sov);
extern dylan_value primitive_apply_spread (dylan_value fn, int n, ...);
extern dylan_value primitive_mep_apply_spread (dylan_value fn, dylan_value nm, int n, ...);
extern dylan_value primitive_xep_apply (dylan_simple_method* fn, int n, dylan_value a[]);
extern dylan_value primitive_mep_apply_with_optionals
  (dylan_simple_method* fn, dylan_value new_next_methods, dylan_value a);
extern dylan_value primitive_mep_apply (dylan_simple_method* fn, dylan_value next_methods, dylan_value a[]);
extern dylan_value primitive_iep_apply (dylan_simple_method* fn, int n, dylan_value a[]);
extern dylan_value primitive_engine_node_apply_with_optionals (dylan_value engD, dylan_value gfD, dylan_value args);

#define APPLYN(fn,n) (dylan_value)(primitive_apply_spread((fn),n
#define APPLY0(fn) APPLYN(fn,0)))
#define APPLY1(fn,a1) APPLYN(fn,1),(a1)))
#define APPLY2(fn,a1,a2) APPLYN(fn,2),(a1),(a2)))
#define APPLY3(fn,a1,a2,a3) APPLYN(fn,3),(a1),(a2),(a3)))
#define APPLY4(fn,a1,a2,a3,a4) APPLYN(fn,4),(a1),(a2),(a3),(a4)))
#define APPLY5(fn,a1,a2,a3,a4,a5) APPLYN(fn,5),(a1),(a2),(a3),(a4),(a5)))
#define APPLY6(fn,a1,a2,a3,a4,a5,a6) APPLYN(fn,6),(a1),(a2),(a3),(a4),(a5),(a6)))
#define APPLY7(fn,a1,a2,a3,a4,a5,a6,a7) APPLYN(fn,7),(a1),(a2),(a3),(a4),(a5),(a6),(a7)))
#define APPLY8(fn,a1,a2,a3,a4,a5,a6,a7,a8) APPLYN(fn,8),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)))
#define APPLY9(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9) APPLYN(fn,9),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)))
#define APPLY10(fn,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) APPLYN(fn,10),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10)))

#define MEP_APPLYN(fn,nm,n) (dylan_value)(primitive_mep_apply_spread((fn),(nm),n
#define MEP_APPLY0(fn,nm) MEP_APPLYN(fn,nm,0)))
#define MEP_APPLY1(fn,nm,a1) MEP_APPLYN(fn,nm,1),(a1)))
#define MEP_APPLY2(fn,nm,a1,a2) MEP_APPLYN(fn,nm,2),(a1),(a2)))
#define MEP_APPLY3(fn,nm,a1,a2,a3) MEP_APPLYN(fn,nm,3),(a1),(a2),(a3)))
#define MEP_APPLY4(fn,nm,a1,a2,a3,a4) MEP_APPLYN(fn,nm,4),(a1),(a2),(a3),(a4)))
#define MEP_APPLY5(fn,nm,a1,a2,a3,a4,a5) MEP_APPLYN(fn,nm,5),(a1),(a2),(a3),(a4),(a5)))
#define MEP_APPLY6(fn,nm,a1,a2,a3,a4,a5,a6) MEP_APPLYN(fn,nm,6),(a1),(a2),(a3),(a4),(a5),(a6)))
#define MEP_APPLY7(fn,nm,a1,a2,a3,a4,a5,a6,a7) MEP_APPLYN(fn,nm,7),(a1),(a2),(a3),(a4),(a5),(a6),(a7)))
#define MEP_APPLY8(fn,nm,a1,a2,a3,a4,a5,a6,a7,a8) MEP_APPLYN(fn,nm,8),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8)))
#define MEP_APPLY9(fn,nm,a1,a2,a3,a4,a5,a6,a7,a8,a9) MEP_APPLYN(fn,nm,9),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9)))
#define MEP_APPLY10(fn,nm,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10) MEP_APPLYN(fn,nm,10),(a1),(a2),(a3),(a4),(a5),(a6),(a7),(a8),(a9),(a10)))


#define ENGINE_NODE_APPLYN(e,gf,n) (dylan_value)(primitive_engine_node_apply_spread((e),(gf),n


/* VALUES PRIMITIVES */

extern dylan_value primitive_values (dylan_value v);

/* OPERATING SYSTEM PRIMITIVES */

extern dylan_value Tcommand_nameT;
extern dylan_value pseudo_primitive_command_name (void);

extern dylan_value Tcommand_argumentsT;
extern dylan_value pseudo_primitive_command_arguments (void);

extern void  primitive_exit_application (DSINT code) NORETURN_FUNCTION;

/* TEMPORARY PRIMITIVES FOR ASSIGNMENT */

extern dylan_value* MAKE_DYLAN_VALUE_CELL(dylan_value);
#define GET_DYLAN_VALUE_CELL_VAL(c)       (*(dylan_value*)c)
#define SET_DYLAN_VALUE_CELL_VAL(c, v)    (*(dylan_value*)c = v)

extern dylan_value* MAKE_DBCHR_CELL(DBCHR);
#define GET_DBCHR_CELL_VAL(c)           (*(DBCHR*)c)
#define SET_DBCHR_CELL_VAL(c, v)        (*(DBCHR*)c = v)

extern dylan_value* MAKE_DDBYTE_CELL(DDBYTE);
#define GET_DDBYTE_CELL_VAL(c)          (*(DDBYTE*)c)
#define SET_DDBYTE_CELL_VAL(c, v)       (*(DDBYTE*)c = v)

extern dylan_value* MAKE_DWORD_CELL(DWORD);
#define GET_DWORD_CELL_VAL(c)           (*(DWORD*)c)
#define SET_DWORD_CELL_VAL(c, v)        (*(DWORD*)c = v)

extern dylan_value* MAKE_DDWORD_CELL(DDWORD);
#define GET_DDWORD_CELL_VAL(c)          (*(DDWORD*)c)
#define SET_DDWORD_CELL_VAL(c, v)       (*(DDWORD*)c = v)

extern dylan_value* MAKE_DSFLT_CELL(DSFLT);
#define GET_DSFLT_CELL_VAL(c)           (*(DSFLT*)c)
#define SET_DSFLT_CELL_VAL(c, v)        (*(DSFLT*)c = v)

extern dylan_value* MAKE_DDFLT_CELL(DDFLT);
#define GET_DDFLT_CELL_VAL(c)           (*(DDFLT*)c)
#define SET_DDFLT_CELL_VAL(c, v)        (*(DDFLT*)c = v)

/* THREAD SUPPORT */

extern void initialize_threads_primitives(void);

extern dylan_value primitive_release_simple_lock(dylan_value l);
extern dylan_value primitive_release_semaphore(dylan_value s);
extern dylan_value primitive_owned_recursive_lock(dylan_value l);
extern dylan_value primitive_destroy_simple_lock(dylan_value l);
extern dylan_value primitive_wait_for_semaphore_timed(dylan_value s, dylan_value ms);
extern dylan_value primitive_wait_for_semaphore(dylan_value s);
extern dylan_value primitive_wait_for_simple_lock_timed(dylan_value l, dylan_value ms);
extern dylan_value primitive_wait_for_simple_lock(dylan_value l);
extern dylan_value primitive_make_recursive_lock(dylan_value l, dylan_value n);
extern dylan_value primitive_release_recursive_lock(dylan_value l);
extern dylan_value primitive_make_semaphore(dylan_value l, dylan_value n, dylan_value i, dylan_value m);
extern dylan_value primitive_destroy_recursive_lock(dylan_value l);
extern dylan_value primitive_owned_simple_lock(dylan_value l);
extern dylan_value primitive_destroy_semaphore(dylan_value l);
extern dylan_value primitive_wait_for_recursive_lock_timed(dylan_value l, dylan_value ms);
extern dylan_value primitive_wait_for_recursive_lock(dylan_value l);
extern dylan_value primitive_thread_join_multiple(dylan_value v);
extern dylan_value primitive_thread_join_single(dylan_value t);
extern dylan_value primitive_initialize_current_thread(dylan_value t, DBOOL s);
extern dylan_value primitive_initialize_special_thread(dylan_value t);
extern dylan_value primitive_current_thread(void);
extern dylan_value primitive_make_thread(dylan_value t, dylan_value f, DBOOL s);
extern dylan_value primitive_destroy_thread(dylan_value t);
extern dylan_value primitive_destroy_notification(dylan_value n);
extern dylan_value primitive_release_all_notification(dylan_value n, dylan_value l);
extern dylan_value primitive_make_notification(dylan_value l, dylan_value n);
extern dylan_value primitive_wait_for_notification_timed(dylan_value n, dylan_value l, dylan_value ms);
extern dylan_value primitive_wait_for_notification(dylan_value n, dylan_value l);
extern dylan_value primitive_release_notification(dylan_value n, dylan_value l);
extern void primitive_detach_thread(dylan_value t);
extern void primitive_thread_yield(void);
extern void primitive_sleep(dylan_value ms);
extern dylan_value primitive_make_simple_lock(dylan_value l, dylan_value n);
extern dylan_value primitive_allocate_thread_variable(dylan_value i);
extern dylan_value primitive_read_thread_variable(dylan_value h);
extern dylan_value primitive_write_thread_variable(dylan_value h, dylan_value nv);
extern dylan_value primitive_unlock_simple_lock(dylan_value l);
extern dylan_value primitive_unlock_recursive_lock(dylan_value l);

/* ATOMIC PRIMITIVES */

#define primitive_sequence_point() SEQUENCE_POINT()
#define primitive_synchronize_side_effects() SYNCHRONIZE_SIDE_EFFECTS()

/* RUN-TIME CALLBACKS */

extern dylan_value primitive_resolve_symbol (dylan_value symbol);
extern dylan_value primitive_string_as_symbol (dylan_value string);
extern dylan_value primitive_preboot_symbols ();

/* ENGINE NODE HANDLER ASSIGNMENTS */

#define ENGINE_absent 0
#define ENGINE_inapplicable 1
#define ENGINE_unkeyed_single_method 2
#define ENGINE_implicit_keyed_single_method 3
#define ENGINE_explicit_keyed_single_method 4
#define ENGINE_unrestricted_keyed_single_method 5
#define ENGINE_reserved_terminal_n_a 6
#define ENGINE_reserved_terminal_n_b 7
#define ENGINE_reserved_terminal_n_c 8
#define ENGINE_reserved_terminal_n_d 9
#define ENGINE_reserved_terminal_n_e 10
#define ENGINE_reserved_terminal_n_f 11
#define ENGINE_reserved_terminal_n_g 12
#define ENGINE_profiling_cache_header 13
#define ENGINE_cache_header 14
#define ENGINE_ambiguous_methods 15
#define ENGINE_boxed_instance_slot_getter 16
#define ENGINE_boxed_instance_slot_setter 17
#define ENGINE_boxed_repeated_instance_slot_getter 18
#define ENGINE_boxed_repeated_instance_slot_setter 19
#define ENGINE_boxed_class_slot_getter 20
#define ENGINE_boxed_class_slot_setter 21
#define ENGINE_raw_byte_repeated_instance_slot_getter 22
#define ENGINE_raw_byte_repeated_instance_slot_setter 23
#define ENGINE_reserved_slot_a_getter 24
#define ENGINE_reserved_slot_a_setter 25
#define ENGINE_reserved_slot_b_getter 26
#define ENGINE_reserved_slot_b_setter 27
#define ENGINE_reserved_repeated_slot_a_getter 28
#define ENGINE_reserved_repeated_slot_a_setter 29
#define ENGINE_reserved_repeated_slot_b_getter 30
#define ENGINE_reserved_repeated_slot_b_setter 31
#define ENGINE_typecheck 32
#define ENGINE_if_type 33
#define ENGINE_linear_by_class 34
#define ENGINE_hashed_by_class 35
#define ENGINE_linear_by_singleton_class 36
#define ENGINE_hashed_by_singleton_class 37
#define ENGINE_immediate_linear_singleton 38
#define ENGINE_immediate_hashed_noreloc_singleton 39
#define ENGINE_immediate_hashed_singleton 40
#define ENGINE_value_object_linear_singleton 41
#define ENGINE_value_object_hashed_singleton 42
#define ENGINE_monomorphic 42
#define ENGINE_reserved_discriminator_a 43
#define ENGINE_reserved_discriminator_b 44
#define ENGINE_reserved_discriminator_c 45
#define ENGINE_reserved_discriminator_d 46
#define ENGINE_reserved_discriminator_e 47
#define ENGINE_reserved_discriminator_f 48
#define ENGINE_reserved_discriminator_g 49
#define ENGINE_reserved_discriminator_h 50
#define ENGINE_reserved_discriminator_i 51
#define ENGINE_reserved_discriminator_j 52
#define ENGINE_reserved_discriminator_k 53
#define ENGINE_reserved_discriminator_l 54
#define ENGINE_reserved_discriminator_m 55
#define ENGINE_reserved_discriminator_n 56
#define ENGINE_reserved_discriminator_o 57
#define ENGINE_reserved_discriminator_p 58
#define ENGINE_reserved_discriminator_q 59
#define ENGINE_reserved_discriminator_r 60
#define ENGINE_reserved_discriminator_s 61
#define ENGINE_reserved_discriminator_t 62
#define ENGINE_reserved_discriminator_u 63

#define ENGINE_first_discriminator 32
#define ENGINE_first_slot 16
#define ENGINE_number_slot_handlers 16

/* ENGINE NODE PROPERTIES BIT DEFINITIONS

There used to be two other bits having to do with permanency and precomputation or
something (I've forgotten!) which want to be in all engine-nodes at some point in
the future.  Adding them will be more convenient when the properties word is made
raw, and we can use the tag bits.


Engine Node
Entry-type is contained in low byte, shifted 2:  mask or shift out low 2 bits.

_31_________________________________________________________8_7____________2_1___________0_
|                             other                          |  entry type  |  fixnum tag  |
-------------------------------------------------------------------------------------------


Discriminator
Argument number to discriminate on is contained in second byte.
Third byte is the number of required arguments, and the following bit indicates whether
there are any optionals.  The sum of that byte and the bit give the number of MEP-style
arguments, which may be of use to primitive-initialize-discriminator.


_31_____25____24_____23___________16_15____________________8_7____________2_1___________0_
|  other  |  restp  |   nrequired   |  discriminator argnum  |  entry type  |  fixnum tag  |
-------------------------------------------------------------------------------------------


 */

#define EPROPS_V_ENTRY_TYPE 2
#define EPROPS_S_ENTRY_TYPE 6
#define EPROPS_M_ENTRY_TYPE 0xfc
#define ENODE_V_DATA_START 16
#define DPROPS_V_ARGNUM 8
#define DPROPS_S_ARGNUM 8
#define DPROPS_M_ARGNUM 0xFF00
#define DPROPS_V_NREQUIRED 16
#define DPROPS_S_NREQUIRED 8
#define DPROPS_M_NREQUIRED 0xFF0000
#define DPROPS_V_OPTIONALS 24
#define DPROPS_M_OPTIONALS 0x1000000

/* <simple-typechecked-cache-header-engine-node>.  See the Dylan definitions in
   dispatch-prologue.dylan for how these are defined.
   */
#define STCHEN_ARGUMENTS_LIMIT 8
#define STCHEN_V_CHECKEDMASK 16
#define STCHEN_S_CHECKEDMASK 8
#define STCHEN_M_CHECKEDMASK 0xff0000


#define SLOTENGINE_V_INDEX ENODE_V_DATA_START

extern dylan_value general_engine_node_1_engine (dylan_value a1);
extern dylan_value general_engine_node_2_engine (dylan_value a1, dylan_value a2);

extern dylan_value general_engine_node_3_engine (dylan_value a1, dylan_value a2, dylan_value a3);
extern dylan_value general_engine_node_n_engine (dylan_value a1, ...);
extern dylan_value general_engine_node_spread_engine (dylan_value a1, ...);

extern dylan_value cache_header_engine_0 ();
extern dylan_value cache_header_engine_1 (dylan_value a1);
extern dylan_value cache_header_engine_2 (dylan_value a1, dylan_value a2);
extern dylan_value cache_header_engine_3 (dylan_value a1, dylan_value a2, dylan_value a3);
extern dylan_value cache_header_engine_4 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4);
extern dylan_value cache_header_engine_5 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5);
extern dylan_value cache_header_engine_6 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6);
extern dylan_value cache_header_engine_7 (dylan_value a1, dylan_value a2, dylan_value a3, dylan_value a4, dylan_value a5, dylan_value a6, dylan_value a7);
extern dylan_value cache_header_engine_n (dylan_value argvec);

extern dylan_value primitive_initialize_discriminator(dylan_value discriminator);
extern dylan_value primitive_initialize_engine_node (dylan_value engine);


/* additions to run-time.c specific to handling pass-by-reference of non-first
   return values of primitives  (gts,9/97) */

extern DMINT primitive_single_float_as_double_integer_byref(DSFLT f, DMINT* v2);
extern DMINT primitive_double_float_as_double_integer_byref(DDFLT f, DMINT* v2);
extern DMINT primitive_cast_double_float_as_machine_words_byref(DDFLT x, DMINT* v2);
extern DMINT primitive_machine_word_divide_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_floorS_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_ceilingS_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_roundS_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_truncateS_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_double_floorS_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_double_ceilingS_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_double_roundS_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_double_truncateS_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_double_divide_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_add_with_overflow_byref(DMINT x, DMINT y, dylan_value* v2);
extern DMINT primitive_machine_word_subtract_with_overflow_byref(DMINT x, DMINT y, dylan_value* v2);
extern DMINT primitive_machine_word_multiply_with_overflow_byref(DMINT x, DMINT y, DMINT* v2, dylan_value* v3);
extern DMINT primitive_machine_word_negative_with_overflow_byref(DMINT x, dylan_value* v2);
extern DMINT primitive_machine_word_abs_with_overflow_byref(DMINT x, dylan_value* v2);
extern DMINT primitive_machine_word_shift_left_with_overflow_byref(DMINT x, DMINT y, DMINT* v2, dylan_value* v3);
extern DMINT primitive_machine_word_multiply_lowShigh_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_multiply_low_with_overflow_byref(DMINT x, DMINT y, dylan_value* v2);
extern DMINT primitive_machine_word_unsigned_add_with_carry_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_subtract_with_borrow_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_multiply_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_divide_byref(DMINT x, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_double_divide_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_double_shift_left_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);
extern DMINT primitive_machine_word_unsigned_double_shift_right_byref(DMINT xl, DMINT xh, DMINT y, DMINT* v2);

#endif /* OPENDYLAN_CRT_RUNTIME_H */

/* eof */
