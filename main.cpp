#include <stdio.h>
#include <list.h>
#include <my_hash_functions.h>
#include <windows.h>

const int Table_Size = 517;
const int Max_word_size = 256;

struct Hashtable{
    struct List** table;
    unsigned int (*hashfunc)(const char*, int);
    int size;
};


Hashtable* Hashtable_Construct(int , unsigned int (*hashfunc)(const char*, int));
void Hashtable_Deconstruct(Hashtable* hashtable);
void Make_First_Line(FILE* flin);
bool Fill_Hash_Line(FILE* flout,Hashtable* hashtable, const char*);
void Make_Hashtable(Hashtable* hashtable,FILE* flin);


#define MAKE_NEW_LINE(whatfunc, name)                                                                            \
hashtable = Hashtable_Construct(Table_Size, (unsigned int(*)(const char*, int)) whatfunc);           \
if (!hashtable) {                                                                                                     \
    printf("cant create hashtable " );                                                                                        \
    abort();                                                            \
}         \
printf("Begin to make hashtable %s\n", name);      \
Make_Hashtable(hashtable, flin);         \
printf("end to make hashtable\n");\
Fill_Hash_Line(flout, hashtable, name);                                 \
Hashtable_Deconstruct(hashtable); printf("created\n");

int main() {
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    FILE* flout = fopen("results.csv", "wt");

    FILE* flin = fopen("text.txt", "rt");



    Hashtable* hashtable = {};

    Make_First_Line(flout);
    MAKE_NEW_LINE(Simple_hash, "Simple")
    MAKE_NEW_LINE(First_letter, "First")
    MAKE_NEW_LINE(String_len, "Length")
    MAKE_NEW_LINE(Char_Sum, "Charsum")
    MAKE_NEW_LINE(Recur, "Recuroll")
    MAKE_NEW_LINE(MurMur_Hash, "MurMur")


    fclose(flin);
    fclose(flout);
    return 0;
}


 bool Fill_Hash_Line(FILE* flout, Hashtable* hashtable, const char* name){
     fprintf(flout, "%s;" , name);
     for ( int i = 0; i < Table_Size; i++) {
         fprintf(flout, "%d;", hashtable -> table[i] -> size);
     }
     fputc('\n', flout);
     return 1;
}

void Make_First_Line(FILE* flout) {
    fprintf(flout, "Lists;");
    for ( int i = 0; i < Table_Size; i++) {
        fprintf(flout, "%d;", i);
    }
    fputc('\n', flout);
}

int getFileLength (FILE* flin){
    fseek(flin, 0, SEEK_END);
    int length = ftell(flin);
    fseek(flin, 0 , SEEK_SET);
    return  length;
}

char * Create_buf(int len){
    return  (char*)calloc(len, sizeof(char));
}


void Make_Hashtable(Hashtable* hashtable, FILE* flin){
    char word[Max_word_size] = {};
    int position = 0;
    int wordlen = 0;

    int len = getFileLength(flin);
    char* buf = Create_buf(len);
     len = fread(buf, 1, len, flin);
    char* cur_pos = buf;
    // int word_counter = 0;

    while(sscanf( cur_pos , "%s%n", word, &position) && (cur_pos + position - buf < len) ){ //necessary \n after last line
        wordlen = strlen(word);
        List* list = hashtable -> table[(*(hashtable -> hashfunc))(word, wordlen) % Table_Size];
        cur_pos += position;
        if(!Find_By_Value(list , word)) {
            Insert_Tail(list, cur_pos - wordlen);
        }
     // printf("%d\n", ++word_counter);
    }

}


Hashtable* Hashtable_Construct(int size, unsigned int (*hashfunc)(const char*, int)){

    Hashtable *new_hashtable = (Hashtable *) calloc(1, sizeof(struct Hashtable));
    do{
        if(!new_hashtable)
            break;
        new_hashtable -> size = size;
        new_hashtable -> table = (List **) calloc(size,
                                                sizeof(List *)); //указатель на указатель на лист (создали массив указателей на списки)
        if(!new_hashtable -> table)
            break;
        for (int i = 0; i < size; i++) {
            new_hashtable -> table[i] = Create_List(2);
            if (!new_hashtable->table[i])
                break;
        }
        new_hashtable->hashfunc = hashfunc;
        return new_hashtable;
    } while (false);

    Hashtable_Deconstruct(new_hashtable);
    return nullptr;
}

void Hashtable_Deconstruct(Hashtable* hashtable){
    assert(hashtable);

    if(hashtable -> table)
        for (int i = 0; i < hashtable -> size; i++) {
            Deconstruct_List(hashtable->table[i]);
        }
    free(hashtable -> table);
    free(hashtable);
}