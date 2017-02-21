/*

此程序是彩票管理系统的用户信息
包含普通用户的账户信息，使用红黑树保存


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/
#include "rbTree.h"
#include "user.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static void rightRoate(rbTree_t* tree,rbTreeNode_t *x);
static void leftRoate(rbTree_t* tree,rbTreeNode_t *x);


//比较两个元素，此函数需要放入头文件  rbTree.h
int cmpUser(void* a,void* b)
{
	userInfo_t *userA = a ;
	userInfo_t *userB = b ;
	return strcmp(userA->name,userB->name);//返回值是0 即a和b相同，返回值1 即不同
}

//创建新的rbTree，返回值为指针 *tree   ，需要放入头文件 rbTree.h
rbTree_t *createRBTree(int dataSize,cmp_t cmpUser)  //此函数为创建新的红黑树，返回值为指针*tree
{
	rbTree_t *tree=(rbTree_t*)calloc(1,sizeof(rbTree_t));
	tree->nil = (rbTreeNode_t*)calloc(1,sizeof(rbTreeNode_t));
	tree->nil->color = BLACK;
	
	tree->root = tree->nil;

	tree->dataSize = dataSize;
	tree->cmpUser = cmpUser;

	return tree;
}


//局部静态变量函数，修正插入平衡，无需写入头文件
static void rbTreeInsertFixup(rbTree_t* tree,rbTreeNode_t *z)
{
	rbTreeNode_t *y = NULL ;
	while(z->parent->color == RED)
	{
		if(z->parent == z->parent->parent->left)
		{
			y = z->parent->parent->right;
			if(y->color == RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
				z = z->parent->parent;
			}else if(z == z->parent->right)
			{
				z = z->parent;
				leftRoate(tree,z);
			}else
			{
				z->parent->color = BLACK;
				z->parent->parent->color = RED;
				rightRoate(tree,z->parent->parent);
			}
		}else
		{
			y = z->parent->parent->left;
			if(y->color==RED)
			{
				z->parent->color = BLACK;
				y->color = BLACK;
				z->parent->parent->color = RED;
			}else if(z == z->parent->right)
			{
				z = z->parent;
				leftRoate(tree,z);
			}else
			{
				z->parent->color = BLACK;
				z->parent->parent->color =RED;
				rightRoate(tree,z->parent->parent);
			}
		}
	}
	tree->root->color=BLACK;
}
//把新的Z节点插入到红黑树中
static void rbTreeInsert(rbTree_t* tree,rbTreeNode_t* z)
{
	rbTreeNode_t *y = tree->nil;
	rbTreeNode_t *x = tree->root;
	if(NULL == tree || NULL==y || NULL ==z || NULL ==x)
		return;
	while(x != tree->nil)  //x变成nil，y变成z的父节点
	{
		y = x;
		if(tree->cmpUser(z->data,x->data)>0)  //z>x
		{
			x = x->left;
		}else
		{
			x = x->right;
		}
	}
	z->parent = y;   //如果z是第一个则它的parent为nil
	if(y == tree->nil)
	{
		tree->root = z;   //z为根节点的时候
	}else if(tree->cmpUser(z->data,y->data)>0) //判断z是y的左孩子还是右孩子
	{
		y->left = z;
	}else
	{
		y->right = z;
	}
	z->right = tree->nil;
	z->left = tree->nil;
	z->color = RED;
	rbTreeInsertFixup(tree,z);
}

//把新的结点插入到红黑树中，此函数需放入头文件  rbTree.h
void rbTreeInsertData(rbTree_t* tree,void* data)
{
	void *insertData = calloc(1,tree->dataSize);
	rbTreeNode_t *node = calloc(1,sizeof(rbTreeNode_t));//申请新结点

	memcpy(insertData,data,tree->dataSize);//将参数传入信息拷贝到insertData申请的存储空间中
	node->data = insertData;//将传入信息存入结点node

	rbTreeInsert(tree,node);//将新结点进行插入树操作
}


/*
//根据传入信息里的user.name与红黑树结点中存储的信息进行对比
void* searchName(rbTree_t* tree,void* data)
{
	void *inputName 
	rbTreeNode_t *node

	return *searchNameInRBTree(tree,)	
}*/


//根据注册时输入账户的名字查找rbTree中保存的用户信息
userInfo_t* searchNameInRBTree(rbTree_t* tree,char* inputName)
{
	rbTreeNode_t* z = tree->root ;
	if(NULL == tree || NULL== z)
		return NULL;
	while(z != tree->nil)  
	{
		if(strcmp(inputName,((userInfo_t*)z->data)->name)>0)  
		{
			z = z->left;
		}else if(strcmp(inputName,((userInfo_t*)z->data)->name)<0)
		{
			z = z->right;
		}else
		{
			return z->data;
		}
	}
	return NULL;	//存在疑问，若设置返回值，则无法正确判断，需要更改！
}	




//局部静态变量函数
static void leftRoate(rbTree_t* tree,rbTreeNode_t *x)
{
    rbTreeNode_t *y = x->right;
   //移动y的左孩子
    x->right = y->left;
    y->left->parent = x;

    //把y变成x的父亲的孩子
    y->parent =x->parent ;
    //确定parent的哪个指针指向y
    if(x->parent == tree->nil)//x是根节点
    {
        tree->root = y;
    }else if(x == x->parent->left)//x是左孩子
    {
        x->parent->left = y;
    }else{                          //x是右孩子
        x->parent->right = y;
    }

    //将x变为y的孩子
    x->parent=y;
    y->left =x;
}

//局部静态变量函数
static void rightRoate(rbTree_t* tree,rbTreeNode_t *x)
{
    rbTreeNode_t *y = x->left;

    x->left = y->right;
    y->right->parent = x;

    y->parent =x->parent ;
    //确定parent的哪个指针指向y
    if(x->parent == tree->nil)//x是根节点
    {
        tree->root = y;
    }else if(x == x->parent->left)//x是左孩子
    {
        x->parent->left = y;
    }else{                          //x是右孩子
        x->parent->right = y;
    }

    //将x变为y的孩子
    x->parent=y;
    y->right =x;
}

