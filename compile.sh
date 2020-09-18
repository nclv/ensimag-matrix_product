#!/bin/bash

gcc -g -c -O3 -march=native matrix-product.c
gcc matrix-product.o -o matrix-product
