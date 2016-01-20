#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tweet.h"
#include "index.h"

#define BUFFER 140

//retorna os rrns dos registros em interseccao nos index
void index_matching (char *filename,int fav_count, char *language) {

    FavoriteIndex *fav_ind = recover_fav_index();
    LanguageIndex *lng_ind = recover_language_index();
    InvertedList *lista_fav = recover_fav_list();
    InvertedList *lista_lng = recover_language_list();
    int i = 0,j,posFav = 0,posLang = 0;;
    Tweet **favTweets = malloc(sizeof(Tweet)*REGISTER_SIZE);
    Tweet **langTweets = malloc(sizeof(Tweet)*REGISTER_SIZE);
    if(number_of_registers()<=TAMANHO_MINIMO){
        printf("\nExistem poucos registros para busca.\n");
        return;
    }
    while(fav_ind != NULL && i< REGISTER_SIZE){
        if(fav_ind[i].favorite == fav_count){
            int rrn = fav_ind[i].RRN;
            while(rrn != -1){
                Tweet *t = retornaTweet(filename,lista_fav[rrn].byteOffSet);
                if (t != NULL){
                    favTweets[posFav] = t;
                    posFav++;
                }
                rrn = lista_fav[rrn].proximo;

            }
            break;
        }
        i++;
    }
    i = 0;
    while(lng_ind != NULL && i< REGISTER_SIZE){
        if(lng_ind[i].language == NULL){
        }
        else if(strcmp(lng_ind[i].language,language) == 0){
            int rrn = lng_ind[i].RRN;
            while(rrn != -1){
                Tweet *t = retornaTweet(filename,lista_lng[rrn].byteOffSet);
                if(t != NULL){
                    langTweets[posLang] = t;
                    posLang++;
                }
                rrn = lista_lng[rrn].proximo;
            }
            break;
        }
        i++;
    }
    ordenaListTweetPorFavorito(favTweets,posFav);
    ordenaListTweetPorFavorito(langTweets,posLang);
    i = 0;
    j = 0;

    if(posFav == 0 && posLang == 0){
        printf("Busca nao retornou nada.");
        return;
    }
    //MATCHING
    while(i<posFav && j < posLang){
        if(favTweets[i]->favorite_count<langTweets[j]->favorite_count){
            i++;
        }
        else if(favTweets[i]->favorite_count>langTweets[j]->favorite_count){
            j++;
        }
        else if(favTweets[i]->favorite_count == langTweets[j]->favorite_count){
            if((strcmp(favTweets[i]->language,language) == 0) && (strcmp(langTweets[j]->language,language) == 0) &&(favTweets[i]->favorite_count == fav_count)){
                if((strcmp(favTweets[i]->text,langTweets[j]->text) == 0) && (strcmp(favTweets[i]->coordinates,langTweets[j]->coordinates) == 0)){
                    printTweet(favTweets[i]);
                    i++;
                    j++;
                }
                else{
                    printTweet(favTweets[i]);
                    printTweet(langTweets[j]);
                    i++;
                    j++;
                }
            }
            else {
                if(strcmp(favTweets[i]->language,language) != 0 && (favTweets[i]->favorite_count == fav_count))
                    i++;
                if(strcmp(langTweets[j]->language,language) != 0 && (langTweets[j]->favorite_count == fav_count))
                    j++;
            }
        }
    }
    while(i<posFav){
        if(!jaPrintouTweet(favTweets[i],langTweets,posLang))
            if(favTweets[i]->favorite_count == fav_count)
                if((strcmp(favTweets[i]->language,language) == 0))
                    printTweet(favTweets[i]);
        i++;
    }
    while(j < posLang){
        if(!jaPrintouTweet(langTweets[j],favTweets,posFav))
            if(langTweets[j]->favorite_count == fav_count)
                if((strcmp(langTweets[j]->language,language) == 0))
                    printTweet(langTweets[j]);
        j++;
    }
    printf("\n");
}

