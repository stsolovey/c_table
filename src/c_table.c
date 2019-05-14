#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int SIZE = 10; // ������ ������� ��������� struct Item

typedef struct Item{
int busy; /* ������� ��������� �������� */
char key[8]; /* ���� �������� */
char *info; /* ��������� �� ���������� */
} item;

// �������� Item
void zeroItem(struct Item *arr_Item){
    for(int i = 0; i < SIZE; ++i){
        arr_Item[i].busy = 0;
        arr_Item[i].key[0] = '\0';
        arr_Item[i].info = NULL;
    }
}

// ��������� ��������� ������� �������
int findfree(item *arr_Item){
    for(int i = 0; i < SIZE; ++i){
        if(arr_Item[i].busy == 0){ // ���� ������� ��������
            return i; // ���������� ��� �����
        }
    }
    return -1; // ���� �������� ��������� ������� ���, ������� �� ���������
}

// ��������� ��� ������� �� ������
int isNotEmpty(item *arr_Item){
    for(int i = 0; i < SIZE; ++i){
        if(arr_Item[i].busy == 1){
            return 1; // ���� ���� ���� ������� �����
        }
    }
    return 0; // ���� ������� ������
}

/* ���������� ������������ ����� */
int unique(item *arr_Item, char k[]){
    for(int i = 0; i < SIZE; ++i){
        if (strcmp(arr_Item[i].key, k) == 0) return 0; // ���� ���� ��������
    }
    return 1; // ���� ���
}

// ������� �����
void clear (void){
        while (getchar() != '\n');
    }

// ��������� �������, ������ �������� �� �������
void addItem(item *arr_Item){

    // ��������� ������� ��������� ��������� � �������
    int free_element = findfree(arr_Item); // ������ ��������� ������� �������
	if (free_element == -1){
		printf("The array is full.");
		return; // ���� ��������� ��������� ���, ������������ � ����������� �������
	}

	// ��������� �������� key
    char k[8]; // ���������� ��� ����� ���� key
    printf("Key: ");
    scanf("%7s", k); // ���� ��������� (�������) ����� ���������
    clear(); // �������� ������� ������� ������
    printf("Info: ");


    // ��������� �������� *info
    char *buf = malloc(sizeof(char)*1024);; // ����� ��� ������ �� �������
	gets(buf);
	int n = 0; // ������� �������� ��������
	while(buf[n]!='\0'){
		n++; // ���� �� �������� ����� ������ - �������
	}
	char *info = malloc(sizeof(char)*n+1); // �������� ������ (+1 ��� ������� ����� ������)
	strcpy(info, buf); // ��������
    free(buf); // ����������� ������

    // �������� ����������� �����
    if (unique(arr_Item, k) == 0){ // ���� ���� ����������
        printf("The array already contains this value: %s", k);
        free(info); // ����������� ������
        return; // ������������ � ����������� ���������
    } else {
        arr_Item[free_element].busy = 1;
        strcpy(arr_Item[free_element].key, k);
        arr_Item[free_element].info = info;
    }
}

    // ��������� ������� � ��������� �������� ���������� (�� ������������)
void addItemByArg(item *arr_Item, char k[8], char info[1024]){
    int free_element = findfree(arr_Item); // ������ ��������� ������� �������
	if (free_element == -1){
		printf("The array is full.");
		return;
	}
    if (unique(arr_Item, k) == 0){ // �������� ����������� �����
        printf("The array already contains this value: %s", k);
        return;
    }
    arr_Item[free_element].busy = 1;
    strcpy(arr_Item[free_element].key, k);
    arr_Item[free_element].info = info;
}

    // �������� �������, ������ �������� busy == 1
void printTable(item *arr_Item){
    if(isNotEmpty(arr_Item)){
        printf("     key info\n");
        for(int i = 0; i < SIZE; ++i){
            if(arr_Item[i].busy==1){
                printf("%8s %s\n",  arr_Item[i].key, arr_Item[i].info);
            }
        }
    }else{
        printf("\nTable is empty. Try add some information.\n");
    }
}

