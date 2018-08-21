#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
	int * words;
} block_struct;


typedef struct {
    block_struct** blocks;
} set_struct;

typedef struct {
    int sets_quantity;
    int blocks_quantity;
    int words_quantity;
    set_struct ** sets;
} cache_struct;

typedef struct	{
	int caches_quantity;
	cache_struct ** caches;
} allCaches_struct;

#endif
