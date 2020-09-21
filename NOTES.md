You can compile with ```gcc -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE` ```.

There is a minimal gain for `matrix_product_1()` : n = 800, 4.275619 -> 4.143644.

See [data locality for implementing 2d array](https://stackoverflow.com/questions/44030148/data-locality-for-implementing-2d-array-in-c-c) and [dynamic array allocation](https://www.cs.swarthmore.edu/~newhall/unixhelp/C_arrays.html#dynamic2D) and [Memory layout of multi-dimensional arrays](https://eli.thegreenplace.net/2015/memory-layout-of-multi-dimensional-arrays#id6).

---

Pointers naturally express simple induction variables while subscripts somewhat intrinsically require more sophisticated compiler optimizations

In many cases just using a subscripted expression requires that an extra layer be added to the problem. A loop that increments a subscript i can be though of as a state machine, and the expression a[i] technically requires, each time it is used, that i be multiplied times the size of each element and added to the base address.

In order to transform that access pattern to use pointers the compiler must analyze the entire loop and determine that, say, each element is being accessed. Then the compiler can replace the multiple instances of multiplying the subscript by the element size with a simple increment of the previous loop value. This process combines optimizations called common subexpression elimination and induction variable strength reduction.

When writing with pointers, the entire optimization process is not necessary because the programmer will typically just step through the array to start with.

Sometimes the compiler can do the optimization and sometimes it can't. It's more common in recent years to have a sophisticated compiler at hand, so pointer-based code is not always faster.

Because arrrays must usually be contiguous, another advantage for pointers is in creating incrementally allocated composite structures.

---

The speed is gained in loops, most of all. When you use an array, you would use a counter which you increment. "To calculate the position, the system multiplies this counter with the size of the array element, then adds the address of the first element to get the address." However This entire sequence of steps are attained by a single assembly instruction( mov ecx, DWORD PTR _a[eax*4]) atleast in x86 thus granting no advantage to access through pointers as far as I can see. With pointers, all you need to do to go to the next element is to increase the current pointer with the size of the element to get the next one, assuming all elements are next to each other in-memory.

---

Indexing an array is dirt cheap in ways where I've never found any kind of performance boost by directly using pointers instead. That includes some very performance-critical areas like looping through each pixel of an image containing millions of them -- still no measurable performance difference between indices and pointers (though it does make a difference if you can access an image using one sequential loop over two).

I've actually found many opposite cases where turning pointers into 32-bit indices boosted performance after 64-bit hardware started becoming available when there was a need to store a boatload of them.

One of the reasons is obvious: you can take half the space now with 32-bit indices (assuming you don't need more than ~4.3 billion elements). If you're storing a boatload of them and taking half the memory as in the case of a graph data structure like indexed meshes, then typically you end up with fewer cache misses when your links/adjacency data can be stored in half the memory space.

But on a deeper level, using indices allows a lot more options. You can use purely contiguous structures that realloc to new sizes without worrying about invalidation as dasblinkenlight points out. The indices will also tend to be more dense (as opposed to sparsely fragmented across the entire 64-bit addressing space), even if you leave holes in the array, allowing for effective compression (delta, frame of reference, etc) if you want to squash down memory usage. You can then also use parallel arrays to associate data to something in parallel without using something much more expensive like a hash table. That includes parallel bitsets which allow you to do things like set intersections in linear time. It also allows for SoA reps (also parallel arrays) which tend to be optimal for sequential access patterns using SIMD.

(A more important consideration is that indexes are more robust, because they often survive array reallocation.

Consider an example: let's say you have an array that grows dynamically as you add elements to its back, an index into that array, and a pointer into that array. You add an element to the array, exhausting its capacity, so now it must grow. You call realloc, and get a new array (or an old array if there was enough extra memory after the "official" end). The pointer that you held is now invalid; the index, however, is still valid.)

You get a lot more room to optimize with indices, and I'd consider it mostly just a waste of memory if you keep pointers around on top of indices. The downside to indices for me is mostly just convenience. We have to have access to the array we're indexing on top of the index itself, while the pointer allows you to access the element without having access to its container. It's often more difficult and error-prone to write code and data structures revolving around indices and also harder to debug since we can't see the value of an element through an index. That said, if you accept the extra burden, then often you get more room to optimize with indices, not less.

---

In general I recommend avoiding using pointer arithmetic to access array buckets: it is easy to make errors and very hard to debug when you do).

---

**2D vs 1D dynamic array**

_A static 2d array is a contiguous block of memory and therefore not subject to the downsides I'm going to present here._

For dense matrices the 1D approach is likely to be faster since it offers better memory locality and less allocation and deallocation overhead.

Dynamic-1D consumes less memory than the 2D approach. The latter also requires more allocations. 

"I can imagine, that recalculating indices for 1D arrays (y + x*n) could be slower than using 2D array (x, y)" No. Look at non inlined assembly.

The difference is mov (2d) vs. lea (1d). The former has a latency of 3 cycles and a a maximum throughput of 2 per cycle while the latter has a latency of 2 cycles and a maximum throughput of 3 per cycle. Since the differences are minor, I think there should not be a big performance difference arising from index recalculation. I expect it to be very unlikely to identify this difference itself to be the bottleneck in any program.

But 1D could be in CPU cache. True, but 2d could be in CPU cache, too.

If you have a properly aligned 4 times 4 matrix of 32 bit values, a processor with a 64 byte cache line (typical value) is able to "one-shot" the data (4*4*4 = 64 bytes). If you start processing and the data isn't already in the cache you'll face a cache miss and the data will be fetched from main memory. This load can fetch the whole matrix at once since it fits into a cache line, if and only if it is contiguously stored (and properly aligned). There will probably not be any more misses while processing that data.

In case of a dynamic, "real two-dimensional" system with unrelated locations of each row/column, the processor needs to load every memory location seperately. Eventhough only 64 bytes are required, loading 4 cache lines for 4 unrelated memory positions would -in a worst case scenario- actually transfer 256 bytes and waste 75% throughput bandwidth. If you process the data using the 2d-scheme you'll again (if not already cached) face a cache miss on the first element. But now, only the first row/colum will be in the cache after the first load from main memory because all other rows are located somewhere else in memory and not adjacent to the first one. As soon as you reach a new row/column there will again be a cache miss and the next load from main memory is performed.

Long story short: The 2d pattern has a higher chance of cache misses with the 1d scheme offering better potential for performance due to locality of the data.

Moreover 2d pattern brings Memory consumption overhead, Frequent Allocation / Deallocation and then Risk of memory leaks.

You should use a contiguous block of memory and map your rows onto that block.

---

In C (but not C++) there is a third option; you can have a contiguous 2-D array that is dynamically allocated and has runtime dimensions:
```c
int (*p)[num_columns] = malloc(num_rows * sizeof *p);
```
and this is accessed like p[row_index][col_index]. 

---

For C-style allocations you can actually have the best of both worlds:
```c
double **allocate_matrix(int NumRows, int NumCol) {
  double **x;
  int i;

  x = (double **)malloc(NumRows * sizeof(double *));
  x[0] = (double *)calloc(NumRows * NumCol, sizeof(double)); // <<< single contiguous memory allocation for entire array
  for (i = 1; i < NumRows; ++i) x[i] = x[i - 1] + NumCols;
  return x;
}
```
This way you get data locality and its associated cache/memory access benefits, and you can treat the array as a double ** or a flattened 2D array (array[i * NumCols + j]) interchangeably. You also have fewer calloc/free calls (2 versus NumRows + 1).

---