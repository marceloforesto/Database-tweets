#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tweet.h"
#include "index.h"

void remove_new_line(char *line) {
    size_t ln = strlen(line) - 1;
    int i;
    for (i=0; i<ln; i++) {
		if (line[ln] == '\n') {
			line[ln] = '\0';
		}
    }
}

void new_tweet(char *filename){
    FILE *datafile;
	int pos_ideal = -1, tam_ideal = 1000;
	int *del_num, size_trash = 1,i;
	SizeList *list = malloc(sizeof(SizeList) * number_of_registers());
	datafile = fopen(filename, "a+");
	Tweet new_tweet;

	char *delimitador = (char*) malloc (sizeof(char));
	strcpy(delimitador,"|");

	new_tweet.text = (char*) malloc (sizeof(char)*BUFFER);
	new_tweet.user = (char*) malloc (sizeof(char)*BUFFER);
	new_tweet.coordinates = (char*) malloc (sizeof(char)*BUFFER);
	new_tweet.language = (char*) malloc (sizeof(char)*BUFFER);

	int register_size = 4; // ja conta inicialmente com o tamanho dos delimitadores
	printf ("insert text: ");
	fgets (new_tweet.text, BUFFER, stdin);
	remove_new_line (new_tweet.text);

	printf ("insert user: ");
	fgets (new_tweet.user, BUFFER, stdin);
	remove_new_line (new_tweet.user);

	printf ("insert coordinates: ");
	fgets (new_tweet.coordinates, BUFFER, stdin);
	remove_new_line (new_tweet.coordinates);

	printf ("insert language: ");
	fgets (new_tweet.language, BUFFER, stdin);
	remove_new_line (new_tweet.language);

	printf ("insert favorite count: ");
	scanf ("%d", &new_tweet.favorite_count);

	printf ("insert retweet count: ");
	scanf ("%d", &new_tweet.retweet_count);

	printf ("insert view count: ");
	scanf ("%d", &new_tweet.views_count);

    register_size += strlen(new_tweet.text)+strlen(new_tweet.user)+strlen(new_tweet.coordinates)+strlen(new_tweet.language)+3*sizeof(int);
    //estrategia best fit
   /* list = recover_size_list(filename, del_num);
    for(i=0; i < *del_num; i++){
        if(list[i].tamanho == register_size){
            pos_ideal = list[i].byteOffset;
            break;
        }
        else if(list[i].tamanho >= (register_size + sizeof(char) + sizeof(int))){
            if(list[i].tamanho < tam_ideal){
                tam_ideal = list[i].tamanho;
                pos_ideal = list[i].byteOffset;
            }
        }
    }
    if(pos_ideal != -1){
        fseek(datafile, pos_ideal, SEEK_SET);
        if(i == *del_num){
            fwrite(&size_trash, sizeof(int), 1, datafile);
            fwrite("*", sizeof(char), 1, datafile);
        }
    }*/
    fwrite(&register_size,sizeof(int),1,datafile);
    fwrite(new_tweet.text,sizeof(char), strlen(new_tweet.text), datafile);
    fwrite(delimitador,sizeof(char),strlen(delimitador),datafile);
    fwrite(new_tweet.user,sizeof(char), strlen(new_tweet.user), datafile);
    fwrite(delimitador,sizeof(char),strlen(delimitador),datafile);
    fwrite(new_tweet.coordinates,sizeof(char), strlen(new_tweet.coordinates), datafile);
    fwrite(delimitador,sizeof(char),strlen(delimitador),datafile);
    fwrite(new_tweet.language,sizeof(char), strlen(new_tweet.language), datafile);
    fwrite(delimitador,sizeof(char),strlen(delimitador),datafile);
    fwrite(&new_tweet.favorite_count,sizeof(int), 1, datafile);
    fwrite(&new_tweet.retweet_count,sizeof(int), 1, datafile);
    fwrite(&new_tweet.views_count,sizeof(int), 1, datafile);

    printf ("\nNEW TWEET POSTED!\n");

    fclose(datafile);
}

