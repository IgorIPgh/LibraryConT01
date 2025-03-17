#include <fstream>
#include <windows.h>

#include "TestDatabase.h"

#include "sqlite3pp.h"
#include "sqlite3ppext.h"


// =========================================================

// ---------------------------------------------------------
// �������� ������
//
//char *sqlDropAllTables =
//  "DROP TABLE IF EXISTS book;"
//  "DROP TABLE IF EXISTS reader;"
//  "DROP TABLE IF EXISTS issue;"
//  "DROP TABLE IF EXISTS user;"
//  "DROP TABLE IF EXISTS ticket;"
//  "DROP TABLE IF EXISTS role;"
//  "DROP TABLE IF EXISTS genre;";

// ---------------------------------------------------------
// �������� ������� book
char *sqlCreateTableBook =
  "CREATE TABLE IF NOT EXISTS book("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "author TEXT NOT NULL, " // -- �����
  "title TEXT NOT NULL, " // -- ��������
  "publisher TEXT NULL, " // -- ������������
  "year INTEGER NULL, " // -- ��� �������
  "volume  INTEGER NULL, " //-- ����� ����
  "year_p INTEGER NULL, " //-- ��� (���������)
  "volume_p INTEGER NULL, " // -- ����� (���������)
  "genre TEXT NULL, " //-- ����
  "UDK TEXT NULL, " // -- ���
  "BBK TEXT NULL, " // -- ���
  "ISBN TEXT NULL);";

// ---------------------------------------------------------
// �������� ������� reader
char *sqlCreateTableReader =
  "CREATE TABLE IF NOT EXISTS reader("
  "id INTEGER PRIMARY KEY AUTOINCREMENT,  "
  "user_id INTEGER NOT NULL, " // -- ��� ������������ + login + psw
  "ticket_id INTEGER NOT NULL, " // -- Library card - ������������ �����
  "fname TEXT NOT NULL, " // -- First name - ���
  "mname TEXT NOT NULL, " // -- Middle name - ��������
  "lname TEXT NOT NULL, " // -- Last name - �������
  "date_birth TEXT NULL, " // -- ���� ��������
  "passport TEXT NULL, " // -- ����� ��������
  "address TEXT NULL, " // -- �����
  "phone TEXT NULL, " // -- �������
  "email TEXT NULL);"; // -- �����

// ---------------------------------------------------------
// �������� ������� ������ = issue
char *sqlCreateTableIssue =
  "CREATE TABLE IF NOT EXISTS issue("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- ��� ��������
  "book_id   INTEGER NOT NULL, " // -- ��� ����������
  "date_order   TEXT NOT NULL, " // -- ���� ������
  "date_issue   TEXT NOT NULL, " // -- ���� ������
  "date_backw   TEXT NOT NULL, " // -- ���� ��������
  "returned  INTEGER NOT NULL);";// -- ����������

// ---------------------------------------------------------
// �������� ������� �������������� = user
char *sqlCreateTableUser =
  "CREATE TABLE IF NOT EXISTS user("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- ��� ��������
  "role_id   INTEGER NOT NULL, " // -- ��� ������������
  "login     TEXT    NOT NULL, " // -- login - �����
  "password  TEXT    NOT NULL);";// -- ������

// ---------------------------------------------------------
// �������� ������� ������������� ����� = The reader's ticket
char *sqlCreateTableTicket =
  "CREATE TABLE IF NOT EXISTS ticket("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "number  INTEGER NOT NULL, " // -- ����� ������
  "date_in  TEXT NOT NULL, "   // -- ���� �����������
  "date_out TEXT NOT NULL);";  // -- ���� �������

// ---------------------------------------------------------
// �������� ������� ������ = role: ��������, ������������, �������, �������, ������
char *sqlCreateTableRole =
  "CREATE TABLE IF NOT EXISTS role("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "[descr] TEXT NOT NULL);"; // -- ������������

// ---------------------------------------------------------
// �������� ������� ����� = genre
char *sqlCreateTableGenre =
  "CREATE TABLE IF NOT EXISTS genre("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "descr TEXT NOT NULL);"; // -- ������������

