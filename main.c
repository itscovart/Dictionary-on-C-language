#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif
#include "Diccionario.h"

void limpiar_pantalla();
void limpiar_buffer();
void menu_principal();

void limpiar_buffer()
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

void menu_principal()
{
    printf("         DICCIONARIO INTERACTIVO        \n");
    printf("1. Cargar archivo de definiciones\n");
    printf("2. Agregar palabra y su definición\n");
    printf("3. Buscar palabra\n");
    printf("4. Modificar definición\n");
    printf("5. Eliminar palabra\n");
    printf("6. Ver estadísticas hash\n");
    printf("7. Salir\n");
    printf("Selecciona una opción (1-7): ");
}

void limpiar_pantalla()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void imprimir_diccionario_archivo(lista dictionary[size_dic])
{
    FILE *archivo = fopen("diccionarioEjecucion.txt", "w");
    if (archivo == NULL)
    {
        printf("Error al crear el archivo diccionarioEjecucion.txt\n");
        return;
    }

    fprintf(archivo, "DICCIONARIO - ESTADO DE EJECUCIÓN\n");
    fprintf(archivo, "=================================\n\n");

    for (int i = 0; i < size_dic; i++)
    {
        if (!Empty(&dictionary[i]))
        {
            fprintf(archivo, "Hash [%d]\n", i);
            fprintf(archivo, "-----------------\n");

            posicion p = First(&dictionary[i]);
            while (p != NULL)
            {
                elemento e = Position(&dictionary[i], p);
                fprintf(archivo, "Palabra: %s\n", e.word);
                fprintf(archivo, "Definición: %s\n\n", e.def);

                p = Following(&dictionary[i], p);
            }

            fprintf(archivo, "\n");
        }
    }

    fclose(archivo);
    printf("Diccionario exportado correctamente a diccionarioEjecucion.txt\n");
}

int main(int argc, char const *argv[])
{
    lista dictionary[size_dic];
    hash_selector hash_func = 'A'; // Por defecto usa hash_A
    char filename[256];
    char palabra[word_max];
    char definicion[def_max];
    int opcion;

    setlocale(LC_ALL, "es_ES.UTF-8");
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    initialize_dic(dictionary);

    do
    {
        menu_principal();
        scanf("%d", &opcion);
        limpiar_buffer();

        switch (opcion)
        {
        case 1:
            printf("\nIngresa el nombre del archivo: ");
            fgets(filename, sizeof(filename), stdin);
            filename[strcspn(filename, "\n")] = '\0';

            printf("Selecciona la función hash (A/B) [A]: ");
            char hash_opt = getchar();
            limpiar_buffer();

            if (hash_opt == 'B' || hash_opt == 'b')
            {
                hash_func = 'B';
                printf("Usando función hash B\n");
            }
            else
            {
                hash_func = 'A';
                printf("Usando función hash A\n");
            }

            read_document(dictionary, filename, hash_func);
            printf("Archivo cargado exitosamente.\n");
            getchar();
            break;

        case 2:
            printf("\nIngresa la palabra: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            printf("Ingresa la definición: ");
            fgets(definicion, sizeof(definicion), stdin);
            definicion[strcspn(definicion, "\n")] = '\0';

            // Verificar si la palabra ya existe
            int h = (hash_func == 'A') ? hash_A(palabra) : hash_B(palabra);
            posicion existente = Search(&dictionary[h], palabra);

            if (ValidatePosition(&dictionary[h], existente))
            {
                printf("La palabra '%s' ya existe en el diccionario.\n", palabra);
                printf("¿Deseas reemplazar la definición? (s/n): ");
                char respuesta = getchar();
                limpiar_buffer();

                if (respuesta == 's' || respuesta == 'S')
                {
                    Replace(&dictionary[h], existente, (elemento){.word = "", .def = ""});
                    strcpy(existente->e.word, palabra);
                    strcpy(existente->e.def, definicion);
                    printf("Definición reemplazada exitosamente.\n");
                }
            }
            else
            {
                elemento nueva;
                strcpy(nueva.word, palabra);
                strcpy(nueva.def, definicion);
                Add(&dictionary[h], nueva);
                printf("Palabra agregada exitosamente.\n");
            }
            getchar();
            break;

        case 3:
            printf("\nIngresa la palabra a buscar: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            printf("\nResultado de búsqueda\n");
            search_word(palabra, dictionary, hash_func);
            getchar();
            break;

        case 4:
            printf("\nIngresa la palabra a modificar: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            printf("\nModificar definición\n");
            def_change(palabra, dictionary, hash_func);
            getchar();
            break;

        case 5:
            printf("\nIngresa la palabra a eliminar: ");
            fgets(palabra, sizeof(palabra), stdin);
            palabra[strcspn(palabra, "\n")] = '\0';

            printf("\nEliminar palabra\n");
            del_word(palabra, dictionary, hash_func);
            printf("\n");
            getchar();
            break;

        case 6:
            printf("\nEstadísticas del diccionario\n");
            hash_statics(dictionary);
            getchar();
            break;

        case 7: // Salir
            printf("\nSaliendo..\n");
            break;

        default:
            printf("\nOpción no válida. Por favor, selecciona una opción del 1 al 7.\n");
            break;
        }

        if (opcion != 7)
        {
            limpiar_pantalla();
        }

    } while (opcion != 7);

    // imprimir_diccionario_archivo(dictionary);
    destroy_dic(dictionary);

    return 0;
}