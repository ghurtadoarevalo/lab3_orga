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
	int count = 0;
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
    //printf("%s\n",dataFileChar );

	//Se separan las palabras por \n
	temp = malloc(sizeof(char)*(size+1));
	strcpy(temp,dataFileChar);
	token = strtok(temp, "\n");
    //printf("%s\n",token );
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

    return lines;
}

void allCaches(int cache_size,int words_in_block)
{
    int total_blocks_quantity = (cache_size/8)/(4*words_in_block);
    int n = 0;
    int sets_quantity = 0;
    int blocks_quantity = 0;
    int words_quantity = words_in_block;

    while(sets_quantity != total_blocks_quantity)
    {
        sets_quantity = pow(2,n);
        blocks_quantity = (total_blocks_quantity/sets_quantity);

        printf("Sets %d, Blocks por set: %d, Words por block: %d\n",sets_quantity,blocks_quantity, words_quantity );
        n++;
    }
    //cache_struct ** caches = malloc(sizeof(cache_struct*)*)
}

cache_struct * build_cache(int words_quantity ,int blocks_quantity, int sets_quantity)
{
    cache_struct * cache = malloc(sizeof(cache_struct));
    cache->sets = malloc(sizeof(set_struct*)*sets_quantity);
    cache->sets_quantity = sets_quantity;
    for (size_t i = 0; i < sets_quantity; i++)
    {
        cache->sets[i] = malloc(sizeof(set_struct));
        cache->sets[i]->blocks = malloc(sizeof(block_struct*)*blocks_quantity);
        cache->sets[i]->blocks_quantity = blocks_quantity;

        for (size_t j = 0; j < blocks_quantity; j++)
        {
            cache->sets[i]->blocks[j] = malloc(sizeof(block_struct));
            cache->sets[i]->blocks[j]->words = malloc(sizeof(int)*words_quantity);
        }
    }

    return cache;
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
    data =  readData(fp_source_name);
    //printf("%d bytes\n",4096/8);
    printf("bloque = %d\n", ((20/4)*4) % (8));
    //printf("%s\n",data[1] );
    //allCaches(cache_size,words_in_block);
    int words_quantity = 2;
    int blocks_quantity = 64;
    int sets_quantity = 1;
    cache_struct * cache = build_cache(words_quantity ,blocks_quantity, sets_quantity);

    cache->sets[0]->blocks[0]->words[0] = 10;
    //printf("%d\n",  cache->sets[0]->blocks[0]->words[0] );

}
