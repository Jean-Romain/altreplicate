# altreplicate

A toy package for playing around with ALTREP framework. The `R` function `rep(x, n)` allocates `n * sizeof(x)` bytes of memory. Consequently `rep(0, 1e8)` uses almost 800 MB to store virtually nothing. `altreplicate` implement `alt_rep` using the ALTREP framework to store only the two numbers `x` and `n` and consequently create a compact version of `rep`. The implementation is polymorphic and serializable.

``` r
library(altreplicate)

system.time(rep(0,1e8))[3]           #> 0.5 sec
system.time(alt_rep(0,1e8))[3]       #> 23 microsec

lobstr:::obj_size(rep(TRUE,1e8))     #> 400,000,048 B
lobstr:::obj_size(alt_rep(TRUE,1e8)) #> 648 B

lobstr:::obj_size(alt_rep(0,1e8))    #> 800,000,048 B
lobstr:::obj_size(alt_rep(0,1e8))    #> 664 B
```

The vector is materialized if needed when calling non altrep-aware functions. Once materialized it takes memory.

```r
u = alt_rep(2.5, 1e5)
is_altrep(u)
#> TRUE
is_materialized(u)
#> FALSE
lobstr:::obj_size(u)
#> 664 B

range(u)
#> [1] 2.5 2.5

is_altrep(u)
#> TRUE
is_materialized(u)
#> TRUE
lobstr:::obj_size(u)
#> 800,712 B
```

Based on:

- Romain Francois alt-rep tutorials: https://github.com/romainfrancois/altrepisode
- ALTREP-examples: https://github.com/ALTREP-examples
- Arrow altrep.cpp source code: https://github.com/apache/arrow/blob/master/r/src/altrep.cpp
