#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define _CRT_SECURE_NO_WARNINGS

char* readline();
char** split_string(char* str);

struct node_t;
typedef struct node_t node;
typedef node* pnode; 

struct node_t {
    char *word;
    struct node_t *next_node; 
};

void print_node(char *txt, pnode node_pointer)
{
    printf("%s : ", txt);
    pnode head = node_pointer;
    while (head) 
    {
        printf("%s->", head->word);
        head = head->next_node;
    }
    printf("\n");
}


//typedef struct node_t *pnode;
pnode create_node(char *new_word) {
    if( new_word == NULL ) {
        return NULL;
    }
    pnode new_node = (pnode)malloc(sizeof(node));
    if (new_node == NULL){
        return NULL;
    }
    new_node->word = new_word;
    new_node->next_node = NULL;
    
    return new_node;
}


pnode create_list(char **text , int word_num ){
    if (text == NULL){ // not sure if *text or text
        return NULL;
    }
    pnode head = NULL;
    pnode curr_node = NULL;
    for ( int i=0; i < word_num; i++){
        if( i==0){
            head = create_node(text[i]);
            if ( head == NULL){
                return NULL;
            }
            curr_node = head;
        }
        else {
            curr_node->next_node = create_node(text[i]);
            if ( curr_node->next_node == NULL){
                return NULL;//reach here in case malloc didn't work should i free the memory and exit the program ?
            }
            curr_node = curr_node->next_node; 
        }
    }
    return head;        
}

/*
// the function returns true if the word exists in the list, and then it frees the words node. otherwise it returns false.
bool does_word_exist(char *check_word, pnode list ){
    if (check_word == NULL || list == NULL){
        return false;
    }
    printf("\n\nn does_word_exist\n");
    pnode curr_node = list;
    pnode prev_node = list;
    bool first_iteration = true;
    while (curr_node != NULL){

        print_node("while begin prev_node", prev_node);
        print_node("while begin curr_node", curr_node);

        if(strcmp(check_word, curr_node->word)==0){
            printf("in the strcmp\n");
            //if there is a match with the first word
            if (first_iteration){
                tmp = curr_node;
                list = curr_node->next_node;
                print_node("curr_node", curr_node);
                free(tmp);
                return true;

            }
            //if there is a match with a regular node 
            else if (curr_node->next_node != NULL){
                tmp = curr_node;
                prev_node->next_node = curr_node->next_node;
                print_node("curr_node middle", curr_node);
                free(tmp);
                return true;
            }
            // if there  is a match with the last word 
            else {
                tmp = curr_node;
                prev_node->next_node = NULL;
                free(tmp);
                return true;
            }
        }
        else{
            if(first_iteration){
                curr_node = curr_node->next_node;
                first_iteration = false;
            }
            else{
                print_node("last prev_node", prev_node);
                prev_node = prev_node->next_node;
                curr_node = curr_node->next_node;
            }
        }
    }
    return false;
}*/


bool does_word_exist(char *check_word, node **list)
{
    if (check_word == NULL || list == NULL){
        return false;
    }
    
    node *curr_node = *list;
    node *temp_node;

    if(strcmp(check_word, curr_node->word) ==0)
    {
        //if there is a match with the first word
        temp_node = curr_node;
        *list = curr_node->next_node;
        free(temp_node);
        return true;
    }

    while (curr_node->next_node)
    {
        if(strcmp(check_word, curr_node->next_node->word)==0)
        {
            temp_node = curr_node->next_node;
            curr_node->next_node = curr_node->next_node->next_node;
            free(temp_node);
            return true;
        }
        curr_node = curr_node->next_node;  
    }   


    if(strcmp(check_word, curr_node->word) ==0)
    {
        //if there is a match with the first word
        temp_node = curr_node;
        *list = curr_node->next_node;
        free(temp_node);
        return true;
    }
    return false;
}




    
void free_list(pnode list){
    if (list == NULL){
        return;
    }
    pnode curr_node = list;
    pnode temp = NULL;
    while (curr_node != NULL){
        temp = curr_node;
        curr_node = curr_node->next_node;
        free(temp);
    }    
}


// Complete the checkMagazine function below.
void checkMagazine(int magazine_count, char** magazine, int note_count, char** note) {
    if (magazine == NULL || note == NULL){
        return;
    }
    pnode magazine_list = create_list(magazine, magazine_count);
    if (magazine_list == NULL){
        return;
    }
    pnode note_list = create_list(note, note_count);
    if (note_list == NULL){
        return;
    }
    pnode curr_node = note_list;

    print_node("magazine_list", magazine_list);
    print_node("note_list", note_list);
    printf("\n\n");
    while(curr_node != NULL){
        print_node("entering curr_node",curr_node);
        print_node("magazine_list", magazine_list);

        if( does_word_exist(curr_node->word, &magazine_list) == false ){
            printf("No");
            free_list(magazine_list);
            free_list(note_list);
            return;
        }
        printf("\n");
        curr_node = curr_node->next_node;

    }
    printf("Yes");
    free_list(magazine_list);
    free_list(note_list);
}




int main()
{
    char** mn = split_string(readline());

    char* m_endptr;
    char* m_str = mn[0];
    int m = strtol(m_str, &m_endptr, 10);

    if (m_endptr == m_str || *m_endptr != '\0') { exit(EXIT_FAILURE); }

    char* n_endptr;
    char* n_str = mn[1];
    int n = strtol(n_str, &n_endptr, 10);

    if (n_endptr == n_str || *n_endptr != '\0') { exit(EXIT_FAILURE); }

    char** magazine_temp = split_string(readline());

    char** magazine = malloc(m * sizeof(char*));

    for (int i = 0; i < m; i++) {
        char* magazine_item = *(magazine_temp + i);

        *(magazine + i) = magazine_item;
    }

    int magazine_count = m;

    char** note_temp = split_string(readline());

    char** note = malloc(n * sizeof(char*));

    for (int i = 0; i < n; i++) {
        char* note_item = *(note_temp + i);

        *(note + i) = note_item;
    }

    int note_count = n;

    checkMagazine(magazine_count, magazine, note_count, note);

    return 0;
}

char* readline() {
    size_t alloc_length = 1024;
    size_t data_length = 0;
    char* data = malloc(alloc_length);

    while (true) {
        char* cursor = data + data_length;
        char* line = fgets(cursor, alloc_length - data_length, stdin);

        if (!line) {
            break;
        }

        data_length += strlen(cursor);

        if (data_length < alloc_length - 1 || data[data_length - 1] == '\n') {
            break;
        }

        alloc_length <<= 1;

        data = realloc(data, alloc_length);

        if (!line) {
            break;
        }
    }

    if (data[data_length - 1] == '\n') {
        data[data_length - 1] = '\0';

        data = realloc(data, data_length);
    } else {
        data = realloc(data, data_length + 1);

        data[data_length] = '\0';
    }

    return data;
}

char** split_string(char* str) {
    char** splits = NULL;
    char* token = strtok(str, " ");

    int spaces = 0;

    while (token) {
        splits = realloc(splits, sizeof(char*) * ++spaces);

        if (!splits) {
            return splits;
        }

        splits[spaces - 1] = token;

        token = strtok(NULL, " ");
    }

    return splits;
}
