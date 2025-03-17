#ifndef TESTDATABASE_H_INCLUDED
#define TESTDATABASE_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include <string>
#include <sstream>
#include <iostream>

using namespace std;

// ---------------------------------------------------------
class TestDatabase
{
private:
    char buf[256];

public:

    TestDatabase(){
        srand(time(NULL));
    }

    // �������: ��������� ������ ����� � ���������
    int genInt(int min, int max);

    // �������: ������ � ������
    char* StrInt(char* s, int i);
    //string StrInt(char* s, int i);
    //string StrInt(string s, int i);

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
    void saveSqlToFile(char* fname);
    void saveSqlBook  (char* fname);
    void saveSqlReader(char* fname);
    void saveSqlIssue (char* fname);
    void saveSqlUser  (char* fname);
    void saveSqlTicker(char* fname);

};


#endif // TESTDATABASE_H_INCLUDED
