#include "pch.h"
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 5;

typedef struct Inf { // значения динамического размера
	char* info;	// информация
} Info;

typedef struct Itm { // значения постоянного размера
	int busy; // занята/свободна строчка
	//char key[8]; // ключ char по старому варианту тз
	int key; // ключ int - изменено условие ТЗ
	int offset; // отступ от начала файла
	int len;	// длина информации
} Item;

typedef struct Tbl { // вспомогательные данные о таблице и файлах
	Item *itemVector; // указатель на массив постоянного размера
	Info *infoVector; // указатель на массив динамического размера
	char *fDATname; // имя файла "*.dat"
	char *fTABname; // имя файла "*.tab"
	FILE *ftab; //= NULL; // файл данных постоянного размера
	FILE *fdat; //= NULL; // файл данных info
} Table;


void deallocateMemory(Table *ptab); // освобождение памяти
void dialog(Table *ptab); // диалоговая функция
int fopening(Table *ptab); // функция пытается открыть существующие файлы
int fcreating(Table *ptab); // функция пытается создать новые файлы или переоткрыть с правом "w"
void loading(Table *ptab); // загрузка информации, открытие и создание файлов для работы
void clear(void); // очистка буфера
void fnaming(Table *ptab); // ввод имени файла с которым будет работать программа
void swap(int *r1, int *r2); // меняет местами значения int по адресам
//void D_Add(Table *ptab); // диалоговая функция вставки нового значения
void addItem(Table *ptab); // добавить значение
int findNotBusy(Table *ptab); // поиск свободной строки
//int findKey(Table *ptab, char *key); // поиск ключа
int findKey(Table *ptab, int key); // поиск ключа
int getKey(Table *ptab, int n); // добавить ключ
void rebuildOffset(Table *ptab); // заполнить значения offset
void getInfo(Table *ptab, int n); // добавить информацию и всё что с ней связано
void printTable(Table *ptab);
void printFullTable(Table *ptab); // печать полной таблицы
void printByKeyRange(Table *ptab); // печать по диапазону key
void deleteItem(Table *ptab, int n); // удаление строки по номеру элемента
void removeByKeyRange(Table *ptab); //удаление по диапазону key
void datawrite(FILE *ft, FILE *fd, Table *ptab);
void dataread(FILE *ft, FILE *fd, Table *ptab);
void save(Table *ptab); // запись, сохранение файлов
void unload(Table *ptab); // закрытие файлов

int main()
{
	Table table;
	table.itemVector = (Item *)calloc(SIZE, sizeof(Item));
	table.infoVector = (Info *)malloc(sizeof(Item) * SIZE);
	for (int i = 0; i < SIZE; ++i) {
		table.infoVector[i].info = NULL;
	}
	loading(&table); // открытие файлов и загрузка информации, создание файлов

	dialog(&table); // диалоговая функция, пользовательское меню

	save(&table); // сохранение, запись файлов

	unload(&table); //закрытие файлов

	deallocateMemory(&table);

	puts("\nThat's it! Bue !!!\n");

	return 0;
}

void dialog(Table *ptab) {

	int n = -1;
	while (n != 0) {
		puts("\n0 - Exit.");
		puts("1 - Add item.");
		puts("2 - Remove items by key range.");
		puts("3 - Print items where busy == 1.");
		puts("4 - Print items by key range.");
		puts("5 - Print full table.\n");
		printf("Enter menu item: --> ");
		scanf("%d", &n);
		if (n == 1) {
			addItem(ptab); // вставка нового значения
		}
		else if (n == 2) {

			removeByKeyRange(ptab); // удаление данных по диапазону key
		}
		else if (n == 3) {
			printTable(ptab); // печать всех key и info со значением busy == 1
		}
		else if (n == 4) {
			printByKeyRange(ptab); // печать данных по диапазону key
		}
		else if (n == 5) {
			printFullTable(ptab); // печать всех данных всей таблицы
		}
	}
}

// функция открытия, создания файлов, загрузки информации для работы программы
void loading(Table *ptab) {
	fnaming(ptab); // запрос имени файла
	// попытка открыть файл
	printf("Trying to open files %s and %s ...\n", ptab->fTABname, ptab->fDATname);
	if (fopening(ptab)) { // если файлы открылись, то
		printf("Files \"%s\" and \"%s\" were opened successfully !\n", ptab->fTABname, ptab->fDATname);
		dataread(ptab->ftab, ptab->fdat, ptab); // считываем информацию
	}
	else if (fcreating(ptab)) { // если файлы не открылись то пытаемся их создать
		// файлы успешно созданы
		printf("Can't open files.. Trying to create it ...\nFiles created successfully !\n");
	}
	else {
		// создать файлы не получилось
		printf("Can't open files.. Trying to create it...\nCan't create files. Check your access permissions !\n");
	}
}

