/*#include "altrepisode.h"
#include <type_traits>

template<typename T>
struct rep {
  unsigned int size;
  T value;
  rep(int n, T v) : size(n), value(v) {}
};

static R_altrep_class_t alt_rep_integer;
static R_altrep_class_t alt_rep_real;
static R_altrep_class_t alt_rep_logical;

template<typename T>
struct alt_rep
{
  // constructor function
  static SEXP Make(rep<T>* data, bool owner)
  {
    Rprintf("Making a rep with %d x %d\n", data->value, data->size);

    R_altrep_class_t class_t;

    SEXP xp = PROTECT(R_MakeExternalPtr(data, R_NilValue, R_NilValue));
    if (owner) {
      R_RegisterCFinalizerEx(xp, alt_rep<T>::Finalize, TRUE);
    }

    if (std::is_same<T, int>::value)
      class_t = alt_rep_integer;

    if (std::is_same<T, double>::value)
      class_t = alt_rep_real;

    if (std::is_same<T, bool>::value)
      class_t = alt_rep_logical;

    SEXP res = R_new_altrep(class_t, xp, R_NilValue);
    UNPROTECT(1);
    return res;
  }

  // finalizer for the external pointer
  static void Finalize(SEXP xp){
    Rprintf("Deleting a rep at %p\n", R_ExternalPtrAddr(xp));
    delete static_cast<rep<T>*>(R_ExternalPtrAddr(xp));
  }

  // get the std::vector<string>* from the altrep object `x`
  static rep<T>* Ptr(SEXP x) {
    return static_cast<rep<T>*>(R_ExternalPtrAddr(R_altrep_data1(x)));
  }

  // same, but as a reference, for convenience
  static rep<T>& Get(SEXP vec) {
    return *Ptr(vec) ;
  }

  // ALTREP methods -------------------
  // The length of the object
  static R_xlen_t Length(SEXP vec){
    return Get(vec).size;
  }

  static SEXP int_Max(SEXP vec, Rboolean writable){
    return Rf_ScalarInteger(Get(vec).value);
  }

  static SEXP int_Min(SEXP vec, Rboolean writable){
    return Rf_ScalarInteger(Get(vec).value);
  }

  static SEXP real_Max(SEXP vec, Rboolean writable){
    return Rf_ScalarReal(Get(vec).value);
  }

  static SEXP real_Min(SEXP vec, Rboolean writable){
    return Rf_ScalarReal(Get(vec).value);
  }

  // What gets printed when .Internal(inspect()) is used
  static Rboolean Inspect(SEXP x, int pre, int deep, int pvec, void (*inspect_subtree)(SEXP, int, int, int)){
    if (std::is_same<T, int>::value)
      Rprintf("compact representaion of %d x %d\n", Get(x).value, Length(x));
    else if (std::is_same<T, double>::value)
      Rprintf("compact representaion of %lf x %d\n", Get(x).value, Length(x));
    else if (std::is_same<T, bool>::value)
      Rprintf("compact representaion of %s x %d\n", Get(x).value ? "TRUE" : "FALSE", Length(x));

    return TRUE;
  }

  // ALTVEC methods ------------------

  // The start of the data, i.e. the underlying string array from the random_string_data
  //
  // This is guaranteed to never allocate (in the R sense)
  static const void* Dataptr_or_null(SEXP vec) {
    Rprintf("Calling Dataptr_or_null on a alt_rep_int at %p\n", R_ExternalPtrAddr(vec));
    SEXP data2 = R_altrep_data2(vec);
    if (data2 == R_NilValue) return nullptr;
    return STDVEC_DATAPTR(data2);
  }

  // same in this case, writeable is ignored
  static void* Dataptr(SEXP vec, Rboolean writeable)
  {
    SEXP data2 = R_altrep_data2(vec);
    if (data2 != R_NilValue)
    {
      Rprintf("Returning pointer to materialized alt_rep_int at %p\n", R_ExternalPtrAddr(vec));
      return data2;
    }

    Rprintf("Materializing an alt_rep_int at %p\n", R_ExternalPtrAddr(vec));
    int n = Length(vec);
    int v = Get(vec).value;
    SEXP val = PROTECT(Rf_allocVector(INTSXP, n));
    int *data = INTEGER(val);
    for (R_xlen_t i = 0; i < n; i++) { data[i] = v; }
    R_set_altrep_data2(vec, val);
    UNPROTECT(1);
    return STDVEC_DATAPTR(val);
  }

  // ALTINT methods -----------------
  // the element at the index `i`
  // this does not do bounds checking because that's expensive, so
  // the caller must take care of that
  static int int_Elt(SEXP vec, R_xlen_t i) {
    Rprintf("int_Elt method called at %d \n", i+1);
    if (i > Length(vec)) { return NA_INTEGER; }
    return Get(vec).value;
  }

  static double real_Elt(SEXP vec, R_xlen_t i) {
    Rprintf("real_Elt method called at %d \n", i+1);
    if (i > Length(vec)) { return NA_REAL; }
    return Get(vec).value;
  }

  static int logical_Elt(SEXP vec, R_xlen_t i) {
    Rprintf("real_Elt method called at %d \n", i+1);
    if (i > Length(vec)) { return NA_LOGICAL; }
    return Get(vec).value;
  }

  static SEXP extract_subset(SEXP x, SEXP indx, SEXP call) {
    Rprintf("Extracting subset\n");
    if (x == R_NilValue) return x;

    R_xlen_t n = XLENGTH(indx);
    auto v = Get(x).value;

    auto viu = new rep<T>(n, v);
    return alt_rep<T>::Make(viu, true);
  }

  // -------- initialize the altrep class with the methods above
  static void InitInt(DllInfo* dll)
 {
    R_altrep_class_t class_t = R_make_altinteger_class("alt_rep_int", "altreptest", dll);

    // altrep
    R_set_altrep_Length_method(class_t, Length);
    R_set_altrep_Inspect_method(class_t, Inspect);

    // altvec
    R_set_altvec_Dataptr_method(class_t, Dataptr);
    R_set_altvec_Dataptr_or_null_method(class_t, Dataptr_or_null);
    R_set_altvec_Extract_subset_method(class_t, extract_subset);

    // altint
    R_set_altinteger_Elt_method(class_t, int_Elt);
    R_set_altinteger_Min_method(class_t, int_Max);
    R_set_altinteger_Max_method(class_t, int_Min);
  }

  // -------- initialize the altrep class with the methods above
  static void InitReal(DllInfo* dll)
  {
    R_altrep_class_t class_t = R_make_altinteger_class("alt_rep_real", "altreptest", dll);

    // altrep
    R_set_altrep_Length_method(class_t, Length);
    R_set_altrep_Inspect_method(class_t, Inspect);

    // altvec
    R_set_altvec_Dataptr_method(class_t, Dataptr);
    R_set_altvec_Dataptr_or_null_method(class_t, Dataptr_or_null);
    R_set_altvec_Extract_subset_method(class_t, extract_subset);

    // altreal
    R_set_altreal_Elt_method(class_t, real_Elt);
    R_set_altreal_Min_method(class_t, real_Max);
    R_set_altreal_Max_method(class_t, real_Min);
  }

  static void InitLogical(DllInfo* dll){
    R_altrep_class_t class_t = R_make_altinteger_class("alt_rep_logical", "altreptest", dll);

    // altrep
    R_set_altrep_Length_method(class_t, Length);
    R_set_altrep_Inspect_method(class_t, Inspect);

    // altvec
    R_set_altvec_Dataptr_method(class_t, Dataptr);
    R_set_altvec_Dataptr_or_null_method(class_t, Dataptr_or_null);
    R_set_altvec_Extract_subset_method(class_t, extract_subset);

    // altlogical
    R_set_altlogical_Elt_method(class_t, logical_Elt);
  }
};

// Called when the package is loaded (needs Rcpp 0.12.18.3)
// [[Rcpp::init]]
void init_alt_rep(DllInfo* dll){
  alt_rep<int>::InitInt(dll);
  alt_rep<double>::InitReal(dll);
  alt_rep<bool>::InitLogical(dll);
}

// [[Rcpp::export]]
SEXP R_alt_rep(int n, SEXP v) {

  switch( TYPEOF(v) )
  {
    case REALSXP: {
      auto x = new rep<double>(n, *REAL(v));
      return alt_rep<double>::Make(x, true);
      break;
    }
    case INTSXP: {
      auto x = new rep<int>(n, *INTEGER(v));
      return alt_rep<int>::Make(x, true);
      break;
    }
    case LGLSXP: {
      auto x = new rep<bool>(n, *LOGICAL(v));
      return alt_rep<bool>::Make(x, true);
      break;
    }
    default:
      Rf_error("Only, numeric, integer and logical types are supported for alt_rep");
      break;
  }
}

// [[Rcpp::export]]
bool R_is_alt_rep(SEXP x) {
  if (ALTREP(x))
    return true;
  else
    return false;
}
*/
