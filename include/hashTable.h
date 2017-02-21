/*

此程序是彩票管理系统的用户购买记录的哈西表头文件
包含哈西表的生成，创建等函数声明


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

extern int prime_array[28];

unsigned long DJBHash(char* str,int len);

typedef struct lottery
{
    int issueNum;     //发行期号
    int onlyId;       //单张彩票唯一ID
    char betNum[30]; //彩票投注号码，使用字符串形式，3位字符，每个数位数字是0-9
    char userName[30];      //购买此期彩票用户名，唯一的
    int numOfBets;         //单注彩票重复投注次数
    int winOrNot;  //是否赢得彩票，彩票只设置一等奖，完全匹配为>获奖
    int bonus;//获奖金额，奖金池金额/200或者固定200元，单注彩票单价为2元   
}lottery_t;

typedef struct hashNode
{
	lottery_t *lottery;
	struct hashNode* next;
}hashNode_t;

typedef struct hashHead
{
	hashNode_t *nodeList;
}hashHead_t;

typedef struct hashTable
{
	hashHead_t *heads;
	int size;
}hashTable_t;






hashTable_t* createHashTable(unsigned int level);

/*
keyStr和len: 用于生成key，由外界提供
data: 要存入哈希表中的数据（data必须指向堆空间，且不能随意释放）
table: 哈希表
*/
void inserHashTabletData(hashTable_t* table,lottery_t *lottery,char *keyStr,int len );

/*
keyStr和len: 用于生成key，由外界提供
data: 要存入哈希表中的数据
table: 哈希表
*/
void insertDataSafe(hashTable_t* table,lottery_t *lottery,int dataSize,char *keyStr,int len );

hashNode_t *searchByKey(hashTable_t* table,int key);

hashNode_t *searchByStrKey(hashTable_t* table,char *strKey,int len);


void freeHashTable(hashTable_t* table);
void freeNodeList(hashNode_t *head);



#endif


