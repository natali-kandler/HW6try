#include <assert.h>
#include <limits.h>
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* readline();
char** split_string(char*);

typedef struct node_t {
  char *word;
  struct node_t *next_node;
} *pnode_t, node_t;

pnode_t create_node(char *new_word);
pnode_t create_list(char **text, int word_num);
bool does_word_exist(char *check_word, node_t **list);
void free_list(pnode_t list);

/**
 * creating a new node; memory allocation for both the word and the node itself.
 * @param new_word: (from the original string).
 * @return pointer to this node.
 */
pnode_t create_node(char *new_word) {
  if (new_word == NULL) {
    return NULL;
  }
  pnode_t new_node = (pnode_t) malloc(sizeof(node_t));
  if (new_node == NULL) {
    return NULL;
  }
  new_node->word = (char *) malloc(sizeof(char) * (strlen(new_word) + 1));
  if (new_node->word == NULL) {
    free(new_node);
    return NULL;
  }
  strcpy(new_node->word, new_word);
  new_node->next_node = NULL;

  return new_node;
}
/**
 * creating a new list and putting the words string in each node.
 * @param text: string of words.
 * @param word_num: amount of words in the string.
 * @return pointer to the beginning of the list.
 */
pnode_t create_list(char **text, int word_num) {
  if (text == NULL) {
    return NULL;
  }
  pnode_t head = NULL;
  pnode_t curr_node = NULL;
  for (int i = 0; i < word_num; i++) {
    if (i == 0) {
      head = create_node(text[i]);
      if (head == NULL) {
        return NULL;
      }
      curr_node = head;
    } else {
      curr_node->next_node = create_node(text[i]);
      if (curr_node->next_node == NULL) {
        free_list(head);
        return NULL;
      }
      curr_node = curr_node->next_node;
    }
  }
  return head;
}
/**
 * check's if a word (parameter) exists in a list. if it does the containing
 *  node is deleted.
 * @param check_word: word to check if exists in the list.
 * @param list: list of nodes.
 * @return true if the word exists, false otherwise.
 */
bool does_word_exist(char *check_word, node_t **list) {
  if (check_word == NULL || list == NULL) {
    return false;
  }

  node_t *curr_node = *list;
  node_t *temp_node = NULL;

  //check's if there is a match with the FIRST node.
  if (strcmp(check_word, curr_node->word) == 0) {
    temp_node = curr_node;
    *list = curr_node->next_node;
    free(temp_node->word);
    free(temp_node);
    return true;
  }

  while (curr_node->next_node) {

    if (strcmp(check_word, curr_node->next_node->word) == 0) {
      temp_node = curr_node->next_node;
      curr_node->next_node = curr_node->next_node->next_node;
      free(temp_node->word);
      free(temp_node);
      return true;
    }
    //check's if there is a match with the LAST node.
    if (curr_node->next_node->next_node == NULL) {
      pnode_t temp_last = curr_node->next_node;

      if (strcmp(check_word, temp_last->word) == 0) {
        temp_node = temp_last;
        curr_node->next_node = NULL;
        free(temp_node->word);
        free(temp_node);
        return true;
      }
    }
    curr_node = curr_node->next_node;
  }
  return false;
}
/**
 * free's up the memory of an entire list. also free's up each node's word
 * memory.
 * @param list: the list to free its memory.
 */
void free_list(pnode_t list) {
  if (list == NULL) {
    return;
  }
  pnode_t curr_node = list;
  pnode_t temp = NULL;
  while (curr_node != NULL) {
    temp = curr_node;
    curr_node = curr_node->next_node;
    free(temp->word);
    free(temp);
  }
}

// Complete the checkMagazine function below.
void checkMagazine(int magazine_count,
                   char **magazine,
                   int note_count,
                   char **note) {
  if (magazine == NULL || note == NULL) {
    return;
  }
  if (magazine_count < note_count) {
    printf("No");
    return;
  }
  pnode_t magazine_list = create_list(magazine, magazine_count);
  if (magazine_list == NULL) {
    return;
  }
  pnode_t note_list = create_list(note, note_count);
  if (note_list == NULL) {
    free_list(magazine_list);
    return;
  }
  pnode_t curr_node = note_list;

  while (curr_node != NULL) {

    if (does_word_exist(curr_node->word, &magazine_list) == false) {
      printf("No");
      free_list(magazine_list);
      free_list(note_list);
      return;
    }
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