void list_tweets (char *filename) {
    FILE *file;
	//tweet *tweet = malloc (sizeof(tweet));
	char *token;
	int tamanho = 0,valor;
	size_t nread;

	file = fopen (filename, "r+");

	if (file) {
		while ((nread = fread(&tamanho, 1, sizeof(int), file)) > 0) {
            char* lido = malloc(sizeof(char)*tamanho);
            int imprimir = 1;
            fread(lido,1,tamanho-12,file);	//le só os campos variaveis (-12 = 3 cmps fixos)
            token = strtok(lido, "|");
            if(token[0] == '*')
                imprimir = 0;
            if (imprimir)
                printf ("text: %s\n", token);

            token = strtok(NULL, "|");
            if (imprimir)
                printf ("user: %s\n", token);
            token = strtok(NULL, "|");
            if (imprimir)
                printf ("coordinates: %s\n", token);
            token = strtok(NULL, "|");
            if (imprimir)
                printf ("language: %s\n", token);
            fread(&valor, 1, sizeof(int), file);
            if (imprimir)
                printf ("favorite count: %d\n", valor);
            fread(&valor, 1, sizeof(int), file);
            if (imprimir)
                printf ("retweet count: %d\n", valor);
            fread(&valor, 1, sizeof(int), file);
            if (imprimir){
                printf ("views count: %d\n", valor);
                printf("\n");
            }
		}
	}
	fclose(file);
}

void search_user (char *filename) {
    FILE *file;
    char *token;
    char *user = malloc(sizeof(char)*BUFFER);
	int tamanho = 0,valor,imprime = 0;
	size_t nread;
    printf("Type user: ");
    fgets (user, BUFFER, stdin);
    remove_new_line(user);
    file = fopen (filename, "r+");
    char* texto = malloc(sizeof(char));
	if (file) {
		while ((nread = fread(&tamanho, 1, sizeof(int), file)) > 0) {
            char* lido = malloc(sizeof(char)*tamanho);
            fread(lido,1,tamanho-12,file);	//le só os campos variaveis (-12 = 3 cmps fixos)
            token = strtok(lido, "|");
            strcpy(texto,token);
            token = strtok(NULL, "|");
            if(strcmp(user,token) == 0)
                imprime = 1;
            if(texto[0] == '*')
                imprime = 0;
            if(imprime){
                printf ("text: %s\n", texto);
                printf ("user: %s\n", token);
            }
            token = strtok(NULL, "|");
            if(imprime)
                printf ("coordinates: %s\n", token);
            token = strtok(NULL, "|");
            if(imprime)
                printf ("language: %s\n", token);
            fread(&valor, 1, sizeof(int), file);
            if(imprime)
                printf ("favorite count: %d\n", valor);
            fread(&valor, 1, sizeof(int), file);
            if(imprime)
                printf ("retweet count: %d\n", valor);
            fread(&valor, 1, sizeof(int), file);
            if(imprime){
                printf ("views count: %d\n", valor);
                printf("\n");
            }
            imprime = 0;
		}
	}
	fclose(file);
}


/*
 * Permita a recuperação dos dados de um ou mais registros com base nos valores dos
campos FAVORITE_COUNT E LANGUAGE simultaneamente
 */
void fav_and_language (char *filename) {
	int fav_count;
	char *language = malloc(sizeof(char)*25);

	printf ("insert language: ");
	fgets (language, 25, stdin);
	remove_new_line(language);
	printf ("insert favorite count: ");
	scanf ("%d", &fav_count);

	index_matching(filename,fav_count,language);
}

/*
 * Permita a recuperação dos dados de um ou mais registros com base nos valores dos
campos FAVORITE_COUNT OU LANGUAGE simultaneamente
 */
