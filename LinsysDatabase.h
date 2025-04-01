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
    string par; // ������������ ���������
    string col; // ������������ �������
    string var; // ����� ��� ������
    int sd; // ��� ���������: 0-�����, 1-�����
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

    // �������: ��������� ������ ����� � ���������
    char* genInt(int min, int max);
    int   genint(int min, int max);

    // �������: ������ � ������
    char* StrInt(char* s, int i);

    // �������: ��������� ����
    char* generateDate(char* buf, int i1, int i2);

    // �������: ��������� ����, ��������� �� days ����
    char*  addDaysToDate(char * date, int days);
    string addDaysToDate(string date, int days);

    // �������: ��������� INSERT'�
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // �������: ���������������� �������� ��
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

    // �������: ��������� ��� ������ � ��������� ����
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // LinsysDatabase_H_INCLUDED
