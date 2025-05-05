/*
Sistem Wallet electronic
Aplicatia permite utilizatorului sa isi gestioneze cardurile. Utilizatorul poate adauga carduri noi,
poate vizualiza cardurile existente si poate sterge carduri din portofelul electronic. Pentru 
a avea o experienta individualizata cu aplicatia, in lista cardurilor, ordinea acestora este data
crescator in functie de cel mai vizualizat card. Un card are informatii precum numar card,
nume posesor, data expirare etc..

MENIU:
1. Vizualizare carduri -> vizualizare date carduri
2. Adaugare card
3. stergere card
4. Iesire
*/

#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h> 
#include <ctype.h>

#define UP 119
#define DOWN 115
#define ENTER 13

typedef struct card {
    char numar[17], nume[30], expira[6], ccv[4];
    int utilizari;
} Card;

void displayMenu(int selected_option, char *options[], int size);
void salvare(const char *file, Card **lista, int nr_cards);
void menu(Card **lista, int nr_cards);
void addCard(Card ***lista, int *size);
void deleteCard(Card **lista, int *size);
void screenClear();
Card *citireCard(FILE *fisier);
Card **citire(const char *file, int *size);
void mergeSort(Card **arr, int left, int right);
void merge(Card **arr, int left, int mid, int right);
void viewCards(const char text[], Card **lista, int *nr_cards);
void  Delete(Card **lista, int* size, int selected_option);
void ShowCard(int op, Card **lista);


void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main(void) 
{
    int nr_cards;
    Card **lista = citire("input.txt", &nr_cards);
    
    mergeSort(lista, 0, nr_cards - 1);
    menu(lista, nr_cards);
    
    return 0;
}

Card *citireCard(FILE *fisier) {
    if (fisier == NULL) 
        return NULL;

    Card *c = (Card *)malloc(sizeof(Card));
    if (c == NULL)
        return NULL;  

    if (fgets(c->numar, sizeof(c->numar), fisier) == NULL) { free(c); return NULL; }
    c->numar[strcspn(c->numar, "\n")] = '\0';
    fgetc(fisier);  

    if (fgets(c->ccv, sizeof(c->ccv), fisier) == NULL) { free(c); return NULL; }
    c->ccv[strcspn(c->ccv, "\n")] = '\0';
    fgetc(fisier);

    if (fgets(c->nume, sizeof(c->nume), fisier) == NULL) { free(c); return NULL; }
    c->nume[strcspn(c->nume, "\n")] = '\0';

    if (fgets(c->expira, sizeof(c->expira), fisier) == NULL) { free(c); return NULL; }
    c->expira[strcspn(c->expira, "\n")] = '\0';
    fgetc(fisier);

    char aux[10];
    if (fgets(aux, sizeof(aux), fisier) == NULL) { free(c); return NULL; }
    aux[strcspn(aux, "\n")] = '\0';
    c->utilizari = atoi(aux);

    return c;
}

Card **citire(const char *file, int *size) 
{
    FILE *fisier = fopen(file, "r");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului!\n");
        exit(1);
    }

    fscanf(fisier, "%d\n", size);
    
    Card **lista = (Card **)malloc((*size) * sizeof(Card *));
    if (lista == NULL) {
        printf("Eroare la alocarea memoriei pentru lista de carduri!\n");
        exit(1);
    }
    
    for (int i = 0; i < *size; i++) {
        lista[i] = citireCard(fisier);
    }

    fclose(fisier);
    return lista;
}

void screenClear() {
    printf("\033[H\033[J");
    fflush(stdout);  
}

void displayMenu(int selected_option, char *options[], int size) {
    screenClear();  
    printf("\t Sistem Wallet electronic\n\n");
    printf("Deplasarea indicatorilor (> <) se face cu tastele W - sus si S - jos.\n");
    printf("Dupa ce ati ales optiunea dorita, apasati tasta Enter.\n\n");
    printf("\t MENIU\n");
    
    for (int i = 0; i < size; i++) {
        if (i == selected_option)
            printf("  > \033[1m%s\033[0m < \n", options[i]);  
        else 
            printf("    %s    \n", options[i]);
    }
}

