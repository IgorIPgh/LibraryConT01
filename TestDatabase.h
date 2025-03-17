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

    // отладка: генераци€ целого числа в диапазоне
    int genInt(int min, int max);

    // отладка: строка с числом
    char* StrInt(char* s, int i);
    //string StrInt(char* s, int i);
    //string StrInt(string s, int i);

    // отладка: √енераци€ даты
    char* generateDate();

    // отладка: формируем INSERT'ы
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // отладка: непосредственное создание Ѕƒ
    void makeNewDatabase(char* dbname);
    int  testPickBooks(string sql);

    // отладка: сохранить все данные в текстовый файл
    void saveSqlToFile(char* fname);
    void saveSqlBook  (char* fname);
    void saveSqlReader(char* fname);
    void saveSqlIssue (char* fname);
    void saveSqlUser  (char* fname);
    void saveSqlTicker(char* fname);

};


#endif // TESTDATABASE_H_INCLUDED
