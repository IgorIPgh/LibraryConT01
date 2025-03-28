#ifndef TESTDATABASE_H_INCLUDED
#define TESTDATABASE_H_INCLUDED

#include <time.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;


// ---------------------------------------------------------


// ---------------------------------------------------------
class TestDatabase
{
private:
    char buf[256];
    string dbname;
    string ofname;
    vector<string> queryResult;

public:

    TestDatabase(){
        srand(time(NULL));
        dbname = "LInSys.db";
        ofname = "db_out.txt";
    }
    vector<string>& getQueryResult() { return queryResult; }

    // �������: ��������� ������ ����� � ���������
    char* genInt(int min, int max);
    int   genint(int min, int max);

    // �������: ������ � ������
    char* StrInt(char* s, int i);

    // �������: ��������� ����
    char* generateDate(char* buf, int i1, int i2);

    // �������: ��������� ����, ��������� �� days ����
    char* addDaysToDate(char* date, int days);

    // �������: ��������� INSERT'�
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // �������: ���������������� �������� ��
    void makeNewDatabase(char* dbname);
    void createDatabaseSql(char* dbname);
    int  testPickBooks(string sql);


    // �������: ��������� ��� ������ � ��������� ����
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // TESTDATABASE_H_INCLUDED
