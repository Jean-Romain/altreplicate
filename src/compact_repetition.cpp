#include "altrepisode.h"

struct repetition {
  unsigned int size;
  int value;
  repetition(int n, int v) : size(n), value(v) {}
};

struct compact_repetition
{
  static R_altrep_class_t class_t;

  // constructor function
  static SEXP Make(repetition* data, bool owner)
  {
    Rprintf("Making a compact repetition with %d x %d\n", data->value, data->size);

    SEXP xp = PROTECT(R_MakeExternalPtr(data, R_NilValue, R_NilValue));
    if (owner) R_RegisterCFinalizerEx(xp, compact_repetition::Finalize, TRUE);

    SEXP res = R_new_altrep(class_t, xp, R_NilValue);
    UNPROTECT(1);
    return res;
  }

  // finalizer for the external pointer
  static void Finalize(SEXP xp){
    Rprintf("Deleting a compact repetition at %p\n", R_ExternalPtrAddr(xp));
    delete static_cast<repetition*>(R_ExternalPtrAddr(xp));
  }

  // get the std::vector<string>* from the altrep object `x`
  static repetition* Ptr(SEXP x) {
    return static_cast<repetition*>(R_ExternalPtrAddr(R_altrep_data1(x)));
  }

  // same, but as a reference, for convenience
  static repetition& Get(SEXP vec) {
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

  // What gets printed when .Internal(inspect()) is used
  static Rboolean Inspect(SEXP x, int pre, int deep, int pvec, void (*inspect_subtree)(SEXP, int, int, int)){
    Rprintf("compact representaion of %d x %d\n", Get(x).value, Length(x));
    return TRUE;
  }

  // ALTVEC methods ------------------

  // The start of the data, i.e. the underlying string array from the random_string_data
  //
  // This is guaranteed to never allocate (in the R sense)
  static const void* Dataptr_or_null(SEXP vec) {
    Rprintf("Calling Dataptr_or_null on a compact_repetition at %p\n", R_ExternalPtrAddr(vec));
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
      Rprintf("Returning pointer to materialized compact_repetition at %p\n", R_ExternalPtrAddr(vec));
      return STDVEC_DATAPTR(data2);
    }

    Rprintf("Materializing a compact repetition at %p\n", R_ExternalPtrAddr(vec));
    int n = Length(vec);
    int v = Get(vec).value;
    SEXP val = PROTECT(Rf_allocVector(INTSXP, n));
    Rprintf("n = %d, v = %d\n", n, v);
    int *p = INTEGER(val);
    for (int i = 0; i < n; i++) p[i] = v;
    Rprintf("val[1] = %d\n", INTEGER(val)[0]);
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

  static SEXP extract_subset(SEXP x, SEXP indx, SEXP call) {
    Rprintf("Extracting subset\n");
    if (x == R_NilValue) return x;

    int *p = INTEGER(indx);
    R_xlen_t n = XLENGTH(indx);
    R_xlen_t s = Length(x);
    auto v = Get(x).value;

    bool indx_out_of_bound = false;
    for (int i = 0; i < n; i++)
    {
      if (p[i] > s || p[i] < 1)
      {
        indx_out_of_bound = true;
        break;
      }
    }

    if (indx_out_of_bound)
    {
      SEXP out = PROTECT(Rf_allocVector(INTSXP, n));
      int *pout = INTEGER(out);
      p = INTEGER(indx);
      for (int i = 0; i < n; i++)
      {
        if (p[i] <= s && p[i] > 0)
          pout[i] = v;
        else
          pout[i] = NA_INTEGER;
      }
      UNPROTECT(1);
      return out;
    }
    else
    {
      auto viu = new repetition(n, v);
      return compact_repetition::Make(viu, true);
    }
  }

  // -------- initialize the altrep class with the methods above
  static void InitInt(DllInfo* dll)
  {
    class_t = R_make_altinteger_class("compact repetition (int)", "altreplicate", dll);

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
};

R_altrep_class_t compact_repetition::class_t;

// Called when the package is loaded (needs Rcpp 0.12.18.3)
// [[Rcpp::init]]
void init_alt_rep(DllInfo* dll){
  compact_repetition::InitInt(dll);
}

// [[Rcpp::export]]
SEXP R_compact_rep(int n, int v)
{
  auto x = new repetition(n, v);
  return compact_repetition::Make(x, true);
}
