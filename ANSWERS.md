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

$$W_x(n) = n^3$$ 
$$W_+(n) = n^3$$ 
$n^2$ additions sur `ptC` et $n^3$ additions sur `ptA` et `ptB` pour le programme `(i, j, k)`.

$n^2$ additions sur `ptA` et $n^3$ additions sur `ptB` et `ptB` pour le programme `(i, k, j)`.
$$W_{ptr+}(n) = n^3$$ 
$$W_{ptrx}(n) = 2n^2(n + 1)$$ 

Les deux programmes effectuent un nombre analogue d'opérations.

# Q4.

Si le **compilateur sait** (il le sait généralement) que le foncteur M(i, j) est implémenté par M(i ∗ n + j) où la valeur de n est un attribut de M qui n’est pas modifié par l’appel M(i, j).

Dans les 2 programmes, il n'est **(pas) utile** d’éliminer les multiplications pour l’accès aux coefficients des 3 matrices.

---

Dans de nombreux cas, le simple fait d'utiliser un indexage nécessite d'ajouter une couche supplémentaire au problème. Lorsque vous utilisez un tableau, vous utilisez un compteur que vous incrémentez. Pour calculer la position, le système multiplie ce compteur par la taille de l'élément du tableau, puis ajoute l'adresse du premier élément pour obtenir l'adresse. Toute cette séquence d'étapes est atteinte par une seule instruction (`mov ecx, DWORD PTR _a[eax*4]`) en x86, ce qui ne donne aucun avantage à l'accès par des pointeurs. Cette optimisation est propre au compilateur. Il est devenu plus courant d'avoir un compilateur sophistiqué à portée de main, de sorte que le code à base de pointeurs n'est pas toujours plus rapide.

Afin de transformer `M(i, j)` pour utiliser des pointeurs, le compilateur doit analyser toute la boucle et déterminer que, par exemple, chaque élément est accédé. Ce processus combine des optimisations appelées _Recherche de sous-expressions communes_ et _induction variable strength reduction_.

Lors de l'écriture avec des pointeurs, l'ensemble du processus d'optimisation n'est pas nécessaire car le programmeur se contente généralement de parcourir le tableau. Comme les tableaux doivent généralement être **contigus**, un autre avantage des pointeurs est la création de structures allouées de manière incrémentielle. En `C`, on a de nombreux choix d'allocation mémoire pour représenter une matrice. Une déclaration statique assure que la localité des données est respectée. Cette propriété n'est généralement pas assurée par l'allocation dynamique d'un tableau 2d mais est possible (impossible en C++). Utiliser un tableau 1d pour représenter une matrice assure la localité des données et les avantages qui y sont associés en termes d'accès au cache/mémoire mais impose un accès par `M[i ∗ n + j]`.

---

Il est important de noter que les index sont plus robustes, car ils survivent souvent à la réaffectation des tableaux. Prenons un exemple : disons que vous avez un tableau qui croît dynamiquement lorsque vous ajoutez des éléments, un index dans ce tableau et un pointeur sur ce tableau. Vous ajoutez un élément au tableau, épuisant ainsi sa capacité, et il doit maintenant augmenter sa taille. Vous appelez `realloc`, et vous obtenez un nouveau tableau (ou l'ancien tableau s'il y avait suffisamment de mémoire supplémentaire après la fin donnée). Le pointeur que vous aviez est maintenant invalide , l'index cependant est toujours valide.

L'inconvénient des indices est surtout la commodité. Nous devons avoir accès au tableau que nous indexons en plus de l'index lui-même, alors que le pointeur vous permet d'accéder à l'élément sans avoir accès à son conteneur.

# Q.5

On considère un cache LL1 supposé vide de taille 32Kbits avec des lignes de 64 bits (obtenu sur ma machine avec la commande `getconf -a | grep CACHE`).

**Le programme `(i, j, k)`**

