#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include <unistd.h>
#include "functions.h"

int main(int argc, char** argv)
{
    int opt;
    char* fp_source_name;
    char** data;
    int cache_size;
    int words_in_block;

    printf("\n\n ///////////////// Selector de mejores cachés V 1.0.0 ///////////////////// \n\n");

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

    printf("%s\n","Creando archivos de salidad output_1 y output_2, que contienen el/los mejores cachés según tasa de hits" );
    printf("%s\n","." );
    printf("%s\n","." );
    printf("%s\n","." );
    printf("%s\n","." );
    selectBetterCaches(AllCaches, atoi(data[0]));
    printf("%s\n","Archivos de salidad output_1 y output_2 creados" );


}