//retorna os rrns dos registros em combinacao nos index
void index_merging(char *filename,int fav_count, char *language) {

    FavoriteIndex *fav_ind = recover_fav_index();
    LanguageIndex *lng_ind = recover_language_index();
    InvertedList *lista_fav = recover_fav_list();
    InvertedList *lista_lng = recover_language_list();

    int i = 0,j,posFav = 0,posLang = 0;;
    Tweet **favTweets = malloc(sizeof(Tweet)*REGISTER_SIZE);
    Tweet **langTweets = malloc(sizeof(Tweet)*REGISTER_SIZE);
    if(number_of_registers()<=TAMANHO_MINIMO){
        printf("\nExistem poucos registros para busca.\n");
        return;
    }
    while(fav_ind != NULL && i< REGISTER_SIZE){
        if(fav_ind[i].favorite == fav_count){
            int rrn = fav_ind[i].RRN;
            while(rrn != -1){
                Tweet *t = retornaTweet(filename,lista_fav[rrn].byteOffSet);
                if(t != NULL){
                    favTweets[posFav] = t;
                    posFav++;
                }
                rrn = lista_fav[rrn].proximo;
            }
            break;
        }
        i++;
    }
    i = 0;
    while(lng_ind != NULL && i< REGISTER_SIZE){
        if(lng_ind[i].language == NULL){
        }
        else if(strcmp(lng_ind[i].language,language) == 0){
            int rrn = lng_ind[i].RRN;
            while(rrn != -1){
                Tweet *t = retornaTweet(filename,lista_lng[rrn].byteOffSet);
                if(t != NULL){
                    langTweets[posLang] = t;
                    posLang++;
                }
                rrn = lista_lng[rrn].proximo;
            }
            break;
        }
        i++;
    }
    ordenaListTweetPorFavorito(favTweets,posFav);
    ordenaListTweetPorFavorito(langTweets,posLang);
    i = 0;
    j = 0;

    if(posFav == 0 && posLang == 0){
        printf("Busca nao retornou nada.");
        return;
    }
    //MERGING
    while(i<posFav && j < posLang){
        if(favTweets[i]->favorite_count<langTweets[j]->favorite_count){
            printTweet(favTweets[i]);
            i++;
        }
        else if(favTweets[i]->favorite_count>langTweets[j]->favorite_count){
            printTweet(langTweets[j]);
            j++;
        }
        else if(favTweets[i]->favorite_count == langTweets[j]->favorite_count){
            if((strcmp(favTweets[i]->text,langTweets[j]->text) == 0) && (strcmp(favTweets[i]->coordinates,langTweets[j]->coordinates) == 0))
                 printTweet(favTweets[i]);
            else{
                printTweet(favTweets[i]);
                printTweet(langTweets[j]);
            }
            i++;
            j++;
        }
    }
    while(i<posFav){
        if(!jaPrintouTweet(favTweets[i],langTweets,posLang))
            printTweet(favTweets[i]);
        i++;
    }
    while(j < posLang){
        if(!jaPrintouTweet(langTweets[j],favTweets,posFav))
            printTweet(langTweets[j]);
        j++;
    }
    printf("\n");
}

int jaPrintouTweet(Tweet *t, Tweet **tweets,int tamanho){
    int i;
    for(i=0;i<tamanho;i++){
        if(t->favorite_count == tweets[i]->favorite_count)
            if((strcmp(t->text,tweets[i]->text) == 0) && (strcmp(t->coordinates,tweets[i]->coordinates) == 0))
                return 1;
    }
    return 0;
}

int number_of_registers () {
	int num_of_registers = 0;
	FILE *sizefile;

	sizefile = fopen (SIZEFILENAME, "r+");
	size_t nread;
	if (sizefile != NULL) {
		if ((nread = fread(&num_of_registers, 1, sizeof(int), sizefile)) > 0) {
			return num_of_registers;
		}
	}
	return 0;
}

// atualiza o numero de registros, verifica se é hora de criar indice (>10)
//chama o update a cada insercao ou remocao
void index_manager(char *filename) {

	int num_of_registers = 0;
    FavoriteIndex *fav_ind = malloc(sizeof(FavoriteIndex)* REGISTER_SIZE);
    LanguageIndex *lng_ind = malloc(sizeof(LanguageIndex)* REGISTER_SIZE);
    InvertedList *lista_fav = malloc(sizeof(InvertedList) * REGISTER_SIZE);
    InvertedList *lista_lng = malloc(sizeof(InvertedList) * REGISTER_SIZE);

    num_of_registers = number_of_registers();
	if (num_of_registers >= TAMANHO_MINIMO) {
		create_favorite_index(filename,fav_ind,lista_fav);
		create_language_index(filename, lng_ind, lista_lng);
	}
}

