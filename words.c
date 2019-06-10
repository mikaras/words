#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#include <time.h>

#define MAX 130

typedef struct Node{
    char *word;         // fraasi
    char *text;         // fraasiin liittyv‰ fraasi
    int count;          // fraasien lukum‰‰r‰
    struct Node *next;
}Node;

Node *hash[MAX];        // taulukko fraaseille ja m‰‰rille
Node *hash_all[MAX];    // taulukko fraasien ja niihin liittyville fraaseille

int uniqphrases = 0;    // erilaisten fraasien m‰‰r‰
int num = 0;            // fraasien kokonaism‰‰r‰


// Funktio antaa yksilˆllisen arvon fraasille
int HashIndex(char *x)
{
	int sum=0;
	int i;
	for(i=0;x[i];i++)
		sum=sum+x[i];
	return sum%MAX;
}

// Tarkistetaan onko fraasi jo taulussa
struct Node *search(char *s){

    struct Node *p;

    for(p = hash[HashIndex(s)]; p!=NULL; p=p->next){        //etsit‰‰n fraasia
        if (strcmp(s, p->word) == 0){
            //p->count++;
            return p; //fraasi lˆytyi
        }
    }
    return NULL;  //fraasia ei lˆytynyt

}

// Tallennetaan fraasi tauluun, jos fraasi lˆytyy jo, sitten sen m‰‰r‰‰ lis‰t‰‰n yhdell‰
Node *insert(char *word/*, char *text*/){

    int i;
    Node *p;

    if((p = search(word)) == NULL) {            //tarkistetaan onko fraasi jo tallennettu

        p = malloc(sizeof(*p));                 //varataan tila muistista
        assert(p != NULL);
        p->word = malloc(strlen(word) + 1 );
        p->count =1;                            //asetetaan arvoksi yksi, koska fraasi tallennetaan ensimm‰ist‰ kertaa
        assert(p->word != NULL);
        strcpy(p->word, word);

        /*p->text = malloc(strlen(text) + 1);
        assert(p->text != NULL);
        strcpy(p->text, text);
*/
        unsigned hashval = HashIndex(word);
        p->next = hash[hashval];
        hash[hashval] = p;

        uniqphrases++;                          //lis‰t‰‰n erilaisten fraasien m‰‰r‰‰

    } else {
        p->count++;         // jos fraasi lˆytyy jo lis‰t‰‰n fraasin m‰‰r‰‰

    }

    return p;

}

//Etsit‰‰n fraasia ja palauettaan NULL, jos fraasia ei lˆytynyt
Node *search_all(char *word){

    int i;
    Node *w;

    for(w = hash_all[HashIndex(word)]; w!=NULL; w=w->next){
      if (strcmp(word, w->word) == 0){

       //printf("%s %s", w->word, w->text);
       //printf("Find");
         return w; //fraasi lˆytyi
      }
    }
    return NULL;  //fraasia ei lˆytynyt
}

// K‰ytt‰j‰n halutessa n‰hd‰ m‰‰r‰llisesti isoimpaan fraasiin liittyv‰t fraasit tallennetaan kaikki fraasit tauluun
Node *insert_all(char *word, char *text){

    Node *all;

    all = malloc(sizeof(*all));                 //varataan tila muistista
    assert(all != NULL);
    all->word = malloc(strlen(word) + 1 );      //varataan tila fraasille muistista
    //all->count =1;
    assert(all->word != NULL);
    strcpy(all->word, word);

    all->text = malloc(strlen(text) + 1);       //varataan muistista tila fraasiin liittyv‰lle fraasille
    assert(all->text != NULL);
    strcpy(all->text, text);
    //printf("%s %i %s", p->word, p->count, p->text);

    unsigned hashval = HashIndex(word);
    all->next = hash_all[hashval];
    hash_all[hashval] = all;

    return all;

}

// Etsit‰‰n fraasi, jolla eniten esiintymi‰ ja tulostetaan fraasit
int lookup (char* biggest){

    int i;
    Node *ptr;
    int found = 0;

    for(i = 0; i < MAX; i++){                       //k‰yd‰‰n taulukkoa l‰pi
        if(hash_all[i]==NULL){                      //tarkistetaan onko tyhj‰
            printf("Hash table is empty!");
        }else{
            for(ptr=hash_all[i];ptr!=NULL;ptr=ptr->next){
                if (strcmp(biggest, ptr->word) == 0)            //jos isoin fraasi lˆytyy tulostetaan se ja siihen liittyv‰t fraasit
                printf("\n %-16s %s\n", ptr->word, ptr->text);

            }
        }
    }
}

// Laitetaan fraasit suuruusj‰rjestykseen
Node* maximum (){

    int i;
    int max = 0;
    Node *ptr =0;
    Node *max_ptr=0;

    for(i = 0; i < MAX; i++){                           //k‰yd‰‰n taulukkoa l‰vitse

        if(hash[i]!=NULL)

            for(ptr=hash[i];ptr!=NULL;ptr=ptr->next){

                if(ptr->count > max){               //verrataan lukuja kesken‰‰n ja vaihdetaan paikkaa tarvittaessa
                    max_ptr = ptr;
                    max = ptr->count;
                }
            }
    }
    max_ptr->count = -max_ptr->count;       //negatiiviseksi, jotta saadaan tulostettua 100 yleisint‰

    return max_ptr;
}

