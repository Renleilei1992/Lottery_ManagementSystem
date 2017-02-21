/*

此程序是彩票管理系统控制文件，包含普通用户控制、管理员控制、公证人控制


Author   : Renleilei
Date     : 2016-11-21
Version  : 1.0


*/

#include "control.h"
#include "view.h"
#include "user.h"
#include "rbTree.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include<termios.h>
#include<unistd.h>
#include<assert.h>
#include<unistd.h>





static void bet(hashTable_t *betTable,userInfo_t *tmp,adminInfo_t *adminList);

static void buyLottery(userInfo_t *tmp,hashTable_t *betTable,adminInfo_t *adminList);

static void createKey(char *userName1,int issueNum,char *strKey);

static void chooseNum(char *num);

static void searchInHashTable(hashTable_t *table,userInfo_t *tmp);

static void printUserInfo(userInfo_t *tmp);

static void searchWinOrNot(adminInfo_t *head,hashTable_t *table,userInfo_t *tmp);

static void getUserInfo(hashTable_t *table,rbTree_t* userTree);

static void printfAllTree(rbTree_t* tree);

static void printTreeParent(rbTree_t* tree,rbTreeNode_t* parent);
/*
static void freeTable(hashTable_t* table);
*/
static void fileListSave(adminInfo_t *head);

static void fileTreeSave(rbTree_t *tree);

static void fileTreeSaveFunc(rbTree_t *tree,rbTreeNode_t *parent);

static void fileTableSave(hashTable_t *table);

static void destroyRBTree(rbTree_t *tree,rbTreeNode_t *parent);

static void notaryDraw(adminInfo_t *head);

static void draw(adminInfo_t *head,char *num);

static void lotteryDraw(adminInfo_t *head);

static int getch();

static int getch()   //定义一个不需要切换至控制台的getch函数
{
	int c = 0;
    struct termios org_opts, new_opts;
	int res=0;
 	res=tcgetattr(STDIN_FILENO, &org_opts);
 	assert(res==0);
  	memcpy(&new_opts, &org_opts, sizeof(new_opts));
 	new_opts.c_lflag &= ~(ICANON | ECHO | ECHOE | ECHOK | ECHONL | ECHOPRT | ECHOKE | ICRNL);
 	tcsetattr(STDIN_FILENO, TCSANOW, &new_opts);
	c=getchar();
	res=tcsetattr(STDIN_FILENO, TCSANOW, &org_opts);
	assert(res==0);

	return c;
}



void mainMenuCtl(rbTree_t *userTree,adminInfo_t *adminList,hashTable_t *betTable)
{
	char choice = 0 ;
	while(1)
	{
		mainMenu();
		scanf("%c",&choice);
		while(getchar()!='\n');  //清空输入缓存
		
		switch(choice)
		{
		case '1':
			userLogin(userTree,adminList,betTable);     //用户登录，包括普通用户登录及管理员公证人登录
			break;
		case '2':
			createNewUser(userTree);   //注册新用户
			break;
		case '3':
			system("clear");
//			destroyList(&adminList);
//			destroyRBTreeAll(userTree);
//			freeTable(betTable);
			printf("=====谢谢使用！=====\n");
			exit(0);
		default:
			printf("输入错误，请重新选择！\n");
			break;
		}
	}
}

//============三个登录菜单的控制函数==============

void adminMenuCtl(adminInfo_t *adminList,hashTable_t *betTable,rbTree_t *userTree)      //管理员菜单主控函数
{
	char choice = 0 ;
	while(1)
	{
		adminMenu();	
		scanf("%c",&choice);
		while(getchar()!='\n');
		switch(choice)
		{
		case '1':		//发行一期新的彩票
			releaseNewIssue(adminList);              //发行新一期彩票的函数
			break;
		case '2':		//查看彩民信息
			getUserInfo(betTable,userTree);
			break;
		case '3':		//排序
			printfAllTree(userTree);
			break;
		case '4':		//保存     
			fileListSave(adminList);
			fileTableSave(betTable);
			fileTreeSave(userTree);
			printf("文件保存成功!\n");
			break;  
		case '5':		//返回主菜单
//			destroyList(adminList);
			return;
		case '6':		//退出程序
			system("clear");
			printf("=====谢谢使用！=====\n");
			exit(0);
		default:
			printf("输入错误，请重新选择功能或退出程序！\n");
			break;
		}
	}
}