int search_index_language(LanguageIndex *ind,int tamanho,char *valor){
	int i;
	for(i = 0;i<tamanho;i++){
		if(strcmp(ind[i].language,valor)==0)
			return i;
	}
	return -1;
}

void aumentaArqTamanho(){
    FILE *sizefile;
    int num_of_registers = 0;
    size_t nread;
    sizefile = fopen ("sizefile.dat", "r+");	//guarda a quantidade de registros
	if (sizefile == NULL) {
		//arquivo nao existia
		sizefile = fopen ("sizefile.dat", "w+");
	}

	fseek(sizefile, 0, SEEK_SET);
	if ((nread = fread(&num_of_registers, 1, sizeof(int), sizefile)) > 0) {
		//le o tamanho e soma 1
		num_of_registers++;
		fseek(sizefile, 0, SEEK_SET);
		fwrite (&num_of_registers, sizeof(int), 1, sizefile);
	} else {
		//primeira insercao, coloca 1 como tamanho
		num_of_registers++;
		fwrite (&num_of_registers, sizeof(int), 1, sizefile);
	}
	fclose(sizefile);
}
void diminuiArqTamanho(){
    FILE *sizefile;
    int num_of_registers = 0;
    size_t nread;
    sizefile = fopen ("sizefile.dat", "r+");	//guarda a quantidade de registros
	if (sizefile == NULL) {
		//arquivo nao existia
		sizefile = fopen ("sizefile.dat", "w+");
	}

	fseek(sizefile, 0, SEEK_SET);
	if ((nread = fread(&num_of_registers, 1, sizeof(int), sizefile)) > 0) {
		//le o tamanho e soma 1
		num_of_registers--;
		fseek(sizefile, 0, SEEK_SET);
		fwrite (&num_of_registers, sizeof(int), 1, sizefile);
	} else {
		//primeira insercao, coloca 1 como tamanho
		num_of_registers++;
		fwrite (&num_of_registers, sizeof(int), 1, sizefile);
	}
	fclose(sizefile);
}


void print_register (char *filename, int offset) {
	FILE *datafile;
	char *token;
	int tamanho = 0,valor;
	size_t nread;
	if ((datafile = fopen (filename, "r+")) != NULL) {
		fseek(datafile, offset, SEEK_SET);

		if ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
			char* lido = malloc(sizeof(char)*tamanho);
			int imprimir = 1;
            fread(lido,1,tamanho-12,datafile);	//le só os campos variaveis (-12 = 3 cmps fixos)
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
            fread(&valor, 1, sizeof(int), datafile);
            if (imprimir)
                printf ("favorite count: %d\n", valor);
            fread(&valor, 1, sizeof(int), datafile);
            if (imprimir)
                printf ("retweet count: %d\n", valor);
            fread(&valor, 1, sizeof(int), datafile);
            if (imprimir){
                printf ("views count: %d\n", valor);
                printf("\n");
            }
		}
	}
	fclose(datafile);
}
Tweet* retornaTweet(char *filename, int offset){
	FILE *datafile;
	char *token;
	int tamanho = 0,valor;
	size_t nread;
	Tweet *retorno = malloc(sizeof(Tweet));
	retorno->text = malloc(sizeof(char)*BUFFER);
	retorno->user = malloc(sizeof(char)*BUFFER);
	retorno->coordinates = malloc(sizeof(char)*BUFFER);
	retorno->language = malloc(sizeof(char)*BUFFER);

	if ((datafile = fopen (filename, "r+")) != NULL) {
		fseek(datafile, offset, SEEK_SET);

		if ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
			char* lido = malloc(sizeof(char)*tamanho);
			fread(lido,1,tamanho-12,datafile);	//le só os campos variaveis (-12 = 3 cmps fixos)
			token = strtok(lido, "|");
			if(token[0] == '*')
                return NULL;
            strcpy(retorno->text,token);

			token = strtok(NULL, "|");
			strcpy(retorno->user,token);

			token = strtok(NULL, "|");
			strcpy(retorno->coordinates,token);

			token = strtok(NULL, "|");;
			strcpy(retorno->language,token);

			fread(&valor, 1, sizeof(int), datafile);
			retorno->favorite_count = valor;
			fread(&valor, 1, sizeof(int), datafile);
			retorno->retweet_count = valor;
			fread(&valor, 1, sizeof(int), datafile);
			retorno->views_count = valor;
			return retorno;
		}
	}
	fclose(datafile);
	return NULL;
}