void menu(Card **lista, int nr_cards) 
{
    char *options[] = {"Vizualizare carduri", "Adaugare card", "Stergere card", "Iesire"};
    int nr_options = sizeof(options) / sizeof(options[0]);
    int selected_option = 0;
    char direction;

    while (1) {
        displayMenu(selected_option, options, nr_options);
        direction = _getch();

        if (direction != ENTER) { 
            if (direction == UP && selected_option > 0)
                selected_option--;
            if (direction == DOWN && selected_option < nr_options - 1)
                selected_option++;
        } else {
            screenClear();    
            if (selected_option == nr_options - 1)
                {
                    salvare("input.txt", lista, nr_cards);
                    break; 
                }
            switch(selected_option + 1) {
                case 1:
                    viewCards("VIZUALIZARE LISTA CARDURI", lista, &nr_cards);
                    break;
                case 2:
                    addCard(&lista,&nr_cards);
                    break;
                case 3:
                    deleteCard(lista, &nr_cards);
                    break;
            }
        }
    }
}

void ShowCard(int op, Card **lista) {
    if (lista[op] == NULL) {
        printf("Cardul este NULL!\n");
        return;
    }
    printf("\033[1;32mNumar card:\033[0m %s\n", lista[op]->numar);
    printf("\033[0;36mCCV:\033[0m %s\n", lista[op]->ccv);
    printf("\033[1;32mNume detinator\033[0m: %s\n", lista[op]->nume);
    printf("\033[0;36mData expirare:\033[0m %s\n", lista[op]->expira);
    printf("\033[1;32mVizualizari card\033[0m: %d\n\n", lista[op]->utilizari);
    lista[op]->utilizari++;
}

void merge(Card **arr, int left, int mid, int right) {
    int n1 = mid - left + 1;
    int n2 = right - mid;

    Card **L = (Card **)malloc(n1 * sizeof(Card *));
    Card **R = (Card **)malloc(n2 * sizeof(Card *));

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int j = 0; j < n2; j++)
        R[j] = arr[mid + 1 + j];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i]->utilizari > R[j]->utilizari)
            arr[k] = L[i++];
        else 
            arr[k] = R[j++];
        k++;
    }

    while (i < n1) {
        arr[k++] = L[i++];
    }
    while (j < n2) {
        arr[k++] = R[j++];
    }

    free(L);
    free(R);
}

void mergeSort(Card **arr, int left, int right) {
    if (left < right) {
        int mid = left + (right - left) / 2;
        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);
        merge(arr, left, mid, right);
    }
}

void viewCards(const char text[], Card **lista, int *nr_cards) {
    char direction;
    int selected_option = 0;
    int exit_index = *nr_cards;  

    while (1) {
        screenClear();   
        printf("\t Sistem Wallet electronic\n\n");
        printf("Deplasarea indicatorilor (> <) se face cu tastele W - sus si S - jos.\n");
        printf("Pentru selectarea cardului dorit, apasati Enter.\n\n");
        printf("\t %s \n", text);

        for (int i = 0; i < *nr_cards; i++) {
            if (i == selected_option)
                printf("\033[1m > Card %d - **** \033[37m%c%c%c%c\033[0m < \033[0m\n", i,
                       lista[i]->numar[12], lista[i]->numar[13],
                       lista[i]->numar[14], lista[i]->numar[15]);
            else
                printf("   Card %d - **** \033[35m%c%c%c%c\033[0m \n", i,
                       lista[i]->numar[12], lista[i]->numar[13],
                       lista[i]->numar[14], lista[i]->numar[15]);
        }
        if (selected_option == exit_index)
            printf(" > \033[1mIesire\033[0m < \n");
        else 
            printf("   Iesire  \n");

        direction = _getch();

        if (direction != ENTER) { 
            if (direction == UP && selected_option > 0)
                selected_option--;
            if (direction == DOWN && selected_option < exit_index)
                selected_option++;
        } else {
            if (selected_option < exit_index) {
                screenClear();    
                printf("Ai selectat: Cardul %d\n", selected_option);
                ShowCard(selected_option, lista);
                printf("Pentru a reveni la meniu, apasati orice tasta.\n");
                _getch(); 
                mergeSort(lista, 0, (*nr_cards) - 1); 
            } else {
                break;
            }
        }
    }    
}

