/*
  Овчинников М. В. 8О-103Б
  4. Упростить выражения, выполнив деление:
  		Пример: 4*a/2 -> 2*a
*/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "stack.h"
#include "stack.c"

typedef enum _side {

	NONE,
	LEFT,
	RIGHT
} side;

typedef struct _Node {

	char _varOp;
	int _num;
	struct _Node *_left;
	struct _Node *_right;

} Node;

// Функции непосредственно для задания:
int NOD(int a, int b);
Node *findDiv(Node **node, Node **startPos, Node **divNode, int *firstNumSide, int *anotherNumSide);
Node *findAnother(Node **node, Node **firstNumNode, int *firstNumSide, int *anotherNumSide);
int deleteOnesInNumerator(Node **node, Node **div);
void deleteEmptyNodes(Node **node);
int deleteVarOp(Node **root,Node **node,Node **prev);

// Функции для работы с деревом и стеком:
Node *treeNodeCreate(void);
Node *treeCopy(Node **node);
int treeIsMinusNode(Node **node);
void treeBuild(Node **node, Stack *st);
void treeDestroy(Node **node);
void treePowReduce(Node **node);
void PKL(Node **node, const int level);
void LKP(Node **node);
int isLetter(const char ch);
int isNumber(const char ch);
int isOp(const char ch);
int isOpHigh(const char op1, const char op2);
void postOrder(const char *str, Stack *st);

int main(void)
{
	int action;
	char expr[255];
	Node *root = NULL, *root2 = NULL;
	Stack stPost;

	while (1)
	{
		printf("Меню:\n");
		printf("1) Ввести выражение\n");
		printf("2) Печать исходного выражения\n");
		printf("3) Печать преобразованного выражения\n");
		printf("4) Печать исходного дерева\n");
		printf("5) Печать преобразованного дерева\n");
		printf("6) Выход\n");
		printf("Выберите действие: ");
		scanf("%d", &action);

		switch (action)
		{
			case 1:
			{
				printf("Введите выражение: ");
				scanf("%s", expr);

				treeDestroy(&root);
				treeDestroy(&root2);
				stackCreate(&stPost);
				postOrder(expr, &stPost);
				treeBuild(&root, &stPost);
				stackDestroy(&stPost);
				
				root2 = treeCopy(&root);

				Node *prev = NULL; // Указатель на предыдущий арифметический знак
				Node *node = root2;
				/*
				Переменные firstNumSide и secondNumSide принимают значения относительно
				расположения чисел к дроби. 0 если число не пренадлежит дроби, 1 если
				число на ходится в числителе, 2 если в знаменателе.
				*/
				int *firstNumSide = NULL;
				firstNumSide = malloc(sizeof(int));
				int *secondNumSide = NULL;
				secondNumSide = malloc(sizeof(int));
				/*
				Указатель startPos содержит узел, откуда возможно найти второе число
				для деления.
				*/
				Node *startPos = root2;
				Node *firstNumNode = NULL; // Узел с первым числом
				Node *secondNumNode = NULL; // Узел со вторым числом
				int nod; // Содержит наибольший общий делитель
				Node *divNode = NULL; // Указатель на текущий узел со знаком деления

				while(1) {

					// Находим число, попутно проверяя, если другое число с общим делителем больше 1
					firstNumNode = findDiv(&node,&startPos,&divNode,firstNumSide,secondNumSide);

					*secondNumSide = NONE;

					if(firstNumNode != NULL) {

						secondNumNode = findAnother(&startPos,&firstNumNode,firstNumSide,secondNumSide);

						if(secondNumNode != NULL) {

							nod = NOD(firstNumNode->_num,secondNumNode->_num);
							firstNumNode->_num = firstNumNode->_num / nod;

							if (firstNumNode->_num == 1) {

								if(*firstNumSide != LEFT) {

									free(firstNumNode);
									firstNumNode = NULL;

								} else if(deleteOnesInNumerator(&(divNode->_left), &firstNumNode)){

									// Удаляем единицу в числителе, если нету других чисел, букв.
									free(firstNumNode);
									firstNumNode = NULL;
								}
							}

							secondNumNode->_num = secondNumNode->_num / nod;
							if (secondNumNode->_num == 1) {

								if(*secondNumSide != LEFT) {

									free(secondNumNode);
									secondNumNode = NULL;

								} else if(deleteOnesInNumerator(&(divNode->_left), &secondNumNode)) {

									free(secondNumNode);
									secondNumNode = NULL;
								}
							}
							
						} else break;

					} else break;

					node = root2;
					// Узлы с удаленными числами все еще не равны NULL
					deleteEmptyNodes(&node);

					/* 
					Если у узла с арифметическим знаком только 1 подузел не равен NULL,
					то удаляем этот узел и соединяем его подузел с прошлым узлом. Если 
					предыдущего узла нет, то данный подузел становится началом дерева.
					*/
					while(1) {
						if(deleteVarOp(&root2,&node,&prev) == 0)
							break;
						node = root2;
					}

					prev = NULL;
					node = root2;
					startPos = root2;
					*firstNumSide = NONE;
					*secondNumSide = NONE;
				}

				break;
			}

			case 2:
			{
				printf("Исходное выражение: %s\n", expr);

				break;
			}

			case 3:
			{
				LKP(&root2);
				printf("\n");

				break;
			}

			case 4:
			{
				if (root != NULL) {

					printf("Дерево исходного выражения\n");
					PKL(&root, 0);
				}
				else
					printf("Дерево исходного выражения пусто\n");

				break;
			}

			case 5:
			{
				if (root2 != NULL) {

					printf("Дерево преобразованного выражения\n");
					PKL(&root2, 0);
				}
				else
					printf("Дерево преобразованного выражения пусто\n");

				break;
			}

			case 6: break;

			default:
			{
				printf("Ошибка. Такого пункта меню не существует\n");

				break;
			}
		}

		if (action == 6)
			break;
	}

	treeDestroy(&root);
	treeDestroy(&root2);

	return 0;	
}