void userMenuCtl(userInfo_t *tmp,hashTable_t *betTable,adminInfo_t *adminList) 			//用户菜单主控函数
{
//	userInfo_t userAccount;

//  memset(&userAccount,0,sizeof(userInfo_t));
//	while(userLogin(userTree)==1)
	while(1)
	{
		char choice = 0;
//		double addBalance = 0;
		userMenu();
		scanf("%c",&choice);
		while(getchar()!='\n');

		switch(choice)
		{
		case '1':   //彩票投注
			bet(betTable,tmp,adminList);
			break;   
		case '2':	//查询账户信息
	//		system("clear");
			printUserInfo(tmp);
			break;
		case '3':	//账户充值
			system("clear");
			addBalance(tmp);
			break;
		case '4':	//查询购买记录
			searchInHashTable(betTable,tmp);
			break;
		case '5':	//查询是否中奖
			searchWinOrNot(adminList,betTable,tmp);
			break;
		case '6':	//修改密码
			changePwd(tmp);     //新增修改红黑树中该账户对应密码的函数
			break;
		case '7':	//返回主菜单
			system("clear");
			return;
		case '8':	//退出
			system("clear");
			printf("=====谢谢使用，欢迎下次投注！=====\n");
			exit(0);
			break;
		default:
			printf("输入错误，请重新选择功能或退出程序！\n");
			break;
		}	
	}
}

void notaryMenuCtl(adminInfo_t *head)
{
	while(1)
	{
		char choice = 0 ;
		notaryMenu();
	//	printf("%c",choice);
		scanf("%c",&choice);
		printf("%c",choice);
		while(getchar()!='\n');

		switch(choice)
		{
		case '1':
			lotteryDraw(head);   //彩票开奖，需要传入彩票发行状态
			break;
		case '2':					//返回上层菜单
			system("clear");
			return;
		case '3':                 //退出程序
			system("clear");
			printf("=====谢谢使用!===== \n");
			exit(0);
		default:
			printf("输入信息有误，请重新选择功能或退出程序!\n");
			break;
		}
	}	

}


//========菜单主控函数中使用到的功能型函数=============

//====公证员菜单中功能函数====

static void lotteryDraw(adminInfo_t *head)
{
	adminInfo_t *last = head ;
	while(last->next != NULL)
	{
		last = last->next ;
	}
	printf("====彩票发行状态为:%d====\n",last->stateOfLottery);

	if(last->stateOfLottery == 1)
	{
		char choice1 = 0 ;
		printf("本期彩票正在发行!\n");
		printf("是否开奖?\n");
		printf("1,输入密码并开奖\n");
		printf("2,返回上层菜单\n");
		printf("3,退出\n");
		scanf("%c",&choice1);
		while(getchar()!='\n');
		switch(choice1)
		{
		case '1':
			notaryDraw(head);
			break;
		case '2':
			return;
		case '3':
			printf("=====谢谢使用!=====\n");
			exit(0);
		default:
			printf("您的输入有误!\n");
			break;
		}
	}

}

static void notaryDraw(adminInfo_t *head)
{
	int i = 0 ;
	char pwdIn[30] = {0};
	char pd[128] = {0};
	char num[30] = {0} ;
	adminInfo_t *last = head ;
	while(last->next != NULL)
	{
		last = last->next ;
	}
	printf("请输入您的密码:");
    for(i=0;;i++)
    {
        pd[i]=getch();
        if(pd[i]=='\n')
        {
            pd[i]='\0';
            break;
        }
        if(pd[i]==127)
        {
            printf("\b \b");
            i=i-2;
        }
        else
        {
            printf("*");
        }
        if(i<0)
        {
            pd[0]='\0';
        }
    }
    strcpy(pwdIn,pd);
	printf("\n");
//	fgets(pwdIn,30,stdin);
//	pwdIn[strlen(pwdIn)-1] = 0;
	if(strncmp("notary",pwdIn,30) == 0)
	{
		draw(head,num);  //由于将选取号码作为了字符串
		strncpy(last->winNum,num,30);
		last->stateOfLottery = 0;
		sleep(2);
		printf("中奖号码为: %s\n",last->winNum);
		return;
	}
}




