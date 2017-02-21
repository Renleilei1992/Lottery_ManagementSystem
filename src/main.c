/*

此程序是彩票管理系统主函数


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#include "view.h"
#include "control.h"
#include "user.h"
#include "rbTree.h"
#include "hashTable.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc,char **argv)
{
	//定义一个用户信息树，使用红黑树定义
	rbTree_t *userTree = createRBTree(sizeof(userInfo_t),cmpUser);
	readRBTree(userTree);
	//定义一个管理员信息表，使用链表定义
	adminInfo_t *adminList = createLinkList(sizeof(adminInfo_t));
	readList(adminList);
	//定义一个管理用户购买记录的信息表，使用哈西表定义
	hashTable_t *betTable = createHashTable(1);
	readTable(betTable);
	
	
//	loadUserInfo(head);
	mainMenuCtl(userTree,adminList,betTable);       //未完待续

	return 0 ;
}

