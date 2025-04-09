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
#define LINSYS_READERS  7
#define LINSYS_ISSUES   6
#define LINSYS_ABONENTS 5
#define LINSYS_ROLES    2
#define LINSYS_GENRES   2
#define LINSYS_BOOKINLIBR 4
#define LINSYS_REPORT   5

typedef struct EntityItem {
    string par; // ������������ ���������
    string col; // ������������ �������
    string var; // ����� ��� ������
    int sd; // ��� ���������: 0-�����, 1-�����
} entity;

extern entity biArr[]; // book
extern entity riArr[]; // reader
extern entity iiArr[]; // issue
extern entity aiArr[]; // abonent
extern entity miArr[]; // role
extern entity ilArr[]; // BookInLibr
extern entity irArr[]; // report

struct tblBook {
  int id; // -- id
  string author; // -- �����
  string title; // -- ��������
  string publisher;  // -- ������������
  int year;  // -- ��� �������
  int volume;  // -- ����� ����
  int year_p; // -- ��� (���������)
  int volume_p;// -- ����� (���������)
  string genre; // -- ����
  string udk; // -- ���
  string bbk; // -- ���
  string isbn; // -- ISBN
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(id,buf,10));
        str.append("------------------\r\n");
        str.append("�����: "+author+"\r\n");
        str.append("��������: "+title+"\r\n");
        str.append("������������: "+publisher+"\r\n");
        if(year!=0) {
        sprintf(buf, "���:%4d\r\n", year);
        str.append(buf);
        sprintf(buf, "���:%2d\r\n", volume);
        str.append(buf);
        }
        if(year_p!=0) {
        sprintf(buf, "���:%4d\r\n", year_p);
        str.append(buf);
        sprintf(buf, "�����:%2d\r\n", volume_p);
        str.append(buf);
        }
        str.append("����: "+genre+"\r\n");
        str.append("���: "+udk+"\r\n");//"���: "+
        str.append("���: "+bbk+"\r\n");//"���: "+
        str.append("ISBN: "+isbn+"\r\n");//"ISBN: "+
      return str;
  }
};

struct tblReader {
  int read_id; // -- id
  int role_id; // -- ���������
  int abon_id; // -- �������
  string fname; // -- ���
  string mname; // -- ��������
  string lname;  // -- �������
  string date_birth; // -- ���� ��������
  string address; // -- �����
  string phone; // -- �������
  string email; // -- �����
  string srole, sticket;
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(read_id,buf,10));
        str.append("------------------\r\n");
        str.append("���: "+fname+"\r\n");
        str.append("��������: "+mname+"\r\n");
        str.append("�������: "+lname+"\r\n");
        str.append("���� ��������: "+date_birth+"\r\n");
        str.append("�����: " +address+"\r\n");
        str.append("�������: "+phone+"\r\n");
        str.append("�.�����: "+email+"\r\n");
        str.append("���������: "+srole+"        \r\n");
        str.append("���.�����: "+sticket+"\r\n");
        return str;
  }
};

struct tblAbonent {
  int abon_id; // -- id
  int read_id; // -- ������ �� ��������
  int role_id; // -- ������ �� ��� ��������
  string login; // -- �����
  string password; // -- ������
  string ticket;  // -- ���. �����
  string date_in; // -- ���� �����������
  string date_out; // -- ���� �������
  string sreader, srole;
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(abon_id,buf,10));
        str.append("------------------\r\n");
        str.append("�������: "+sreader+"      \r\n");
        str.append("���������: "+srole+"         \r\n");
        str.append("����� : "+login+"            \r\n");
        str.append("������: "+password+"         \r\n");
        str.append("���. �����: " +ticket+"      \r\n");
        str.append("���� �����������: "+date_in+"\r\n");
        str.append("����    ��������: "+(date_out.size()==0?"          ":date_out)+"\r\n");
        return str;
  }
};

// ---------------------------------------------------------
class LinsysDatabase
{
private:
    char buf[256];
    string dbname;
    string ofname;
    string logfile;
    vector<string> queryResult;
//    vector<tblBook> vectorIssuedBook;

public:
    LinsysDatabase(){
        srand(time(NULL));
        dbname = "LInSys.db";
        ofname = "db_out.txt";
        logfile= "logfile.txt";
    };
    vector<string>& getQueryResult() { return queryResult; }

    // ��������� INSERT'�
    char *makeInsertBook   (int i);
    char *makeInsertIssue  (int i);
    char *makeInsertReader (int i);
    char *makeInsertAbonent(int i);

    string  makeBookQuery(int sortby);
    string  makeReaderQuery();
    string  makeAbonentQuery();

    int RequestBook   (string sql, vector<tblBook>    & vec);
    int RequestReader (string sql, vector<tblReader>  & vec);
    int RequestAbonent(string sql, vector<tblAbonent> & vec);

    int  SearchBook  (string sql);

    int  ListOfReadersDebts      (string par);
    int  ListOfReadersRegistered (string par);
    int  ListOfBooksIssued       (string par);
    int  ListOfBooksAvailabled   (string par);
    int  CalcIssueStatistics     (string par);

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

    // �������: ���������������� �������� ��
    void makeTestDatabase (char* dbname);
    void createDatabaseSql(char* dbname);

    // �������: ����� � ���� ���������� queryResult
    int  SaveAndShowResult(string file = "");

    // �������: ��������� ��� ������ � ��������� ����
    void saveSqlBook   (char* fname, int num);
    void saveSqlIssue  (char* fname, int num);
    void saveSqlReader (char* fname, int num);
    void saveSqlAbonent(char* fname, int num);

};


#endif // LinsysDatabase_H_INCLUDED