static void draw(adminInfo_t *head,char *num)
{
	char num1[30] = {0} ;
	char num2[30] = {0} ;
	char num3[30] = {0} ;
	char num4[30] = {0} ;
	char num5[30] = {0} ;
	char num6[30] = {0} ;
//	int arr[5] = {0} ;
//	int i,j,k,count = 0;	
	printf("请选择第一个中奖号码(1~35):");
	fgets(num1,30,stdin);
	num1[strlen(num1)-1]=0;
	printf("请选择第二个中奖号码(1~35):");
	fgets(num2,30,stdin);
	num2[strlen(num2)-1]=0;
	printf("请选择第三个中奖号码(1~35):");
	fgets(num3,30,stdin);
	num3[strlen(num3)-1]=0;
	printf("请选择第四个中奖号码(1~35):");
	fgets(num4,30,stdin);
	num4[strlen(num4)-1]=0;
	printf("请选择第五个中奖号码(1~35):");
	fgets(num5,30,stdin);
	num5[strlen(num5)-1]=0;
	printf("请选择第六个中奖号码(1~35):");
	fgets(num6,30,stdin);
	num6[strlen(num6)-1]=0;

	printf("您选择的彩票号码为: %s %s %s %s %s %s \n",num1,num2,num3,num4,num5,num6);
	strcat(num,num1);
	strcat(num,num2);
	strcat(num,num3);
	strcat(num,num4);
	strcat(num,num5);
	strcat(num,num6);
	
/*
   	srand((int)time(NULL));

	while(count != 0)
	{
		count = 0;
		for(i=0;i<6;i++)
		{
    		arr[i] = rand()%35 + 1;              //随机数生成在1-35范围内
		}
		for(j=0;j<5;j++)
		{
			for(k=j+1;k<6;k++)
			{
				if(arr[j] == arr[k])
				{
					count++;
				}
			}
		}
	}
	printf("中奖号码为:%d %d %d %d %d %d \n",arr[0],arr[1],arr[2],arr[3],arr[4],arr[5]);
	sprintf(num1,"%d",arr[0]);
	sprintf(num2,"%d",arr[1]);
	sprintf(num3,"%d",arr[2]);
	sprintf(num4,"%d",arr[3]);
	sprintf(num5,"%d",arr[4]);
	sprintf(num6,"%d",arr[5]);
	
	strcat(num,num1);
	strcat(num,num2);
	strcat(num,num3);
	strcat(num,num4);
	strcat(num,num5);
	strcat(num,num6);

	printf("num = %s \n",num);*/
}




//====彩民菜单中功能型函数====

static void bet(hashTable_t *betTable,userInfo_t *tmp,adminInfo_t *adminList)
{

	buyLottery(tmp,betTable,adminList);
	
}

//彩民购买彩票函数，静态
static void buyLottery(userInfo_t *tmp,hashTable_t *betTable,adminInfo_t *adminList)
{
	lottery_t lotteryInfo;
	adminInfo_t *last = adminList;
	while(last->next != NULL)
	{
		last = last->next;
	}
	
	printf("本期彩票期号为: %d\n",2016000+last->issueNumAdmin);
	printf("本期彩票单价为: %d\n",last->price);
	printf("本期彩票发行状态为: %d\n",last->stateOfLottery);
	printf("注:发行状态为1时可以购买！\n");
	printf("您的可用余额为: %lf \n",tmp->balance);
	double m = 0 ;
	double n = last->price;
	double i = 0 ;
	char strKey[30] = {0};
	char num[30] = {0} ;
	int a = 0 ;
//	char ch1[30] = {0};

	srand((int)time(NULL));
	a = rand()%1000;	

	if(NULL == tmp || betTable == NULL)
	{
		printf("程序异常！\n");
		exit(0);
	}
	while(last->next != NULL)
	{
		last = last->next;
	}
	if(last->stateOfLottery != 1)
	{
		printf("本期彩票尚未发行，请稍后再购买！\n");
		return;
	}
	chooseNum(num);
	printf("想要购买此号码注数为:");
	scanf("%lf",&m);
	while(getchar()!='\n');
	m>5?(m=5):(m=m);

	i = tmp->balance - m*n ;
	if(i>=0)
	{
		printf("购买成功!\n");
		last->soldOutNum = last->soldOutNum + m;//存入管理员链表
		last->bonusPool = last->bonusPool + m*(last->price);
		lotteryInfo.numOfBets = (int)m ;//准备传入注数
//		sprintf(ch1,"%d",2016000+last->issueNumAdmin);//准备传入期号
//		strncpy(lotteryInfo.issueNum,ch1,30);//准备传入期号
		strncpy(lotteryInfo.userName,tmp->name,30);
		lotteryInfo.issueNum = 2016000+last->issueNumAdmin;
		lotteryInfo.onlyId = 2016000+a;		//准备传入唯一ID
		strncpy(lotteryInfo.betNum,num,30);          //准备传入购买号码
//		lotteryInfo.winOrNot = x ;


		createKey(tmp->name,lotteryInfo.issueNum,strKey);//创建键值

		insertDataSafe(betTable,&lotteryInfo,sizeof(lottery_t),strKey,30);

		tmp->balance = i;
		printf("账户名:%s\n",lotteryInfo.userName);
		printf("期号: %d\n",lotteryInfo.issueNum);
		printf("唯一ID:%d\n",lotteryInfo.onlyId);
		printf("注数:%d \n",lotteryInfo.numOfBets);
		printf("键:%s\n",strKey);
//		printf("键的大小:%lu\n",sizeof(strKey));
		return ; 
	}else
	{
		printf("您的余额不足，请充值！\n");
		addBalance(tmp);
		return;
	}
}