int NOD(int a, int b) {
	while(a != b) {

		if (a > b)
			a -= b;
		else
			b -= a;
	}
	return a;
}

Node *treeNodeCreate() {

	Node *tmpNode = (Node *)malloc(sizeof(Node));

	tmpNode->_varOp = '\0';
	tmpNode->_num = 0.0;
	tmpNode->_left = NULL;
	tmpNode->_right = NULL;

	return tmpNode;
}

Node *treeCopy(Node **node) {

	Node *tmpNode = NULL;

	if (*node == NULL)
		return NULL;

	tmpNode = treeNodeCreate();
	tmpNode->_varOp = (*node)->_varOp;
	tmpNode->_num = (*node)->_num;
	tmpNode->_left = treeCopy(&(*node)->_left);
	tmpNode->_right = treeCopy(&(*node)->_right);

	return tmpNode;
}

int treeIsMinusNode(Node **node) {

	if (*node == NULL)
		return 0;

	if ((*node)->_left == NULL || (*node)->_right == NULL)
		return 0;

	return ((*node)->_varOp == '-' && (*node)->_left->_varOp == '\0' && (*node)->_left->_num == 0.0);
}

void treeBuild(Node **node, Stack *st) {

	Token token;

	if (stackEmpty(st))
		return;

	token = stackTop(st);

	stackPop(st);

	(*node) = treeNodeCreate();
	(*node)->_varOp = token._varOp;
	(*node)->_num = token._num;

	if (isOp((*node)->_varOp)) {

		treeBuild(&(*node)->_right, st);
		treeBuild(&(*node)->_left, st);
	}
}

void treeDestroy(Node **node) {

	if (*node == NULL)
		return;

	if ((*node)->_left != NULL)
		treeDestroy(&(*node)->_left);

	if ((*node)->_right != NULL)
		treeDestroy(&(*node)->_right);

	free(*node);

	*node = NULL;
}