void search_language (char *filename) {

	char* search_value = (char*) malloc (sizeof(char)*BUFFER);

	printf ("insert language to be searched: ");
	fgets (search_value, BUFFER, stdin);
	remove_new_line(search_value);

	int i;
	int atual;
	int achou = 0;

	int num_of_registers = number_of_registers();
	if (num_of_registers > TAMANHO_MINIMO) {
		//recupera os dados de indice e lista invertida de seus respectivos arquivos
		InvertedList *lng_list = recover_language_list();
		LanguageIndex *lng_idx = recover_language_index();
		for (i=0; i<REGISTER_SIZE; i++) {
			if (lng_idx[i].language != NULL && (strcmp(lng_idx[i].language, search_value)) == 0) {
				atual = lng_idx[i].RRN;
				print_register(filename, lng_list[lng_idx[i].RRN].byteOffSet);
				while (lng_list[atual].proximo != -1) {
					atual = lng_list[atual].proximo;
					print_register(filename, lng_list[atual].byteOffSet);
				}
				achou = 1;
				break;
			}
		}
		if(!achou)
            printf("No results");
	} else {
		//busca sequencialmente pelo arquivo de dados
		printf ("sequencial search through datafile\n");
		FILE *datafile = fopen (filename, "r+");
		size_t nread;
		Tweet new_tweet;
		new_tweet.text = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.user = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.coordinates = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.language = (char*) malloc (sizeof(char)*BUFFER);
		int tamanho = 0;

		if (datafile != NULL) {
			while ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
				char* lido = malloc(sizeof(char)*tamanho);
				fread(lido,1,tamanho-12,datafile);	//le só os campos variaveis (-12 = 3 cmps fixos)

				new_tweet.text = strtok(lido, "|");
				new_tweet.user = strtok(NULL, "|");
				new_tweet.coordinates = strtok(NULL, "|");
				new_tweet.language = strtok(NULL, "|");;

				fread(&new_tweet.favorite_count, 1, sizeof(int), datafile);
				fread(&new_tweet.retweet_count, 1, sizeof(int), datafile);
				fread(&new_tweet.views_count, 1, sizeof(int), datafile);

				if (strcmp (new_tweet.language, search_value) == 0 && new_tweet.text[0] != '*') {
					printf ("text: %s\n", new_tweet.text);
					printf ("user: %s\n", new_tweet.user);
					printf ("coordinates: %s\n", new_tweet.coordinates);
					printf ("language: %s\n", new_tweet.language);
					printf ("favorite count: %d\n", new_tweet.favorite_count);
					printf ("retweet count: %d\n", new_tweet.retweet_count);
					printf ("views count: %d\n", new_tweet.views_count);
					printf("\n");
                    achou = 1;
				}
			}
		}
		if(!achou)
            printf("No results");
    fclose(datafile);
	}
}

