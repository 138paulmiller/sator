#!/bin/sh
make 
 valgrind --leak-check=yes  --leak-check=full --show-leak-kinds=all  ./sator