/*
26. Найти абитуриентов-немедалистов, суммарный балл которых выше среднего.
*/

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "person.h"

int main(int argc, char *argv[]) {

    int sum = 0, cnt = 0, avg = 0;
    Person p;
    FILE *file = NULL;

    if (argc < 3) {

        printf("Использование: %s [файл] [флаг]\nФлаги:\n-f - печать базы данных\n-p - выполнить задание\n", argv[0]);

        return 1;
    }

    file = fopen(argv[1], "rb");

    if (file == NULL) {

        printf("Произошла ошибка при открытии файла\n");

        return 1;
    }

    if (!strcmp(argv[2], "-f")) {

        printf("╔════════════╦══════════╦═══════╦═══════╦════════╦═════╦═════╦═════╦═══════════╗\n");
        printf("║  Фамилия   ║ Инициалы ║  Пол  ║ Школа ║ Медаль ║ Мат ║ Инф ║ Рус ║ Сочинение ║\n");
        while (fread(&p, sizeof(p), 1, file) == 1) {
            
            printf("╟════════════╬══════════╬═══════╬═══════╬════════╬═════╬═════╬═════╬═══════════║\n");
            printf("║%12s║%10s║%7s║%7d║%8s║%5d║%5d║%5d║%11s║\n",
                p.surname,
                p.ini,
                p.gender == 'm' ? "Male" : "Female",
                p.schoolNum,
                p.medal == 'y' ? "Yes" : "No",
                p.math,
                p.inf,
                p.rus,
                p.comp == 'y' ? "Yes" : "No"
            );
        

        }
        printf("╚════════════╩══════════╩═══════╩═══════╩════════╩═════╩═════╩═════╩═══════════╝\n");
    }
    else if (!strcmp(argv[2], "-p")) {
        while(fread(&p, sizeof(p), 1, file) == 1) {

            sum += p.math + p.inf + p.rus;
            cnt++;

        }

        avg = (int)round((double)sum / cnt);

        printf("Абитуриенты без медалей и у которых суммарный балл выше среднего:\n");

        fseek(file, 0, SEEK_SET);

        while (fread(&p, sizeof(p), 1, file) == 1) {

            sum = p.math + p.inf + p.rus;

            if (p.medal == 'n' && sum > avg)
                printf("%s %d\n", p.surname, sum);
        }
    }
        
    fclose(file);

    return 0;
}