#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <cstdlib.>
#include <fstream>
#include <typeinfo>


using namespace std;

typedef map<string, string> Header;
typedef map<string, void*> Row;


struct DBtable {
	string tableName;
	Header columnHeaders;
	vector <Row>data;
	Row& operator[](int ind)
	{
		return data[ind];
	}
};

typedef vector<DBtable> Tables;
void readTable(string tabName, DBtable&);
void readTables(string hdr_tabName, Tables& tables);
void printTable(DBtable&);
int GetType(string columnName);
void* GetValue(string value, string columnName);

int main()
{
	system(" chcp 1251 > nul");
	//массив из DBtable

	Tables tables;
	readTables("DBtables", tables);
	cout << tables.size();
	//for (int i = 0; i < tables.size()-1; i++) {
		//printTable(tables[i]);
	//}

    return 0;
}

void* GetValue(string value, string columnName) {
	switch (GetType(columnName)) {
		case 1: 
			return new (string)(value); 
			break;
		case 2: 
			return new (int)(atoi(value.c_str())); 
			break;
		default: 
			return NULL; 
			break;
	}
}

int GetType(string columnName) {
	if (columnName == "String") return 1;
	if (columnName == "Int") return 2;
}

void readTables(string hdr_tabName, Tables &tables) {
	ifstream in(hdr_tabName + ".txt");
	if (!in.is_open()) { cout << "Не удается открыть файл." << endl; return; }

	char line[200]; char* token, *nexttoken = line;

	vector<string> h;
	in.getline(line, 200);
	while (token = strtok_s(nexttoken, "|", &nexttoken)) 
		h.push_back(token);
	
	for (int i = 0; i < h.size()-1; i++) {
		DBtable temp;
		temp.tableName = h[i];
		readTable(temp.tableName, tables[i]);
		tables.push_back(temp);
	}
	
}
void readTable(string tabName, DBtable& table) {
	ifstream in(tabName +".txt");
	if (!in.is_open()) { cout << "Не удается открыть файл." << endl; /*return 1;*/ }

	table.tableName = tabName;
	char line[200]; char* token, *nexttoken=line;
	vector<string> h;

	in.getline(line, 200);
	while (token = strtok_s(nexttoken,"|",&nexttoken)) 
		h.push_back(token);
	
	for (unsigned int i = 0; i < h.size(); i+=2) 
		table.columnHeaders[h[i]] = h[i+1];

	int count = 0;
	
	while (in.getline(line, 200)) {
		nexttoken = line;
		int nmb = 0;
		Row temp;
		while (token = strtok_s(nexttoken, "|", &nexttoken)) {
			temp[h[nmb]] = GetValue(token, h[nmb + 1]);
			//cout << *(string*)(temp[h[nmb]]) << " ";
			nmb+=2;
		}
		table.data.push_back(temp);
		count++;
	}
}

void printTable(DBtable& table) {
	cout << endl << setw(20) << "TABLE NAME:" << table.tableName << endl;
	for (auto it = table.columnHeaders.begin(); it != table.columnHeaders.end(); ++it) 
		cout << setw(10) << it->first << " ";
	cout << endl;
	for (int i = 0; i < table.data.size(); i++) {
		for (auto it = table.columnHeaders.begin(); it != table.columnHeaders.end(); ++it) {
			switch (GetType(it->second)) {
			case 1:
				cout << setw(10) << *(string*)(table.data[i][it->first]) << " ";
				break;
			case 2:
				cout << setw(10) << *(int*)(table.data[i][it->first]) << " ";
				break;
			}
		}
		cout << endl;
	}

}