// ---------------------------------------------------------
// ������� � �������
char *sqlInsertDataBook =
  "INSERT INTO book (author, title, publisher, year, volume, year_p, volume_p, genre, UDK, BBK, ISBN) "
  "VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s);";

char *sqlInsertDataReader =
  "INSERT INTO reader (user_id, ticket_id, fname, mname, lname, date_birth, passport, address, phone, email) "
  "VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s);";

char *sqlInsertDataIssue =
  "INSERT INTO issue (reader_id, book_id, date_order, date_issue, date_backw, returned) "
  "VALUES (%s, %s, %s, %s, %s, %s);";

char *sqlInsertDataUser =
  "INSERT INTO user (reader_id, role_id, login, password) "
  "VALUES (%s, %s, %s, %s);";

char *sqlInsertDataTicket =
  "INSERT INTO ticket (number, date_in, date_out) "
  "VALUES (%s, %s, %s);";

char *sqlInsertDataRole =
  "INSERT INTO role (descr) VALUES"
  "('��������'),"
  "('������������'),"
  "('�������'),"
  "('�������'),"
  "('������');";

char *sqlInsertDataGenre =
  "INSERT INTO genre (descr) "
  "VALUES (%s);";


// ---------------------------------------------------------
char *sqlDeleteById = // table, id
	"DELETE FROM %s WHERE id=%d;";

char *sqlUpdateRow = // table, col-list, where-list
	"UPDATE %s SET %s WHERE %s;";

char *sqlSelectAll = // table
	"SELECT * FROM %s;";


// ---------------------------------------------------------

// �������: ��������� INSERT ��� book reader issue user ticket

//"INSERT INTO book (author, title, publisher, year, volume, year_p, volume_p, genre, UDK, BBK, ISBN) "
//"VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)";
char* TestDatabase::makeInsertBook(int i) {
    static char buf[512];
    char txt[11][32];
    sprintf(txt[0], "'%s%03d'", "�����",i);
    sprintf(txt[1], "'%s%03d'", "���������",i);
    sprintf(txt[2], "'%s%03d'", "���-��",i);
    sprintf(txt[3], "%d", genInt(2000, 2025));
    sprintf(txt[4], "%d", genInt(1, 5));
    sprintf(txt[5], "%d", genInt(2000, 2025));
    sprintf(txt[6], "%d", genInt(1, 9));
    sprintf(txt[7], "%d", genInt(1, 2));
    sprintf(txt[8], "'%s%03d'", "���",i);
    sprintf(txt[9], "'%s%03d'", "���",i);
    sprintf(txt[10],"'%s%03d'", "ISBN",i);

    sprintf(buf, sqlInsertDataBook,
        txt[0], // 1 author
        txt[1], // 2 title
        txt[2], // 3 publisher
        txt[3], // 4 year
        txt[4], // 5 volume
        txt[5], // 6 year_p
        txt[6], // 7 volume_p
        txt[7], // 8 genre_id
        txt[8], // 9 UDK
        txt[9], // 0 BBK
        txt[10] // 1 ISBN
    );
    return buf;
}

//"INSERT INTO reader (user_id, ticket_id, fname, mname, lname, date_birth, passport, address, phone, email) "
//"VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s);";
char* TestDatabase::makeInsertReader(int i) {
    static char buf[512];
    char txt[10][32];
    //int id = genInt(1, 100);
    sprintf(txt[0], "%d", i+1);
    sprintf(txt[1], "%d", i+1);
    sprintf(txt[2], "'%s%03d'", "���",i);
    sprintf(txt[3], "'%s%03d'", "��������",i);
    sprintf(txt[4], "'%s%03d'", "�������",i);
    sprintf(txt[5], "'%s'", generateDate());
    sprintf(txt[6], "'%s%03d'", "�������",i);
    sprintf(txt[7], "'%s%03d'", "���",i);
    sprintf(txt[8], "'%s%03d'", "���",i);
    sprintf(txt[9], "'%s%03d'", "���",i);

    sprintf(buf, sqlInsertDataReader,
        txt[0], // 1 user_id
        txt[1], // 2 ticket_id
        txt[2], // 3 fname
        txt[3], // 4 mname
        txt[4], // 5 lname
        txt[5], // 6 date_birth
        txt[6], // 7 passport
        txt[7], // 8 address
        txt[8], // 9 phone
        txt[9]  // 0 email
    );
    return buf;
}

