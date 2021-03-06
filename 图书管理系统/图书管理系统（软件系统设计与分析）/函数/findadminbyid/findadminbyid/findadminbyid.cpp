// findadminbyid.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include<list>
#include<iostream>
#include <fstream>
using namespace std;
typedef struct {
	int id;
	char name[40];//一个unicode字符编码是两个字节，因此书名最长可以有20个字符
	int author[10];//存储一本书的author的id号，用一个整型数组表示
				   //不够10个的后面补0 在按作者号查询作者信息时，遍历这个数组
				   //遇到有元素为0 就停止，然后去查询作者信息即可



	char publisher[40];//出版社名称
	int numberInLib;//馆藏数量
	int amount;//该类图书总量
	int type[10];//用来表述哪一类图书，该整型数组存储该图书所属类别的id组成的数组
				 //同样，不够10个类别的在后面补0 查询方法同作者id

	int exist;//该属性是为了做假删除用的
	char ISBN[14];//ISBN编号，不包括分隔符，多出来的一位是存储字符结束符的
}Book;
typedef struct {
	int id;
	char password[22];//密码要求：5-10位！！！！！！！
	int exist;
}Admin;
typedef struct {
	int id;
	char name[40];//一个字符是两个字节，所以这个数组最多可以存储20个字符的作者姓名
}Author;
list<Admin> getAdminById(int id) //根据id号查找管理员
{
	list<Admin> adminList;//建立list表
	fstream fs;
	fs.open("admin.bat", ios::out|ios::in | ios::binary|ios::ate);//打开文件，其中in代表输入，out代表输出，ate表示让文件指针指向最后
	if (!fs)//如果文件不存在，输出error
	{
		cout << "error" << endl;
	}
	else
	{
		long pos = sizeof(Admin)*(id - 1);//通过id号计算指针偏移量
		if (pos<0 || pos>fs.tellg())  cout << "error" << endl;//如果偏移量小于零或者大于文件长度，输出error,返回空list
		else 
		{
			fs.seekg(pos, ios::beg);//指针跳转到文件头
			char * buffer = new char[sizeof(Admin)];//定义adamin长度的buffer变量
			fs.read(buffer, sizeof(Admin));//读文件
			Admin adminById = *((Admin*)buffer);//将buffer强制转换为admin类型，并赋值给adminById
			if (adminById.exist == 1)//如果未被删除，将记录添加到List末尾
			{
				adminList.push_back(adminById);
			}
		}
	}
	fs.close();
	return adminList;
}
list<Author> getAuthorById(int id) //根据id号查找作者
{
	list<Author> authorList;//建立list表
	fstream fs;
	fs.open("author.bat", ios::out | ios::in | ios::binary | ios::ate);//打开文件，其中in代表输入，out代表输出，ate表示让文件指针指向最后
	if (!fs)//如果文件不存在，输出error
	{
		cout << "error" << endl;
	}
	else
	{
		long pos = sizeof(Author)*(id - 1);//通过id号计算指针偏移量
		if (pos<0 || pos>fs.tellg())  cout << "error" << endl;//如果偏移量小于零或者大于文件长度，输出error,返回空list
		else
		{
			fs.seekg(pos, ios::beg);//指针跳转到文件头
			char * buffer = new char[sizeof(Author)];//定义author长度的buffer变量
			fs.read(buffer, sizeof(Author));//读文件
			Author authorById = *((Author*)buffer);//将buffer强制转换为author类型，并赋值给authorbyid
			authorList.push_back(authorById);//将记录添加到List末尾
		}
	}
	fs.close();
	return authorList;
}
list<Author> getAuthorByName(string name) //根据姓名模糊查询作者
{
	list<Author> authorList;//建立list表
	fstream fs;
	Author a;
	string s="";//定义字符串用于存放姓名
	fs.open("author.bat",  ios::binary | ios::in);//以读的方式打开文件
	if (!fs)//如果文件不存在，输出error
	{
		cout << "error" << endl;
	}
	else
	{
		fs.seekg(0, ios::end);
		long len = fs.tellg();//获取文件长度
		fs.seekg(0, ios::beg);//指针指向文件头
		char*buffer = new char[sizeof(Author)];//定义author长度的buffer变量
		while (fs.tellg < len)//当文件没结束时，遍历文件
		{
			fs.read(buffer, sizeof(Author));//读文件
			a = *((Author*)buffer);//强制转换
			s = a.name;
			if (s.find(name) != string::npos)//进行模糊查询
			{
				authorList.push_back(a);
			}
		}
	}
	fs.close();
	return authorList;
}
list<Book> getBookByISBN(string ISBN)//根据ISBN查找书
{
	list<Book> bookList;//建立list表
	fstream fs;
	Book b;
	string s = "";//定义字符串用于存放ISBN
	fs.open("book.bat",  ios::binary | ios::in);//以读的方式打开文件
	if (!fs)//如果文件不存在，输出error
	{
		cout << "error" << endl;
	}
	else
	{
		fs.seekg(0, ios::end);
		long len = fs.tellg();//获取文件长度
		fs.seekg(0, ios::beg);//指针指向文件头
		char*buffer = new char[sizeof(Book)];//定义book长度的buffer变量
		while (fs.tellg < len)//当文件没结束时，遍历文件
		{
			fs.read(buffer, sizeof(Book));//读文件
			b = *((Book*)buffer);//强制转换
			if (b.exist == 0)continue;//如果书被假删除了，跳过这条记录，读取下一条记录
			s = b.ISBN;
			if (s==ISBN)//进行查询
			{
				bookList.push_back(b);
			}
		}
	}
	fs.close();
	return bookList;
}


int main()
{
    return 0;
}

