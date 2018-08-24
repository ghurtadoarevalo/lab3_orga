#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"
#include <math.h>

//Función encargada de remover todos los espacios de un string
//Entradas: String
//Salidas: String sin espacios
void removeSpaces(char *str)
{
    int count = 0;


    for (int i = 0; str[i]; i++)
        if (str[i] != ' ')
            str[count++] = str[i];

    str[count] = '\0';
}

void queueInsert(int item, queue_struct * queue)
{
    if (queue->rear == queue->MAX - 1){}
    //printf("Queue Overflow \n");
    else
    {
        if (queue->front == - 1)
        {
            /*If queue is initially empty */
            queue->front = 0;
        }

        queue->rear = queue->rear + 1;
        queue->queue_array[queue->rear] = item;
        //printf("Element inserted in queue is: %d\n", item);
    }
} /*End of insert()*/

int queueDelete(queue_struct * queue)
{
    if (queue->front == - 1 || queue->front > queue->rear)
    {
        //printf("Queue Underflow \n");
    }
    else
    {
        //printf("Element deleted from queue is : %d\n", queue->queue_array[queue->front]);
        queue->front = queue->front + 1;
    }

    return queue->queue_array[queue->front-1];
} /*End of delete() */

void queueDisplay(queue_struct * queue)
{
    int i;
    if (queue->front == - 1){}
        //printf("Queue is empty \n");
    else
    {
        //printf("Queue is : \n");
        for (i = queue->front; i <= queue->rear; i++)
            printf("%d ", queue->queue_array[i]);
        printf("\n");
    }
} /*End of display() */

//Función encargada de leer todas las lineas del archivo con el nombre que contenga la variable fp_source_name_1
//Entradas: String
//Salidas: Arreglo de arreglos de strings (Instrucciones o Lineas de control)
char** readData(char* fp_source_name_1)
{
	FILE *fp = NULL;
	int size = 0;
	int count = 1;
	int linesNumber = 0;
	int lineLen = 1;
	char* dataFileChar = NULL;
	char* temp = NULL;
	char* token = NULL;
	char** lines = NULL;


	fp = fopen (fp_source_name_1, "r" );

    if (fp==NULL)
    {
        printf("No existe archivo con nombre: %s, asegúrese de que se encuentre en el directorio correcto o exista el archivo.\n", fp_source_name_1);
        exit (1);
    }

	fseek(fp,0,SEEK_END);
	size = ftell(fp);
	rewind(fp);

	dataFileChar = malloc(sizeof(char)*(size+1));
	fread(dataFileChar,sizeof(char), size, fp);
	fclose(fp);
    dataFileChar[size] = '\0';
	//Se recorre el char* para encontrar la cantidad de palabras
	for (int i = 0; i < size ; ++i)
	{
		if(dataFileChar[i] == '\n' || dataFileChar[i] == '\0')
		{
			linesNumber++;
		}
	}

	//Se asigna memoria para cada linea dentro del arreglo
	lines = malloc(sizeof(char*)*(linesNumber+1));

	//Se asigna memoria para cada linea dentro del arreglo
	linesNumber = 0;
	for (int i = 0; i <= size; ++i)
	 {
	 	if(dataFileChar[i] == '\n' || dataFileChar[i] == '\0')
		{
			lines[linesNumber] = malloc(sizeof(char)*(lineLen));
			linesNumber++;
			lineLen = 1;
		}

		else if(dataFileChar[i] != '\n')
		{
			lineLen++;;
		}
	}

	//Se separan las palabras por \n
	temp = malloc(sizeof(char)*(size+1));
	strcpy(temp,dataFileChar);
	token = strtok(temp, "\n");
    lines[0] = malloc(sizeof(char)*2);
	while(token != NULL)
	{
        //printf("%s\n",token );
        lines[count] = malloc(sizeof(char)*(size));
		memcpy(lines[count],token,sizeof(char)*(size));
        //printf("%s\n",lines[count]);
        //lines[count][strlen(token)-1] = '\0';
		token = strtok(NULL, "\n");
		count++;
	}

    sprintf(lines[0],"%d", count-1);

    return lines;
}

