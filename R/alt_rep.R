#' @useDynLib altreplicate, .registration=TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Compact replication
#'
#' Compact replication using the ALTREP framework. It creates a sequence of n identical values but does
#' not materialize it unless it is necessary.
#'
#' @param x scalar. Supports double, integer and bool
#' @param times an integer giving the umber of times to repeat x
#'
#' @examples
#' x = alt_rep(2L, 1000L)
#' @export
alt_rep = function(x, times)
{
  stopifnot(length(x) == 1L)
  stopifnot(length(times) == 1L)
  return(R_compact_rep(times, x))
}


#' ALTREP tools
#'
#' ALTREP tools
#'
#' @param x R object
#'
#' @examples
#' u = alt_rep(2.1, 5)
#' is_altrep(u)
#' is_materialized(u)
#' @export
is_altrep <- R_is_altrep

#' @export
#' @rdname is_altrep
is_materialized <- R_is_materialized
