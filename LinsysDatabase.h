#ifndef LinsysDatabase_H_INCLUDED
#define LinsysDatabase_H_INCLUDED

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;

// ---------------------------------------------------------

#define LINSYS_BOOKS   11
#define LINSYS_READERS 10
#define LINSYS_ISSUES  6
#define LINSYS_TICKETS 2
#define LINSYS_USERS   4
#define LINSYS_ROLES   2
#define LINSYS_BOOKINLIBR 4
#define LINSYS_REPORT  5

typedef struct EntityItem {
    string par; // наименование параметра
    string col; // наименование столбца
    string var; // текст для поиска
    int sd; // тип параметра: 0-текст, 1-цифра
} entity;

extern entity biArr[];
extern entity riArr[];
extern entity iiArr[];
extern entity tiArr[];
extern entity uiArr[];
extern entity miArr[];
extern entity ilArr[];
extern entity irArr[];

// ---------------------------------------------------------
class LinsysDatabase
{
private:
    char buf[256];
    string dbname;
    string ofname;
    string logfile;
    vector<string> queryResult;

public:
    LinsysDatabase(){
        srand(time(NULL));
        dbname = "LInSys.db";
        ofname = "db_out.txt";
        logfile= "logfile.txt";
    };
    vector<string>& getQueryResult() { return queryResult; }

    // отладка: генерация целого числа в диапазоне
    char* genInt(int min, int max);
    int   genint(int min, int max);

    // отладка: строка с числом
    char* StrInt(char* s, int i);

    // отладка: Генерация даты
    char* generateDate(char* buf, int i1, int i2);

    // отладка: получение даты, смещённой на days дней
    char*  addDaysToDate(char * date, int days);
    string addDaysToDate(string date, int days);

    // отладка: формируем INSERT'ы
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // отладка: непосредственное создание БД
    void makeNewDatabase  (char* dbname);
    void createDatabaseSql(char* dbname);
    int  SearchBook  (string sql);
    int  SearchReader(string sql);
    int  ListOfReadersDebts      (string par);
    int  ListOfReadersRegistered (string par);
    int  ListOfBooksIssued       (string par);
    int  ListOfBooksAvailabled   (string par);
    int  CalcIssueStatistics     (string par);

    int  SaveAndShowResult(string file = "");

    // отладка: сохранить все данные в текстовый файл
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // LinsysDatabase_H_INCLUDED
