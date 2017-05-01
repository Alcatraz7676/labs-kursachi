#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "string.h"

list* first = NULL;

list* new_list(char data[]) {

    list* new_L = (list*)malloc(sizeof(list));
    strcpy(new_L->data, data);
    first = new_L;
    new_L->next = first;
    return new_L;
}

list* new_element(char data[], list* next) {

    list* new_El = (list*)malloc(sizeof(list));
    strcpy(new_El->data, data);
    new_El->next = next;
    return new_El;
}

list* last_element(list* L) {

    while (L->next != first) L = L->next;
    return L;
}

int list_is_empty(list* L) {

    if (L == NULL) return 1;
    return 0;
}

int list_len(list* L) {

    if (list_is_empty(L)) return 0;

    int len = 0;

    do {
        len++;
        L = L->next;
    } while(L != first);

    return len;
}

list* insert(list* L, int index, char data[]) {

    if (list_is_empty(L)) return new_list(data);

    if (!index) {
        list* new_El = new_element(data, first);
        list* last = last_element(L);
        last->next = new_El;
        first = new_El;
    }
    else if (index < list_len(L)) {

        int i;
        for (i = 0 ; i < index-1 ; i++ ) L = L->next;

        list* new_El = new_element(data,L->next);
        L->next = new_El;
    }
    else if (index == list_len(L)) {

        list* last = last_element(L);
        list* new_El = new_element(data, first);
        last->next = new_El;
    }

    return first;
}

list* del_index(list* L, int index) {

    if ((!index) && (list_len(L) == 1)) {

        free(L);
        first = NULL;
    }
    else if (!index) {

       list* last = last_element(L);
       last->next = L->next;
       first = last->next;
       free(L);
    }
    else if (index < list_len(L)) {

        int i;
        for (i = 0 ; i < index-1 ; i++ ) L = L->next;

        list* tmp = L->next;
        L->next = tmp->next;
        free(tmp);
    }

    return first;
}

list* del_value(list* L, char val[]) {

    int index = 0;
    int len = list_len(L);

    while (strcmp(L->data, val) != 0) {

        index++;
        L = L->next;
        if (index >= len) {
            printf("\nТакого значения в списке нет\n");
            return first;
        }
    }

    first = del_index(first, index);
    printf("\nСтрока успешно удалена\n");
    return first;
}

list* del_list(list* L) {

    if (!list_is_empty(L)) {

        list* tmp;

        while (L->next != first) {

            tmp = L->next;
            free(L);
            L = tmp;
        }

        free(L);
        first = NULL;
    }

    return NULL;
}

void swap_data(list* L) {

    char tmp[25];
    int len = list_len(L);
    int i;

    for (i = 0 ; i < len/2 ; i++ ) {

        strcpy(tmp, L->data);
        strcpy(L->data, L->next->data);
        strcpy(L->next->data, tmp);
        L = L->next->next;
    }
}

int print_list(list* L) {

    int i = 1;

    if (list_is_empty(L)) return 1;

    do {
        printf(" %s [%d]", L->data, i);
        i++;
        L = L->next;
    } while (L != first);

    printf("\n");
    return 0;
}