int isAllDigits(const char* str) 
{
    for (int i = 0; str[i]; i++)
        if (!isdigit(str[i])) return 0;
    return 1;
}


int isValidCardNumber(const char* str) 
{
    return strlen(str) == 16 && isAllDigits(str);
}

int isValidExpDate(const char* str) 
{
    return strlen(str) == 5 && isdigit(str[0]) && isdigit(str[1]) && str[2] == '/' && isdigit(str[3]) && isdigit(str[4]);
}

int isValidCCV(const char* str) {
    return strlen(str) == 3 && isAllDigits(str);
}



void addCard(Card ***lista, int *size)
{
    printf("\t ADAUGARE CARD\n\n");

    printf("Datele adaugate vor avea urmatorul format:\n");
    printf("\t- Nume si prenume separate printr-un spatiu\n");
    printf("\t- Numar card - 16 cifre fara separatori intre cifre\n");
    printf("\t- Data expirare - MM/YY\n");
    printf("\t- CCV - 3 cifre\n\n");
    Card *nou = (Card*)malloc(sizeof(Card));
    if (!nou)
    {
        printf("Eroare la alocarea memoriei!\n");
        return;
    }
        
    while(1)
    {
        printf("Nume si prenume titular card: ");
        if (fgets(nou->nume, sizeof(nou->nume), stdin) != NULL) 
        {   
            size_t len = strlen(nou->nume);
            if (len > 0 && nou->nume[len - 1] == '\n')
                nou->nume[len - 1] = '\0';
            else
                clearInputBuffer();
            break;
        }
        else
        {
            printf("Nume invalid! Va rog sa introduceti un nume si un prenume valid!\n");

        }
    }
        
    while(1)
    {
        printf("Numar card: ");
        if (fgets(nou->numar, 17, stdin) != NULL && isValidCardNumber(nou->numar)) 
        {
            size_t len = strlen(nou->numar);
            if (len > 0 && nou->numar[len - 1] == '\n')
                nou->numar[len - 1] = '\0';
            else
                clearInputBuffer();
            break;
        }
        else
        {
            printf("Numar invalid! Va rog sa introduceti un numar de 16 valid!\n");
        }
    }
        
    while(1)
    {
        printf("Data expirare: ");
        if (fgets(nou->expira, 6, stdin) != NULL && isValidExpDate(nou->expira)) 
        {
            size_t len = strlen(nou->expira);
            if (len > 0 && nou->expira[len - 1] == '\n')
                nou->expira[len - 1] = '\0';
            else
                clearInputBuffer();
            break;
        }
        else
        {
            printf("Data de expirare invalida! Va rog sa introduceti o data de expirare valida (format MM/YY)!\n");
        }
    }
        
    while(1)
    {
        printf("CCV: ");
        if (fgets(nou->ccv, 4, stdin) != NULL && isValidCCV(nou->ccv)) 
        {
            size_t len = strlen(nou->ccv);
            if (len > 0 && nou->ccv[len - 1] == '\n')
                nou->ccv[len - 1] = '\0';
            else
                clearInputBuffer();
            break;
        }
        else
        {
            printf("CCV invalid! Va rog sa introduceti un ccv format din 3 cifre valid!\n");
        }
    }
    
    nou->utilizari = 0;
    
    printf("\nNume: %s\nNumar: %s\nData: %s\nCCV: %s\n\n", nou->nume, nou->numar, nou->expira, nou->ccv);
    
    Card **nouaLista = realloc(*lista, ((*size) + 1) * sizeof(Card*));
    if (!nouaLista)
    {
        printf("Eroare la realocarea memoriei!\n");
        free(nou);
        return;
    }
    
    nouaLista[*size] = nou;
    *lista = nouaLista;
    (*size)++;
    
    printf("Card adaugat cu succes!\n");
    printf("Pentru a reveni la meniu, apasati orice tasta.\n");
    _getch();
}

