#include <stdio.h>
#include <stdlib.h>
#include "list.c"
void menu(){
    printf("\n 1 - Добавить строку по номеру элемента\n");
    printf(" 2 - Удалить строку по номеру\n");
    printf(" 3 - Удалить строку по значению\n");
    printf(" 4 - Длина списка\n");
    printf(" 5 - Печать списка\n");
    printf(" 6 - Попарный обмен значений списка(задание)\n");
    printf(" 7 - Удалить список\n");
    printf(" 8 - Очистить экран\n");
    printf(" 9 - Выход\n");
}

int main(){
    int Key, val;
    char data[25];
    list* L = NULL;
    list* t;
    menu();
    while (1) {
        printf("\nВыберите действие: ");
        scanf("\n%d", &Key);
        switch(Key) {
            case 1:

                printf("\nВведите номер и значение: ");
                scanf("\n%d %s", &val, data);
                val--;

                if (val > list_len(L)) {
                    printf("\nНеправильный номер\n"); break;
                    }

                L = insert(L, val, data);
                break;

            case 2:

                if(list_is_empty(L)) {
                    printf("\nСписок пуст\n");
                    break;
                }
                printf("\nВведите номер: ");
                scanf("%d", &val);
                val--;

                if (val >= list_len(L)) {
                    printf("\nНеправильный номер\n");
                    break;
                }
                    
                L = del_index(L,val);
                break;

            case 3:

                if(list_is_empty(L)) {
                    printf("\nСписок пуст\n");
                    break;
                }
                printf("\nВведите значение: ");
                scanf("\n%s", data);
                L = del_value(L,data);
                break;

            case 4:

                printf("\nДлина списка: %d\n", list_len(L));
                break;

            case 5:

                if (print_list(L)) printf("\nСписок пуст\n");
                break;

            case 6:
                if(list_is_empty(L)) {
                    printf("\nСписок пуст\n");
                    break;
                }
                swap_data(L);
                break;

            case 7:
                if(list_is_empty(L)) {
                    printf("\nСписок уже пуст\n");
                    break;
                }
                L = del_list(L);
                break;

            case 8:

                system("clear");
                menu();
                break;

            case 9:

                printf("\n");
                return 0;
                break;

            default:

                printf("Такое действие отсутствует! \n");
                break;
         }
    }
    return 0;
}