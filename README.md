## Links

See :
 - https://levelup.gitconnected.com/c-programming-hacks-4-matrix-multiplication-are-we-doing-it-right-21a9f1cbf53
 - https://www.cse.iitm.ac.in/~rupesh/teaching/hpc/jun16/examples-cache-mm.pdf
 - https://cs.brown.edu/courses/cs033/lecture/18cacheX.pdf
 - https://www.quora.com/How-can-we-use-cache-s-for-optimization-of-matrix-multiplication
 - https://akkadia.org/drepper/cpumemory.pdf (6.2.1)

# Commands

Use `getconf LEVEL1_DCACHE_LINESIZE` to get L1 cache size linesize.
You may use **`getconf -a | grep CACHE`** or `lscpu` for additionnal information. (`x86info -c` didn't gave me much).

```bash
$valgrind --tool=cachegrind --log-file=valgrind-cachegrindijk.txt ./matrix-product
matrix_product_1() took 9.052678 seconds to execute for an entry n = 500
matrix_product_2() took 4.476539 seconds to execute for an entry n = 500
```

The I1 miss rates tells us there was no instruction cache miss.
The D1 miss rates tells us there was a lot of cache L1 misses
The LL miss rates tells us there was some Last Level cache misses.

# Some infos

```gcc -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE` ```
gain minimal pour `matrix_product_1()` : n = 800, 4.275619 -> 4.143644

# Q1.

Le programme `(i, j, k)` est le plus long à l'exécution.