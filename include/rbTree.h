/*

此程序是彩票管理系统的用户信息结构体红黑树头文件
包含红黑树的生成，创建等函数声明


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#ifndef _RBTREE_H
#define _RBTREE_H

#define RED 1
#define BLACK 2

//=====model=====
typedef struct RBTreeNode
{
	void *data;
	int color;
	struct RBTreeNode *parent;
	struct RBTreeNode *left;
	struct RBTreeNode *right;
}rbTreeNode_t;

typedef int(*cmp_t)(void* a,void* b);

typedef struct RBTree
{
	rbTreeNode_t *root;
	rbTreeNode_t *nil;
	
	int dataSize;
	cmp_t cmpUser;   //int(*cmpUser)(void*,void*)
}rbTree_t;


rbTree_t *createRBTree(int data,cmp_t cmpUser);

//把新的节点插入到红黑树中
void rbTreeInsertData(rbTree_t* tree,void *data);

void printAllTree(rbTree_t* tree,void(*print)(void*));


#endif


