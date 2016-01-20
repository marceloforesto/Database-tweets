
#ifndef TWEET_H_
#define TWEET_H_

#define BUFFER 140
#define REGISTER_SIZE 100
#define SIZEFILENAME "sizefile.dat"

typedef struct tweet {
	char *text;
	char *user;
	char *coordinates;
	int favorite_count;
	char *language;
	int retweet_count;
	int views_count;
} Tweet;

typedef struct FavoriteIndex{
    int favorite;
    int RRN;
}FavoriteIndex;

typedef struct LanguageIndex{
    char *language;
    int RRN;
}LanguageIndex;

typedef struct listaInvertida{
    int proximo;
    int byteOffSet;
}InvertedList;

typedef struct sizeList{
    int byteOffset;
    int tamanho;
}SizeList;


/* @brief: cria um novo tweet, preenche e o salva no arquivo
 * @param: nome do arquivo onde tweet sera salvo
 */
void new_tweet (char *filename);

/* @brief: le o arquivo de tweets e os lista sequencialmente (rrn)
 * @param: nome do arquivo lido
 */
void list_tweets (char *filename);

/* @brief: procura os tweets do usuario no arquivo e os deleta logicamente
 * @param: nome do arquivo para procurar
 */
void delete_tweet_user (char *filename);

/* @brief: deleta logicamente o tweet encontrado no rrn especificado
 * @param: nome do arquivo a ser percorrido
 */
void delete_tweet_rrn (char *filename);

/* @brief: procura o tweet no rrn especificado
 * @param: nome do arquivo a ser percorrido
 */
void search_rrn (char *filename);

/* @brief: procura o usuario no arquivo de tweets
 * @param: nome do arquivo a ser percorrido
 */
void search_user (char *filename);

/* @brief: funcao que evita o fgets de manter o /n no fim do buffer
 *
 */
void remove_new_line(char *line);

void printTweet(Tweet *t);

void fav_and_language (char *filename);

void fav_or_language (char *filename);

void delete_favorite (char *filename);

void delete_rrn(char *filename, int rrn);

SizeList* recover_size_list(char *filename, int *del_num);

void delete_offset (char *filename, int offset);

#endif /* TWEET_H_ */