void showCache(cache_struct * cache)
{
    for (size_t i = 0; i < cache->sets_quantity; i++)
    {
        printf("\nConjunto %ld: \n", i);
        for (size_t j = 0; j < cache->blocks_quantity; j++)
        {
            printf("Bloque %ld: \n", j);
            printf("Bloque frecuency: %d\n",cache->sets[i]->blocks[j]->frecuency);
            for (size_t k = 0; k < cache->words_quantity; k++)
            {
                printf("Palabra %ld: Mem(%d)\n",k,cache->sets[i]->blocks[j]->words[k]);
            }
        }
    }
}

cache_struct * build_cache(int words_quantity ,int blocks_quantity, int sets_quantity, int option)
{
    cache_struct * cache = malloc(sizeof(cache_struct));
    cache->sets = malloc(sizeof(set_struct*)*sets_quantity);
    cache->sets_quantity = sets_quantity;
    cache->blocks_quantity = blocks_quantity;
    cache->words_quantity = words_quantity;
    // 1 = Fifo, 2 = MRU, 3 = LRU
    cache->replaceType = option;

    for (size_t i = 0; i < sets_quantity; i++)
    {
        cache->sets[i] = malloc(sizeof(set_struct));
        cache->sets[i]->blocks = malloc(sizeof(block_struct*)*blocks_quantity);
        cache->sets[i]->queue = malloc(sizeof(queue_struct));
        cache->sets[i]->queue->MAX = 9999;
        cache->sets[i]->queue->front = -1;
        cache->sets[i]->queue->rear = -1;
        cache->sets[i]->queue->queue_array = malloc(sizeof(int*)*(cache->sets[i]->queue->MAX));
        cache->hits = 0;
        cache->miss = 0;
        for (size_t j = 0; j < blocks_quantity; j++)
        {
            cache->sets[i]->blocks[j] = malloc(sizeof(block_struct));
            cache->sets[i]->blocks[j]->words = malloc(sizeof(int)*words_quantity);
            cache->sets[i]->blocks[j]->frecuency = 0;

            for (size_t k = 0; k < words_quantity; k++)
                {
                cache->sets[i]->blocks[j]->words[k] = -1;
            }
        }
    }

    return cache;
}

