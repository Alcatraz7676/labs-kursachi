#include <stdio.h>
#include "person.h"

int readPerson(Person *p);

int main(int argc, char *argv[]) {

	
	if (argc != 2) {

		printf("Использование: %s [файл]\n", argv[0]);
		return 1;
	}

	Person p;
	FILE *file = fopen(argv[1], "wb");

	if (file == NULL) {

		printf("Произошла ошибка при открытии файла\n");
		return 1;
	}

	while (readPerson(&p))
		fwrite(&p, sizeof(p), 1, file);

	fclose(file);

	return 0;
}

int readPerson(Person *p) {
	
	int ret = scanf("%s %s %c %d %c %d %d %d %c",
		p->surname,
		p->ini,
		&p->gender,
		&p->schoolNum,
		&p->medal,
		&p->math,
		&p->inf,
		&p->rus,
		&p->comp
	);
	
	return (ret == 9);
}