# Q1.

Le programme `(i, j, k)` est le plus long à l'exécution.

# Q2.

`valgrind-cachegrind` pour la boucle `(i, j, k)`.


```
$valgrind --tool=cachegrind --log-file=valgrind-cachegrindijk-1000.txt ./matrix-product
matrix2d_product_ijk() took 86.578572 seconds to execute for an entry n = 1000

--198803-- warning: L3 cache found, using its data for the LL simulation.
==198803== brk segment overflow in thread #1: can't grow to 0x483b000
==198803== (see section Limitations in user manual)
==198803== NOTE: further instances of this message will not be shown
==198803== 
==198803== I   refs:      7,135,382,486
==198803== I1  misses:            1,254
==198803== LLi misses:            1,249
==198803== I1  miss rate:          0.00%
==198803== LLi miss rate:          0.00%
==198803== 
==198803== D   refs:      3,063,569,074  (3,044,367,354 rd   + 19,201,720 wr)
==198803== D1  misses:    1,253,011,068  (1,251,756,391 rd   +  1,254,677 wr)
==198803== LLd misses:      125,759,735  (  125,380,280 rd   +    379,455 wr)
==198803== D1  miss rate:          40.9% (         41.1%     +        6.5%  )
==198803== LLd miss rate:           4.1% (          4.1%     +        2.0%  )
==198803== 
==198803== LL refs:       1,253,012,322  (1,251,757,645 rd   +  1,254,677 wr)
==198803== LL misses:       125,760,984  (  125,381,529 rd   +    379,455 wr)
==198803== LL miss rate:            1.2% (          1.2%     +        2.0%  )
```

`valgrind-cachegrind` pour la boucle `(i, k, j)`.

```
$valgrind --tool=cachegrind --log-file=valgrind-cachegrindikj-1000.txt ./matrix-product
matrix2d_product_ikj() took 26.232037 seconds to execute for an entry n = 1000

--199022-- warning: L3 cache found, using its data for the LL simulation.
==199022== brk segment overflow in thread #1: can't grow to 0x483b000
==199022== (see section Limitations in user manual)
==199022== NOTE: further instances of this message will not be shown
==199022== 
==199022== I   refs:      4,130,887,438
==199022== I1  misses:            1,256
==199022== LLi misses:            1,251
==199022== I1  miss rate:          0.00%
==199022== LLi miss rate:          0.00%
==199022== 
==199022== D   refs:      2,063,569,053  (1,545,367,338 rd   + 518,201,715 wr)
==199022== D1  misses:      125,899,531  (  125,644,851 rd   +     254,680 wr)
==199022== LLd misses:      125,896,038  (  125,641,837 rd   +     254,201 wr)
==199022== D1  miss rate:           6.1% (          8.1%     +         0.0%  )
==199022== LLd miss rate:           6.1% (          8.1%     +         0.0%  )
==199022== 
==199022== LL refs:         125,900,787  (  125,646,107 rd   +     254,680 wr)
==199022== LL misses:       125,897,289  (  125,643,088 rd   +     254,201 wr)
==199022== LL miss rate:            2.0% (          2.2%     +         0.0%  )
```

Le programme `(i, k, j)` fait `4,130,887,438` instructions et `125,899,531` défauts de cache `L1`.

Le programme `(i, j, k)` fait `7,135,382,486` instructions et `1,253,011,068` défauts de cache `L1`. C'est 10 fois plus de défauts de cache !!

---

```bash
$./matrix-product 
matrix2d_product_ijk() took 8.568009 seconds to execute for an entry n = 1000
```

Soit 68.4 ns par défaut de cache.

```bash
$./matrix-product 
matrix2d_product_ikj() took 0.785889 seconds to execute for an entry n = 1000
```

Soit 62.4 ns par défaut de cache.

On peut donner 65 ns comme temps moyen d'un défaut de cache.

# Q3.

$W_x(n) = $ 



