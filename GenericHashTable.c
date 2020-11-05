/* Ex1 - GenericHashTable.c
Written by Odeya Teherani
ID: 318967148 */

//~~~~ INCLUDE ~~~~//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "GenericHashTable.h"

//~~~~~~~~~~~~~~~~ CREATE TABLE ~~~~~~~~~~~~~~~~~~~~~//
Table* createTable(int size, int dType, int listLength) {
    
	Table* table = (Table*)malloc(sizeof(Table));
	assert(table != NULL);

	table->arr = (Object**)malloc(size * sizeof(Object*));
	//assert(table->arr != NULL);

	for (int i = 0; i < size; i++)
		table->arr[i] = NULL;
    
	int x = 0;
	table->d = x;
	if(dType == INT_TYPE || dType == STR_TYPE)
	    table->type = dType;
	else
	    return NULL;
	   
	if(listLength > 0)
	    table->listLength = listLength;
	else
	    return NULL;
    if(size > 0)
	    table->tableFirstLength = size;
	else
	    return NULL;
	return table;
}


//~~~~~~~~~~~~~~~~ ADD ~~~~~~~~~~~~~~~~~~~~~//
int add(Table* table, void* data) {
if(data != NULL && table != NULL){
	int count = 1, flag = NOT_EQUAL;
	Object* newObj;
	if(table->type == STR_TYPE){
	    char* copyData = (char*)malloc(sizeof(char)*strlen(data)+1);
	    strcpy(copyData, data);
	    newObj = createObject(copyData);
	}
	else
	    newObj = createObject(data);
	newObj->next = NULL;
	int place = placeInTable(table, data);
	
	while (count <= pow(2, table->d)) {
		if (howMenyObject(table->arr[place]) == table->listLength) { //if the list is full
			count++;
			place++;
		}
		else {
			flag = EQUAL;
			break;
		}
	}

	if (flag == EQUAL) {     //the list didn't full
		Object* temp = table->arr[place];
		if (temp != NULL) {
			while (temp->next != NULL)
				temp = temp->next;
			temp->next = newObj;
		}
		else
			table->arr[place] = newObj;

		return place;
	}

	else {
		MultiplyTable(table);
		table->arr[(place - 1) * 2 + 1] = newObj;
		return ((place - 1) * 2 + 1);
	}
	return place;
	}
}

//~~~~~~~~~~~~~~~~ PLACE IN TABLE ~~~~~~~~~~~~~~~~~~~~~//
Object* search(Table* table, void* data) {
if(data != NULL && table != NULL){
	int count = 1;

	int place = placeInTable(table, data);

	while (count <= pow(2, table->d)) {
		Object *p = table->arr[place];

		while (p != NULL) { //Move on objects - on the list
			int result = isEqual(table->type, p->data, data);
			if (result == EQUAL)
				return p;
			p = p->next;
		}
		count++;
		place++;
	}
	}
	return NULL;
}

//~~~~~~~~~~~~~~~~ REMOVE OBJ ~~~~~~~~~~~~~~~~~~~~~//
int removeObj(Table* table, void* data) {
if(data != NULL && table != NULL){
	int place = placeInTable(table, data);

	for (int i = place; i < (pow(2, table->d) + place); i++) {
		if (table->arr[i] != NULL) {
			Object* p = table->arr[i], *prev = table->arr[place];
			if (isEqual(table->type, p->data, data) == EQUAL) {	//if the value in the head
				table->arr[i] = table->arr[i]->next;
				freeObject(p, table->type);
				return i;
			}
			while (p->next != NULL) { //Move on objects - on the list
				if (isEqual(table->type, p->data, data) == EQUAL) { //if the value in the middle
					prev->next = p->next;
					freeObject(p, table->type);
					return i;
				}
				prev = p;
				p = p->next;
			}
			if (isEqual(table->type, p->data, data) == EQUAL) { //if the value in the end
				prev->next = NULL;
				freeObject(p, table->type);
				return i;
			}
		}
	}
	}
	return -1;
}

//~~~~~~~~~~~~~~~~ PRINT TABLE ~~~~~~~~~~~~~~~~~~~~~//
void printTable(Table* table) {
if(table != NULL){
	int i = 0;
	int tableSize = ((table->tableFirstLength))*((int)pow(2, (table->d)));
	while (i < tableSize) {
		printf("[%d]\t", i);
		Object *p = table->arr[i];
		while (p != NULL) {
			if ((table->type) == INT_TYPE) {
				int* data = (int*)p->data;
				printf("%d\t-->\t", *data);
			}
			if ((table->type) == STR_TYPE)
				printf("%s\t-->\t", (char*)(p->data));
			p = p->next;
		}
		printf("\n");
		i++;
	}
	}
}