Node *findDiv(Node **node, Node **startPos, Node **divNode, int *firstNumSide, int *anotherNumSide) {

	if(*node == NULL) return NULL;

	if ((*node)->_num != 0 && (*node)->_num != 1 && (*firstNumSide == LEFT || *firstNumSide == RIGHT)) {

		if(findAnother(&(*startPos), &(*node), firstNumSide, anotherNumSide) != NULL)
			return *node;
		else
			return NULL;
	}

	if ((*node)->_varOp == '+' || (*node)->_varOp == '-') {

		if(*firstNumSide != NONE) 
			return NULL;

		*startPos = NULL;

	} else if(((*node)->_varOp == '*' || (*node)->_varOp == '/') && *startPos == NULL && *firstNumSide == NONE) {

		*startPos = *node;
	}

	if((*node)->_varOp == '/') {

		*divNode = *node;

		*firstNumSide = LEFT;

		Node *tmpNode;

		tmpNode = findDiv(&(*node)->_left, &(*startPos), &(*divNode), firstNumSide, anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

		*firstNumSide = RIGHT;

		tmpNode = findDiv(&(*node)->_right, &(*startPos), &(*divNode), firstNumSide, anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

	} else {

		Node *tmpNode;

		tmpNode = findDiv(&(*node)->_left, &(*startPos), &(*divNode), firstNumSide, anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

		tmpNode = findDiv(&(*node)->_right, &(*startPos), &(*divNode), firstNumSide, anotherNumSide);
		if(tmpNode != NULL) return tmpNode;
	}

	return NULL;
}
int deleteOnesInNumerator(Node **node, Node **divNumNode) {

	if(*node == NULL) return 0;

	if (((*node)->_num != 0 && ((*node) != (*divNumNode))) || isLetter((*node)->_varOp)) 
		return 1;

	if ((*node)->_varOp == '+' || (*node)->_varOp == '-')
		return 0;

	if(deleteOnesInNumerator(&(*node)->_left,&(*divNumNode)))
		return 1;

	if(deleteOnesInNumerator(&(*node)->_right,&(*divNumNode)))
		return 1;

	return 0;
}
Node *findAnother(Node **node, Node **firstNumNode, int *firstNumSide, int *anotherNumSide) {

	if(*node == NULL) return NULL;

	if ((*node)->_num != 0 && (*node)->_num != 1 && ((*node) != (*firstNumNode)) && NOD((*firstNumNode)->_num, (*node)->_num) != 1) {

		if(*firstNumSide == LEFT && *anotherNumSide == RIGHT)
			return *node;
		else if(*firstNumSide == RIGHT && *anotherNumSide != RIGHT)
			return *node;
		else
			return NULL;

	}

	if ((*node)->_varOp == '+' || (*node)->_varOp == '-')
		return NULL;

	if((*node)->_varOp == '/') {

		Node *tmpNode;
		
		*anotherNumSide = LEFT;

		tmpNode = findAnother(&(*node)->_left,&(*firstNumNode),firstNumSide,anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

		*anotherNumSide = RIGHT;

		tmpNode = findAnother(&(*node)->_right,&(*firstNumNode),firstNumSide,anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

	} else {

		Node *tmpNode;

		tmpNode = findAnother(&(*node)->_left,&(*firstNumNode),firstNumSide,anotherNumSide);
		if(tmpNode != NULL) return tmpNode;

		tmpNode = findAnother(&(*node)->_right,&(*firstNumNode),firstNumSide,anotherNumSide);
		if(tmpNode != NULL) return tmpNode;
	}

	return NULL;
}

void deleteEmptyNodes(Node **node) {

	if(*node == NULL) return;

	if (!(isOp((*node)->_varOp) || isLetter((*node)->_varOp)) && (*node)->_num == 0) {

		*node = NULL;
		return;
	}

	deleteEmptyNodes(&(*node)->_left);
	deleteEmptyNodes(&(*node)->_right);

	return;

}

int deleteVarOp(Node **root, Node **node, Node **prev) {

	if ((*node)->_varOp == '\0' && (*node)->_num == 0) {

		free(*node);
		*node = NULL;
		return 1;
	}

	if((*node)->_varOp != '\0' && !isLetter((*node)->_varOp)) {

		if((*node)->_left == NULL && (*node)->_right == NULL) {

			free(*node);
			*node = NULL;
			return 1;

		} else if((*node)->_right == NULL) {

			if(*prev == NULL) {

				Node *tmpNode = *node;
				*node = (*node)->_left;
				*root = *node;
				free(tmpNode);
				tmpNode = NULL;
				return 1;

			} else {

				if((*prev)->_right == *node) {

					Node *tmpNode = *node;
					(*prev)->_right = (*node)->_left;
					free(tmpNode);
					return 1;

				} else {

					Node *tmpNode = *node;
					(*prev)->_left = (*node)->_left;
					free(tmpNode);
					return 1;
				}
			}
		} else if((*node)->_left == NULL) {

			if(*prev == NULL) {

				Node *tmpNode = *node;
				*node = (*node)->_right;
				*root = *node;
				free(tmpNode);
				tmpNode = NULL;
				return 1;

			} else {

				if((*prev)->_right == *node) {

					Node *tmpNode = *node;
					(*prev)->_right = (*node)->_right;
					free(tmpNode);
					return 1;

				} else {

					Node *tmpNode = *node;
					(*prev)->_left = (*node)->_right;
					free(tmpNode);
					return 1;
				}
			} 
		}
	}
	if ((*node)->_left != NULL) {

		if(deleteVarOp(&(*root),&(*node)->_left,&(*node))) {

			return 1;

		}
	}
	if ((*node)->_right != NULL) {

		if(deleteVarOp(&(*root),&(*node)->_right,&(*node))){

			return 1;

		}
	}
	return 0;
}

void PKL(Node **node, const int level) {

	if (*node == NULL)
		return;

	if ((*node)->_right != NULL)
		PKL(&(*node)->_right, level + 1);

	if ((*node)->_varOp != '\0')
		printf("%*s%c\n", level * 4, "", (*node)->_varOp);
	else
		printf("%*s%d\n", level * 4, "", (*node)->_num);

	if ((*node)->_left != NULL)
		PKL(&(*node)->_left, level + 1);
}

void LKP(Node **node) {

	if (*node == NULL)
		return;

	if ((*node)->_left != NULL && !treeIsMinusNode(node)) {

		if ((*node)->_left->_left != NULL)
			printf("(");
		
		LKP(&(*node)->_left);
		
		if ((*node)->_left->_left != NULL)
			printf(")");
	}

	if ((*node)->_varOp != '\0')
		printf("%c", (*node)->_varOp);
	else
		printf("%d", (*node)->_num);

	if ((*node)->_right != NULL) {

		if ((*node)->_right->_left != NULL)
			printf("(");

		LKP(&(*node)->_right);
		
		if ((*node)->_right->_left != NULL)
			printf(")");
	}
}

int isLetter(const char ch) {

	return ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'));
}

int isNumber(const char ch) {

	return (ch >= '0' && ch <= '9');
}

int isOp(const char ch) {

	return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^' || ch == '(' || ch == ')');
}

int opPrior(const char op) {

	if (op == '^')
		return 4;

	if (op == '*' || op == '/')
		return 3;

	if (op == '+' || op == '-')
		return 2;

	return 1;
}

int isOpHigh(const char op1, const char op2) {

	if (op1 == '(' || op2 == '(' || op2 == ')')
		return 0;

	if (op1 == op2 && op2 == '^')
		return 0;
	
	return (opPrior(op1) >= opPrior(op2));
}

void postOrder(const char *str, Stack *st) {

	int i = 0, step = -1, isBracket = 0, isDot = 0;
	char tmpCh;
	Token tk;
	Stack stOp;

	stackCreate(&stOp);

	tk._varOp = '\0';
	tk._num = 0.0;

	while (str[i] != '\0') {

		if (str[i] == '.')
			isDot = 1;
		else if (isLetter(str[i])) {

			tk._varOp = str[i];

			stackPush(st, tk);
		}
		else if (isNumber(str[i])) {

			tk._varOp = '\0';

			if (!isDot)
				tk._num = tk._num * 10.0 + str[i] - '0';
			else {

				tk._num = tk._num + pow(10.0, step) * (str[i] - '0');
				step--;
			}

			if (str[i + 1] != '.' && !isNumber(str[i + 1])) {

				stackPush(st, tk);

				tk._num = 0.0;
				step = -1;
				isDot = 0;
			}
		}
		else if (isOp(str[i])) {

			tk._varOp = str[i];

			if (str[i] == ')')
				isBracket = 1;
			else if (str[i] == '-' && (i == 0 || str[i - 1] == '(')) {

				tmpCh = tk._varOp;
				tk._varOp = '\0';
				tk._num = 0.0;

				stackPush(st, tk);

				tk._varOp = tmpCh;
			}

			while (!stackEmpty(&stOp) && (isOpHigh(stackTop(&stOp)._varOp, str[i]) || isBracket)) {

				if (stackTop(&stOp)._varOp == '(')
					isBracket = 0;
				else
					stackPush(st, stackTop(&stOp));

				stackPop(&stOp);
			}

			if (str[i] != ')')
				stackPush(&stOp, tk);
		}

		i++;
	}

	while (!stackEmpty(&stOp)) {

		stackPush(st, stackTop(&stOp));
		stackPop(&stOp);
	}

	stackDestroy(&stOp);
}