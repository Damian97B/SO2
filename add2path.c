#include <stdio.h>
#include <unistd.h>
#include <string.h>

int main(int argc, char *argv[]) {
char sciezkacalkowita[2000] = {};
char nowasciezka[2000] = {};
char argument[2000];
char path[2000] = {};
char dir;

        if(argc < 2) { //jesli parametr jest samo ./add2path to dodaje tylko sciezke
                char cwd[2000];
                getcwd(cwd, sizeof(cwd));
                strcat(sciezkacalkowita, cwd);
        }
        if (argc >= 2){
                strcpy(argument, argv[1]);

                if (argv[1][0] == '/') {
                        strcpy(sciezkacalkowita, argument); //skopiuj do argumentu z argv[1]
                } else {
                        char cwd[2000];
                        getcwd(cwd, sizeof(cwd));
                        strcat(sciezkacalkowita, cwd);    //strcat dopisywanie do konca tablicy
                        strcat(sciezkacalkowita, "/");
                        strcat(sciezkacalkowita, argument);
                }
        }
        FILE *path_file = fopen("/etc/environment", "r");

        dir = fgetc(path_file);         //wczytanie pliku
        while (dir != EOF){             //EOF to koniec pliku
                strncat(path, &dir, 1);
                dir = fgetc(path_file);
        }

        fclose(path_file); //zamkniecie pliku
        char *start = strstr(path, sciezkacalkowita);

//sprawdzanie czy to jest poczatkowy path z enterem
char sprawdzane[2000] = {};
strcat(sprawdzane, path);
int len1 = (strlen(sprawdzane) - 2);
        if(*(sprawdzane+(len1)) == '\"'){ //sprawdzenie czy element -2 to "
                printf("zawartosc przed: %s\n",path);
                strncpy(nowasciezka, path, strlen(path) - 2); //dla pierwszego razu musi byc -2 poniewaz musi zastapic " oraz enter
                strcat(nowasciezka, ":");
                strcat(nowasciezka, sciezkacalkowita);
                strcat(nowasciezka, "\"");//w PATH musi byc zakonczenie " dlate$
                printf("sciezka zostala dodana do $PATH\n");
        }

        else if (start == NULL){
                printf("zawartosc przed: %s\n",path);
                strncpy(nowasciezka, path, strlen(path) - 1); //musi byc -1 poniewaz musi nadpisac " a -2 jesli musi od poczatku
                strcat(nowasciezka, ":");
                strcat(nowasciezka, sciezkacalkowita);
                strcat(nowasciezka, "\"");//w PATH musi byc zakonczenie " dlatego "\""
                printf("sciezka zostala dodana do $PATH\n");
        } else {
                printf("zawartosc przed: %s\n",path); //wypisanie zawartosci PATH przed operacja usuniecia
                strncpy(nowasciezka, path, strlen(path) - strlen(start));

                strcat(nowasciezka, start + strlen(sciezkacalkowita));
                printf("sciezka zostala usunieta z $PATH\n");

                int len = (strlen(nowasciezka) - 2); //dlugosc sciezki do sprawdzenia czy ostatni element to :

                if (*(nowasciezka+(len)) == ':'){          //sprawdzanie czy ostatni element PATH to ":"
                        int idxtodel = len; //index do usuniecia czyli :
                        memmove(&nowasciezka[idxtodel], &nowasciezka[idxtodel + 1], strlen(nowasciezka) - idxtodel);
                }
        }
        path_file = fopen("/etc/environment", "w");             //nadpisywanie $PATH

        if (path_file == NULL){
                printf("prosze uruchomic program z uprawnieniami administratora (sudo ./add2path)\n");
                return 1;
        }
        fprintf(path_file, "%s", nowasciezka);
        printf("zawartosc %s\n",nowasciezka);
        fclose(path_file);
        printf("prosze sie ponownie zalogowac aby zmiany w ($PATH) (pod komenda echo $PATH) byly widoczne");
        return 1;
}

