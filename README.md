# Links

See :
 - https://levelup.gitconnected.com/c-programming-hacks-4-matrix-multiplication-are-we-doing-it-right-21a9f1cbf53
 - https://www.cse.iitm.ac.in/~rupesh/teaching/hpc/jun16/examples-cache-mm.pdf
 - https://cs.brown.edu/courses/cs033/lecture/18cacheX.pdf
 - https://www.quora.com/How-can-we-use-cache-s-for-optimization-of-matrix-multiplication
 - https://akkadia.org/drepper/cpumemory.pdf (6.2.1)

# Commands and explainations

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

---

**ijk**
```bash
$sudo perf stat -e task-clock,cycles,instructions,cache-references,cache-misses ./matrix-product
1000 12.058128 

 Performance counter stats for './matrix-product':

         12 096,02 msec task-clock                #    0,995 CPUs utilized          
    27 655 923 827      cycles                    #    2,286 GHz                      (37,19%)
     7 195 103 470      instructions              #    0,26  insn per cycle           (53,84%)
       160 614 882      cache-references          #   13,278 M/sec                    (66,66%)
       127 941 970      cache-misses              #   79,658 % of all cache refs      (83,36%)
     3 001 572 602      L1-dcache-loads           #  248,146 M/sec                    (83,31%)
     1 507 670 021      L1-dcache-load-misses     #   50,23% of all L1-dcache hits    (33,27%)

      12,151104421 seconds time elapsed

      12,074769000 seconds user
       0,027999000 seconds sys
```

**ikj**
```bash
$sudo perf stat -e task-clock,cycles,instructions,cache-references,cache-misses,,L1-dcache-loads,L1-dcache-load-misses ./matrix-product
1000 1.006262 

 Performance counter stats for './matrix-product':

          1 045,28 msec task-clock                #    0,998 CPUs utilized          
     2 464 033 043      cycles                    #    2,357 GHz                      (35,75%)
     4 149 347 020      instructions              #    1,68  insn per cycle           (52,56%)
        70 023 951      cache-references          #   66,991 M/sec                    (66,59%)
        20 188 624      cache-misses              #   28,831 % of all cache refs      (83,39%)
     1 558 986 613      L1-dcache-loads           # 1491,457 M/sec                    (82,69%)
       128 811 001      L1-dcache-load-misses     #    8,26% of all L1-dcache hits    (33,03%)

       1,047782942 seconds time elapsed

       1,030066000 seconds user
       0,016032000 seconds sys
```

The cache-misses event represents the number of memory access that could not be served by any of the cache. 

The ratio of cache-misses to instructions will give an indication how well the cache is working; the lower the ratio the better.

The higher IPC (Instruction per clock cycle) the more efficiently the processor is executing instruction on the system. The IPC will be affected by delay due to cache misses.

# Commands gnuplot

apt-get install gnuplot
gnuplot 5.2 the latest version

`gnuplot` to start running the program
gnuplot> plot "my-file" using 1:2 title "my-title" with lines, "my-file" using 1:3 title "my-title" with lines
is drawing on screen a graph with 2 and 3 in function of 1

# Some informations

See [notes.md](NOTES.md)

# Answers

See [answers.md](ANSWERS.md)