void populateCache(cache_struct * cache, char ** data)
{
    int option;
    int block;
    int set;
    int word;
    bool hit;

    //Si es directo
    if (cache->blocks_quantity == 1)
    {
        // printf("\n%s\n","Soy directo" );
        // showCache(cache);
        // printf("Tipo de reemplazo: %d\n",cache->replaceType );
        option = 1;
    }

    //Si es full asociativo
    else if(cache->sets_quantity == 1)
    {
        //printf("\n%s\n","Soy Full asociativo");
        //showCache(cache);
        //printf("Tipo de reemplazo: %d\n",cache->replaceType );
        option = 2;
    }

    //Si es n-asociativo
    else
    {
        // printf("\n%s\n","Soy n-asociativo");
        // showCache(cache);
        // printf("Tipo de reemplazo: %d\n",cache->replaceType );
        option = 3;
    }

    switch (option) {
        case 1:
            //printf("Sets Quantity: %d , blocks_quantity: %d, words_quantity: %d\n", cache->sets_quantity,cache->blocks_quantity, cache->words_quantity );
            for (size_t i = 1; i <= atoi(data[0]); i++)
            {
                hit = false;
                block = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);
                word = block%(cache->words_quantity);

                for (size_t j = 0; j < cache->words_quantity; j++)
                {
                    if (cache->sets[block]->blocks[0]->words[j] == atoi(data[i]))
                    {
                        cache->hits += 1;
                        hit = true;
                        break;
                    }
                }

                if (hit == false )
                {
                    cache->miss += 1;
                    for (size_t j = 0; j < cache->words_quantity; j++)
                    {
                        if (j < word)
                        {
                            cache->sets[block]->blocks[0]->words[j] = atoi(data[i]) - word + j;
                        }

                        else if(j > word)
                        {
                            cache->sets[block]->blocks[0]->words[j] = atoi(data[i]) + word + j;
                        }

                        else
                        {
                            cache->sets[block]->blocks[0]->words[j] = atoi(data[i]);
                        }
                    }
                }
            }
            //showCache(cache);
            //printf("Hits: %d, Miss: %d\n",cache->hits, cache->miss );
            break;

        case 2:
            switch (cache->replaceType)
            {
                //Fifo
                case 0:
                    for (size_t i = 1; i <= atoi(data[0]); i++)
                    {
                        block = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);
                        word = block%(cache->words_quantity);
                        int j;

                        for (j = 0; j < cache->blocks_quantity; j++)
                        {
                            hit = false;
                            for (size_t k = 0; k < cache->words_quantity; k++)
                            {
                                if (cache->sets[0]->blocks[j]->words[k] == atoi(data[i]))
                                {
                                    cache->hits += 1;
                                    hit = true;
                                    break;
                                }
                            }

                            if (hit == true)
                            {
                                break;
                            }
                        }

                        if (hit == true)
                        {
                            continue;
                        }

                        cache->miss += 1;

                        for (j = 0; j < cache->blocks_quantity; j++)
                        {
                            if (cache->sets[0]->blocks[j]->words[0] == -1)
                            {
                                for (size_t k = 0; k < cache->words_quantity; k++)
                                {
                                    if (k < word)
                                    {
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                    }

                                    else if(k > word)
                                    {
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                    }

                                    else
                                    {
                                        queueInsert(atoi(data[i]),cache->sets[0]->queue);
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]);
                                    }
                                }
                                break;
                            }
                        }

                        if (j == cache->blocks_quantity)
                        {
                            int deleted = queueDelete(cache->sets[0]->queue);
                            int word = 0;
                            for (j = 0; j < cache->blocks_quantity; j++)
                            {
                                bool validator = false;

                                for (size_t k = 0; k < cache->words_quantity; k++)
                                {
                                    if (cache->sets[0]->blocks[j]->words[k] == deleted)
                                    {
                                        validator = true;
                                        word = k;
                                        break;
                                    }

                                }

                                if (validator == true)
                                {
                                    for (size_t k = 0; k < cache->words_quantity; k++)
                                    {
                                        if (k < word)
                                        {
                                            cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                        }

                                        else if(k > word)
                                        {
                                            cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                        }

                                        else if(k == word)
                                        {
                                            queueInsert(atoi(data[i]),cache->sets[0]->queue);
                                            cache->sets[0]->blocks[j]->words[k] = atoi(data[i]);
                                        }
                                    }
                                }
                            }
                        }
                    }
                    //printf("Hits: %d, Miss: %d\n",cache->hits, cache->miss );
                    break;

                //MRU
                case 1:
                    for (size_t i = 1; i <= atoi(data[0]); i++)
                    {
                        block = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);
                        //printf("Block: %d, dirección: %d\n",block,atoi(data[i]));
                        word = block%(cache->words_quantity);
                        int j;
                        for (j = 0; j < cache->blocks_quantity; j++)
                        {
                            hit = false;
                            for (size_t k = 0; k < cache->words_quantity; k++)
                            {
                                if (cache->sets[0]->blocks[j]->words[k] == atoi(data[i]))
                                {
                                    cache->sets[0]->MRUBlock = j;
                                    cache->hits += 1;
                                    hit = true;
                                    break;
                                }
                            }

                            if (hit == true)
                            {
                                break;
                            }
                        }

                        if (hit == true)
                        {
                            continue;
                        }

                        cache->miss += 1;

                        for (j = 0; j < cache->blocks_quantity; j++)
                        {
                            if (cache->sets[0]->blocks[j]->words[0] == -1)
                            {
                                for (size_t k = 0; k < cache->words_quantity; k++)
                                {
                                    if (k < word)
                                    {
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                    }

                                    else if(k > word)
                                    {
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                    }

                                    else
                                    {
                                        cache->sets[0]->MRUBlock = j;
                                        cache->sets[0]->blocks[j]->frecuency = 0;
                                        cache->sets[0]->blocks[j]->words[k] = atoi(data[i]);
                                    }
                                }
                                break;
                            }
                        }

                        if (j == cache->blocks_quantity)
                        {

                            int MRUBlock = cache->sets[0]->MRUBlock;
                            for (size_t k = 0; k < cache->words_quantity; k++)
                            {
                                if (k < word)
                                {
                                    cache->sets[0]->blocks[MRUBlock]->words[k] = atoi(data[i]) - word + k;
                                }

                                else if(k > word)
                                {
                                    cache->sets[0]->blocks[MRUBlock]->words[k] = atoi(data[i]) + word + k;
                                }

                                else if(k == word)
                                {
                                    cache->sets[0]->MRUBlock = j-1;
                                    cache->sets[0]->blocks[MRUBlock]->frecuency = 0;
                                    cache->sets[0]->blocks[MRUBlock]->words[k] = atoi(data[i]);
                                }
                            }
                        }
                    }

                    //showCache(cache);

                    break;

                //LRU
                case 2:
                    for (size_t i = 1; i <= atoi(data[0]); i++)
                    {
                         set = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);

                         for (size_t j = 0; j < cache->sets_quantity; j++)
                         {
                             for (size_t k = 0; k < cache->blocks_quantity; k++)
                             {
                                 cache->sets[j]->blocks[k]->frecuency += 1;
                             }
                         }
                         word = set%(cache->words_quantity);
                         int j;
                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             hit = false;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {

                                 if (cache->sets[0]->blocks[j]->words[k] == atoi(data[i]))
                                 {
                                     cache->sets[0]->blocks[j]->frecuency = 0;
                                     cache->hits += 1;
                                     hit = true;
                                     break;
                                 }
                             }

                             if (hit == true)
                             {
                                 break;
                             }
                         }

                         if (hit == true)
                         {
                             continue;
                         }

                         cache->miss += 1;

                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             if (cache->sets[0]->blocks[j]->words[0] == -1)
                             {
                                 for (size_t k = 0; k < cache->words_quantity; k++)
                                 {
                                     if (k < word)
                                     {
                                         cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                     }

                                     else if(k > word)
                                     {
                                         cache->sets[0]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                     }

                                     else
                                     {
                                         cache->sets[0]->blocks[j]->frecuency = 0;
                                         cache->sets[0]->blocks[j]->words[k] = atoi(data[i]);
                                     }
                                 }
                                 break;
                             }
                         }

                         if (j == cache->blocks_quantity)
                         {
                             int max = -99999;
                             for (j = 0; j < cache->blocks_quantity; j++)
                             {
                                 if (cache->sets[0]->blocks[j]->frecuency > max)
                                 {
                                     max = cache->sets[0]->blocks[j]->frecuency;
                                     cache->sets[0]->LRUBlock = j;
                                 }
                             }

                             int LRUBlock = cache->sets[0]->LRUBlock;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {
                                 if (k < word)
                                 {
                                     cache->sets[0]->blocks[LRUBlock]->words[k] = atoi(data[i]) - word + k;
                                 }

                                 else if(k > word)
                                 {
                                     cache->sets[0]->blocks[LRUBlock]->words[k] = atoi(data[i]) + word + k;
                                 }

                                 else if(k == word)
                                 {
                                     cache->sets[0]->blocks[LRUBlock]->frecuency = 0;
                                     cache->sets[0]->blocks[LRUBlock]->words[k] = atoi(data[i]);
                                 }
                             }
                         }
                     }
                     break;
            }
            break;

        case 3:
            switch (cache->replaceType)
            {
                 //Fifo
                 case 0:
                     for (size_t i = 1; i <= atoi(data[0]); i++)
                     {
                         set = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);
                         word = set%(cache->words_quantity);
                         int j;

                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             hit = false;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {
                                 if (cache->sets[set]->blocks[j]->words[k] == atoi(data[i]))
                                 {
                                     cache->hits += 1;
                                     hit = true;
                                     break;
                                 }
                             }

                             if (hit == true)
                             {
                                 break;
                             }
                         }

                         if (hit == true)
                         {
                             continue;
                         }

                         cache->miss += 1;

                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             if (cache->sets[set]->blocks[j]->words[0] == -1)
                             {
                                 for (size_t k = 0; k < cache->words_quantity; k++)
                                 {
                                     if (k < word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                     }

                                     else if(k > word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                     }

                                     else
                                     {
                                         queueInsert(atoi(data[i]),cache->sets[set]->queue);
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]);
                                     }
                                 }
                                 break;
                             }
                         }

                         if (j == cache->blocks_quantity)
                         {
                             int deleted = queueDelete(cache->sets[set]->queue);
                             int word = 0;
                             for (j = 0; j < cache->blocks_quantity; j++)
                             {
                                 bool validator = false;

                                 for (size_t k = 0; k < cache->words_quantity; k++)
                                 {
                                     if (cache->sets[set]->blocks[j]->words[k] == deleted)
                                     {
                                         validator = true;
                                         word = k;
                                         break;
                                     }

                                 }

                                 if (validator == true)
                                 {
                                     for (size_t k = 0; k < cache->words_quantity; k++)
                                     {
                                         if (k < word)
                                         {
                                             cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                         }

                                         else if(k > word)
                                         {
                                             cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                         }

                                         else if(k == word)
                                         {
                                             queueInsert(atoi(data[i]),cache->sets[set]->queue);
                                             cache->sets[set]->blocks[j]->words[k] = atoi(data[i]);
                                         }
                                     }
                                 }
                             }
                         }
                     }
                     break;

                //MRU
                 case 1:
                     for (size_t i = 1; i <= atoi(data[0]); i++)
                     {
                         set = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);
                         //printf("Block: %d, dirección: %d\n",block,atoi(data[i]));
                         word = set%(cache->words_quantity);
                         int j;
                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             hit = false;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {
                                 if (cache->sets[set]->blocks[j]->words[k] == atoi(data[i]))
                                 {
                                     cache->sets[set]->MRUBlock = j;
                                     cache->hits += 1;
                                     hit = true;
                                     break;
                                 }
                             }

                             if (hit == true)
                             {
                                 break;
                             }
                         }

                         if (hit == true)
                         {
                             continue;
                         }

                         cache->miss += 1;

                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             if (cache->sets[set]->blocks[j]->words[0] == -1)
                             {
                                 for (size_t k = 0; k < cache->words_quantity; k++)
                                 {
                                     if (k < word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                     }

                                     else if(k > word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                     }

                                     else
                                     {
                                         cache->sets[set]->MRUBlock = j;
                                         cache->sets[set]->blocks[j]->frecuency = 0;
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]);
                                     }
                                 }
                                 break;
                             }
                         }

                         if (j == cache->blocks_quantity)
                         {

                             int MRUBlock = cache->sets[set]->MRUBlock;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {
                                 if (k < word)
                                 {
                                     cache->sets[set]->blocks[MRUBlock]->words[k] = atoi(data[i]) - word + k;
                                 }

                                 else if(k > word)
                                 {
                                     cache->sets[set]->blocks[MRUBlock]->words[k] = atoi(data[i]) + word + k;
                                 }

                                 else if(k == word)
                                 {
                                     cache->sets[set]->MRUBlock = j-1;
                                     cache->sets[set]->blocks[MRUBlock]->frecuency = 0;
                                     cache->sets[set]->blocks[MRUBlock]->words[k] = atoi(data[i]);
                                 }
                             }
                         }
                     }
                     break;

                //LRU
                 case 2:
                    for (size_t i = 1; i <= atoi(data[0]); i++)
                    {
                         set = (atoi(data[i])/(4*cache->words_quantity))%(cache->sets_quantity);

                         for (size_t j = 0; j < cache->sets_quantity; j++)
                         {
                             for (size_t k = 0; k < cache->blocks_quantity; k++)
                             {
                                 cache->sets[j]->blocks[k]->frecuency += 1;

                             }
                         }

                         word = set%(cache->words_quantity);
                         int j;
                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             hit = false;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {

                                 if (cache->sets[set]->blocks[j]->words[k] == atoi(data[i]))
                                 {
                                     cache->sets[set]->blocks[j]->frecuency = 0;
                                     cache->hits += 1;
                                     hit = true;
                                     break;
                                 }
                             }

                             if (hit == true)
                             {
                                 break;
                             }
                         }

                         if (hit == true)
                         {
                             continue;
                         }

                         cache->miss += 1;

                         for (j = 0; j < cache->blocks_quantity; j++)
                         {
                             if (cache->sets[set]->blocks[j]->words[0] == -1)
                             {
                                 for (size_t k = 0; k < cache->words_quantity; k++)
                                 {
                                     if (k < word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) - word + k;
                                     }

                                     else if(k > word)
                                     {
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]) + word + k;
                                     }

                                     else
                                     {
                                         cache->sets[set]->blocks[j]->frecuency = 0;
                                         cache->sets[set]->blocks[j]->words[k] = atoi(data[i]);
                                     }
                                 }
                                 break;
                             }
                         }

                         if (j == cache->blocks_quantity)
                         {
                             int max = -99999;
                             for (j = 0; j < cache->blocks_quantity; j++)
                             {
                                 if (cache->sets[set]->blocks[j]->frecuency > max)
                                 {
                                     max = cache->sets[set]->blocks[j]->frecuency;
                                     cache->sets[set]->LRUBlock = j;
                                 }
                             }

                             int LRUBlock = cache->sets[set]->LRUBlock;
                             for (size_t k = 0; k < cache->words_quantity; k++)
                             {
                                 if (k < word)
                                 {
                                     cache->sets[set]->blocks[LRUBlock]->words[k] = atoi(data[i]) - word + k;
                                 }

                                 else if(k > word)
                                 {
                                     cache->sets[set]->blocks[LRUBlock]->words[k] = atoi(data[i]) + word + k;
                                 }

                                 else if(k == word)
                                 {
                                     cache->sets[set]->blocks[LRUBlock]->frecuency = 0;
                                     cache->sets[set]->blocks[LRUBlock]->words[k] = atoi(data[i]);
                                 }
                            }
                        }
                    }
                    break;
            }
            break;
    }
}