//  "INSERT INTO issue (reader_id, book_id, date_order, date_issue, date_backw, returned) "
//  "VALUES (%d, %d, %s, %s, %s, %d);";
char* TestDatabase::makeInsertIssue(int i)
{
    static char buf[512];
    char txt[6][32];
    sprintf(txt[0], "%3d", genInt(1,99));
    sprintf(txt[1], "%3d", genInt(1,999));
    sprintf(txt[2], "'%s'", generateDate());
    sprintf(txt[3], "'%s'", generateDate());
    sprintf(txt[4], "'%s'", generateDate());
    sprintf(txt[5], "%d", genInt(0,1));

    sprintf(buf, sqlInsertDataIssue,
        txt[0], // 1 reader_id
        txt[1], // 2 book_id
        txt[2], // 3 date_order
        txt[3], // 4 date_issue
        txt[4], // 5 date_backw
        txt[5]  // 6 returned
    );
    return buf;

}

//  "INSERT INTO user (reader_id, role_id, login, password) "
//  "VALUES (%d, %d, %s, %s);";
char* TestDatabase::makeInsertUser(int i)
{
    static char buf[512];
    char txt[4][32];
    sprintf(txt[0], "%2d", genInt(1,99));
    sprintf(txt[1], "%2d", genInt(1,5));
    sprintf(txt[2], "'%s%03d'", "�����",i);
    sprintf(txt[3], "'%s%03d'", "������",i);

    sprintf(buf, sqlInsertDataUser,
        txt[0], // 1 reader_id
        txt[1], // 2 role_id
        txt[2], // 3 login
        txt[3]  // 4 password
    );
    return buf;
}

//  "INSERT INTO ticket (number, date_in, date_out) "
//  "VALUES (%s, %s, %s);";
char* TestDatabase::makeInsertTicket(int i)
{
    static char buf[512];
    char txt[3][32];
    sprintf(txt[0], "'%s%03d'", "�����",i);
    sprintf(txt[1], "'%s'", generateDate());
    sprintf(txt[2], "'%s'", generateDate());

    sprintf(buf, sqlInsertDataTicket,
        txt[0], // 1 number
        txt[1], // 3 date_in
        txt[2]  // 4 date_out
    );
    return buf;
}

// ---------------------------------------------------------

void TestDatabase::makeNewDatabase(char* dbname){

    sqlite3pp::database db(dbname);

    db.execute("DROP TABLE IF EXISTS book;");
    db.execute(sqlCreateTableBook);
    for(int i=0; i<1000; i++) {
        db.execute(makeInsertBook(i));
    }
    db.execute("DROP TABLE IF EXISTS reader;");
    db.execute(sqlCreateTableReader);
    for(int i=0; i<100; i++) {
        db.execute(makeInsertReader(i));
    }
    db.execute("DROP TABLE IF EXISTS issue;");
    db.execute(sqlCreateTableIssue);
    for(int i=0; i<200; i++) {
        db.execute(makeInsertIssue(i));
    }
    db.execute("DROP TABLE IF EXISTS user;");
    db.execute(sqlCreateTableUser);
    for(int i=0; i<100; i++) {
        db.execute(makeInsertUser(i));
    }
    db.execute("DROP TABLE IF EXISTS ticket;");
    db.execute(sqlCreateTableTicket);
    for(int i=0; i<100; i++) {
        db.execute(makeInsertTicket(i));
    }
    db.execute("DROP TABLE IF EXISTS role;");
    db.execute(sqlCreateTableRole);
    db.execute(sqlInsertDataRole);

    db.execute("DROP TABLE IF EXISTS genre;");
    db.execute(sqlCreateTableGenre);
//    for(int i=0; i<100; i++) {
//        db.execute(makeInsertGenre(i));
//    }

}


// �������: ��������� ��� ������ � ��������� ����
void TestDatabase::saveSqlToFile(char* fname){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
//            putsxy(1,25,"He ������� ������� ���� ��� �������� ������");
        return;
    }
