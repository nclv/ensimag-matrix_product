# UTILISATION
# make
# make matrix-product équivalent à gcc matrix-product.c matrix-product.c -o matrix-product

# make clean
# make clean_executable
# make clean_all

CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).

CFLAGS = -DLEVEL1_DCACHE_LINESIZE=`getconf LEVEL1_DCACHE_LINESIZE` -std=c99 -Wall -Wextra -g3 -O3 -Werror -Wshadow -Wconversion -Wdouble-promotion -Wformat=2 -Wformat-truncation=2 -Wundef -fno-common -fstack-usage -Wfloat-equal -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow=5 -Wwrite-strings -Waggregate-return -ffunction-sections -fdata-sections
LDFLAGS = -Wl,--gc-sections -Wl,--print-gc-sections

# typing 'make' will invoke the first target entry in the file

TARGET = matrix-product
OBJS = matrix-product.o

all: $(TARGET)

matrix-product: matrix-product.o
	$(info )
	$(info Compilation de matrix-product)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# this is a suffix replacement rule for building .o's from .c's
# it uses automatic variables $<: the name of the prerequisite of
# the rule(a .c file) and $@: the name of the target of the rule (a .o file)
# .c.o:
# 	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
$(OBJS): %.o: %.c
	$(info )
	$(info Compilation des fichiers .o)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

.PHONY: clean clean_executable clean_all

clean_all: clean clean_executable

clean:
	$(info Suppression des .su, .o, .h.gch)
	rm -rf *.su *.o *.su *.o *~ *.h.gch cachegrind.out.*

clean_executable:
	$(info Supression des exécutables)
	rm -rf $(TARGET)
