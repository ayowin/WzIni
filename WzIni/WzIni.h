
#ifndef _WZINI_H
#define _WZINI_H

#include <string>
#include <map>

/*
	作者：欧阳伟
	日期：2018-2-9
	用途：跨平台操作*.ini文件
*/

struct IniItem;
struct IniSection;

class WzIni
{
public:
	WzIni();
	~WzIni();

	//文件
	bool loadFile(const std::string &fileName); //加载
	bool save(); //保存
	bool saveAs(const std::string &fileName); //另存为
	
	//判断
	bool hasSection(const std::string &section); //判断section是否存在
	bool hasKey(const std::string &section, const std::string &key); //判断key是否存在

	//设置
	void setValue(const std::string &section, const std::string &key, const std::string &value); //设置value
	void deleteSection(const std::string &section); //删除section
	void deleteKey(const std::string &section, const std::string &key); //删除key

	//获取
	std::string getValueString(const std::string &section, const std::string &key); //获取string型value
	int getValueInt(const std::string &section, const std::string &key); //获取int型value
	double getValueDouble(const std::string &section, const std::string &key); //获取double型value

	void print(); //调试打印

private:	
	void release(); //释放内存
	bool parse(); //解析数据

private:
	std::string fileName;
	std::map<std::string, IniSection*> sections;
	typedef std::map<std::string, IniSection*>::iterator SectionsIterator;
};

#endif