_A_: A est indexé comme `A[i][k]`, pour `n` itérations de `k`, pour `i` et `j` fixés à 0, la séquence d'accès est `A[0][0], A[0][1], A[0][2], ... A[0][n-1]`. Étant donné que des éléments de mémoire contigus sont accédés, il y aura un échec tous les `L` accès, soit `n/L` défauts de cache. Lorsque `j` varie, la même ligne sera accédée de manière répétée dans le cache, ce qui se traduira par des hits (`j` n'apparaît pas dans l'indexation de A). Comme la capacité du cache est suffisamment importante pour contenir `n` éléments, aucun défaut de cache supplémentaire ne se produit pour `j` allant de 1 à `n-1`.  Le coût total de l'exécution de toutes les itérations de `j` est juste une fois le coût déjà déterminé pour l'exécution de toutes les itérations de la boucle la plus interne. Les mêmes coûts se répètent pour chaque itération `i`, lorsque différentes lignes de A sont accédées. EN effet, comme on fait varier la boucle la plus extérieure (`i`), pour chaque valeur distincte de `i`, on accède à différentes lignes de A, et on a une répétition du nombre d'échecs correspondant à `i=0`. Le nombre total de défauts de cache est donc de `n*n/L` pour les caches à mappage direct et les caches complètement associatifs. 

_B_: pour `i` et `j` fixés, lorsque `k` varie, on accède aux éléments d'une colonne de B.  Lorsque `j` est incrémenté, la colonne adjacente de B est chargée, mais il y aura des défauts de cache pour un cache à mappage direct (et des hits pour un cache entièrement associatif puisque seules les lignes `n/L` seront utilisées). Cependant, aucune réutilisation n'est possible dans la boucle `i`. Ainsi, les erreurs pour un cache direct seront `n*n`, et `n*(n/L)` pour un cache entièrement associatif.

_C_: similaire à la matrice A, il y aura une réutilisation des coefficients à la fois temporelle et spatiale. Le nombre total de défauts de cache sera donc `n*n/L`.

**Le programme `(i, k, j)`**
_A_: La matrice A prends la place de la matrice C dans le programme `(i, j, k)`. Le nombre total de défauts de cache est `n*n/L`.

_B_: on exploite la localité puisqu'il est accessible par ligne dans la boucle la plus interne. Une réutilisation temporaire est possible, car la capacité est suffisante pour contenir tout B jusqu'à ce que la boucle extérieure `i` change. Ainsi, il y aura `n*n/L` défauts de cache pour les caches à mappage direct et les caches complètement associatifs.

_C_: pour `i` et `k` fixés, lorsque `j` varie, on accède à la ligne i, occupant `n/L` lignes adjacentes dans le cache. Lorsque `k` varie, la même ligne sera accédée de façon répétée dans le cache. Ainsi, on avec un cache à mappage direct ou entièrement associatif `n*n/L` défauts de cache.

# Q.6

Le cache est très petit, donc `Z << n`.

**Le programme `(i, j, k)`**

A la première itération, on a `n/L + n` défauts de cache. Pareil pour la seconde etc...
On a donc `n*n*n*(1 + 1/L)` défauts de cache (sans compter la matrice C qui rajoute `n*n/L`).

_B_: pour `i` et `j` fixés, lorsque `k` varie, on accède aux éléments d'une colonne de B.  Lorsque `j` est incrémenté, la colonne adjacente de B est chargée, mais il y aura des défauts de cache pour un cache à mappage direct (et des hits pour un cache entièrement associatif puisque seules les lignes `n/L` seront utilisées). Cependant, aucune réutilisation n'est possible dans la boucle `i`. Ainsi, les erreurs pour un cache direct seront `n*n*n`, et `n*(n/L)*n` pour un cache entièrement associatif.

**Le programme `(i, k, j`**
_B_: on exploite la localité puisqu'il est accessible par ligne dans la boucle la plus interne. Mais une réutilisation temporaire n'est pas possible, car la capacité est insuffisante pour contenir tout B jusqu'à ce que la boucle extérieure `i` change. Ainsi, il y aura `n*n*n/L` défauts de cache pour les caches à mappage direct et les caches complètement associatifs.

# Q.7

Pour améliorer l'algorithme dans les conditions de la question précédente, on peut effectuer un produit par blocs.

Il faut que la taille `B` du block vérifie `B^2 < Z`. On a alors `B^2/L` défaut de cache à la première itération pour chaque block. Soit `2*n/B*(B^2/L)` défauts de cache sans compter la matrice C. On a la même chose à chaque itération soit `(2*n*B/L)*(n/B)^2` défauts de cache.

On pourrait aussi aligner manuellement les arrays A, B et C.
