/***************************
*
*     注册登录模板
*     register.cpp
*     作者：小叶Little_Ye
*     环境：Dev-C++ 
*     时间：2017-7-31
*     最近修订：2017-8-11
*
***************************/

// 引用头文件 
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <math.h>
#include <fstream>
#include <conio.h>
#include <windows.h>           // 使用Sleep函数，首字母必须大写

// 全局变量维护及声明 
using namespace std;

struct user                    // 自定义用户资料变量 
{
	string name;           // 用户名 
	string psw;            // 密码 
	int type;              // 用户类型：0普通用户，1管理员用户 
};

const int MAXLIST = 1001;      // 最大内存数据库容量 
const int MAXLEN = 31;         // 最大用户名长度 

user list[MAXLIST];            // 内存资料库
int id;                        // 搜索到的账号的ID（临时存储变量） 
int tot;                       // 内存资料库的资料总数
int now_id;                    // 当前用户编号 
string now_name;               // 当前用户名

// 函数声明
void wel_no_sign();            // 未登录时的欢迎界面 
void wel_sign(int l);          // 登录后的欢迎界面
void load_list();              // 读取用户列表
void save_list();              // 保存用户列表
int  search(string a);         // 搜索指定用户名的用户ID 
void help();                   // 显示帮助文件
bool psw_accepted(string a);   // 检测注册密码是否符合要求 
void load_root_user();         // 加载ROOT管理员用户 
void get_list();               // 显示用户列表 
string get_user_type(int l);   // 将用户类型数字转为对应描述字串 