//Función encargada de generar un archivo con los hazards del programa
//Entradas: ExMem Buffer, MeMWB Buffer, Ciclos del programa y validador created_2.
//Salidas: Ninguna.
void writeFile1(cache_struct * cache, bool * created_2)
{
    if (*created_2 == false)
    {
        FILE *fp;
        fp=fopen("File 1.txt", "w");

        if(fp == NULL)
        {
            printf("Error al crear el archivo %s\n","File 1.txt");
            exit(1);
        }

        fprintf(fp, "Cache %d-asociativo \n\n", cache->blocks_quantity);

        switch (cache->replaceType)
        {
            case 0:
                fprintf(fp, "FIFO \n\n");
                break;

            case 1:
                fprintf(fp, "MRU \n\n");
                break;

            case 2:
                fprintf(fp, "LRU \n\n");
                break;
        }

        fprintf(fp, "Cantidad Hit: %d\n", cache->hits);
        fprintf(fp, "Cantidad Miss: %d \n\n", cache->miss);

        fprintf(fp, "\n");
        *created_2 = true;
        fclose(fp);
    }

    else
    {
        FILE *fp;
        fp=fopen("File 1.txt", "a");

        if(fp == NULL)
        {
            printf("Error al crear el archivo %s\n","File 1.txt");
            exit(1);
        }

        fprintf(fp, "Cache %d-asociativo \n\n", cache->blocks_quantity);

        switch (cache->replaceType)
        {
            case 0:
                fprintf(fp, "FIFO \n\n");
                break;

            case 1:
                fprintf(fp, "MRU \n\n");
                break;

            case 2:
                fprintf(fp, "LRU \n\n");
                break;
        }

        fprintf(fp, "Cantidad Hit: %d\n", cache->hits);
        fprintf(fp, "Cantidad Miss: %d \n\n", cache->miss);

        fprintf(fp, "\n");
        fclose(fp);

    }

}