static void createKey(char *userName1,int issueNum1,char *strKey)
{
//	char strKey[30] = {0} ;
	char ch[30] = {0} ;
//	char strKey1[30] = {0} ;
	sprintf(ch,"%d",issueNum1);
	strncat(strKey,userName1,10);
	strncat(strKey,ch,10);

//	return strKey;
}


static void chooseNum(char* num)
{
	char num1[30] = {0} ;
	char num2[30] = {0} ;
	char num3[30] = {0} ;
	char num4[30] = {0} ;
	char num5[30] = {0} ;
	char num6[30] = {0} ;
//	char num[30] = {0} ;

	printf("请选择您需要投注的第一个号码(1~35):");
	fgets(num1,30,stdin);
	num1[strlen(num1)-1]=0;
	printf("请选择您需要投注的第二个号码(1~35):");
	fgets(num2,30,stdin);
	num2[strlen(num2)-1]=0;
	printf("请选择您需要投注的第三个号码(1~35):");
	fgets(num3,30,stdin);
	num3[strlen(num3)-1]=0;
	printf("请选择您需要投注的第四个号码(1~35):");
	fgets(num4,30,stdin);
	num4[strlen(num4)-1]=0;
	printf("请选择您需要投注的第五个号码(1~35):");
	fgets(num5,30,stdin);
	num5[strlen(num5)-1]=0;
	printf("请选择您需要投注的第六个号码(1~35):");
	fgets(num6,30,stdin);
	num6[strlen(num6)-1]=0;

	printf("您选择的彩票号码为: %s %s %s %s %s %s \n",num1,num2,num3,num4,num5,num6);
	strcat(num,num1);
	strcat(num,num2);
	strcat(num,num3);
	strcat(num,num4);
	strcat(num,num5);
	strcat(num,num6);

//	return num;
}

//在哈西表中遍历 键 并打印出信息，信息包括

static void searchInHashTable(hashTable_t *table,userInfo_t *tmp)
{
	char strKey[30] = {0};
	int queryIssueNum = 0 ;
	hashNode_t *nodeFound = NULL ;
	hashNode_t *temp1 = NULL ;
	
	printf("请输入您要查询的期号:");
	scanf("%d",&queryIssueNum);
	while(getchar()!='\n');
	

	createKey(tmp->name,queryIssueNum,strKey);
	nodeFound = searchByStrKey(table,strKey,30);
	if(nodeFound != NULL)
	{
		temp1 = nodeFound->next;
	}else
	{
		printf("您输入的期数不存在!\n");
		return;
	}

	if(temp1 == NULL)
	{
		printf("无此记录！\n");
	}else
	{
		while(temp1 != NULL)
		{
			if(queryIssueNum == temp1->lottery->issueNum
					&& strcmp(tmp->name,temp1->lottery->userName)==0)
			{
				printf("查询账号:%s \n",temp1->lottery->userName);
				printf("查询期号:%d \n",temp1->lottery->issueNum);
				printf("唯一ID:%d \n",temp1->lottery->onlyId);
				printf("投注号码:%s \n",temp1->lottery->betNum);
				printf("投注号码注数:%d \n",temp1->lottery->numOfBets);
			}else
			{
				printf("未找到此期信息!\n");
			}
			temp1 = temp1->next;
		}
	}	
}


static void searchWinOrNot(adminInfo_t *head,hashTable_t *table,userInfo_t *tmp)
{
	char theWinNum[30] = {0} ;
	int queryIssueNum = 0 ;
	char strKey[30] = {0};
	hashNode_t *nodeFound = NULL ;
	hashNode_t *temp2 = NULL ;
	adminInfo_t *temp1 = head ;

	printf("请输入您要查询的期号:");
	scanf("%d",&queryIssueNum);
	while(getchar()!='\n');

//	temp1 = temp1->next;

	while(temp1 != NULL)
	{
		if(temp1->issueNumAdmin == (queryIssueNum-2016000)) 
		{
			if(temp1->stateOfLottery == 1)
			{
				printf("本期尚未开奖！请静待佳音！\n");
				return;
			}else
			{	
				strcpy(theWinNum,temp1->winNum);
				printf("中奖号码为: %s\n",theWinNum);
			}	
		}
		temp1=temp1->next;
	}
	printf("期号为: %d\n",queryIssueNum);
//	printf("中奖号码3为: %s\n",theWinNum);

	createKey(tmp->name,queryIssueNum,strKey);
	nodeFound = searchByStrKey(table,strKey,30);
	if(nodeFound != NULL)
	{
		temp2 = nodeFound->next;
	}else
	{
		printf("未能查找到此期信息!\n");
		return;
	}
	while(temp2 != NULL)
	{	
		if(theWinNum != NULL && (strncmp(temp2->lottery->betNum,theWinNum,30)==0))
		{
			temp2->lottery->winOrNot = 1 ;
//			i = temp2->lottery->soldOutNum ;
			printf("是否获奖:%d\n",temp2->lottery->winOrNot);
			printf("恭喜您中奖啦！%c\n",'\a');//响铃一声
			printf("您的获奖金额为:%d元!\n",temp2->lottery->numOfBets*10);
			tmp->balance = tmp->balance + temp2->lottery->numOfBets*10;
			return;
		}else
		{
			temp2->lottery->winOrNot = 0 ;
			printf("是否获奖:%d\n",temp2->lottery->winOrNot);
			printf("很遗憾您未能中奖!下次再接再厉！\n");
			return;
		}
	}
	
}