void  Delete(Card **lista, int* size, int selected_option)
{
    free(lista[selected_option]);

    for(int i=selected_option; i<=(*size)-1; i++)
        lista[i]=lista[i+1];

    Card **nouaLista = realloc(*lista, (*size - 1) * sizeof(Card*));
    if (nouaLista || *size - 1 == 0)  
        lista = nouaLista;
    
    (*size)--;
    printf("Card sters cu succes!\n");

}

void deleteCard(Card **lista, int *size)
{
    printf("\t STERGERE CARD\n");
    printf("Deplasarea indicatorilor (> <) se face cu tastele W - sus si S - jos.\n");
    printf("Pentru selectarea cardului dorit, apasati Enter.\n\n");
    printf("Alegeti cardul pe care doriti sa il stergeti din lista de mai jos:\n");
    
    char direction;
    int selected_option = 0;
    int exit_index = *size;  

    while (1) {
        screenClear();   
        printf("\t Sistem Wallet electronic\n\n");
        printf("Deplasarea indicatorilor (> <) se face cu tastele W - sus si S - jos.\n");
        printf("Pentru selectarea cardului dorit, apasati Enter.\n\n");
        printf("\t %s \n", "STERGERE CARD");

        for (int i = 0; i < *size; i++) {
            if (i == selected_option)
                printf("\033[1m > Card %d - **** \033[37m%c%c%c%c\033[0m < \033[0m \n", i,
                       lista[i]->numar[12], lista[i]->numar[13],
                       lista[i]->numar[14], lista[i]->numar[15]);
            else
                printf("   Card %d - **** \033[33m%c%c%c%c\033[0m \n", i,
                       lista[i]->numar[12], lista[i]->numar[13],
                       lista[i]->numar[14], lista[i]->numar[15]);
        }
        if (selected_option == exit_index)
            printf(" \033[1m> Iesire <\033[0m \n");
        else 
            printf("   Iesire  \n");

        direction = _getch();

        if (direction != ENTER) { 
            if (direction == UP && selected_option > 0)
                selected_option--;
            if (direction == DOWN && selected_option < exit_index)
                selected_option++;
        } else {
            if (selected_option < exit_index) {
                screenClear();    
                printf("Ai selectat: Cardul %d\n", selected_option);
                Delete(lista, size, selected_option);
                printf("Pentru a reveni la meniu, apasati orice tasta.\n");
                _getch(); 
                mergeSort(lista, 0, (*size) - 1); 
            } else {
                break;
            }
        }
    }    
}

void salvare(const char *file, Card **lista, int nr_cards) 
{
    FILE *fisier = fopen(file, "w");
    if (fisier == NULL) {
        printf("Eroare la deschiderea fisierului pentru salvare!\n");
        return;
    }

    fprintf(fisier, "%d\n", nr_cards);

    for (int i = 0; i < nr_cards; i++) 
    {
        fprintf(fisier, "%s\n", lista[i]->numar);
        fprintf(fisier, "%s\n", lista[i]->ccv);
        fprintf(fisier, "%s\n", lista[i]->nume);
        fprintf(fisier, "%s\n", lista[i]->expira);
        fprintf(fisier, "%d\n", lista[i]->utilizari);
    }

    fclose(fisier);
    printf("Datele au fost salvate cu succes in fisierul %s.\n", file);
}