// {
// void writeFile1(cache_struct * cache)
//
// }

void selectBetterCaches(allCaches_struct * allCaches, int dataQuantity)
{

    bool created = false;
    int maxHitRatio = -99999;
    int hitRatio = 0;
    cache_struct * cache = malloc(sizeof(cache_struct));

    allCaches_struct * bestCaches = malloc(sizeof(allCaches_struct));
    bestCaches->caches_quantity = allCaches->caches_quantity;

    bestCaches->caches = malloc(sizeof(cache_struct)*(bestCaches->caches_quantity));
    {

    for (size_t i = 0; i < allCaches->caches_quantity; i++)
        hitRatio = (allCaches->caches[i]->hits)/ dataQuantity;

        if (hitRatio > maxHitRatio )
        {
            maxHitRatio = hitRatio;
            //cache = allCaches->caches[i];
        }
    }

    for (size_t i = 0; i < allCaches->caches_quantity; i++)
    {
        hitRatio = (allCaches->caches[i]->hits)/ dataQuantity;

        if (hitRatio == maxHitRatio )
        {
            writeFile1(allCaches->caches[i], &created);
            //writeFile2(allCaches->caches[i], &created);
            //cache = allCaches->caches[i];
        }

    }
}

allCaches_struct * allCaches(int cache_size,int words_in_block)
{
    int total_blocks_quantity = (cache_size/8)/(4*words_in_block);
    int n = 0;
    int sets_quantity = 0;
    int blocks_quantity = 0;
    int words_quantity = words_in_block;

    while(sets_quantity != total_blocks_quantity)
    {
        sets_quantity = pow(2,n);
        n++;
    }

    allCaches_struct * allCaches = malloc(sizeof(allCaches_struct));
    //Dado que la cantidad de caches serán por el tipo de política de remplazo y los n-asociativos
    //Pero sólo existe FIFO en directo, por lo que la cantidad será 1 + (n-1)*3.
    allCaches->caches = malloc(sizeof(cache_struct*)*(1 + (n-1)*3));
    allCaches->caches_quantity = 1 + (n-1)*3;

    int j = 0;

    for (size_t i = 0; i < allCaches->caches_quantity ; i++)
    {
        int replaceType = (i)%3;

        sets_quantity = pow(2,j);
        blocks_quantity = (total_blocks_quantity/sets_quantity);
        allCaches->caches[i] = build_cache(words_quantity,blocks_quantity,sets_quantity,replaceType);
        //ck: %d\n",sets_quantity,blocks_quantity, words_quantity );
        //showCache(allCaches->caches[i]);

        if (i%3 == 2)
        {
            j++;
        }
    }

    return allCaches;
}

