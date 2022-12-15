#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>
#include <strings.h>
#include <math.h>
#include "debug.h"
#include <iostream>
using namespace std;
#define MAX_OBJ (1000*1000)
#define N 4

#include "lock.h"
uint Counter = 0;

typedef struct _DATA {
	int key;
	int val;
    //オブジェクトを挿入readlock
    pthread_rwlock_t Lock;
	struct _DATA *next;
} DATA;

typedef struct _NODE {
	bool isLeaf;
	struct _NODE *chi[N];
	//the pointer to the kid node there are 4 kid node pointer here
	int key[N-1]; 
	//the number that determines which way to go. 3 keys here
	int nkey;
	//number of keys in a branch
	struct _NODE *parent;
} NODE;

typedef struct _TEMP {//T, block of memory that can hold n pairs
	bool isLeaf;
	NODE *chi[N+1]; // for internal split (for leaf, only N is enough)
	int key[N]; // for leaf split
	int nkey;
} TEMP;

DATA Head;
DATA *Tail;

NODE *Root = NULL;
