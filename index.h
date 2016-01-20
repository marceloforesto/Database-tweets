
#ifndef INDEX_H_
#define INDEX_H_

#define LANGUAGE_INDEX_NAME "language_idx.dat"
#define LANGUAGE_LIST_NAME "language_list.dat"
#define FAVORITE_INDEX_NAME "favorite_idx.dat"
#define FAVORITE_LIST_NAME "favorite_list.dat"

#define TAMANHO_MINIMO 10

void index_matching (char *filename,int fav_count, char *language);

void index_merging(char *filename,int fav_count, char *language);

int number_of_registers ();

void create_language_index(char *filename,LanguageIndex *ind,InvertedList *listaInvertida);

void create_favorite_index(char *filename,FavoriteIndex *ind,InvertedList *listaInvertida);

void index_manager(char *filename);

void search_language (char *filename);

void search_favorite (char* filename);

FavoriteIndex* recover_fav_index ();

InvertedList* recover_fav_list ();

LanguageIndex* recover_language_index ();

InvertedList* recover_language_list ();

Tweet *retornaTweet(char *filename, int offset);

void ordenaListTweetPorFavorito(Tweet **tweets,int tamanho);
int jaPrintouTweet(Tweet *t, Tweet **tweets,int tamanho);
void ordenaIndLanguage(LanguageIndex *lng_ind,int tamanho);
void ordenaIndFavorito(FavoriteIndex *fav_ind,int tamanho);
void aumentaArqTamanho();
void diminuiArqTamanho();
int search_index_favorite(FavoriteIndex *ind,int tamanho,int valor);
void print_register_todelete(char *filename, int offset);

#endif /* INDEX_H_ */
