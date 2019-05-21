#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 10; // размер массива элементов struct Item

typedef struct Item {
	int busy; /* признак занятости элемента */
	char key[8]; /* ключ элемента */
	char *info; /* указатель на информацию */
	int infosize; // размер поля info
} item;


// обнуляем Item
void zeroItem(struct Item *arr_Item) {
	for (int i = 0; i < SIZE; ++i) {
		arr_Item[i].busy = 0;
		arr_Item[i].key[0] = '\0';
		arr_Item[i].info = NULL;
	}
}

// следующий свободный элемент массива
int findfree(item *arr_Item) {
	for (int i = 0; i < SIZE; ++i) {
		if (arr_Item[i].busy == 0) { // если элемент свободен
			return i; // возвращаем его номер
		}
	}
	return -1; // если сободных элементов массива нет, выходим из программы
}

// проверяем что таблица не пустая
int isNotEmpty(item *arr_Item) {
	for (int i = 0; i < SIZE; ++i) {
		if (arr_Item[i].busy == 1) {
			return 1; // если хоть один элемент занят
		}
	}
	return 0; // если таблица пустая
}

/* определяем уникальность ключа */
int unique(item *arr_Item, char k[]) {
	for (int i = 0; i < SIZE; ++i) {
		if (strcmp(arr_Item[i].key, k) == 0) return 0; // если ключ уникален
	}
	return 1; // если нет
}

// очищаем буфер
void clear(void) {
	while (getchar() != '\n');
}

// добавляем элемент, вводим значения из консоли
void addItem(FILE * fp, item *arr_Item) {

	// проверяем наличие свободных элементов в массиве
	int free_element = findfree(arr_Item); // первый свободный элемент массива
	if (free_element == -1) {
		printf("The array is full.");
		return; // если свободных элементов нет, возвращаемся в управляющую функцию
	}

	// принимаем значение key
	char k[8]; // переменная для ввода поля key
	printf("Key: ");
	scanf("%7s", k); // ввод ограничен семью символами
	clear(); // вызываем функцию очистки буфера


	// проверка уникльности ключа
	if (unique(arr_Item, k) == 0) { // если ключ неуникален
		printf("The array already contains this value: %s", k);
		return; // возвращаемся в управляющую процедуру
	}

	// принимаем значение *info
	char buffer; // буферное значение для getchar
	int numberOfElement = -1; // номер элемента массива
	arr_Item[free_element].info = (char *)malloc(sizeof(char));
	printf("Info: ");
	buffer = getchar();
	while (buffer != '\n') {
		++numberOfElement;
		arr_Item[free_element].info = (char *)realloc(arr_Item[free_element].info, numberOfElement + 1);
		arr_Item[free_element].info[numberOfElement] = buffer;
		buffer = getchar();
	}
	arr_Item[free_element].info[numberOfElement + 1] = '\0';
	arr_Item[free_element].busy = 1;
	strcpy(arr_Item[free_element].key, k);
	arr_Item[free_element].infosize = numberOfElement;

}

// добавляем элемент с передачей значения аргументом (не используется)
void addItemByArg(item *arr_Item, char k[8], char info[1024]) {
	int free_element = findfree(arr_Item); // первый свободный элемент массива
	if (free_element == -1) {
		printf("The array is full.");
		return;
	}
	if (unique(arr_Item, k) == 0) { // проверка уникльности ключа
		printf("The array already contains this value: %s", k);
		return;
	}
	arr_Item[free_element].busy = 1;
	strcpy(arr_Item[free_element].key, k);
	arr_Item[free_element].info = info;
}

// печатаем таблицу, только значения busy == 1
void printTable(item *arr_Item, int size) {
	if (isNotEmpty(arr_Item)) {
		printf("     key info\n");
		for (int i = 0; i < size; ++i) {
			if (arr_Item[i].busy == 1) {
				printf("%8s %s\n", arr_Item[i].key, arr_Item[i].info);
			}
		}
	}
	else {
		printf("\n%d\n", isNotEmpty(arr_Item));
		printf("\nTable is empty. Try add some information.\n");
	}
}

// печатаем один элемент таблицы
void printOneElement(item *arr_Item) {
	int n;
	printf("Enter number of element to print: ");
	scanf("%d", &n);
	if (n < 0 || n >= SIZE) { // проверяем попадает ли n в диапазон массива
		printf("Element %d is out of range. Try another.", n);
	}
	else if (arr_Item[n].busy == 1) { // проверяем пусто он или нет
		printf("     key info\n");
		printf("%8s %s\n", arr_Item[n].key, arr_Item[n].info);
	}
	else {
		printf("Element %d is free. Try another.\n", n);
	}
}

// удаляем элемент массива но номеру элемента
void removeByNumberOfElement(item *arr_Item, int noe) {
	if (noe < 0 || noe > SIZE - 1) {
		printf("\nCan`t remove, item %d is out of range!\n", noe);
		return;
	}
	arr_Item[noe].busy = 0;
	arr_Item[noe].key[0] = '\0';
	arr_Item[noe].info = NULL;
}