// �������� ���� ������� �������
void printOneElement(item *arr_Item){
    int n;
    printf("Enter number of element to print: ");
    scanf("%d", &n);
    if (n < 0 || n >= SIZE){ // ��������� �������� �� n � �������� �������
        printf("Element %d is out of range. Try another.", n);
    }else if(arr_Item[n].busy==1){ // ��������� ����� �� ��� ���
        printf("     key info\n");
        printf("%8s %s\n", arr_Item[n].key, arr_Item[n].info);
    }else{
        printf("Element %d is free. Try another.\n", n);
    }
}

    // ������� ������� ������� �� ������ ��������
void removeByNumberOfElement(item *arr_Item, int noe){
    if(noe < 0 || noe > SIZE - 1){
        printf("\nCan`t remove, item %d is out of range!\n", noe);
        return;
    }
    arr_Item[noe].busy = 0;
    arr_Item[noe].key[0] = '\0';
    arr_Item[noe].info = NULL;
}

    // �������� ����� �������� ������� ����� �������
void getNoEandRemove(item *arr_Item){
    int noe;
    printf("Enter NoE: ");
    scanf("%d", &noe);
    removeByNumberOfElement(arr_Item, noe); // �������� ������� �������� �� ������
}

    // ������� �������� �������� �� ���� key
void removeByKeyRange(item *arr_Item){
    char r1[8]; // ������ �������� ���������
    char r2[8]; // ������� �������� ���������
    printf("Remove items by key value from: ");
    scanf("%s", r1);
    printf("to: ");
    scanf("%s", r2);

    // ���� ������ �������� ������ ��������, ������ �������
    if(strcmp(r1, r2) > 0){
        char temp[8];
        strcpy(temp, r1);
        strcpy(r1, r2);
        strcpy(r2, temp);
    }

	// �������
    for(int i = 0; i < SIZE; ++i){ // �������� �� ���� ���������
        if(strcmp(arr_Item[i].key, r1) >= 0 && strcmp(arr_Item[i].key, r2) <= 0){
            removeByNumberOfElement(arr_Item, i); // ���� ������ � �������� key - �������� �������
        }
    }
}

    // ������ ������� �� ��������� �������� key
void selectByKeyRange(item *arr_Item){
    char r1[8]; // ������ �������� ���������
    char r2[8]; // ������� �������� ���������
    int c = 0; // ����� �������� ������� newArr_Item
    item newArr_Item[SIZE]; // ������������� �������
    zeroItem(newArr_Item); // ��������� �������

    printf("Select items by key value from: ");
    scanf("%s", r1);
    printf("to: ");
    scanf("%s", r2);

    // ���� ������ �������� ������ ��������, ������ �������
    if(strcmp(r1, r2) > 0){
        char temp[8];
        strcpy(temp, r1);
        strcpy(r1, r2);
        strcpy(r2, temp);
    }

    for(int i = 0; i < SIZE; ++i){
		// ��� ���� �������� ��� key ����������� ��������� �������
        if(strcmp(arr_Item[i].key, r1) >= 0 && strcmp(arr_Item[i].key, r2) <= 0){
            newArr_Item[c] = arr_Item[i]; // ����������� ������ ������� �������� ���������
            c++;
        }
    }
    printTable(newArr_Item); // ������� �� ����� �������
}


int main(int argCount, const char* args[]){

    item arr_Item[SIZE]; // ������������� �������
    zeroItem(arr_Item); // ��������� �������

    // menu
    int n;
    while(n != 0){
        printf("\n0 - exit");
        printf("\n1 - add item");
        printf("\n2 - print item by number of element");
        printf("\n3 - print whole table, not empty items");
        printf("\n4 - remove items by key range");
        printf("\n5 - remove item by number of element");
        printf("\n6 - select items by key range");
        puts("");
        scanf("%d", &n);
        if(n == 1){
            addItem(arr_Item); // ��������� ����� �������
        }else if(n == 2){
            printOneElement(arr_Item); // ������� ���� ������� �� ������ ��������
        }else if(n == 3){
            printTable(arr_Item); // ������� �������� ��� busy == 1
        }else if(n == 4){
            removeByKeyRange(arr_Item); // ������� �������� ��������� �� ��������� key
        }else if(n == 5){
            getNoEandRemove(arr_Item); // ������� ���� ������� �� ��� ������
        }else if(n == 6){
            selectByKeyRange(arr_Item); // �������� �������� ��������� � ��������� ������ � ������� �� �����
        }
    }
    return 0;
}