int fopening(Table *ptab) {
	if (ptab->fdat = fopen(ptab->fDATname, "rb+")) {
		if (ptab->ftab = fopen(ptab->fTABname, "rb+")) {
			return 1;// файлы есть и открываются
		}
		else {
			return 0;// второй файл не открылся
		}
	}
	else {
		return 0;// первый файл не открылся
	}
}
int fcreating(Table *ptab) {
	if (ptab->fdat = fopen(ptab->fDATname, "wb+")) {
		if (ptab->ftab = fopen(ptab->fTABname, "wb+")) {
			return 1;// оба файла созданы
		}
		else {
			return 0;// второй файл не создался
		}
	}
	else {
		return 0;// первый файл не создался
	}
}

// добавляем данные
void addItem(Table *ptab) {
	int n;
	n = findNotBusy(ptab); // ищем свободную строку
	if (n == SIZE) {
		printf("The table is full\n");
		return;
	}
	clear();
	printf("Enter key: --> ");
	if (getKey(ptab, n)) { // вводим ключ
		return; // если ключ не был введён то прерываем процедуру
	}
	printf("Enter info: --> ");
	getInfo(ptab, n); // вводим info
	ptab->itemVector[n].len = strlen(ptab->infoVector[n].info) + 1;
	ptab->itemVector[n].busy = 1;
	rebuildOffset(ptab);
}

// заполнение значения поля offset
// возможно каждый раз нужно добавлять 1
void rebuildOffset(Table *ptab) {
	int bufOffset = 0;
	for (int i = 1; i < SIZE; ++i) {
		bufOffset += ptab->itemVector[i - 1].len;
		if (ptab->itemVector[i].busy) {
			ptab->itemVector[i].offset = bufOffset;
		}
	}
}

// функция поиска свободной строки
int findNotBusy(Table *ptab) {
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].busy == 0)
			return i; // строка свободна
	}
	return SIZE; // свободных строк нет
}

// принимаем значение key
int getKey(Table *ptab, int n) {
	//char k[8]; // переменная для ввода поля key по старому варианту ТЗ
	//scanf("%7s", k); // ввод ограничен семью символами
	//clear(); // вызываем функцию очистки буфера
	int k;
	scanf("%d", &k);
	clear();
	if (findKey(ptab, k)) { // если такой ключ уже есть
		printf("\nKey value must be unique..\n");
		return 1; // прерываем программу и возвращаем 1
	}
	// strcpy(ptab->itemVector[n].key, k); // по старому варианту ТЗ
	ptab->itemVector[n].key = k;
	return 0; // если ключ уникальный, вносим изменения и возвр. 0
}

// функция поиска ключа в списке
//int findKey(Table *ptab, char key[]) {
int findKey(Table *ptab, int key) {
	for (int i = 0; i < SIZE; ++i) { // проходим по всем строкам
		if (ptab->itemVector[i].busy == 1) { // в строках занятых
			//if ((strcmp(ptab->itemVector[i].key, key)) == 0) { // сравниваем ключи
			if (ptab->itemVector[i].key == key) {
				return 1; // если найден ключ
			}
		}
	}
	return 0;
}

// принимаем значение *info и добавляем все связанные с этим полем данные
void getInfo(Table *ptab, int n) {
	char buffer; // буферная переменная для getchar
	int noe = -1; // номер элемента массива char *info;
	ptab->infoVector[n].info = (char *)malloc(sizeof(char)); // выделяем память под первый знак строки
	buffer = getchar(); // начинаем принимать знаки с клавиатуры
	while (buffer != '\n') { // до тех пор пока не будет введён перенос строки
		++noe;
		ptab->infoVector[n].info = (char *)realloc(ptab->infoVector[n].info, noe + 1); // выделяем память
		ptab->infoVector[n].info[noe] = buffer;	// копируем знак в строку				// ещё на 1 знак
		buffer = getchar(); // принимаем ещё один знак
	}
	ptab->infoVector[n].info[noe + 1] = '\0'; // указываем конец строки
}

