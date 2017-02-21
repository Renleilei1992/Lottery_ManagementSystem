/*

此程序是彩票管理系统控制函数头文件
包含主菜单控制、用户菜单控制、管理员菜单控制、公证人菜单控制及各类需求函数控制


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#ifndef _CONTROL_H
#define _CONTROL_H
#include "rbTree.h"
#include "user.h"
#include "hashTable.h"

void mainMenuCtl(rbTree_t *userTree,adminInfo_t *adminList,hashTable_t *betTable);

void adminMenuCtl(adminInfo_t *adminList,hashTable_t *betTable,rbTree_t *userTree);

void notaryMenuCtl(adminInfo_t *head);

//=============================

void userLogin(rbTree_t *userTree,adminInfo_t *adminList,hashTable_t *betTable);

void userMenuCtl(userInfo_t *tmp,hashTable_t *betTable,adminInfo_t *adminList);

void changePwd(userInfo_t *tmp);

void addBalance(userInfo_t *tmp);

void createNewUser(rbTree_t *userTree);

//=======管理员菜单操作========

void releaseNewIssue(adminInfo_t *adminList);

adminInfo_t* createLinkList(int size);

void insertIssueNum(adminInfo_t *head);

void runTheLottery(adminInfo_t *adminList);

void addTail(adminInfo_t *head,adminInfo_t *newNode);

void printLinkList(adminInfo_t *head);

void destroyList(adminInfo_t **head);

void destroyRBTreeAll(rbTree_t *tree);

adminInfo_t *searchInLinkList(adminInfo_t *head,int num);

//=======彩民菜单操作=======



//=======读取信息操作======

void readList(adminInfo_t *head);

void readTable(hashTable_t *table);

void readRBTree(rbTree_t *tree);

#endif





