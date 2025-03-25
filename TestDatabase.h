#ifndef TESTDATABASE_H_INCLUDED
#define TESTDATABASE_H_INCLUDED

#include <time.h>

#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// ---------------------------------------------------------
class TestDatabase
{
private:
    char buf[256];
    string dbname;
    string ofname;
    vector<string> queryResult;
    //sqlite3pp::database &dbase;

public:

    TestDatabase(){
        srand(time(NULL));
        dbname = "LInSys.db";
        ofname = "db_out.txt";
    }
    vector<string>& getQueryResult() { return queryResult; }

    // �������: ��������� ������ ����� � ���������
    char* genInt(int min, int max);

    // �������: ������ � ������
    char* StrInt(char* s, int i);

    // �������: ��������� ����
    char* generateDate();

    // �������: ��������� INSERT'�
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // �������: ���������������� �������� ��
    void makeNewDatabase(char* dbname);
    int  testPickBooks(string sql);

    // �������: ��������� ��� ������ � ��������� ����
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // TESTDATABASE_H_INCLUDED