// функция чтения данных из файла
void dataread(FILE *ft, FILE *fd, Table *ptab) {
	printf("\nTrying to read \"%s\" file..", ptab->fDATname);
	if (fread(ptab->itemVector, sizeof(Item), SIZE, ft) != SIZE) { // если кол-во прочитанных элементов не равно размеру таблицы
		printf("An error occurred while reading \"%s\" file\nNumber of Data Items != SIZE of Table\n", ptab->fDATname);
	} else {
		printf("\nFile \"%s\" was read successfully\n", ptab->fDATname);
	}


	printf("\nTrying to read \"%s\" file..\n", ptab->fTABname);
	int counter1 = 0;
	int counter2 = 0;
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].busy) { // если строка не пустая
			ptab->infoVector[i].info = (char*)malloc(ptab->itemVector[i].len); // выделяем память под копирование данных
			// пытаемся копировать и если скопировано сколько и ожидалось
			if (fread(ptab->infoVector[i].info, 1, ptab->itemVector[i].len, fd) == ptab->itemVector[i].len) {
				printf("%d element: copied %d bytes as expected\n", i, ptab->itemVector[i].len);
			}
			else { // если количество прочитанных данных не равно ожидаемому числу
				printf("%d element: WARNING!!! copied %d bytes, wrong number\n", i, ptab->itemVector[i].len);
			}
			
			++counter1; // счётчик прочитанных непустых строк
		} // если же строка пустая
		else { // сообщаем что строка пропущена
			printf("%d element is empty, skipped\n", i);
			++counter2; // считаем количество пропущенных пустых строк
		}
	}
	printf("\nFile \"%s\" was read..\n", ptab->fTABname);
	printf("%d items read and %d skipped ..\n", counter1, counter2);
	printf("Processed %d items. Expected SIZE of table %d items..\n\n", (counter1+counter2), SIZE);
}
// функция выполняет подготовку к записи и запись данных
void save(Table *ptab) {
	// открытие файлов в режиме wb+
	ptab->fdat = freopen(ptab->fDATname, "wb+", ptab->fdat);
	ptab->ftab = freopen(ptab->fTABname, "wb+", ptab->ftab);

	datawrite(ptab->ftab, ptab->fdat, ptab); // запись в файл


	// очистка буфера и сохранение файлов
	puts("Trying to save files");
	if (fflush(ptab->fdat)) {
		printf("An error occurred while saving \"%s\" file\n", ptab->fDATname);
	}
	else {
		printf("File \"%s\" saved successfuly\n", ptab->fDATname);
	}
	if (fflush(ptab->ftab)) {
		printf("An error occurred while saving \"%s\" file\n", ptab->fTABname);
	}
	else {
		printf("File \"%s\" saved successfuly\n", ptab->fTABname);
	}
	puts("End of save function\n");
}
// запись данных в файл (без сохранения)
void datawrite(FILE *ft, FILE *fd, Table *ptab) {
	printf("Trying to write \"%s\" file\n", ptab->fDATname);
	if (fwrite(ptab->itemVector, sizeof(Item), SIZE, ft) != SIZE) {
		printf("An error occurred while writing \"%s\" file\n", ptab->fDATname);
	}
	else {
		printf("File \"%s\" was written successfully\n", ptab->fDATname);
	}
	printf("\nTrying to write \"%s\" file\n", ptab->fTABname);
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].busy) {
			fwrite(ptab->infoVector[i].info, sizeof(char), ptab->itemVector[i].len, fd);
			printf("%d string is busy: info field was written\n", i);
		}
		else {
			printf("%d string is empty: info field wasn't written\n", i);
		}
	}
	puts("End of write function\n");
}


// функция печатает key и info всех занятых строк
void printTable(Table *ptab) {
	printf("     key info\n");
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].busy) {
			printf("%8d %s\n", ptab->itemVector[i].key, ptab->infoVector[i].info);
		}
	}
}

// функция выводит на экран все данные
void printFullTable(Table *ptab) {
	printf("noe busy offset len     key info\n");
	for (int i = 0; i < SIZE; ++i) {
		//printf("%3d %3d %7s %3d %3d info: %s\n", i, ptab->itemVector[i].busy, ptab->itemVector[i].key, ptab->itemVector[i].offset, ptab->itemVector[i].len, ptab->infoVector[i].info);
		printf("%3d %3d %3d %6d %7d %s\n", i, ptab->itemVector[i].busy, ptab->itemVector[i].offset, ptab->itemVector[i].len, ptab->itemVector[i].key, ptab->infoVector[i].info);
	}
}

