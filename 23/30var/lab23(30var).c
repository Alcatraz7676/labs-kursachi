/*
 * Байков А.М. 8О-103Б
 * 30. Определить число нетерминальных вершин дерева.
*/

#include <stdio.h>
#include "tree.c"

void KLP(TreeNode **node, const int level);
void task(TreeNode **node, int *count);
TreeNode *getNodeByPath(TreeNode **node, const char *path);

int main(void)
{
	int count = 0;
	int arg;
	char cmd[255];
	TreeNode *root = NULL, *tmpNode = NULL;

	do
	{
		printf("Введите команду (h - справка):\n");
		scanf("%s", cmd);

		if (cmd[0] == '+')
		{
			scanf(" %d", &arg);

			if (cmd[1] == 'r')
			{
				if (root == NULL)
				{
					treeAddNode(&root, arg);

					printf("Корень %d создан\n", arg);
				}
				else
					printf("Корень уже существует\n");
			}
			else if (root == NULL)
				printf("Корень не создан\n");
			else
			{
				tmpNode = root;

				if (cmd[1] != '\0')
					tmpNode = getNodeByPath(&root, &cmd[1]);

				if (tmpNode == NULL)
					printf("Ошибка. Такого пути не существует\n");
				else
				{
					if (treeAddNode(&tmpNode, arg) != NULL)
						printf("Узел %d добавлен к узлу %d\n", arg, tmpNode->_data);
				}
			}
		}
		else if (cmd[0] == '-')
		{
			scanf(" %d", &arg);

			if (treeRemoveNode(&root, arg))
				printf("Узел %d удален\n", arg);
			else
				printf("Узел %d не найден\n", arg);
		}
		else if (cmd[0] == 'p')
		{
			KLP(&root, 0);
		}
		else if (cmd[0] == 't')
		{
			if (root != NULL) {

				task(&root,&count);
				printf("Количество нетерминальных вершин: %d\n", count);
				count = 0;
			}
			else
				printf("Дерево пусто\n");
		}
		else if (cmd[0] == 'h')
		{
			printf("================================\n");
			printf("Список команд:\n");
			printf("+r INT - создать корень INT\n");
			printf("+ INT - добавить сына INT к корню\n");
			printf("+PATH INT - добавить INT узел по заданому пути (s - сын, b - брат)\n");
			printf("- INT - удалить первый найденный узел INT и его поддерево\n");
			printf("p - распечатать дерево\n");
			printf("t - выполнить задание над деревом\n");
			printf("q - завершить программу\n");
			printf("================================\n");
		}
		else if (cmd[0] != 'q')
		{
			printf("Неизвестная команда\n");
		}
	}
	while (cmd[0] != 'q');

	treeDestroy(&root);

	return 0;
}

void KLP(TreeNode **node, const int level)
{
	if (*node == NULL)
	{
		printf("Дерево пусто\n");

		return;
	}

	printf("%*s%d\n", level * 2, "", (*node)->_data);

	if ((*node)->_son != NULL)
		KLP(&(*node)->_son, level + 1);

	if ((*node)->_bro != NULL)
		KLP(&(*node)->_bro, level);
}

void task(TreeNode **node, int *count)
{

	if ((*node)->_son != NULL) {

		(*count)++;
		task(&(*node)->_son, count);
	}

	if ((*node)->_bro != NULL)
		task(&(*node)->_bro, count);

	return;
}

TreeNode *getNodeByPath(TreeNode **node, const char *path)
{
	int i = 0;
	TreeNode *tmpNode = *node;

	while (path[i] != '\0')
	{
		if (path[i] == 's')
			if (tmpNode->_son != NULL)
				tmpNode = tmpNode->_son;
			else
				return NULL;
		else if (path[i] == 'b')
			if (tmpNode->_bro != NULL)
				tmpNode = tmpNode->_bro;
			else
				return NULL;
		else
			return NULL;

		i++;
	}

	return tmpNode;
}