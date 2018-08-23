#ifndef STRUCT_H
#define STRUCT_H

typedef enum { false, true } bool;

typedef struct
{
	int MAX;
	int rear;
	int front;
	int * queue_array;
} queue_struct;

typedef struct {
	int * words;
	int frecuency;
} block_struct;

typedef struct {
    block_struct** blocks;
	queue_struct * queue;
} set_struct;

typedef struct {
	int hits;
	int miss;
	// 0 = Fifo, 1 = MRU, 2 = LRU
	int replaceType;

	int MRUBlock;
	int LRUBlock;

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
