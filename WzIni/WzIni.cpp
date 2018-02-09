
#include "WzIni.h"

#include <vector>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

const int BUF_SIZE = 2048;

struct IniItem
{
	IniItem(std::string key,std::string value)
	{
		this->key = key;
		this->value = value;
	}
	std::string key;
	std::string value;
};

struct IniSection
{
	std::string name; //�ε�����
	std::vector<IniItem> items; //�ε�����
};

WzIni::WzIni()
{

}

WzIni::~WzIni()
{
	release();
}

bool WzIni::loadFile(const std::string &fileName)
{
	release();
	this->fileName = fileName;
	if (!parse())return false;
	return true;
}

bool WzIni::save()
{
	if (saveAs(this->fileName))return true;
	else return false;
}

bool WzIni::saveAs(const std::string &fileName)
{
	std::string content = "";
	for (SectionsIterator i = sections.begin(); i != sections.end(); i++)
	{
		content = content + "[" + i->second->name + "]\n";
		for (std::vector<IniItem>::iterator j = i->second->items.begin();
			j != i->second->items.end();j++)
		{
			content = content + j->key + "=" + j->value + "\n";
		}
	}
	FILE* fp = fopen(fileName.c_str(), "w");
	if (fp == NULL)return false;
	fwrite(content.c_str(), 1, content.length(), fp);
	fclose(fp);
	return true;
}

bool WzIni::hasSection(const std::string &section)
{
	for (SectionsIterator i = sections.begin(); i != sections.end();i++)
	{
		if (i->second->name == section)return true;
	}
	return false;
}

bool WzIni::hasKey(const std::string &section, const std::string &key)
{
	if (hasSection(section))
	{
		for (std::vector<IniItem>::iterator i = sections[section]->items.begin();
			i != sections[section]->items.end();
			i++)
		{
			if (i->key == key)return true;
		}
	}
	return false;
}

void WzIni::setValue(const std::string &section, const std::string &key, const std::string &value)
{
	if (hasSection(section))
	{
		if (hasKey(section,key))
		{
			for (std::vector<IniItem>::iterator i = sections[section]->items.begin();
				i != sections[section]->items.end();
				i++)
			{
				if (i->key == key)i->value = value;
			}
		}
		else
		{
			sections[section]->items.push_back(IniItem(key, value));
		}
	}
	else
	{
		sections[section] = new IniSection;
		sections[section]->name = section;
		sections[section]->items.push_back(IniItem(key, value));
	}
}

void WzIni::deleteSection(const std::string &section)
{
	if (hasSection(section))
	{
		for (SectionsIterator i = sections.begin(); i != sections.end();i++)
		{
			if (i->first == section)
			{
				delete i->second;
			}
		}
		sections.erase(section);
	}
}

void WzIni::deleteKey(const std::string &section, const std::string &key)
{
	if (hasKey(section, key))
	{
		for (SectionsIterator i = sections.begin(); i != sections.end(); i++)
		{
			if (i->first == section)
			{
				for (std::vector<IniItem>::iterator j = sections[section]->items.begin();
					j != sections[section]->items.end();
					j++)
				{
					if (j->key == key)
					{
						sections[section]->items.erase(j);
						break;
					}
				}
				break;
			}
		}
	}
}

std::string WzIni::getValueString(const std::string &section, const std::string &key)
{
	if (hasKey(section,key))
	{
		for (std::vector<IniItem>::iterator i = sections[section]->items.begin();
			i != sections[section]->items.end();
			i++)
		{
			if (i->key == key)return i->value;
		}
	}	
	return NULL;
}

int WzIni::getValueInt(const std::string &section, const std::string &key)
{
	std::string value = getValueString(section, key);
	return atoi(value.c_str());
}

double WzIni::getValueDouble(const std::string &section, const std::string &key)
{
	std::string value = getValueString(section, key);
	return atof(value.c_str());
}

void WzIni::print()
{
	for (SectionsIterator i = sections.begin(); i != sections.end(); i++)
	{
		std::cout << "[" << i->second->name << "]" << std::endl;
		for (std::vector<IniItem>::iterator j = i->second->items.begin(); 
			j != i->second->items.end();
			j++)
		{
			std::cout << j->key << "=" << j->value << std::endl;
		}
	}
}

void WzIni::release()
{
	fileName = "";
	for (SectionsIterator i = sections.begin(); i != sections.end(); ++i)
	{
		delete i->second;
	}
	sections.clear(); //���map�е���������
}

bool WzIni::parse()
{
	FILE *fp = fopen(this->fileName.c_str(), "r");
	if (fp == NULL) return false;
	char buf[BUF_SIZE];
	memset(buf, 0, BUF_SIZE);

	//Ѱ������
	bool findSection = true;
	int begin = -1, end = -1;
	IniSection* iniSection = NULL;
	std::string sectionName = ""; //section�����洢IniSection����map�ɸ��ݴ�ֵ���IniSection*
	std::string key = ""; //key
	std::string value = ""; //value
	std::string comment = ""; //comment

	while (!feof(fp))
	{
		if (fgets(buf, BUF_SIZE, fp) != NULL)
		{
			//std::cout << buf;

			//������ǰ���Ƿ��ж���,����[section]
			for (int i = 0; i < strlen(buf); i++)
			{
				if (buf[i] == '[') begin = i;
				if (begin >= 0 && buf[i] == ']')end = i;
			}
			if (begin >= 0 && end >= 0)findSection = true;
			else findSection = false;
			begin = -1;
			end = -1;

			if (findSection) //��������
			{
				sectionName.clear(); //���ԭ��sectionName

				for (int i = 0; i < strlen(buf); i++)
				{
					if (buf[i] == '[') begin = i;
					if (begin >= 0 && buf[i] == ']')end = i;
				}
				if (begin >= 0 && end - begin >1) //[]��ֵ������
				{
					for (int i = begin + 1; i <= end - 1; i++)
					{
						sectionName.push_back(buf[i]);
					}
					//std::cout << sectionName << std::endl;
					iniSection = new IniSection;
					iniSection->name = sectionName;
					sections[sectionName] = iniSection;
					//std::cout << sections[sectionName]->name << std::endl;
					begin = -1;
					end = -1;
				}
			}
			else//����key��value��comment
			{	
				if (!sectionName.empty())
				{
					key.clear(); //���ԭkey
					value.clear(); //���ԭvalue

					//�Ƚ�����=���ţ������С�=�����ж�Ϊkey/value
					for (int i = 0; i < strlen(buf); i++)
					{
						if (buf[i] == '=')
						{
							begin = 0;
							end = i;
							break;
						}
					}
					//��ȡkey��value
					if (begin >= 0 && end >= 0) //��ȡkey
					{
						for (int i = begin; i < end; i++)
						{
							key.push_back(buf[i]);
						}
					}
					if (begin >= 0 && end >= 0 && end < strlen(buf) - 1) //��ȡvalue
					{
						begin = end + 1;
						end = strlen(buf);
						for (int i = begin; i < end; i++)
						{
							if (buf[i] == '\n')break; //���з�����ȡ
							value.push_back(buf[i]);
						}
					}
					if (key.compare("")) //���key��Ϊ��
					{
						sections[sectionName]->items.push_back(IniItem(key, value));
						//std::cout << key << std::endl;
						//std::cout << value << std::endl;
					}
					begin = -1;
					end = -1;	
				}
			}	
		}
	}

	fclose(fp);
	return false;
}