//====管理员菜单中功能型函数====

//管理员获取用户信息,购买记录和余额
static void getUserInfo(hashTable_t *table,rbTree_t* userTree)       
{
	userInfo_t userAccount;
	userInfo_t *query;
    memset(&userAccount,0,sizeof(userInfo_t));       
	printf("请输入您要查询的用户名:");
	fgets(userAccount.name,30,stdin);
	userAccount.name[strlen(userAccount.name)-1] = 0 ;

	printf("该用户的账户名为:%s\n",userAccount.name);
//	printf("该用户的账户余额:%lf\n",tmp->balance);
//	printf("该用户手机号码为:%s\n",tmp->phoneNum);
	
	query = searchNameInRBTree(userTree,userAccount.name);

	if(query != NULL)
	{
		searchInHashTable(table,query);  //按用户名查找
		return;
	}else
	{
		printf("您输入的账户名不存在!\n");  
		return;
	}
}

static void fileListSave(adminInfo_t *head)
{
	FILE *listSave;
	//链表存入
	listSave = fopen("listFile.db","w");
	adminInfo_t *last = head->next;
	while(last != NULL)
	{
		fwrite(last,sizeof(adminInfo_t),1,listSave);
		last = last->next;
	}
	fclose(listSave);
}

//哈西表存入
static void fileTableSave(hashTable_t *table)
{
	int key = 0 ;
	int p = 0 ;
	FILE *tableSave;
	p = table->size ;
	hashNode_t *tmp;
	hashHead_t *heads = table->heads ;

	tableSave = fopen("tableFile.db","w");
	for(key=0;key<p;key++)
	{
		if(heads[key].nodeList != NULL)
		{	
			tmp = heads[key].nodeList->next;
			while(tmp != NULL)
			{
				fwrite(tmp->lottery,sizeof(lottery_t),1,tableSave);
				tmp = tmp->next; 
			}
		}
	}
	fclose(tableSave);
}


//红黑树存入

static void fileTreeSave(rbTree_t *tree)
{
	fileTreeSaveFunc(tree,tree->root);
}
static void fileTreeSaveFunc(rbTree_t *tree,rbTreeNode_t *parent)
{
	FILE *treeSave;
    if(tree->nil == parent)
    {   
        return;
    }       
	treeSave = fopen("treeFile.db","a");
	fwrite((userInfo_t*)parent->data,sizeof(userInfo_t),1,treeSave);
	fileTreeSaveFunc(tree,parent->right);
	fileTreeSaveFunc(tree,parent->left);
	fclose(treeSave);
}

//读取链表管理员信息
void readList(adminInfo_t *head)
{
	FILE *listFileOpen;

	if(NULL == head)
	{
		printf("链表初始化失败!\n");
		return;
	}
	listFileOpen = fopen("listFile.db","r");
	if(NULL == listFileOpen)
	{
		printf("管理员信息不存在!\n");
		return;
	}

	while(1)
	{
		adminInfo_t *newNode =(adminInfo_t*)malloc(sizeof(adminInfo_t));//临时
		memset(newNode,0,sizeof(adminInfo_t));
		fread(newNode,sizeof(adminInfo_t),1,listFileOpen);
		if(fread(newNode,sizeof(adminInfo_t),1,listFileOpen)<1)
		{
			free(newNode);
			fclose(listFileOpen);
			return;
		}
		addTail(head,newNode);
	}
}

//读取哈希表彩票信息
void readTable(hashTable_t *table)
{
	char strKey[30]={0};
	FILE *tableFileOpen = NULL ;
	if(NULL == table)
	{
		printf("哈西表初始化失败!\n");
		return;
	}
	lottery_t lotteryInfo;
	memset(&lotteryInfo,0,sizeof(lottery_t));

	tableFileOpen = fopen("tableFile.db","r");
	if(NULL == tableFileOpen)
	{
		printf("彩票信息不存在!\n");
		return;
	}
	while(fread(&lotteryInfo,sizeof(lottery_t),1,tableFileOpen)==1)
	{
		createKey(lotteryInfo.userName,lotteryInfo.issueNum,strKey);//创建键值
		insertDataSafe(table,&lotteryInfo,sizeof(lottery_t),strKey,30);
		memset(&lotteryInfo,0,sizeof(lottery_t));
	}
	printf("彩票信息(哈希表)载入成功!\n");
	fclose(tableFileOpen);
}


