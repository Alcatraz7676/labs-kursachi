#include <stdio.h>
#include <math.h>
#include "vector.c"

typedef enum _kInd {

	END = -3,
	NUM,
	EMPTY
} kInd;

double module(const double num);
void printSourceMatrix(const int *rows, const Vector *cols, const int m, const int n);
void printInnerMatrix(const int *rows, const Vector *cols, const int m);

int main(void) {

	const int N = 100;
	int m, n, i, j, isEmptyRow, lastInd, cnt, row[N], maxCols[N];
	double maxNum, tmpNum;
	Vector col;
	Item tmpItem;

	for (i = 0; i < N; i++)
		maxCols[i] = 0;

	vectorCreate(&col, 0);

	for (i = 0; i < N; i++)
		row[i] = END;

	printf("Введите количество строк: ");
	scanf("%d", &m);
	printf("Введите количество столбцов: ");
	scanf("%d", &n);

	if (m < 1 || m > N) {

		printf("Количество строк должно быть в диапозоне от 1 до %d\n", N);

		return 0;
	}

	if (n < 1 || n > N) {

		printf("Количество столбцов должно быть в диапозоне от 1 до %d\n", N);

		return 0;
	}

	for (i = 0; i < m; i++) {

		isEmptyRow = 1;

		for (j = 0; j < n; j++) {

			printf("Введите число для ячейки [%d][%d]: ", i, j);
			scanf("%lf", &tmpNum);

			if (tmpNum == 0.0)
				continue;

			isEmptyRow = 0;

			if (row[i] == END)
				row[i] = vectorSize(&col);

			tmpItem.ind = j;

			vectorPushBack(&col, tmpItem);

			tmpItem.ind = NUM;
			tmpItem.num = tmpNum;

			vectorPushBack(&col, tmpItem);
		}

		if (isEmptyRow)
			row[i] = EMPTY;
		else {

			tmpItem.ind = EMPTY;

			vectorPushBack(&col, tmpItem);
		}
	}

	tmpItem.ind = END;

	vectorPushBack(&col, tmpItem);

	printf("Обычное представление:\n");
	printSourceMatrix(row, &col, m, n);
	printf("Внутреннее представление\n");
	printInnerMatrix(row, &col, m);

	maxNum = 0.0;
	
	for (i = 0; i < m; i++) {
		
		if (row[i] == EMPTY)
			continue;

		for (j = row[i]; j < vectorLoad(&col, j).ind != END && vectorLoad(&col, j).ind != EMPTY; j++) {

			if (vectorLoad(&col, j).ind != NUM)
				continue;

			if (module(vectorLoad(&col, j).num) > module(maxNum))
				maxNum = vectorLoad(&col, j).num;
		}
	}
	
	printf("Максимальное число по модулю: (%.2lf), модуль равен: %.2lf\n", maxNum, module(maxNum));

	if (maxNum == 0.0) {

		printf("Делить на него нельзя, так как его модуль равен нулю\n");

		return 0;
	}
	
	cnt = 0;

	for (i = 0; i < vectorSize(&col); i++) {

		tmpItem = vectorLoad(&col, i);

		if (tmpItem.ind == NUM && module(maxNum) == module(tmpItem.num)) {
			
			lastInd = vectorLoad(&col, i - 1).ind;
			maxCols[lastInd] = 1;
			cnt++;
		}
	}

	if (cnt > 1)
		for (i = lastInd - 1; i >= 0; i--) {
			if (maxCols[i]) {

				lastInd = i;

				break;
			}
		}
	for (j = 0; j < vectorSize(&col); j++) {

		tmpItem = vectorLoad(&col, j);

		if (tmpItem.ind == lastInd) {

			tmpItem = vectorLoad(&col, j + 1);
			tmpItem.num = tmpItem.num/maxNum;

			vectorSave(&col, j + 1, tmpItem);
		}
	}

	printf("Обычное представление после преобразования:\n");
	printSourceMatrix(row, &col, m, n);
	printf("Внутреннее представление после преобразования:\n");
	printInnerMatrix(row, &col, m);

	vectorDestroy(&col);

	return 0;
}

double module(const double num) {

	return sqrt(pow(num, 2.0));
}

void printSourceMatrix(const int *rows, const Vector *cols, const int m, const int n) {

	int i, j, k;

	for (i = 0; i < m; i++) {

		if (rows[i] == EMPTY) {

			for (j = 0; j < n; j++)
				printf("(%.2lf) ", 0.0);

			printf("\n");

			continue;
		}

		k = 0;
		j = rows[i];

		while (k < n) {

			if (vectorLoad(cols, j).ind == EMPTY) {

				printf("(%.2lf) ", 0.0);

				k++;

				continue;
			}

			while (k < vectorLoad(cols, j).ind) {

				printf("(%.2lf) ", 0.0);

				k++;
			}

			printf("(%.2lf) ", vectorLoad(cols, j + 1).num);
			
			j += 2;
			k++;
		}

		printf("\n");
	}
}

void printInnerMatrix(const int *rows, const Vector *cols, const int m) {

	int i, j;

	printf("Массив M:\n");

	for (i = 0; i < m; i++)
		printf("%d ", rows[i]);

	printf("\nМассив A:\n");

	if (vectorLoad(cols, 0).ind == END) {

		printf("Пуст\n");

		return;
	}

	for (i = 0; vectorLoad(cols, i).ind != END; i++) {

		if (vectorLoad(cols, i).ind == NUM)
			printf("(%.2lf) ", vectorLoad(cols, i).num);
		else
			printf("%d ", vectorLoad(cols, i).ind);
	}
	printf("\n");
}