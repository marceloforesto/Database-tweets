/* TRABALHO 2 - ORG ARQ
 *
 * Alunos:
 * 	Rafael Neto
 * 	Gabriel Cristiano
 * 	Marcelo Foresto
 * 	Leonardo Ventura
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tweet.h"
#include "index.h"

#define CMD_NT  "nt"	//new tweet
#define CMD_LT	"lt"	//list tweets
#define CMD_SU 	"su"	//search by user
#define CMD_SF 	"sf"	//search by favorite count
#define CMD_SL 	"sl"	//search by language
#define CMD_DF	"df"	//delete by favorite count
#define CMD_FOL "fol"	//matching fav or language
#define CMD_FAL "fal"	//merging fav and language

#define CMD_DR	"dr"	//delete by rrn
#define CMD_QT 	"qt"	//quit
#define CMD_LENGTH 25

void interface () {
	printf ("\n----------------------------------\n"
			"* New Tweet - press 'nt'\n"
			"* List all Tweets - press 'lt'\n"
			"* Search via User - press 'su'\n"
			"* Search via Favorite Count - press 'sf'\n"
			"* Search via Language - press 'sl'\n"
			"* Merge (Favorite and Language) - press 'fal\n"
			"* Match (Favorite or Language) - press 'fol\n"
			"* Delete tweet by Favourite Count - press 'df'\n"
			"* QUIT - press 'qt'\n\n:"
			);
}


int main (void) {
	char *cmd = malloc(sizeof(char)*CMD_LENGTH);
    char filename[20] = "register.dat";

    index_manager(filename);    //cria pela primeira vez os indices
	interface();
	while (strcmp(cmd, CMD_QT)) {
		do {
        fgets (cmd, CMD_LENGTH, stdin);
		} while (strlen(cmd) == 1);
		remove_new_line(cmd);
		if (strcmp(CMD_NT, cmd) == 0){
			new_tweet(filename);
			aumentaArqTamanho();
			index_manager(filename);    //atualiza os indices
		}
		else if (strcmp(CMD_LT, cmd) == 0)
			list_tweets(filename);
		else if (strcmp(CMD_DF, cmd) == 0)
			delete_favorite(filename);
		else if (strcmp(CMD_FOL, cmd) == 0)
			fav_or_language(filename);
        else if (strcmp(CMD_FAL, cmd) == 0)
			fav_and_language(filename);
		else if (strcmp(CMD_SF, cmd) == 0)
			search_favorite(filename);
		else if (strcmp(CMD_SL, cmd) == 0)
			search_language(filename);
        else if (strcmp(CMD_SU, cmd) == 0)
			search_user(filename);
		else if (strcmp(CMD_QT, cmd) == 0)
			return 0;
		else
			printf("invalid command: %s", cmd);

		interface();
	}


	return 0;
}
