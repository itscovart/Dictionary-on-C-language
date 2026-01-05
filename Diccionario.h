#include <stdio.h>
#include <stdlib.h>
#include "TADListaDL.h"

#define word_max 101
#define def_max 281
#define hash_base 1117
#define size_dic 2000

typedef unsigned char hash_selector;

void initialize_dic(lista dictionary[size_dic]);

int hash_A(char word[101]);

int hash_B(char word[101]);

void read_document(lista dictionary[size_dic], char filename[], hash_selector s);

int read_word(lista dictionary[size_dic], hash_selector s);

void search_word(char word[word_max], lista dictionary[size_dic], hash_selector s);

void def_change(char word[word_max], lista dictionary[size_dic], hash_selector s);

void del_word(char word[word_max], lista dictionary[size_dic], hash_selector s);

void hash_statics(lista dictionary[size_dic]);

void destroy_dic(lista dictionary[]);