// получаем номер элемента которых хотим удалить
void getNoEandRemove(item *arr_Item) {
	int noe;
	printf("Enter NoE: ");
	scanf("%d", &noe);
	removeByNumberOfElement(arr_Item, noe); // вызываем функцию удаления по номеру
}

// удаляем диапазон значений по полю key
void removeByKeyRange(item *arr_Item) {
	char r1[8]; // нижнее значение диапазона
	char r2[8]; // верхнее значение диапазона
	printf("Remove items by key value from: ");
	scanf("%s", r1);
	printf("to: ");
	scanf("%s", r2);

	// если нижнее значение больше верхнего, меняем местами
	if (strcmp(r1, r2) > 0) {
		char temp[8];
		strcpy(temp, r1);
		strcpy(r1, r2);
		strcpy(r2, temp);
	}

	// удаляем
	for (int i = 0; i < SIZE; ++i) { // проходим по всем элементам
		if (strcmp(arr_Item[i].key, r1) >= 0 && strcmp(arr_Item[i].key, r2) <= 0) {
			removeByNumberOfElement(arr_Item, i); // если входит в диапазон key - вызываем функцию
		}
	}
}

// делаем выборку по диапазону значений key
void selectByKeyRange(item *arr_Item) {
	char r1[8]; // нижнее значение диапазона
	char r2[8]; // верхнее значение диапазона
	int c = 0; // номер элемента массива newArr_Item
	item *newArr_Item; // массив для выборки
	newArr_Item = (item*)malloc(20);

	printf("Select items by key value from: ");
	scanf("%s", r1);
	printf("to: ");
	scanf("%s", r2);

	// если нижнее значение больше верхнего, меняем местами
	if (strcmp(r1, r2) > 0) {
		char temp[8];
		strcpy(temp, r1);
		strcpy(r1, r2);
		strcpy(r2, temp);
	}

	for (int i = 0; i < SIZE; ++i) {
		// для всех значений где key принадлежит диапазону выборки
		if (strcmp(arr_Item[i].key, r1) >= 0 && strcmp(arr_Item[i].key, r2) <= 0) {
			newArr_Item = (item*)realloc(newArr_Item, sizeof(arr_Item[i]) * (c + 1)); // память выделяем
			newArr_Item[c] = arr_Item[i]; // и тут же заполняем
			printf("sizeof %d: %d\n", i, sizeof(arr_Item[i]));
			c++;
		}
	}
	printTable(newArr_Item, c); // выводим на экран выборку
}

// ввод имени файла
void fnaming(char *name) {
	printf("Enter filename: ");
		scanf("%127s", name);
	clear();
}
// открытие файла
FILE * fopening(char *fname) {
	FILE *fptr = fopen(fname, "rb+");
	if (fptr == NULL) {
		puts("Cannot read the file. Trying to create it..");
		fptr = fopen(fname, "wb+"); // создаём его
		if (fptr == NULL) { // если открыть или создать файл не удалось
			puts("Still cannot create and read file");
			return nullptr; // прерываем выполнение функции, возвращаемся
		}
		else {
			puts("File was created successfully.");
		}
	};
	return fptr; // возвращаем файловый указатель
}
// пишем таблицу в файл
void writeTableToFile(FILE * fp, item *arr_Item) {
	fwrite(arr_Item, sizeof(item), SIZE, fp); // пишем
	fflush(fp); // сохраняем
}
int main(int argCount, const char* args[]) {
	item arr_Item[SIZE]; // инициализация массива
	zeroItem(arr_Item); // обнуление массива
	FILE *fp; // указатель на файл
	char fname[128]; // строка имени файла
	fnaming(fname);  // вводим имя файла
	fp = fopening(fname); // открываем файл и инициализируем указатель на файл
	// menu
	int n = -1;
	while (n != 0) {
		printf("\n0 - exit");
		printf("\n1 - add item to file");
		printf("\n2 - print item from file by number of element");
		printf("\n3 - print all not empty items from file");
		printf("\n4 - remove items by key range");
		printf("\n5 - remove item by number of element");
		printf("\n6 - select items by key range");
		puts("");
		scanf("%d", &n);
		if (n == 1) {
			addItem(fp, arr_Item); // добавляем новый элемент
			writeTableToFile(fp, arr_Item); // пишем массив в файл
		}
		else if (n == 2) {
			printOneElement(arr_Item); // выводим один элемент по номеру элемента
		}
		else if (n == 3) {
			printTable(arr_Item, SIZE); // выводим элементы где busy == 1
		}
		else if (n == 4) {
			removeByKeyRange(arr_Item); // удаляем диапазон элементов по значениям key
		}
		else if (n == 5) {
			getNoEandRemove(arr_Item); // удаляем один элемент по его номеру
		}
		else if (n == 6) {
			selectByKeyRange(arr_Item); // выделяем диапазон элементов в отдельный массив и выводим на экран
		}
	}
	return 0;
}