int main(int argc, char** argv)
{
    int opt;
    char* fp_source_name;
    char** data;
    int cache_size;
    int words_in_block;

    while ((opt = getopt(argc, argv, "n:m:p:")) != -1)
    {
        switch (opt)
        {
            case 'n':
                fp_source_name = malloc(sizeof(char)*20);
                strcpy(fp_source_name, optarg);
                break;
            case 'm':
                cache_size = atoi(optarg);
                break;
            case 'p':
                words_in_block = atoi(optarg);
                break;
            default: exit(1);
        }
    }

    data = readData(fp_source_name);

    allCaches_struct * AllCaches = malloc(sizeof(allCaches_struct));
    AllCaches = allCaches(cache_size,words_in_block);

    for (size_t i = 0; i < AllCaches->caches_quantity; i++)
    {
        populateCache(AllCaches->caches[i], data);
    }

    cache_struct * bestCache = malloc(sizeof(cache_struct));
    bestCache = selectBetterCache(AllCaches, atoi(data[0]));
    showCache(bestCache);
    printf("Hits: %d, Miss: %d\n",bestCache->hits, bestCache->miss);

    for (size_t i = 0; i < AllCaches->caches_quantity; i++)
    {
        printf("\n Hits: %d, Miss: %d\n",AllCaches->caches[i]->hits, AllCaches->caches[i]->miss);
        printf("%d - Asociativo con tipo de remplazo: %d \n",AllCaches->caches[i]->blocks_quantity, AllCaches->caches[i]->replaceType);
    }
    //printf("%d\n",  cache->sets[0]->blocks[0]->words[0]);
    //printf("bloque = %d\n", ((20/4*words_in_block)) % (64));

}