// функция выводит в консоль key и info по диапазону key
void printByKeyRange(Table *ptab) {
	int r1, r2; // границы диапазона выборки
	printf("Enter lower key bound: --> ");
	scanf("%d", &r1);
	printf("Enter upper key bound: --> ");
	scanf("%d", &r2);
	if (r1 > r2) { // если нижняя граница больше верхней - меняем местами
		swap(&r1, &r2);
	}
	int dataCounter = 0;
	printf("     key info\n");
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].key >= r1 && ptab->itemVector[i].key <= r2) {
			printf("key: %d info: %s\n", ptab->itemVector[i].key, ptab->infoVector[i].info);
			++dataCounter;
		}
	}
	if (dataCounter == 0) {
		printf("There is no data in %d - %d range\n", r1, r2);
		return;
	}
}

// функция удаления данных по диапазону key
void removeByKeyRange(Table *ptab) {
	int r1, r2; // границы диапазона выборки
	printf("Enter lower key bound: --> ");
	scanf("%d", &r1);
	printf("Enter upper key bound: --> ");
	scanf("%d", &r2);
	if (r1 > r2) { // если нижняя граница больше верхней - меняем местами
		swap(&r1, &r2);
	}
	int dataCounter = 0;
	for (int i = 0; i < SIZE; ++i) {
		if (ptab->itemVector[i].key >= r1 && ptab->itemVector[i].key <= r2) {
			if (ptab->itemVector[i].busy) { // если строка содержит данные
				deleteItem(ptab, i); // удаляем данные
			}
			++dataCounter;
		}
	}
	if (dataCounter == 0) {
		printf("There is no data in %d - %d range\n", r1, r2);
		return;
	}
	rebuildOffset(ptab); // правим offset с учётом изменившейся таблицы
}

void fnaming(Table *ptab) {
	char fn[128];
	printf("Enter filename: -- >");
	scanf("%127s", fn);
	//printf("%s\n", fn);
	clear();
	//strcat(ptab->fname, fn);
	ptab->fDATname = (char*)malloc(strlen(fn) + 5);
	ptab->fTABname = (char*)malloc(strlen(fn) + 5);
	strcpy(ptab->fDATname, fn);
	strcpy(ptab->fTABname, fn);
	strcat(ptab->fDATname, ".dat");
	strcat(ptab->fTABname, ".tab");
	//printf("%s\n", ptab->fDATname);
	//printf("%s\n", ptab->fTABname);
}

// закрытие файлов
void unload(Table *ptab) {
	puts("Closing files");
	if (fclose(ptab->fdat)) {
		printf("An error occurred while closing \"%s\" file\n", ptab->fDATname);
	}
	else {
		printf("File \"%s\" closed successfuly\n", ptab->fDATname);
	}

	if (fclose(ptab->ftab)) {
		printf("An error occurred while closing \"%s\" file\n", ptab->fTABname);
	}
	else {
		printf("File \"%s\" closed successfuly\n", ptab->fTABname);
	}
	puts("End of unload function\n");
}


// функция удаления всей информации из строки таблицы
void deleteItem(Table *ptab, int n) {
	//ptab->itemVector[n] = { 0, 0, 0, 0 };
	//ptab->infoVector[n] = { NULL };
	ptab->itemVector[n].busy = 0;
	ptab->itemVector[n].key = 0;
	ptab->itemVector[n].offset = 0;
	ptab->itemVector[n].len = 0;
	ptab->infoVector[n].info = NULL;
}

// функция меняет местами значения по указанным адресам
void swap(int *r1, int *r2) {
	*r1 = *r1^*r2;
	*r2 = *r1^*r2;
	*r1 = *r1^*r2;
}
// очистка буфера от лишних элементов
void clear(void) {
	while (getchar() != '\n');
}

void deallocateMemory(Table *ptab) {
	puts("\nStart deallocate allocated memory ...");
	// освобождение памяти, выделяемой в функциях
	// dataread и getInfo
	//for (int i = 0; i < SIZE; ++i) {
	//	if (ptab->itemVector[i].busy) {
	//		free(ptab->infoVector[i].info);
	//	}
	//}
	// предыдущий код закомментировал, 
	// так как при условии срабатывания функции getInfo
	// попытка освободить память заканчивается крахом программы

	free(ptab->infoVector);
	free(ptab->itemVector);
	free(ptab->fDATname);
	free(ptab->fTABname);
	
	puts("End of deallocating ...\n");
}