//~~~~~~~~~~~~~~~~ CREATE OBJECT ~~~~~~~~~~~~~~~~~~~~~//
Object* createObject(void* data) {
	Object* newObj = (Object*)malloc(sizeof(Object));
	assert(newObj != NULL);
	newObj->data = data;
	newObj->next = NULL;
	return newObj;
}

//~~~~~~~~~~~~~~~~ FREE OBJECT ~~~~~~~~~~~~~~~~~~~~~//
void freeObject(Object* obj, int type) {
	if (type == STR_TYPE)
		free(obj->data);
	free(obj);
}

//~~~~~~~~~~~~~~~~ IS EQUAL ~~~~~~~~~~~~~~~~~~~~~//
int isEqual(int type, void* data1, void* data2) {
	if (type == INT_TYPE) {
		int* data1_1 = (int*)data1;
		int* data2_2 = (int*)data2;
		if (*data1_1 == *data2_2)
			return EQUAL;
		else
			return NOT_EQUAL;
	}

	if (type == STR_TYPE) {
		if (strcmp((char*)data1 , (char*)data2) == 0)
			return EQUAL;
		else
			return NOT_EQUAL;
	}
}

//~~~~~~~~~~~~~~~~ INT HASH FUN ~~~~~~~~~~~~~~~~~~~~~//
int intHashFun(int* key, int origSize) {
    if( (*key) < 0)
        return (origSize - (abs(*key)% origSize));
	return (*key % origSize);
}

//~~~~~~~~~~~~~~~~ STR HASH FUN ~~~~~~~~~~~~~~~~~~~~~//
int strHashFun(char* key, int origSize) {
	int sum = 0, length = strlen(key);
	for (int i = 0; i < length; i++)
		sum += key[i];
	return (sum%origSize);
}

//~~~~~~~~~~~~~~~~ MULTIPLY TABLE ~~~~~~~~~~~~~~~~~~~~~//
void MultiplyTable(Table* table) {
	int oldTableSize = ((table->tableFirstLength))*((int)pow(2, (table->d)));

	Object** newArr = (Object**)malloc((oldTableSize * 2) * sizeof(Object*));
	assert(newArr != NULL);

	table->d = table->d + 1;

	for (int i = 0; i < oldTableSize * 2; i++)
		newArr[i] = NULL;
	for (int i = 0; i < oldTableSize; i++) {    	//copy value from arr to newArr
		newArr[i * 2] = table->arr[i];
		table->arr[i] = NULL;
	}

	free(table->arr);
	table->arr = (Object**)malloc((oldTableSize * 2) * sizeof(Object*));
	assert(table->arr != NULL);
	for (int i = 0; i < oldTableSize * 2; i++)  	//copy value from newArr to arr
		table->arr[i] = newArr[i];
	free(newArr);
}

//~~~~~~~~~~~~~~~~ HOW MENY OBJECT ~~~~~~~~~~~~~~~~~~~~~//
int howMenyObject(Object* arr) {
	int count = 0;
	Object* temp = arr;
	while (temp != NULL) {
		count++; temp = temp->next;
	}
	return count;
}

//~~~~~~~~~~~~~~~~ PLACE IN TABLE ~~~~~~~~~~~~~~~~~~~~~//
int placeInTable(Table* table, void* data) {
	int place = 0;
	if (table->type == INT_TYPE) {
		int* newData = (int*)data;
		place = intHashFun(newData, table->tableFirstLength)*((int)pow(2, (table->d)));
	}
	if (table->type == STR_TYPE) {
		char* newData = (char*)data;
		place = strHashFun(newData, table->tableFirstLength)*((int)pow(2, (table->d)));
	}
	return place;
}


//~~~~~~~~~~~~~~~~ FREE TABLE ~~~~~~~~~~~~~~~~~~~~~//
void freeTable(Table* table) {
	int i;
	int tableSize = (table->tableFirstLength)*((int)pow(2, table->d));

	for (i = 0; i < tableSize; i++) {
		if (table->arr[i] != NULL) {
			Object *prev = NULL;
			Object* p;
			int length = howMenyObject(table->arr[i]);
			while (length != 1) {
				p = table->arr[i];
				for (int j = 0; j < length - 1; j++) {
					prev = p;
					p = p->next;
				}
				freeObject(p, table->type);
				prev->next = NULL;
				length = length - 1;
			}
			freeObject(table->arr[i], table->type);
			table->arr[i] = NULL;
		}
	}
	free(table->arr);
	free(table);
}



