#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Diccionario.h"
#include "TADListaDL.h"

void initialize_dic(lista dictionary[size_dic])
{
    setlocale(LC_ALL, "");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8); // Consola Windows en UTF-8
#endif
    for (int i = 0; i < size_dic; i++)
    {
        Initialize(&dictionary[i]);
    }
}

int hash_A(char word[word_max])
{
    int c = 0;
    int i = 0;
    while (word[i] != '\0')
    {
        c += word[i];
        i++;
    }
    return c % hash_base;
}

int hash_B(char word[word_max])
{
}

void read_document(lista dictionary[size_dic], char filename[], hash_selector s)
{
    if (freopen(filename, "r", stdin) == NULL)
    {
        printf("Error:\nread_document(dictionary,filename,s): Archivo no existe");
        exit(-1);
    }
    while (read_word(dictionary, s))
    {
    }
#ifdef _WIN32
    freopen("CON", "r", stdin);
#else
    freopen("/dev/tty", "r", stdin);
#endif
    return;
}

int read_word(lista dictionary[size_dic], hash_selector s)
{
    int i = 0;
    int j = 0;
    int c;
    elemento e;
    do
    {
        c = getchar();
        if (c == EOF)
            return 0;
    } while (c == '\n');

    while (c != ':' && c != EOF && i < word_max - 1)
    {
        e.word[i++] = (char)c;
        c = getchar();
    }
    e.word[i] = '\0';
    if (c == EOF)
    {
        return 0;
    }
    while (c == ' ' || c == '\t')
        c = getchar();

    while (c != '\n' && c != EOF && j < def_max - 1)
    {
        e.def[j++] = (char)c;
        c = getchar();
    }
    e.def[j] = '\0';

    if (s == 'A')
    {
        Add(&dictionary[hash_A(e.word)], e);
    }
    else if (s == 'B')
    {
        Add(&dictionary[hash_B(e.word)], e);
    }
    return 1;
}

void search_word(char word[101], lista dictionary[size_dic], hash_selector s)
{
    int i = 0;
    int j = 0;
    posicion wposition;
    int h;
    if (s == 'A')
    {
        h = hash_A(word);
    }
    else if (s == 'B')
    {
        h = hash_B(word);
    }
    wposition = Search(&dictionary[h], word);
    if (ValidatePosition(&dictionary[h], wposition))
    {
        while (wposition->e.word[i] != '\0')
        {
            printf("%c", wposition->e.word[i]);
            i++;
        }
        printf(": ");
        while (wposition->e.def[j] != '\0')
        {
            printf("%c", wposition->e.def[j]);
            j++;
        }
        printf(".\n");
    }
    else
    {
        printf("La palabra %s ", word);
        printf("no existe en el diccionario");
    }
    return;
}

void def_change(char word[101], lista dictionary[size_dic], hash_selector s)
{
    int i = 0;
    int j = 0;
    char c;
    posicion wposition;
    int h;
    if (s == 'A')
    {
        h = hash_A(word);
    }
    else if (s == 'B')
    {
        h = hash_B(word);
    }
    wposition = Search(&dictionary[h], word);
    if (ValidatePosition(&dictionary[h], wposition))
    {
        while ((c = getchar()) != '\n' && c != EOF)
            ;
        fgets(wposition->e.def, def_max, stdin);
        wposition->e.def[strcspn(wposition->e.def, "\n")] = '\0';
        printf("\nLa definición de %s", word);
        printf(" se ha actualizado con éxito");
    }
    else
    {
        printf("La palabra %s ", word);
        printf("no existe en el diccionario");
    }
    return;
}

void del_word(char word[word_max], lista dictionary[size_dic], hash_selector s)
{
    int i = 0;
    int j = 0;
    char c;
    posicion wposition;
    int h;
    if (s == 'A')
    {
        h = hash_A(word);
    }
    else if (s == 'B')
    {
        h = hash_B(word);
    }
    wposition = Search(&dictionary[h], word);
    if (ValidatePosition(&dictionary[h], wposition))
    {
        Remove(&dictionary[h], wposition);
    }
    else
    {
        printf("La palabra %s ", word);
        printf("no existe en el diccionario");
    }
    return;
}

void hash_statics(lista dictionary[size_dic])
{
    int max_size = 0;
    int min_size = size_dic;
    int empty_buckets = 0;
    int total_elements = 0;
    int collisions = 0;

    for (int i = 0; i < size_dic; i++)
    {
        int size = Size(&dictionary[i]);
        total_elements += size;
        if (size > max_size)
            max_size = size;
        if (size < min_size)
            min_size = size;
        if (size == 0)
            empty_buckets++;
        if (size > 1)
            collisions += (size - 1); // Colisiones en este bucket
    }

    printf("Total de palabras: %d\n", total_elements);
    printf("Tamaño máximo de bucket: %d\n", max_size);
    printf("Tamaño mínimo de bucket: %d\n", min_size);
    printf("Buckets vacíos: %d (%.1f%%)\n", empty_buckets, (empty_buckets * 100.0) / size_dic);
    printf("Total de colisiones: %d\n", collisions);
    printf("Factor de carga: %.2f\n", (float)total_elements / size_dic);
    printf("Promedio por bucket: %.2f\n", (float)total_elements / size_dic);
}

void destroy_dic(lista dictionary[])
{
    for (int i = 0; i < size_dic; i++)
    {
        Destroy(&dictionary[i]);
    }
}