//读取红黑树用户数据
void readRBTree(rbTree_t *tree)
{
	FILE *rbTreeOpen;
	if(tree == NULL)
	{
		printf("红黑树初始化失败!\n");
		return;
	}
	printf("请稍等!\n");
	sleep(2);
	rbTreeOpen = fopen("treeFile.db","r");
	if(NULL == rbTreeOpen)
	{
		printf("用户信息不存在!\n");
		return;
	}
	userInfo_t userInfo;
	memset(&userInfo,0,sizeof(userInfo_t));

	while(fread(&userInfo,sizeof(userInfo_t),1,rbTreeOpen)==1)
	{
		rbTreeInsertData(tree,&userInfo);
		memset(&userInfo,0,sizeof(userInfo_t));
	}	
	printf("用户信息(红黑树)载入成功!\n");
	fclose(rbTreeOpen);
}





void releaseNewIssue(adminInfo_t *adminList)      //彩票发行主控函数
{
	runTheLottery(adminList);
//	insertIssueNum(adminList);
//	system("clear");
	printLinkList(adminList);	
}

adminInfo_t* createLinkList(int size)  //创建管理员菜单控制信息链表
{
	//头结点存储在堆中，定义并初始化
	adminInfo_t *head =  (adminInfo_t *)calloc(1,sizeof(adminInfo_t));
	head->next = NULL;   //单向不循环链表，next指向下一个结点
	return head;
}


void runTheLottery(adminInfo_t *head)    //彩票发行受控函数
{
	int i = 0 ;
	head->issueNumAdmin = 0 ;
	head->bonusPool = 0 ;
	adminInfo_t *last = head;
	while(last->next != NULL)
	{
		last = last->next;
	}							//开奖状态需要根据公证员是否开奖而变化
	i = last->stateOfLottery ;  //根据上一次彩票的开奖状态判断是否发行最新一期彩票
	if(i == 0)
	{
		adminInfo_t *newNode = (adminInfo_t *)calloc(1,sizeof(adminInfo_t));
//找到上一个结点last;用来获取结点的期号和状态信息；
		newNode->issueNumAdmin = last->issueNumAdmin+1;		//期号自动生成
		newNode->price = 2 ;								//单价始终为2
		newNode->stateOfLottery = last->stateOfLottery+1;		//状态还需要商榷
		printf("=====发行成功！=====\n");
		addTail(head,newNode);
		return;
	}else
	{
		printf("=====发行失败！(上期彩票还处于发行状态)=====\n");
//		system("pause");
		return;
	}
}	

void addTail(adminInfo_t *head,adminInfo_t *newNode)  //链表尾插 结点
{
	adminInfo_t *last = head ;
	while(last->next != NULL)
	{
		last = last->next;
	}
	last->next = newNode;
	newNode->next = NULL;
}

static void printfAllTree(rbTree_t* tree)
{
	printf("账户名      余额        手机号      \n");
	printTreeParent(tree,tree->root);
}



//使用递归打印红黑树中所有用户，存入红黑树过程中已经排序
static void printTreeParent(rbTree_t* tree,rbTreeNode_t* parent)
{
    if(tree->nil == parent)
    {   
        return;
    }       

	printf("%-12s%-12lf%s\n",((userInfo_t*)parent->data)->name,((userInfo_t*)parent->data)->balance,((userInfo_t*)parent->data)->phoneNum);
    printTreeParent(tree,parent->left);
    printTreeParent(tree,parent->right);
}

//使用递归销毁所有的红黑树结点

void destroyRBTreeAll(rbTree_t *tree)
{
	destroyRBTree(tree,tree->root);
	free(tree);
	free(tree->nil);
	tree = NULL;
}

static void destroyRBTree(rbTree_t *tree,rbTreeNode_t *parent)
{
    if(tree->nil == parent)
    {   
		return;
	}	

	destroyRBTree(tree,parent->left);
	destroyRBTree(tree,parent->right);
	free(parent);
}

/*
void destroyRBTreeNode(rbTreeNode_t *node)
{
	if(NULL != node->left)
	{
		destroyRBTreeNode(node->left);
	}
	if(NULL != node->right)
	{
		destroyRBTreeNode(node->right);
	}
	free(node);
}
*/

