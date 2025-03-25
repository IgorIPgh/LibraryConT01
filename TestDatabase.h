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

    // отладка: генераци€ целого числа в диапазоне
    char* genInt(int min, int max);

    // отладка: строка с числом
    char* StrInt(char* s, int i);

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
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // TESTDATABASE_H_INCLUDED
