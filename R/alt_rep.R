#' @useDynLib altreplicate, .registration=TRUE
#' @importFrom Rcpp sourceCpp
NULL

#' Compact replication
#'
#' Compact replication using the ALTREP framework. It create a sequence of n identical values but does
#' not materialize it unless it is necessary.
#'
#' @param x scalar
#' @param times an integer giving the umber of times to repeat x
#'
#' @examples
#' x = alt_rep(2L, 1000L)
#' @export
alt_rep = function(x, times)
{
  stopifnot(length(x) == 1L)
  return(R_compact_rep(times, x))
}