void fav_or_language (char *filename) {
	int fav_count;
	char *language = malloc(sizeof(char)*25);

    printf ("insert language: ");
	fgets (language, 25, stdin);
	remove_new_line(language);
	printf ("insert favorite count: ");
	scanf ("%d", &fav_count);


	index_merging(filename,fav_count,language);
}


void delete_favorite (char *filename) {
	int delete_value, datasize, delete_index;
	int *deletion_offset;
	int i, count = 0;
	FavoriteIndex *fav_idx;
	InvertedList *fav_list;

	datasize = REGISTER_SIZE;
	deletion_offset = malloc(sizeof(int) * datasize);
	fav_idx = malloc(sizeof(FavoriteIndex) * datasize);
	fav_list = malloc(sizeof(InvertedList) * datasize);

	printf ("insert favorite count for deletion: ");
	scanf ("%d", &delete_value);

	fav_idx = recover_fav_index();
    fav_list = recover_fav_list();
    int qntRegistros = number_of_registers();
    if(qntRegistros<TAMANHO_MINIMO){
        printf("Not enough data to delete by favorite");
        return;
    }
    delete_index = search_index_favorite(fav_idx, datasize, delete_value);
    if(delete_index > -1){
        delete_index = fav_idx[delete_index].RRN;
        while(fav_list[delete_index].proximo != -1){
            deletion_offset[count] = fav_list[delete_index].byteOffSet;
            delete_index = fav_list[delete_index].proximo;
            count++;
        }
        deletion_offset[count] = fav_list[delete_index].byteOffSet;

        //imprime as opções de tweets(se tiver)
       // datafile = fopen(filename, "r+");
        for(i=0; i <= count; i++){
            printf("[tweet %d]\n", i);
            print_register_todelete(filename, deletion_offset[i]);
        }
        printf("which tweet would you like to delete?");
        scanf("%d", &delete_index);
        delete_offset(filename,deletion_offset[delete_index]);
        index_manager(filename);
        diminuiArqTamanho();
        printf("\n Registro Excluido\n");
    }
    else
        printf("no tweets found with this favorite count.\n");
}

void delete_offset (char *filename, int offset) {
    FILE *datafile;

    datafile = fopen(filename, "r+");
    if(datafile){
        fseek(datafile, offset+sizeof(int), SEEK_SET);
        //fread(&aux, 1, sizeof(int), datafile);
        fwrite("*", sizeof(char), 1, datafile);
    }
    fclose(datafile);
}

void delete_rrn(char *filename, int rrn){
    FILE *datafile;
    int i, size;
    int *str;

    datafile = fopen(filename, "a+");
    if(datafile){
        for(i=0; i < rrn; i++){
            fread(&size, 1, sizeof(int), datafile);
            fread(str, 1, size, datafile);
        }
        fread(&size, 1, sizeof(int), datafile);
        fwrite("*", sizeof(char), 1, datafile);
    }
    fclose(datafile);
}


void printTweet(Tweet *t){
    printf("\n");
    printf ("text: %s\n", t->text);
    printf ("user: %s\n", t->user);
    printf ("coordinates: %s\n", t->coordinates);
    printf ("language: %s\n", t->language);
    printf ("favorite count: %d\n", t->favorite_count);
    printf ("retweet count: %d\n", t->retweet_count);
    printf ("views count: %d\n", t->views_count);
}

SizeList* recover_size_list(char *filename, int *del_num){
    int i = 0, offset = 0, size;
    size_t nread;

    int reg_num = number_of_registers();
    SizeList *list = malloc(sizeof(SizeList) * reg_num);
    FILE *datafile = fopen(filename, "r+");

    *del_num = 0;
    if(datafile){
        while((nread = fread(&size, 1, sizeof(int), datafile)) > 0){
            char *reg = malloc(sizeof(char)*size);
            fread(reg, 1, size, datafile);
            if(reg[0] == '*'){
                list[i].byteOffset = offset;
                list[i].tamanho = size;
                i++;
                *del_num = *del_num+1;
            }
            offset += size + sizeof(int);
            free(reg);
        }
    }
    return list;
}
