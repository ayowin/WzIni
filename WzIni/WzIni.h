
#ifndef _WZINI_H
#define _WZINI_H

#include <string>
#include <map>

/*
	���ߣ�ŷ��ΰ
	���ڣ�2018-2-9
	��;����ƽ̨����*.ini�ļ�
*/

struct IniItem;
struct IniSection;

class WzIni
{
public:
	WzIni();
	~WzIni();

	//�ļ�
	bool loadFile(const std::string &fileName); //����
	bool save(); //����
	bool saveAs(const std::string &fileName); //���Ϊ
	
	//�ж�
	bool hasSection(const std::string &section); //�ж�section�Ƿ����
	bool hasKey(const std::string &section, const std::string &key); //�ж�key�Ƿ����

	//����
	void setValue(const std::string &section, const std::string &key, const std::string &value); //����value
	void deleteSection(const std::string &section); //ɾ��section
	void deleteKey(const std::string &section, const std::string &key); //ɾ��key

	//��ȡ
	std::string getValueString(const std::string &section, const std::string &key); //��ȡstring��value
	int getValueInt(const std::string &section, const std::string &key); //��ȡint��value
	double getValueDouble(const std::string &section, const std::string &key); //��ȡdouble��value

	void print(); //���Դ�ӡ

private:	
	void release(); //�ͷ��ڴ�
	bool parse(); //��������

private:
	std::string fileName;
	std::map<std::string, IniSection*> sections;
	typedef std::map<std::string, IniSection*>::iterator SectionsIterator;
};

#endif