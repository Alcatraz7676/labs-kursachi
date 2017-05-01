#ifndef VECTOR_H
#define VECTOR_H

#include <stdlib.h>

typedef struct _Item {

	int ind;
	double num;

} Item;

typedef struct _Vector {

	Item *_data;
	int _size;
	int _capacity;

} Vector;

void vectorCreate(Vector *v, const int size);
int vectorEmpty(const Vector *v);
int vectorSize(const Vector *v);
int vectorCapacity(const Vector *v);
Item vectorLoad(const Vector *v, const int index);
void vectorSave(Vector *v, const int index, const Item value);
int vectorPushBack(Vector *v, const Item value);
void vectorDestroy(Vector *v);

#endif