// Etsit‰‰n fraasi, jota on eniten
Node *findmax(){

    int m;
    int max = 0;
    Node *ptr =0;
    Node *max_ptr=0;

    for(m = 0; m < MAX; m++){           //k‰yd‰‰n taulukko l‰vitse

        if(hash[m]!=NULL)

            for(ptr=hash[m];ptr!=NULL;ptr=ptr->next){

                if(ptr->count < max){       //verrataan lukuja kesken‰‰n ja vaihdetaan paikkaa tarvittaessa
                    max_ptr = ptr;
                    max = ptr->count;
                }
            }
    }
    return max_ptr;
}

// Luetaan tiedosto tauluun
int read_file(FILE *fin, FILE *fout){

    char s[256];
    char fname[50];
    char response;
    int i = 0;
    clock_t t;
    double time;

    printf("\n  *** PHRASE PROGRAM *** \n");
    printf("\n Give a file name: ");
    //printf("\nsmall.txt\n medium.txt\n big.txt)\n");
    scanf("%s", &fname);                                        //luetaan k‰ytt‰j‰n antama tiedoston nimi
    fin = fopen(fname, "r");

    while(fin == NULL) {                                        //jos tiedostoa ei lˆydy kysyt‰‰n uudestaan
        //printf("\n File not found!\n");
        printf("\n Do you want to try again? Y(yes) or N(no): ");
        response = getch();

        if (response == 'Y' || response == 'y'){
            printf("\n\n Give a file name: ");
            scanf("%s", &fname);
            fin =fopen(fname, "r");

        }else if(response == 'N' || response == 'n'){
            printf("\n\n Program will shut down. Bye bye!\n");
            exit(0);
        }else
            printf("\n Choose Y or N.");
    }


    t = clock();            //k‰ynnistet‰‰n kello suoritusajan mittaamiseksi

    while (fgets(s, sizeof(s), fin) != NULL)
    {
        char *part1 = strtok(s, ":");
        char *part2 = strtok(NULL, ":");

        strupr(part1);               //Sana isoiksi kirjaimiksi
        insert(part1);           //Syˆtet‰‰n tiedot tauluun
        insert_all(part1, part2);       //Syˆtet‰‰n tiedot tauluun

        num++;                        //lis‰t‰‰n fraasien kokonaism‰‰r‰‰
    }

    print_result(fout);                         //kutsutaan tulostusfunktiota

    t = clock() - t;
    time = ((double)t)/CLOCKS_PER_SEC;          //lasketaan kauanko toteutuksessa kesti

    printf("\n\n Time: %.3f\n", time);

    fclose(fin);                //suljetaan fraasien tiedosto
    fclose(fout);               //suljetaan tulosten tiedosto

    return 0;
}

// Tulostetaan 100 fraasia suuruusj‰rjestyksess‰ n‰ytˆlle ja tiedostoon.
int print_result(FILE *fout){

    Node *list;
    int i;

    fout=fopen("words_print.txt","w");              //avataan tiedosto tallennusta varten

    if(fout==NULL)
    {
        printf(" Can't open the out file!\n");
        return 1;
    }

    printf(" \n\n     *** WORDS *** \n\n");
    printf(" Total number of words: %d\n\n", num);
    printf(" Number of different words: %d\n\n", uniqphrases);
    printf(" The 100 most common words: \n\n");
    printf(" WORDS        NUMBER OF OCCURRENCIES\n");

    fprintf(fout, "\n\n     *** PHRASES *** \n\n");
    fprintf(fout, "Total number of words: %d\n", num);
    fprintf(fout, "Number of different words: %d\n\n", uniqphrases );
    fprintf(fout, "The 100 most common words: \n");
    fprintf(fout, "WORDS         NUMBER OF OCCURRENCIES\n");

    for(i =0; i<100; i++){
       list = maximum();
       printf("\n %-10s\t %7d", list->word, abs(list->count));           //tiedot n‰ytˆlle
       fprintf(fout, "\n%-10s\t %7d", list->word, abs(list->count));     //tiedot tiedostoon
    }

}

// Tulostetaan isoimpaan fraasiin liittyv‰t fraasit, jos k‰ytt‰j‰ haluaa.
void print_biggest(){

    Node * occu;
    char answer;

    do{
        printf("\n\nSearch word which have biggest number of occurencies? Y/N \n");    //Kysyt‰‰n haluaako k‰ytt‰j‰ n‰hd‰ isoimpaan fraasiin liittyv‰t fraasit
        answer = getch();

        if(answer == 'Y' || answer == 'y') {
            occu = findmax();                       //etsit‰‰n fraasi
            lookup(occu->word);                     //etsit‰‰n isoimpaan fraasiin liittyv‰t fraasit kutsumalla lookup-funktiota
            break;
        }

    }while(answer != 'N' && answer != 'n');
}

int main (){

    FILE *fin, *fout;
    int num = 0;

    read_file(fin, fout);           // luetaan fraasit tiedostosta

    print_biggest();                // tulostetaan isoin fraasi ja siihen liittyv‰t fraasit k‰ytt‰j‰n niin halutessa

    printf("\nThank you for using this program and bye bye!\n");



    return 0;
}


