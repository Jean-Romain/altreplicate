// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

#ifdef RCPP_USE_GLOBAL_ROSTREAM
Rcpp::Rostream<true>&  Rcpp::Rcout = Rcpp::Rcpp_cout_get();
Rcpp::Rostream<false>& Rcpp::Rcerr = Rcpp::Rcpp_cerr_get();
#endif

// R_compact_rep
SEXP R_compact_rep(int n, SEXP v);
RcppExport SEXP _altreplicate_R_compact_rep(SEXP nSEXP, SEXP vSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type n(nSEXP);
    Rcpp::traits::input_parameter< SEXP >::type v(vSEXP);
    rcpp_result_gen = Rcpp::wrap(R_compact_rep(n, v));
    return rcpp_result_gen;
END_RCPP
}
// R_is_altrep
bool R_is_altrep(SEXP x);
RcppExport SEXP _altreplicate_R_is_altrep(SEXP xSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type x(xSEXP);
    rcpp_result_gen = Rcpp::wrap(R_is_altrep(x));
    return rcpp_result_gen;
END_RCPP
}
// R_is_materialized
bool R_is_materialized(SEXP x);
RcppExport SEXP _altreplicate_R_is_materialized(SEXP xSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< SEXP >::type x(xSEXP);
    rcpp_result_gen = Rcpp::wrap(R_is_materialized(x));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_altreplicate_R_compact_rep", (DL_FUNC) &_altreplicate_R_compact_rep, 2},
    {"_altreplicate_R_is_altrep", (DL_FUNC) &_altreplicate_R_is_altrep, 1},
    {"_altreplicate_R_is_materialized", (DL_FUNC) &_altreplicate_R_is_materialized, 1},
    {NULL, NULL, 0}
};

void init_alt_rep(DllInfo* dll);
RcppExport void R_init_altreplicate(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
    init_alt_rep(dll);
}
