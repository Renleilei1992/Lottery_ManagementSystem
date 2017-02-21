/*

此程序是彩票管理系统的菜单视图界面
包含主菜单界面、普通用户菜单界面、管理员菜单界面、公证人菜单界面


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#include "view.h"
#include <stdio.h>

void mainMenu(void)
{
	printf("=====欢迎进入彩票管理系统=====\n");
	printf("1.用户登录\n");
	printf("2.新用户注册\n");
	printf("3.退出\n");
	return;
}

void userMenu()
{
	printf("=====欢迎您进入彩票系统^_^=====\n");  //有时间可以尝试将"您"换为用户输入的账户名
	printf("1.彩票投注\n");
	printf("2.查询账户信息\n");
	printf("3.账户充值\n");
	printf("4.查询购买记录\n");
	printf("5.查询中奖记录\n");
	printf("6.修改密码\n");
	printf("7.返回主菜单\n");
	printf("8.退出程序\n");
	return;
}

void adminMenu(void)
{
	printf("=====欢迎管理员进入系统！=====\n");
	printf("1.发行最新一期彩票\n");
	printf("2.查询彩民信息    \n");         //按照彩民账户名查询
	printf("3.排序            \n");         //按照彩民账户名排序
	printf("4.保存            \n");         //要求是彩票信息存储于链表中，硬盘中文件形式
	printf("5.返回主菜单        \n");                //功能暂等待开发
	printf("6.退出程序        \n");
	return;
}

void notaryMenu(void)
{
	printf("=====欢迎公证人进入系统！=====\n");
	printf("1.最新一期彩票开奖 \n");
	printf("2.返回上层菜单 \n");
//	printf("2.查询历史开奖记录 \n");
//	printf("3.投注号码查询     \n");
	printf("3.退出程序         \n");
	return;
}