void printLinkList(adminInfo_t *head)   //打印链表结点信息
{
	adminInfo_t *toPrint = head->next;
	while(toPrint != NULL)
	{
		printf("彩票期号:====201600%d====\n",toPrint->issueNumAdmin);
		printf("彩票单价:%d元\n",toPrint->price);
		printf("彩票发行状态:%d\n",toPrint->stateOfLottery);
		toPrint = toPrint->next;
	}
}

void destroyList(adminInfo_t **head)   //销毁链表
{
	adminInfo_t *toFree = NULL ;
	adminInfo_t *tmp = NULL ;
	toFree = (*head)->next;
	while(toFree != NULL)
	{
		tmp = toFree->next ;
		free(toFree);
		toFree = tmp ;
	}
	free(*head);   //销毁头结点
	*head = NULL ; //将头结点指针置空，防止出现野指针
}

//根据彩票期号查询彩票信息，包括号码和开奖状态
adminInfo_t *searchInLinkList(adminInfo_t *head,int num)
{
	adminInfo_t *ret = NULL ;
	ret = head->next;
	
	while(ret != NULL)
	{
		if(num != ret->issueNumAdmin)
		{
			return ret;    //链表中遍历，当传入值num和issueNumAdmin(期号)一致时返回此结点
		}
		ret = ret->next;   
	}
	return NULL;
}





//=======用户菜单调用函数=====

void userLogin(rbTree_t* userTree,adminInfo_t *adminList,hashTable_t *betTable) 	//用户菜单主控函数中 userMenuCtl中  用户登录函数
{
//	char accountName[30]={0};
//	char pwd[20]={0};
	int wrongNum = 0;
	int x,y,inputAnswer = 0;
	int i = 0 ;
	char pd[128]={0};

	srand((int)time(NULL));
	x = rand()%60+1;
	y = rand()%40+1;

	while(1)
	{
		userInfo_t userAccount;
		memset(&userAccount,0,sizeof(userInfo_t));     //
		printf("请输入账户名:");
		fgets(userAccount.name,30,stdin);
		userAccount.name[strlen(userAccount.name)-1]=0;  //将输入字符串最后的回车清除

		printf("请输入密码:");
		for(i=0;;i++)
		{
			pd[i]=getch();
			if(pd[i]=='\n')
			{
				pd[i]='\0';
				break;
			}
			if(pd[i]==127)
			{
				printf("\b \b");
				i=i-2;
			}
			else
			{
				printf("*");
			}
			if(i<0)
			{
				pd[0]='\0';
			}
		}
		strcpy(userAccount.pwd,pd);
/*		fgets(userAccount.pwd,20,stdin);
		userAccount.pwd[strlen(userAccount.pwd)-1]=0;   */
		
		printf("\n");
		printf("请输入验证码的计算结果:");
		printf("%d+%d=?\n",x,y);
		scanf("%d",&inputAnswer);
		while(getchar()!='\n');
		if(inputAnswer != (x+y))
		{
			printf("您输入的验证码有误！\n");
			return;
		}

		if(strncmp(userAccount.name,"admin",20) == 0
				&& strncmp(userAccount.pwd,"admin",20) == 0 )
		{
			adminMenuCtl(adminList,betTable,userTree);
			return ;
		}else if(strncmp(userAccount.name,"notary",20) == 0
					&& strncmp(userAccount.pwd,"notary",20) == 0)
		{
			notaryMenuCtl(adminList);
			return ;
		}else if(searchNameInRBTree(userTree,userAccount.name) != NULL)
		{
			userMenuCtl(searchNameInRBTree(userTree,userAccount.name),betTable,adminList);  //查找输入的帐号
			return ;   
		}else
		{	
			wrongNum++;
			if(wrongNum == 3)
			{
				printf("您输入的密码错误已经3次，程序自动退出！\n");
				getchar();
				exit(1);
			}
			printf("密码错误，还剩余%d次机会，请重新输入！\n",3-wrongNum);
		}
	}
}


static void printUserInfo(userInfo_t *tmp)
{
		printf("您的账户名为: %s \n",tmp->name);
		printf("您的账户余额: %lf \n",tmp->balance);
		printf("您的绑定手机号为: %s \n",tmp->phoneNum);
}