void search_favorite (char* filename) {
	int search_value;
	printf ("insert favorite search value: ");
	scanf ("%d", &search_value);
	int i;
	int atual;
    int achou = 0;

	int num_of_registers = number_of_registers();

	if (num_of_registers > TAMANHO_MINIMO) {
		//recupera os dados de indice e lista invertida de seus respectivos arquivos
		FavoriteIndex *fav_idx = recover_fav_index();
		InvertedList *fav_list = recover_fav_list();
		for (i=0; i<REGISTER_SIZE; i++) {
			if (fav_idx[i].favorite == search_value) {
				atual = fav_idx[i].RRN;
				print_register(filename, fav_list[fav_idx[i].RRN].byteOffSet);
				while (fav_list[atual].proximo != -1) {
					atual = fav_list[atual].proximo;
					print_register(filename, fav_list[atual].byteOffSet);
				}
				achou = 1;
                break;
			}
		}
        if(!achou)
            printf("No results");
	} else {
		//busca sequencialmente pelo arquivo de dados
		FILE *datafile = fopen (filename, "r+");
		size_t nread;
		Tweet new_tweet;
		new_tweet.text = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.user = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.coordinates = (char*) malloc (sizeof(char)*BUFFER);
		new_tweet.language = (char*) malloc (sizeof(char)*BUFFER);
		int tamanho = 0;

		if (datafile != NULL) {
			while ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
				char* lido = malloc(sizeof(char)*tamanho);
				fread(lido,1,tamanho-12,datafile);	//le só os campos variaveis (-12 = 3 cmps fixos)

				new_tweet.text = strtok(lido, "|");
				new_tweet.user = strtok(NULL, "|");
				new_tweet.coordinates = strtok(NULL, "|");
				new_tweet.language = strtok(NULL, "|");;

				fread(&new_tweet.favorite_count, 1, sizeof(int), datafile);
				fread(&new_tweet.retweet_count, 1, sizeof(int), datafile);
				fread(&new_tweet.views_count, 1, sizeof(int), datafile);

				if (new_tweet.favorite_count == search_value && new_tweet.text[0] != '*') {
					printf ("text: %s\n", new_tweet.text);
					printf ("user: %s\n", new_tweet.user);
					printf ("coordinates: %s\n", new_tweet.coordinates);
					printf ("language: %s\n", new_tweet.language);
					printf ("favorite count: %d\n", new_tweet.favorite_count);
					printf ("retweet count: %d\n", new_tweet.retweet_count);
					printf ("views count: %d\n", new_tweet.views_count);
					printf("\n");
                    achou = 1;
				}
			}
		}
        if(!achou)
            printf("No results");
    fclose(datafile);
	}

}


int search_index_favorite(FavoriteIndex *ind,int tamanho,int valor){
	int i;
	for(i = 0;i<tamanho;i++){
		if(ind[i].favorite == valor)
			return i;
	}
	return -1;
}

void create_language_index(char *filename,LanguageIndex *lng_idx,InvertedList *listaInvertida){
	FILE *datafile, *language_index = NULL,*language_list;
	//tweet *tweet = malloc (sizeof(tweet));
	char *token;
	int i;
	int tamanho = 0,valor,posicaoIndice = 0,posicaoLista = 0,soma = 0, indiceCorreto = 1;
	size_t nread;

	datafile = fopen (filename, "r+");

	if (datafile) {
		while ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
			char *lido = malloc(sizeof(char)*tamanho);
			fread(lido,1,tamanho-12,datafile);
			token = strtok(lido, "|");
			if(token[0] == '*')
                indiceCorreto = 0;
			token = strtok(NULL, "|");
			token = strtok(NULL, "|");
			token = strtok(NULL, "|");
            if(indiceCorreto){
                listaInvertida[posicaoLista].byteOffSet = soma;
                soma+= (tamanho + sizeof(int));
                int posicao = search_index_language(lng_idx,posicaoIndice,token);
                if(posicao == -1){
                    lng_idx[posicaoIndice].language = malloc(sizeof(char)*BUFFER);
                    strcpy(lng_idx[posicaoIndice].language,token);
                    lng_idx[posicaoIndice].RRN = posicaoLista;
                    listaInvertida[posicaoLista].proximo = -1;
                    posicaoIndice++;
                }
                else{
                    listaInvertida[posicaoLista].proximo = lng_idx[posicao].RRN;
                    lng_idx[posicao].RRN = posicaoLista;
                }
                posicaoLista++;
            }
            indiceCorreto = 1;
			fread(&valor, 1, sizeof(int), datafile);
			fread(&valor, 1, sizeof(int), datafile);
			fread(&valor, 1, sizeof(int), datafile);
		}
	}
    ordenaIndLanguage(lng_idx,posicaoIndice);
    int t;
	language_index = fopen (LANGUAGE_INDEX_NAME, "w+");
	if (language_index != NULL) {
		for (i=0; i<posicaoIndice; i++) {
            t = strlen(lng_idx[i].language);
            fwrite(&t, sizeof(int), 1, language_index);
			fwrite(&lng_idx[i].RRN, sizeof(int), 1, language_index);
            fwrite(lng_idx[i].language, sizeof(char),t, language_index);
		}
	}
    //salva a lista invertida num arquivo
	language_list = fopen (LANGUAGE_LIST_NAME, "w+");
	if (language_list != NULL) {
		for (i=0; i<posicaoLista; i++) {
            fwrite(&listaInvertida[i].byteOffSet, sizeof(int), 1, language_list);
			fwrite(&listaInvertida[i].proximo, sizeof(int), 1, language_list);
		}
	}

	fclose(language_list);
	fclose (language_index);
	fclose(datafile);
}

