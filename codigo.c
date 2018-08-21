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
            for (size_t k = 0; k < cache->words_quantity; k++)
            {
                printf("Palabra %ld: Mem(%d)\n",k,cache->sets[i]->blocks[j]->words[k]);
            }
        }
    }
}

cache_struct * build_cache(int words_quantity ,int blocks_quantity, int sets_quantity)
{
    cache_struct * cache = malloc(sizeof(cache_struct));
    cache->sets = malloc(sizeof(set_struct*)*sets_quantity);
    cache->sets_quantity = sets_quantity;
    cache->blocks_quantity = blocks_quantity;
    cache->words_quantity = words_quantity;

    for (size_t i = 0; i < sets_quantity; i++)
    {
        cache->sets[i] = malloc(sizeof(set_struct));
        cache->sets[i]->blocks = malloc(sizeof(block_struct*)*blocks_quantity);

        for (size_t j = 0; j < blocks_quantity; j++)
        {
            cache->sets[i]->blocks[j] = malloc(sizeof(block_struct));
            cache->sets[i]->blocks[j]->words = malloc(sizeof(int)*words_quantity);

            for (size_t k = 0; k < words_quantity; k++)
            {
                cache->sets[i]->blocks[j]->words[k] = -1;
            }
        }
    }

    return cache;
}

void populateCache(cache_struct * cache, char * fp_source_name)
{
    char** data;
    int option;
    int block;
    int set;
    int block;
    data = readData(fp_source_name);

    //Si es directo
    if (cache->sets_quantity == 1)
    {
        option = 1;
    }

    //Si es full asociativo
    else if(cache->blocks_quantity == 1)
    {
        option = 2;
    }

    //Si es n-asociativo
    else
    {
        option = 3;
    }

    switch (option) {
        case 1:
                printf("Sets Quantity: %d , blocks_quantity: %d, words_quantity: %d\n", cache->sets_quantity,cache->blocks_quantity, cache->words_quantity );
                for (size_t i = 1; i <= atoi(data[0]); i++) {
                    set = ((atoi(data[i])/(4*cache->words_quantity)) % cache->blocks_quantity);
                    block = ((atoi(data[i])/(4*cache->words_quantity)) % cache->blocks_quantity);

                    if (cache->words_quantity > 1)
                    {
                            for (size_t i = 0; i < caches->words_quantity; i++)
                            {
                                word = block%(cache->words_quantity); 
                                cache->sets[block]->blocks[block]->words[word];
                            }
                    }

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
    allCaches->caches = malloc(sizeof(cache_struct*)*n);
    allCaches->caches_quantity = n;

    for (size_t i = 0; i < n ; i++)
    {
        sets_quantity = pow(2,i);
        blocks_quantity = (total_blocks_quantity/sets_quantity);
        allCaches->caches[i] = build_cache(words_quantity,blocks_quantity,sets_quantity);
        printf("Sets %d, Blocks por set: %d, Words por block: %d\n",sets_quantity,blocks_quantity, words_quantity );
    }

    // for (size_t i = 0; i < allCaches->caches_quantity; i++)
    // {
    //     printf("\n///////////////////////////////////////////////////////////////////////////\n");
    //     showCache(allCaches->caches[i]);
    //     printf("\n///////////////////////////////////////////////////////////////////////////\n");
    // }

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

    allCaches_struct * AllCaches = malloc(sizeof(allCaches_struct));
    AllCaches = allCaches(cache_size,words_in_block);

    for (size_t i = 0; i < AllCaches->caches_quantity; i++)
    {
        populateCache(AllCaches->caches[i], fp_source_name);
    }
    //printf("%d\n",  cache->sets[0]->blocks[0]->words[0]);
    printf("bloque = %d\n", ((20/4*words_in_block)) % (64));

}