void createNewUser(rbTree_t *userTree)  //创建新用户函数
{
	int i = 0 ;
	char pd[128] = {0} ;
	char verificationCode[20] = {0} ;
    userInfo_t user;
    memset(&user,0,sizeof(userInfo_t));
    printf("请输入姓名: ");
    fgets(user.name,20,stdin);
    user.name[strlen(user.name)-1]=0;

    printf("请输入密码: ");
    for(i=0;;i++)
    {   
       pd[i]=getch();
       if(pd[i]=='\n')
       {   
        pd[i]='\0';
           break;
       }       
       if(pd[i]==127)
       {   
           printf("\b \b");
           i=i-2;
       }       
       else
       {   
           printf("*");
       }       
       if(i<0)
       {   
           pd[0]='\0';
       }
    }
    strcpy(user.pwd,pd);
//  fgets(user.pwd,20,stdin);
//  user.pwd[strlen(user.pwd)-1]=0;
	printf("\n");
	printf("请输入您的手机号: ");
	fgets(user.phoneNum,20,stdin);
	user.phoneNum[strlen(user.phoneNum)-1]=0;
	printf("免费获取验证码! \n");   //身份验证
	printf("请输入您收到的验证码: ");   //身份验证
	fgets(verificationCode,20,stdin);
	verificationCode[strlen(verificationCode)-1] = 0;

	if(strcmp(verificationCode,"pass") != 0)
	{
		printf("您输入的验证码有误!\n");
		return;
	}

								//if中判断是否账户名有重复
    if(searchNameInRBTree(userTree,user.name) != NULL
								 || strcmp(user.name,"admin")==0
								 || strcmp(user.name,"notary")==0)
	{	
		printf("注册失败！该用户名已被注册！\n");
		return;
	}else
	{
		user.balance = 0;
		rbTreeInsertData(userTree,&user);    //将user的信息传入userTree
		printf("注册成功！\n");
		return;
	}
}

void addBalance(userInfo_t *tmp)              //充值函数
{
	double addMoney = 0 ;
	printf("请输入您需要充值的金额: \n");
	scanf("%lf",&addMoney);
	while(getchar()!='\n');
	tmp->balance = tmp->balance + addMoney ;
	printf("您充值的金额为: %lf\n",addMoney);
	printf("您帐户的余额为：%lf\n",tmp->balance);
}

void changePwd(userInfo_t *tmp)
{
	int i = 0 ;
	char oldPwd[30]={0};
	char newPwd1[30]={0};
	char newPwd2[30]={0};
	char pd[128]={0};
	printf("请输入您的旧密码:");
    for(i=0;;i++)
    {   
	    pd[i]=getch();
        if(pd[i]=='\n')
        {   
            pd[i]='\0';
            break;
        }       
        if(pd[i]==127)
        {   
            printf("\b \b");
            i=i-2;
        }       
        else
        {   
            printf("*");
        }       
        if(i<0)
        {   
            pd[0]='\0';
        }
    }
    strcpy(oldPwd,pd);
	printf("\n");
//	fgets(oldPwd,30,stdin);
//	oldPwd[strlen(oldPwd)-1]=0;
	while(1)
	{
		if(strcmp(tmp->pwd,oldPwd) == 0)
		{
			printf("请输入您的新密码:");
	        for(i=0;;i++)
       		{   
            	pd[i]=getch();
            	if(pd[i]=='\n')
            	{	   
            	    pd[i]='\0';
               		break;
            	}	       
            	if(pd[i]==127)
            	{   
            	    printf("\b \b");
            	    i=i-2;
            	}       
            	else
            	{   
               		printf("*");
            	}       
            	if(i<0)
            	{   
               		pd[0]='\0';
            	}
        	}
        	strcpy(newPwd1,pd);
			printf("\n");
//			fgets(newPwd1,30,stdin);
//			newPwd1[strlen(newPwd1)-1]=0;
			printf("请再次输入您的新密码:");
            for(i=0;;i++)
            {
                pd[i]=getch();
                if(pd[i]=='\n')
                {
                    pd[i]='\0';
                    break;
                }
                if(pd[i]==127)
                {
                    printf("\b \b");
                    i=i-2;
                }
                else
                {
                    printf("*");
                }
                if(i<0)
                {
                    pd[0]='\0';
                }
            }
            strcpy(newPwd2,pd);
			printf("\n");
//			fgets(newPwd2,30,stdin);
//			newPwd2[strlen(newPwd2)-1]=0;
			if(strcmp(newPwd1,newPwd2) == 0)
			{
				strcpy(tmp->pwd,newPwd1);
				printf("密码修改成功！\n");
				return;
			}else
			{
				printf("您输入的两次密码不相符！\n");
				break;
			}
		}else
		{
			printf("您输入的密码有误！\n");
			return;
		}
	}
}
/*
//摧毁哈西表
static void freeTable(hashTable_t *table)
{
	int key = 0 ;
	int p = 0 ;
	p = table->size ;
	hashNode_t *tmp;
	hashHead_t *heads = table->heads ;

	for(key=0;key<p;key++)
	{
		if(heads[key].nodeList != NULL)
		{	
			tmp = heads[key].nodeList;
			while(tmp != NULL)
			{
				freeNodeList(tmp);
			}
		}
	}
//	free(table);
}*/