// 主函数
int main()
{
	bool sign = false;
	int lev;
	string cmd, name, psw, cmd_t;
	load_root_user();
	while (cmd != "exit")
	{
		system("cls");
		if (sign) wel_sign(now_name.size());
		else wel_no_sign();
		cin>>cmd;
		
		if (cmd == "help")     // 显示帮助文件 
		{
			help();
			system("pause");
		}
		
		if (cmd == "signout")  // 登出操作
		{
			if (!sign)
			{
				printf("当前未登录，无法登出！\n");
				Sleep(2000);
			}
			else
			{
				printf("登出成功！\n");
			    Sleep(2000);
			    sign = false;
			}
		}
		
		if (cmd == "signin")   // 登录操作
		{
			do
			{
				printf("请输入账号：");
				cin>>name;
				if (name != "exit")
				 if (search(name) == -1) printf("账号不存在！\n");
				 else
				 {
					id = search(name);
					break;	
				 }
			}
			while (name != "exit");
			if (name != "exit")
			 do
			 {
				printf("请输入密码：");
				cin>>psw;
				if (psw != "exit")
				 if (list[id].psw != psw) printf("密码错误！\n");
				 else
				 {
					printf("密码正确，登入成功！\n");
					Sleep(2000);
					sign = true;
					now_name = name;
					now_id = id;	
				 }
			 }
			 while (psw != "exit" && !sign);
		}
		
		if (cmd == "register") // 注册操作
		{
			do
			{
				printf("请输入注册号：");
			    cin>>name;
			    if (name != "exit")
			     if (search(name) != -1) printf("该账号已被注册！\n");
			     else
			     {
			    	printf("账号符合要求！\n");
			    	break;	
			     }
			}
			while (name != "exit");
			if (name != "exit")
			{
				printf("请输入注册密码：");
				cin>>psw;
				if (psw != "exit")
				 if (psw_accepted(psw))
				 {
					printf("密码符合要求，注册成功！\n");
					Sleep(2000);
					tot++;
					list[tot].type = 0;
					list[tot].name = name;
					list[tot].psw = psw;
				 }
				 else
				 {
					printf("密码不符合要求！密码需大于等于8位

且包含字母。\n");
					Sleep(2000);
				 }
			}
		} 
		
		if (cmd == "list")     // 显示列表操作
		{
			if (list[now_id].type >= 1 && sign)
			{
				get_list();
				system("pause");
			}
			else
			{
				printf("当前用户没有使用此指令的权限，指令无法执行

。\n");
				Sleep(2000);
			}
		} 
		
		if (cmd == "edit")     // 编辑用户权限操作 
		{
			if (list[now_id].type >= 2 && sign)
			 while (cmd_t != "exit")
			 {
				system("cls");
				wel_sign(now_name.size());
				printf("edit\n"
				       "正在加载用户列表......\n");
				get_list();
				cin>>cmd_t;
				if (cmd_t == "UID")
				{
					cin>>id;
					if (id > tot && id < 0)
					{
						printf("指定用户不存在！\n");
						Sleep(2000);
					}
					else if (id == 0)
					{
						printf("无法对指定用户进行操作！

\n");
						Sleep(2000);
					}
					 else
					 {
						cin>>lev;
						list[id].type = lev;
						printf("操作成功！\n");
						Sleep(2000); 
					 }
				}
				else if (cmd_t == "NAME")
				{
					cin>>name;
					id = search(name);
					if (name=="ROOT_USER") id = 0;
					if (id > tot && id < 0)
					{
						printf("指定用户不存在！\n");
						Sleep(2000);
					}
					else if (id == 0)
					{
						printf("无法对指定用户进行操作！

\n");
						Sleep(2000);
					}
					 else
					 {
						cin>>lev;
						list[id].type = lev;
						printf("操作成功！\n");
						Sleep(2000); 
					 }
				}
			 }
			else
			{
				printf("当前用户没有使用此指令的权限，指令无法执行

。\n");
				Sleep(2000);
			}
		}
		
//		if (cmd == "load")     // 读取用户列表操作
//		{
//			load_list();
//		} 
		
		if (cmd == "exit")     // 退出操作
		{
		    printf("系统退出成功！\n");
		    save_list();
		}
	}
	
	system("pause");
	return 0;
}

// 函数定义
void wel_no_sign()             // 未登录时的欢迎界面 
{
	cout<<"===================="<<'\n'
	    <<"     资料库系统     "<<'\n'
	    <<"    当前用户：无    "<<'\n'
		<<"===================="<<'\n';
	return; 
}

void wel_sign(int l)           // 登录后的欢迎界面 
{
	cout<<"====================";
	for (int i = 1;i <= 2 * l;i++) cout<<'=';
	cout<<'\n';
	for (int i = 1;i <= l;i++) cout<<' ';
	cout<<"     资料库系统"<<'\n';
	for (int i = 1;i <= round(l * 1.0 / 2 + 5);i++) cout<<' ';
	cout<<"当前用户："<<now_name<<"    "<<'\n';
	cout<<"====================";
	for (int i = 1;i <= 2 * l;i++) cout<<'=';
	cout<<'\n';
	return; 
}

void load_list()               // 读取用户列表 
{
	string name, psw;
	int type;
	tot = 0;
	ifstream fin("user_list.txt");
	while (name == "-1" && psw == "-1" && type == -1)
	{
		fin>>name>>psw>>type;
		tot++;
		list[tot].name = name;
		list[tot].psw = psw;
		list[tot].type = type;
	}
	return;
} 

void save_list()               // 保存用户列表 
{
	ofstream fout("user_list.txt");
	for (int i = 1;i <= tot;i++)
	 fout<<list[i].name<<' '<<list[i].psw<<' '<<list[i].type<<'\n';
	fout<<"-1 -1 -1"<<'\n';
	return;
}

int search(string a)           // 搜索指定用户名的用户ID 
{
	for (int i = 0;i <= tot;i++)
	 if (a == list[i].name) return i;
	return -1;
}

void help()                    // 显示帮助文件 
{
	printf("help       显示帮助文件\n"
	       "register   执行注册操作\n"
		   "signin     执行登录操作\n"
		   "signout    执行登出操作\n"
		   "exit       退出程序或在注册和登陆操作中退回到主菜单\n"
		   "list       显示用户列表（仅限管理员）\n"
		   "edit       编辑用户权限（仅限管理员）\n"
		   "load       读取用户列表\n");
	return;
}

bool psw_accepted(string a)    // 检测注册密码是否符合要求 
{
	bool b = false;            // 检测字符串是否携带字母 
	
	for (int i = 1;i <= a.size();i++)
	 if (a[i] >= 'a' && a[i] <= 'z' || a[i] >= 'A' && a[i] <= 'Z')
	 {
		b = true;
		break;
	 }
	if (a.size() < 8 || (!b)) return false;
	else return true;
}

void load_root_user()          // 加载ROOT管理员用户 
{
	tot = 0;
	list[0].type = 2;
	list[0].name = "ROOT_USER";
	list[0].psw = "ROOT_USER_BY_LY";
	return;
}

void get_list()                // 显示用户列表
{
	printf("==========================================\n"
	       "用户编号   用户名   密码   用户类型       \n"
		   "==========================================\n");
	for (int i = 0;i <= tot;i++)
	 if (list[now_id].type <= 1 && list[i].type >= 2 || i == 0 && now_id != 0) 

cout<<i<<"   "<<list[i].name<<"   "<<"******"<<"   "<<get_user_type(list[i].type)

<<'\n';
	 else cout<<i<<"   "<<list[i].name<<"   "<<list[i].psw<<"   

"<<get_user_type(list[i].type)<<'\n';
	printf("==========================================\n");
	return;
}

string get_user_type(int l)    // 将用户类型数字转为对应描述字串 
{
	if (l == 0) return "Normal";
	else if (l == 1) return "Admin";
	 else if (l == 2) return "Super Admin";
}
