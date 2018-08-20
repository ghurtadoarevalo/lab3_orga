#ifndef STRUCT_H
#define STRUCT_H

typedef struct {
    int words_quantity;
	int * words;
} block_struct;


typedef struct {
    int blocks_quantity;
    block_struct** blocks;
} set_struct;

typedef struct {
    int sets_quantity;
    set_struct ** sets;
} cache_struct;

#endif
