/*

此程序是彩票管理系统的用户信息结构体头文件
包含比较的函数


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/
#ifndef _USER_H
#define _USER_H

//=====model=====
typedef struct UserInfo
{
	char name[30];
	char pwd[30];
	char phoneNum[30];
	double balance;
}userInfo_t;

typedef struct adminInformation
{
    int issueNumAdmin;  //自动生成发布期号20161115++这种
    int price;       //彩票单价
    int stateOfLottery; //开奖状态，使用返回值0,1判断
    char winNum[30];         //开奖号码，随机生成6位数
    int soldOutNum;     //卖出彩票数量
    int bonusPool;   //奖金池金额
	struct adminInformation *next;
}adminInfo_t;



int cmpUser(void* a,void* b);

userInfo_t* searchNameInRBTree(rbTree_t* tree,char* inputName);

#endif