void reseta(LanguageIndex *lng_ind){
    int i;
    for (i = 0; i < REGISTER_SIZE; i++){
        lng_ind[i].language = NULL;
    }
}


//index fortemente ligado com a chave Fav_count
void create_favorite_index(char *filename,FavoriteIndex *fav_idx,InvertedList *listaInvertida){
	FILE *datafile, *favorite_index,*favorite_list;
	//tweet *tweet = malloc (sizeof(tweet));
	char *token;
	char* lido;
	int i;
	int tamanho = 0,valor_favorite, aux, posicaoIndice = 0,posicaoLista = 0,soma = 0,indiceCorreto = 1;
	size_t nread;

	datafile = fopen (filename, "r+");

	if (datafile) {
		while ((nread = fread(&tamanho, 1, sizeof(int), datafile)) > 0) {
            lido = malloc(sizeof(char)*tamanho);
			fread(lido,1,tamanho-12,datafile);	//le os campos "char*"
			token = strtok(lido, "|");
			if(token[0] == '*')
                indiceCorreto = 0;
			token = strtok(NULL, "|");
			token = strtok(NULL, "|");
			token = strtok(NULL, "|");
			fread(&valor_favorite, 1, sizeof(int), datafile);	//le o valor de favorite do arq
            if(indiceCorreto){
                listaInvertida[posicaoLista].byteOffSet = soma;
                soma+= (tamanho + sizeof(int));
                int posicao = search_index_favorite(fav_idx,posicaoIndice,valor_favorite);
                if(posicao == -1){
                    fav_idx[posicaoIndice].favorite = valor_favorite;
                    fav_idx[posicaoIndice].RRN = posicaoLista;
                    listaInvertida[posicaoLista].proximo = -1;
                    posicaoIndice++;
                }
                else{
                    listaInvertida[posicaoLista].proximo = fav_idx[posicao].RRN;
                    fav_idx[posicao].RRN = posicaoLista;
                }
                posicaoLista++;
            }
			fread(&aux, 1, sizeof(int), datafile);	//le o valor de retweet, avanca o ponteiro do arq
			fread(&aux, 1, sizeof(int), datafile);	//le o valor de views, avanca o ponteiro do arq
			indiceCorreto = 1;
		}
	}
	ordenaIndFavorito(fav_idx,posicaoIndice);

	favorite_index = fopen (FAVORITE_INDEX_NAME, "w+");
	if (favorite_index != NULL) {
		for (i=0; i<posicaoIndice; i++) {
			fwrite(&fav_idx[i].favorite, sizeof(int), 1, favorite_index);
			fwrite(&fav_idx[i].RRN, sizeof(int), 1, favorite_index);
		}
	}
    favorite_list = fopen (FAVORITE_LIST_NAME, "w+");
	if (favorite_list != NULL) {
		for (i=0; i<posicaoLista; i++) {
            fwrite(&listaInvertida[i].byteOffSet, sizeof(int), 1, favorite_list);
			fwrite(&listaInvertida[i].proximo, sizeof(int), 1, favorite_list);
		}
	}

	fclose(favorite_list);
	fclose(favorite_index);
	fclose(datafile);
}

void ordenaIndFavorito(FavoriteIndex *fav_ind,int tamanho){
    int i,j;
    for(i = 0; i<tamanho; i++){
        for (j=i;j<tamanho;j++){
            if(fav_ind[i].favorite > fav_ind[j].favorite){
                FavoriteIndex aux = fav_ind[i];
                fav_ind[i] = fav_ind[j];
                fav_ind[j] = aux;
            }
        }
    }
}

void ordenaIndLanguage(LanguageIndex *lng_ind,int tamanho){
    int i,j;
    for(i = 0; i<tamanho; i++){
        for (j=i;j<tamanho;j++){
            if(strcmp(lng_ind[i].language,lng_ind[j].language)>0){
                LanguageIndex aux = lng_ind[i];
                lng_ind[i] = lng_ind[j];
                lng_ind[j] = aux;
            }
        }
    }
}