#define F 			1 //free t1, t2, t3 --15 points

// check serach method
#define Test1 			1 // 5 points S1
#define Test2 			1 // 5 points S2
#define Test3 			1 // 5 points S3
#define Test4 			1 // 5 points S4
#define Test5 			1 // 5 points S5
#define Test6 			1 // 5 points S6
#define Test7 			1 // 5 points S7

// check remove method
#define Test8 			1 // 5 points R1
#define Test9 			1 // 5 points R2
#define Test10 			1 // 5 points R3
#define Test12 			1 // 5 points R4
#define Test11 			1 // 5 points R5
#define Test13 			1 // 5 points R6
#define Test14 			1 // 5 points R7

#define CHECK_INT 	1 // 5 points (create int table)
#define CHECK_STRING 1 // 5 points (create string table)
#define FREE_TABLE 	1 // 10 points if all heap blocks were freed
int count_test=0;
int main()
{
	//check all classes if they can get null data
    //check method 1
    //check method 2
    //check method n
    //----------------------------------------------
    //Input Validation
	Table* t1 = createTable(-1, 1, 1);
	printf("-------------------------------------------------------\n");
	printf("***Input Validation***\n");
	if(t1 != NULL)
	{
		printf("Didnt check size\n");
		if(F)
			freeTable(t1);
	}
    else
    {
        printf("Valid1-size checked +5\n");
        count_test+=5;
    }
	Table* t2 = createTable(1, -1, 1);
	if(t2 != NULL)
	{
		printf("Didnt check type\n");
		if(F)
			freeTable(t2);
	}
    else
    {
        printf ("Valid2-type checked +5\n");
        count_test+=5;
    }

	Table* t3 = createTable(1, 1, -1);
	if(t3 != NULL)
	{
		printf("Didnt check length\n");
		if(F)
			freeTable(t3);
	}
    else
    {
        printf ("Valid3-length checked +5\n");
        count_test+=5;
    }
	printf("-------------------------------------------------------\n");
	//Create Table
	if(CHECK_INT)
	{
		Table* t = createTable(4, INT_TYPE, 3);
		printf("***Create Table***\n");
		if(t == NULL)
    {
      printf("Error on CREATE\n");
  	}
    else
    {
      printf ("(Type INT) table created +5\n");
      count_test+=5;
    }
		printTable(t);
		printf("1-------------------------------------------------------\n");
		int x = 1;
		int y = 2;
		int z = 3;
		printf("***Add***\n");
		printf("New element index: %d\n", add(t, &y));
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));
		printTable(t);
		printf("2-------------------------------------------------------\n");
		printf("New element index: %d\n", add(t, &x));
		printTable(t);
		printf("3-------------------------------------------------------\n");
		printf("New element index: %d\n", add(t, &x));
		printf("New element index: %d\n", add(t, &x));
    printf("New element index: %d\n", add(t, &x));
    printTable(t);
    printf("4-------------------------------------------------------\n");
    printf("New element index: %d\n", add(t, &x));
    printf("New element index: %d\n", add(t, &x));
    printf("New element index: %d\n", add(t, &x));
    printTable(t);
    printf("5-------------------------------------------------------\n");
    printf("New element index: %d\n", add(t, &x));
    printf("New element index: %d\n", add(t, &x));
    printf("New element index: %d\n", add(t, &x));
		printTable(t);
		printf("6-------------------------------------------------------\n");
		printf("***Search***\n");
    if(Test2)
    {
				if(search(t, &x) == NULL && search(t, &y) == NULL)
        {
					printf("Test2-Error on search, didn't find %d\n", x);
        }
        else
        {
            count_test+=5;
            printf("Test2 -succeed, found %d : +5\n",x);
        }
    }
    if(Test4){
				if(search(t, &z) != NULL)
  			{
						printf("Test4-Error on search, found non existing element\n");
        }
        else
        {
            count_test+=5;
            printf("Test4 - succeed, +5\n"); // the z is not on list
        }
		}
		printf("***Remove***\n");
    if(Test9)
    {
			if(removeObj(t, &x) == -1){
				printf("Test9 - Error on remove\n");
      }
      else
      {
        count_test+=5;
      	printf("Test9 - succeed, obj removed +5\n");
      }
    }
		printTable(t);
    if(Test10)
    {
			if(removeObj(t, &y) == -1){
				printf("Test10 - Error on remove\n");
      }
      else
      {
        count_test+=5;
        printf("Test10 - succeed, obj removed +5\n");
      }
    }
    if(Test12)
    {
			if(removeObj(t, &z) != -1){
				printf("Test12 - Error on remove\n"); // you are try delete obj of not in list
      }
      else
      {
        count_test+=5;
        printf("Test12 - succeed, +5\n"); // z is not in list
      }
    }
    printf("7-------------------------------------------------------\n");
		printTable(t);
		if(FREE_TABLE)
		{
			freeTable(t);
		}
	}
	printf("-------------------------------------------------------\n");
	//====================String====================
	if(CHECK_STRING)
	{
		printf("******String Type******\n");
		Table* tStr = createTable(4, STR_TYPE, 3);
		printf("***Create Table***\n");
		if(tStr == NULL){
			printf("Error on CREATE\n");
    }
    else
    {
      printf ("(Type STRING) table created +5\n");
      count_test+=5;
    }
    printf("8-------------------------------------------------------\n");
		printTable(tStr);
    char* a = "11";
		char* b = "22";
		char* c = "33";
		printf("***Add***\n");
		printf("New element index: %d\n", add(tStr, a));
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));
		printf("9-------------------------------------------------------\n");
		printTable(tStr);
		printf("New element index: %d\n", add(tStr, b));
		printf("10-------------------------------------------------------\n");
		printTable(tStr);
		printf("New element index: %d\n", add(tStr, b));
		printf("New element index: %d\n", add(tStr, b));
    printf("New element index: %d\n", add(tStr, b));
    printf("11-------------------------------------------------------\n");
    printTable(tStr);
    printf("New element index: %d\n", add(tStr, b));
    printf("New element index: %d\n", add(tStr, b));
    printf("New element index: %d\n", add(tStr, b));
    printf("12-------------------------------------------------------\n");
    printTable(tStr);
    printf("New element index: %d\n", add(tStr, b));
    printf("New element index: %d\n", add(tStr, b));
    printf("New element index: %d\n", add(tStr, b));
		printf("13-------------------------------------------------------\n");
		printTable(tStr);
		if(Test6)
		{
			if(search(tStr, a) == NULL && search(tStr, b) == NULL){
				printf("Test6-Error on search\n");
      }
      else
      {
        printf("Test6 - succeed, found a && b : +5\n");
				count_test+=5;
      }
    }
		if(Test7)
		{
			if(search(tStr, c) != NULL){
				printf("Test7-Error on search\n");
      }
			else
      {
        count_test+=5;
        printf("Test7 - succeed, Didnt find c +5\n");
      }
		}
		if(Test13)
		{
			if(removeObj(tStr, a) == -1 && removeObj(tStr, b) == -1){
				printf("Test13 - Error on remove\n");
    	}
      else
      {
        count_test+=5;
        printf("Test13 - succeed, obj removed +5\n");
      }
    }
    if(Test14)
    {
			if(removeObj(tStr, c) != -1){
				printf("Test14 - Error on remove\n");
      }
      else
      {
        count_test+=5;
        printf("Test14 - succeed, obj removed +5\n");
      }
    }
    printf("14-------------------------------------------------------\n");
		printTable(tStr);
		printf("input validation to add \n");
    if(add(NULL, a) != -1 )
		{
        printf("Didnt check that table is not null in add\n");
		}
		else
		{
			printf ("Valid table checked in add +5\n");
			count_test+=5;
		}
		if(add(tStr, NULL) != -1 )
		{
        printf("Didnt check that data is not null in add\n");
		}
		else
		{
			printf ("Valid data checked in add +5\n");
			count_test+=5;
		}
		printf("input validation to search \n");
		if(search(NULL, a) != NULL )
				printf("Didnt check that table is not null in search\n");
		else
		{
			printf ("Valid table checked in search +5\n");
			count_test+=5;
		}
		if(Test5)
		{
			printf("***Search***\n");
			if(search(tStr, NULL) != NULL){
				printf("Test5-Error on search\n"); //you are search for null obj
      }
      else
      {
        printf("Test5 - succeed serch +5\n");
        count_test+=5;
      }
    }
		if(Test8)
		{
			if(removeObj(tStr, NULL) != -1){
					printf("Test8 - Error on remove\n"); //you are try delete null obj
      }
      else
      {
      	count_test+=5;
      	printf("Test8 - succeed +5\n");
    	}
    }

		if(FREE_TABLE)
		{
			freeTable(tStr);
		}
	}
  printf("***The grade is : %d ***\n",count_test); //the grade not include the Valgrind test, valgrind = 10 pts
	return 0;
}

