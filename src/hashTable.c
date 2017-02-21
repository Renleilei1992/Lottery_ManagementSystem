#include "hashTable.h"
#include <string.h>
#include <stdlib.h>

int prime_array[28] = {
    17,             /* 0 */
    37,             /* 1 */
    79,             /* 2 */
    163,            /* 3 */
    331,            /* 4 */
    673,            /* 5 */
    1361,           /* 6 */
    2729,           /* 7 */
    5471,           /* 8 */
    10949,          /* 9 */
    21911,          /* 10 */
    43853,          /* 11 */
    87719,          /* 12 */
    175447,         /* 13 */
    350899,         /* 14 */
    701819,         /* 15 */
    1403641,        /* 16 */
    2807303,        /* 17 */
    5614657,        /* 18 */
    11229331,       /* 19 */
    22458671,       /* 20 */
    44917381,       /* 21 */
    89834777,       /* 22 */
    179669557,      /* 23 */
    359339171,      /* 24 */
    718678369,      /* 25 */
    1437356741,     /* 26 */
    2147483647,      /* 27 (largest signed int prime) */
};

unsigned long DJBHash(char *str,int len)  
{  
	unsigned long hash = 5381;  
	int i = 0;
	for(i = 0; i < len && str[i]!=0; i++)  
    {  
    	hash = ((hash << 5) + hash) + str[i];  
  	}  
	return hash;  
}  

//新建一个哈西表，并初始化申请的堆空间
hashTable_t* createHashTable(unsigned int level)
{
	hashTable_t *table = (hashTable_t*)calloc(1,sizeof(hashTable_t));
	table->size = prime_array[level];	

	table->heads = (hashHead_t*)calloc(table->size,sizeof(hashHead_t));

	return table;
}


/*
    keyStr和len: 用于生成key，由外界提供
    data: 要存入哈希表中的数据（data必须指向堆空间，且不能随意释放）
    table: 哈希表
*/
void insertHashTableData(hashTable_t* table,lottery_t *lottery,char *keyStr,int len )
{
	int key = 0;
	hashHead_t *heads = table->heads;

	key = DJBHash(keyStr,len)%(table->size);
	
	//初始化nodelist
	if(heads[key].nodeList==NULL)
	{
		heads[key].nodeList=(hashNode_t*)calloc(1,sizeof(hashNode_t));
		heads[key].nodeList->next = NULL;
	}

	//创建存放lottery的新节点
	hashNode_t *newNode = (hashNode_t*)calloc(1,sizeof(hashNode_t));
	newNode->lottery=lottery;

	//将新节点插入nodeList
	newNode->next = heads[key].nodeList->next;
	heads[key].nodeList->next = newNode;

}

/*
    keyStr和len: 用于生成key，由外界提供
    data: 要存入哈希表中的数据
    table: 哈希表
*/
void insertDataSafe(hashTable_t* table,
					lottery_t *lottery,
					int dataSize,
					char *keyStr,
					int len )
{
	void *p_insertData = calloc(1,dataSize);		
	memcpy(p_insertData,lottery,dataSize);
	
	insertHashTableData(table,p_insertData,keyStr,len);  //调用函数存入哈西表
}


//根据 键 在哈西表中寻找此键对应的数据，并返回一个指针
hashNode_t *searchByKey(hashTable_t* table,int key)
{
	hashHead_t *heads = table->heads;

	hashNode_t *nodeList = heads[key].nodeList;
	return nodeList;
}

/*
	strKey和len： 用于生成key的字符串
*/
hashNode_t *searchByStrKey(hashTable_t* table,char *strKey,int len)
{
	int key = DJBHash(strKey,len)%(table->size);
	return searchByKey(table,key);
}


void freeNodeList(hashNode_t *head)
{
	hashNode_t *toFree = NULL;
	hashNode_t *tmp = NULL;
	if(NULL == head)
		return;

	toFree = head->next;
	while(toFree != NULL)
	{
		tmp = toFree->next;
		free(toFree->lottery);
		free(toFree);
		toFree =tmp;
	}
	free(head);
}

void freeHashTable(hashTable_t* table)
{
	hashHead_t *heads = table->heads;
	int size = table->size;
	int i= 0;
	for(i = 0;i<size;i++)
	{
		freeNodeList(heads[i].nodeList);
	}
	free(heads);
	free(table);
}