void ordenaListTweetPorFavorito(Tweet **tweets,int tamanho){
    int i,j;
    for(i = 0; i<tamanho; i++){
        for (j=i;j<tamanho;j++){
            if(tweets[i]->favorite_count > tweets[j]->favorite_count){
                Tweet *t = tweets[i];
                tweets[i] = tweets[j];
                tweets[j] = t;
            }
        }
    }
}

FavoriteIndex* recover_fav_index () {
	FILE *index_file;
	size_t nread;
	int i=0, aux;

	FavoriteIndex *fav_idx = malloc (sizeof(FavoriteIndex) * REGISTER_SIZE);

	index_file = fopen (FAVORITE_INDEX_NAME, "r+");

	if (index_file) {
		while ((nread = fread(&aux, 1, sizeof(int), index_file)) > 0) {
			fav_idx[i].favorite = aux;
			fread(&aux, 1, sizeof(int), index_file);
			fav_idx[i].RRN = aux;
			i++;
		}
	}
	fclose(index_file);
    return fav_idx;
}

InvertedList* recover_fav_list () {
	FILE *list_file;
	size_t nread;
	int i=0, aux;


	InvertedList *fav_list = malloc (sizeof(InvertedList) * REGISTER_SIZE);


	list_file = fopen (FAVORITE_LIST_NAME, "r+");
	fseek (list_file, 0, SEEK_SET);

	if (list_file) {
		while ((nread = fread(&aux, 1, sizeof(int), list_file)) > 0) {
            fav_list[i].byteOffSet = aux;
			fread(&aux, 1, sizeof(int), list_file);
            fav_list[i].proximo = aux;
//			printf ("offset: %d\n", fav_list[i].byteOffSet);
//			printf ("proximo: %d\n", fav_list[i].proximo);
			i++;
		}
	}
	fclose(list_file);
    return fav_list;
}

LanguageIndex* recover_language_index () {
	FILE *index_file;
	size_t nread;
	int i=0;
	int i_aux;
	int tamanho;
	char *c_aux = malloc(sizeof(char));

	LanguageIndex *lng_idx = malloc (sizeof(FavoriteIndex) * REGISTER_SIZE);
    reseta(lng_idx);

	index_file = fopen (LANGUAGE_INDEX_NAME, "r+");

	if (index_file) {
		while ((nread = fread(&tamanho, 1, sizeof(int), index_file)) > 0) {
            fread(&i_aux, 1, sizeof(int), index_file);
			lng_idx[i].RRN = i_aux;
            fread(c_aux, 1, tamanho, index_file);
            c_aux[tamanho] = '\0'; //vem com lixo
            lng_idx[i].language = malloc(sizeof(char)*tamanho);
			strcpy(lng_idx[i].language,c_aux);
			i++;
		}
	}
    return lng_idx;
}

InvertedList* recover_language_list () {
	FILE *list_file;
	size_t nread;
	int i=0, aux;

	InvertedList *lng_list = malloc (sizeof(InvertedList) * REGISTER_SIZE);

	list_file = fopen (LANGUAGE_LIST_NAME, "r+");

	if (list_file) {
		while ((nread = fread(&aux, 1, sizeof(int), list_file)) > 0) {
			lng_list[i].byteOffSet = aux;
			fread(&aux, 1, sizeof(int), list_file);
			lng_list[i].proximo = aux;
			i++;
		}
	}
    return lng_list;
}
void print_register_todelete(char *filename, int offset){
    FILE *datafile;
    char *token, *lido;
    int size = 0;
    size_t nread;
    datafile = fopen(filename, "r+");
    if(datafile){
        fseek(datafile, offset, SEEK_SET);
        if((nread = fread(&size, 1, sizeof(int), datafile)) > 0){
            lido = malloc(sizeof(char)* size);
            fread(lido, 1, size-12, datafile);
            token = strtok(lido, "|");
            if(strlen(token) > 15){
                memmove(token, token, 15);
                strcat(token, "...");
            }
            printf("text: %s\n", token);
            token = strtok(NULL, "|");
            printf("user: %s\n", token);
        }
    }
    fclose(datafile);
}