//        putsxy(1,25,"�������� ������");
    for(int i=0; i<1000; i++)
    {
//        fprintf(fpDump,"%s\n", makeInsertBook(i));
    }
//        putsxy(1,26,"�������� ���������. ������� ����� �������");
//        _getch();
    fclose(fpDump);
}

void TestDatabase::saveSqlBook(char* fname){
    FILE *fpDump;

    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<1000; i++)
    {
        fprintf(fpDump,"%s\n", makeInsertBook(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlReader(char* fname){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<100; i++)
    {
        fprintf(fpDump,"%s\n", makeInsertReader(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlIssue(char* fname){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<50; i++)
    {
        fprintf(fpDump,"%s\n", makeInsertIssue(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlUser(char* fname){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<50; i++)
    {
        fprintf(fpDump,"%s\n", makeInsertUser(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlTicker(char* fname){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<50; i++)
    {
        fprintf(fpDump,"%s\n", makeInsertTicket(i));
    }
    fclose(fpDump);
}


// ---------------------------------------------------------

// �������: ��������� ������ ����� � ���������
int TestDatabase::genInt(int min, int max) {
    return min + rand()%(max-min+1);
}
// �������:
char* TestDatabase::StrInt(char* s, int i) {
    static char buf[32];
    sprintf(buf, "%s%03d", s, i);
    return buf;
}
// �������: ��������� ����
char* TestDatabase::generateDate(){
    int yea, mon, day;
    yea = genInt(2012, 2023);
    mon = genInt(1, 12);
    day = genInt(1, 31);
    sprintf(buf, "%04d-%02d-%02d", yea, mon, day);
    // time_t date = time(NULL);
    // struct tm *u = localtime(&date);
    // sprintf(buf, "%04d-%02d-%02d", u->tm_year+1900, u->tm_mon+1, u->tm_mday);
    return buf;
}

// ---------------------------------------------------------

int  TestDatabase::testPickBooks(string sql)  {
    //string sql = madeBookWhere();
    //putsxy( 8,26, (char*)sql.c_str());

    sqlite3pp::database db("LInSys.db");

    ofstream outf("db_out.txt");
	// ���� �� �� ����� ������� ���� ���� ��� ������ ������ � ����
	if (!outf)
	{
		// �� ������� ��������� �� ������ � ��������� exit()
		cerr << "Uh oh, SomeText.txt could not be opened for writing!" << endl;
		exit(1);
	}

    sqlite3pp::query query(db, sql.c_str());

    string id, author, title, publisher, year, volume, year_p, volume_p, udk, bbk, isbn, count;
    for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
      (*i).getter() >> id >> title >> author >> publisher >> year >> year_p >> volume >> volume_p >> udk >> bbk >> isbn >> count;

      outf << "---" << id << "------------------\nauthor: " << author << "\ntitle: " << title << "\npublisher: " << publisher <<
      "\nyear:" << year << "\nvolume: " << volume << "\nyear_p: " << year_p << "\nvolume_p: " << volume_p << "\nudk: " << udk <<
      "\nbbk: " << bbk << "\nisbn: " << isbn << "\ncount: " << count << endl;

      cout << "---" << id << "------------------\nauthor: " << (author) << "\ntitle: " << (title) << "\npublisher: " << (publisher) <<
      "\nyear:" << year << "\nvolume: " << volume << "\nyear_p: " << year_p << "\nvolume_p: " << volume_p << "\nudk: " << (udk) <<
      "\nbbk: " << (bbk) << "\nisbn: " << (isbn) << "\ncount: " << count << endl;

//      cout << "---" << id << "------------------\nauthor: " << narrow(author) << "\ntitle: " << narrow(title) << "\npublisher: " << narrow(publisher) <<
//      "\nyear:" << year << "\nvolume: " << volume << "\nyear_p: " << year_p << "\nvolume_p: " << volume_p << "\nudk: " << narrow(udk) <<
//      "\nbbk: " << narrow(bbk) << "\nisbn: " << narrow(isbn) << "\ncount: " << count << endl;
    }

    ShellExecute(NULL, "open", "db_out.txt", NULL,NULL,1);

    return 1;
}
