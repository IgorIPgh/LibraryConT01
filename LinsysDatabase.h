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
    string par; // наименование параметра
    string col; // наименование столбца
    string var; // текст для поиска
    int sd; // тип параметра: 0-текст, 1-цифра
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
  string author; // -- Автор
  string title; // -- Название
  string publisher;  // -- Издательство
  int year;  // -- Год издания
  int volume;  // -- Номер тома
  int year_p; // -- Год (периодика)
  int volume_p;// -- Номер (периодика)
  string genre; // -- жанр
  string udk; // -- УДК
  string bbk; // -- ББК
  string isbn; // -- ISBN
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(id,buf,10));
        str.append("------------------\r\n");
        str.append("Автор: "+author+"\r\n");
        str.append("Заглавие: "+title+"\r\n");
        str.append("Издательство: "+publisher+"\r\n");
        if(year!=0) {
        sprintf(buf, "Год:%4d\r\n", year);
        str.append(buf);
        sprintf(buf, "Том:%2d\r\n", volume);
        str.append(buf);
        }
        if(year_p!=0) {
        sprintf(buf, "Год:%4d\r\n", year_p);
        str.append(buf);
        sprintf(buf, "Номер:%2d\r\n", volume_p);
        str.append(buf);
        }
        str.append("Жанр: "+genre+"\r\n");
        str.append("УДК: "+udk+"\r\n");//"УДК: "+
        str.append("ББК: "+bbk+"\r\n");//"ББК: "+
        str.append("ISBN: "+isbn+"\r\n");//"ISBN: "+
      return str;
  }
};

struct tblReader {
  int read_id; // -- id
  int role_id; // -- категория
  int abon_id; // -- абонент
  string fname; // -- Имя
  string mname; // -- Отчество
  string lname;  // -- Фамилия
  string date_birth; // -- Дата рождения
  string address; // -- Адрес
  string phone; // -- Телефон
  string email; // -- Почта
  string srole, sticket;
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(read_id,buf,10));
        str.append("------------------\r\n");
        str.append("Имя: "+fname+"\r\n");
        str.append("Отчество: "+mname+"\r\n");
        str.append("Фамилия: "+lname+"\r\n");
        str.append("Дата рождения: "+date_birth+"\r\n");
        str.append("Адрес: " +address+"\r\n");
        str.append("Телефон: "+phone+"\r\n");
        str.append("Э.почта: "+email+"\r\n");
        str.append("Категория: "+srole+"        \r\n");
        str.append("Чит.билет: "+sticket+"\r\n");
        return str;
  }
};

struct tblAbonent {
  int abon_id; // -- id
  int read_id; // -- ссылка на читателя
  int role_id; // -- ссылка на тип читателя
  string login; // -- Логин
  string password; // -- Пароль
  string ticket;  // -- Чит. билет
  string date_in; // -- Дата регистрации
  string date_out; // -- Дата выбытия
  string sreader, srole;
  string toString()
  {
      string str; char buf[32];
        str.append("---");
        str.append(itoa(abon_id,buf,10));
        str.append("------------------\r\n");
        str.append("Абонент: "+sreader+"      \r\n");
        str.append("Категория: "+srole+"         \r\n");
        str.append("Логин : "+login+"            \r\n");
        str.append("Пароль: "+password+"         \r\n");
        str.append("Чит. билет: " +ticket+"      \r\n");
        str.append("Дата регистрации: "+date_in+"\r\n");
        str.append("Дата    закрытия: "+(date_out.size()==0?"          ":date_out)+"\r\n");
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

    // формируем INSERT'ы
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

    // отладка: непосредственное создание БД
    void makeTestDatabase (char* dbname);
    void createDatabaseSql(char* dbname);

    // отладка: вывод в файл содержимое queryResult
    int  SaveAndShowResult(string file = "");

    // отладка: сохранить все данные в текстовый файл
    void saveSqlBook   (char* fname, int num);
    void saveSqlIssue  (char* fname, int num);
    void saveSqlReader (char* fname, int num);
    void saveSqlAbonent(char* fname, int num);

};


#endif // LinsysDatabase_H_INCLUDED
