// ������ � ����� ������
//

#include "logger.h"
#include "TestDatabase.h"

#include "sqlite3pp.h"
#include "sqlite3ppext.h"


// =========================================================

//Logger logger("logfile.txt"); // Create logger instance

// ---------------------------------------------------------
// �������� ������
//
// ---------------------------------------------------------
// �������� ������� book
char *sqlCreateTableBook =
  "CREATE TABLE IF NOT EXISTS book("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "author TEXT NOT NULL, " // -- �����
  "title  TEXT NOT NULL, " // -- ��������
  "publisher TEXT NULL, " // -- ������������
  "year   INTEGER NULL, " // -- ��� �������
  "volume INTEGER NULL, " //-- ����� ����
  "year_p INTEGER NULL, " //-- ��� (���������)
  "volume_p INTEGER NULL, " // -- ����� (���������)
  "genre  TEXT NULL, " //-- ����
  "UDK    TEXT NULL, " // -- ���
  "BBK    TEXT NULL, " // -- ���
  "ISBN   TEXT NULL);";

// ---------------------------------------------------------
// �������� ������� reader
char *sqlCreateTableReader =
  "CREATE TABLE IF NOT EXISTS reader("
  "id INTEGER PRIMARY KEY AUTOINCREMENT,  "
  "user_id INTEGER NOT NULL, " // -- ��� ������������ + login + psw
  "ticket_id INTEGER NOT NULL, " // -- Library card - ������������ �����
  "fname TEXT NOT NULL, " // -- First name - ���
  "mname TEXT     NULL, " // -- Middle name - ��������
  "lname TEXT NOT NULL, " // -- Last name - �������
  "date_birth TEXT NULL, " // -- ���� ��������
  "passport   TEXT NULL, " // -- ����� ��������
  "address    TEXT NULL, " // -- �����
  "phone      TEXT NULL, " // -- �������
  "email      TEXT NULL);"; // -- �����

// ---------------------------------------------------------
// �������� ������� ������ = issue
char *sqlCreateTableIssue =
  "CREATE TABLE IF NOT EXISTS issue("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- ��� ��������
  "book_id   INTEGER NOT NULL, " // -- ��� ����������
  "date_order   TEXT NOT NULL, " // -- ���� ������
  "date_issue   TEXT     NULL, " // -- ���� ������
  "date_backw   TEXT     NULL, " // -- ���� ��������
  "returned  INTEGER     NULL);";// -- ����������

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
  "date_in    TEXT NOT NULL, " // -- ���� �����������
  "date_out   TEXT     NULL);";// -- ���� �������

// ---------------------------------------------------------
// �������� ������� ������ = role: ��������, ������������, �������, �������, ������
char *sqlCreateTableRole =
  "CREATE TABLE IF NOT EXISTS role("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "descr TEXT NOT NULL);"; // -- ������������

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
    sprintf(txt[3], "%s", i%2==0 ? genInt(2000, 2025) : "NULL");
    sprintf(txt[4], "%s", i%2==0 ? genInt(1, 5) : "NULL");
    sprintf(txt[5], "%s", i%2==1 ? genInt(2000, 2025) : "NULL");
    sprintf(txt[6], "%s", i%2==1 ? genInt(1, 9) : "NULL");
    sprintf(txt[7], "%s", genInt(1, 5)); // genre
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
    sprintf(txt[0], "%s", genInt(1,99));
    sprintf(txt[1], "%s", genInt(1,999));
    sprintf(txt[2], "'%s'", generateDate());
    sprintf(txt[3], "'%s'", generateDate());
    sprintf(txt[4], "'%s'", generateDate());
    sprintf(txt[5], "%s", genInt(0,1));

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
    sprintf(txt[0], "%s", genInt(1,99));
    sprintf(txt[1], "%s", genInt(1,5));
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
// �������� �� � ����:
//    ������������ � ���������� ������ ��������� �������
//
void TestDatabase::makeNewDatabase(char* dbname)
{
    Logger logger("logfile.txt"); // Create logger instance
    try
    {
        sqlite3pp::database dbase(dbname);
        {
            sqlite3pp::transaction xct(dbase);
            {
                dbase.execute("DROP TABLE IF EXISTS book;");
                dbase.execute(sqlCreateTableBook);
                for(int i=0; i<1000; i++){
                    dbase.execute(makeInsertBook(i));
                }
                logger.log(LOGlevel::INF, "������� book ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS reader;");
                dbase.execute(sqlCreateTableReader);
                for(int i=0; i<100; i++){
                    dbase.execute(makeInsertReader(i));
                }
                logger.log(LOGlevel::INF, "������� reader ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS issue;");
                dbase.execute(sqlCreateTableIssue);
                for(int i=0; i<200; i++){
                    dbase.execute(makeInsertIssue(i));
                }
                logger.log(LOGlevel::INF, "������� issue ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS user;");
                dbase.execute(sqlCreateTableUser);
                for(int i=0; i<100; i++){
                    dbase.execute(makeInsertUser(i));
                }
                logger.log(LOGlevel::INF, "������� user ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS ticket;");
                dbase.execute(sqlCreateTableTicket);
                for(int i=0; i<100; i++){
                    dbase.execute(makeInsertTicket(i));
                }
                logger.log(LOGlevel::INF, "������� ticket ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS role;");
                dbase.execute(sqlCreateTableRole);
                dbase.execute(sqlInsertDataRole);
                logger.log(LOGlevel::INF, "������� role ������� � ���������");

                dbase.execute("DROP TABLE IF EXISTS genre;");
                dbase.execute(sqlCreateTableGenre);
                logger.log(LOGlevel::INF, "������� genre �������, �� ���������");
                //for(int i=0; i<10; i++) {
                //    dbase.execute(makeInsertGenre(i));
                //}
            }
            xct.commit();
        }
    }
    catch (exception & ex)
    {
        logger.log(LOGlevel::ERR, ex.what());
    }
}

// ---------------------------------------------------------
// �������: ��������� ��� ������ � ��������� ����

void TestDatabase::saveSqlBook(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertBook(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlReader(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertReader(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlIssue(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertIssue(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlUser(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertUser(i));
    }
    fclose(fpDump);
}
void TestDatabase::saveSqlTicker(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertTicket(i));
    }
    fclose(fpDump);
}


// ---------------------------------------------------------

// �������: ��������� ������ ����� � ���������
char* TestDatabase::genInt(int min, int max) {
    static char text[10];
    sprintf(text, "%d", min + rand()%(max-min+1));
    return text;
}
// �������:
char* TestDatabase::StrInt(char* s, int i) {
    static char buf[32];
    sprintf(buf, "%s%03d", s, i);
    return buf;
}
// �������: ��������� ����
char* TestDatabase::generateDate(){
//    char yea[5], mon[3], day[3];
//    yea = genInt(2012, 2023);
//    mon = genInt(1, 12);
//    day = genInt(1, 31);
//    sprintf(buf, "%04d-%02d-%02d", yea, mon, day);
    sprintf(buf, "%s-%s-%s",
            genInt(2012, 2023),
            genInt(1, 12),
            genInt(1, 31));
    // time_t date = time(NULL);
    // struct tm *u = localtime(&date);
    // sprintf(buf, "%04d-%02d-%02d", u->tm_year+1900, u->tm_mon+1, u->tm_mday);
    return buf;
}

// ---------------------------------------------------------

int  TestDatabase::testPickBooks(string sql)  {

    int records=0;

    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                queryResult.clear();

                sqlite3pp::query query(dbase, sql.c_str());

                string id, author, title, publisher, year, volume, year_p, volume_p, genre, udk, bbk, isbn, count;

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string bookinfo;
                    (*i).getter() >> id >> author >> title >> publisher >> year >> volume
                                >> year_p >> volume_p >> genre >> udk >> bbk >> isbn >> count;
                    bookinfo.append("---"+id+"------------------"+"\r\n");
                    bookinfo.append("�����: "+author+"\r\n");
                    bookinfo.append("��������: "+title+"\r\n");
                    bookinfo.append("������������: "+publisher+"\r\n");
                    bookinfo.append("��� �������: "+year+"\r\n");
                    bookinfo.append("����� ����: "+volume+"\r\n");
                    bookinfo.append("��� (���������): "+year_p+"\r\n");
                    bookinfo.append("����� (���������): "+volume_p+"\r\n");
                    bookinfo.append("����: "+genre+"\r\n");
                    bookinfo.append("���: "+udk+"\r\n");
                    bookinfo.append("���: "+bbk+"\r\n");
                    bookinfo.append("ISBN: "+isbn+"\r\n");
                    //bookinfo.append("�����������: "+count+"\r\n");
                    queryResult.push_back(bookinfo);
                    records++;
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger("logfile.txt"); // Create logger instance
        logger.log(LOGlevel::ERR, ex.what());
    }

    ////////////////////////////////////////////////////////
    // ������ ��� �������:
  	// ���� �� �� ����� ������� ���� ���� ��� ������ ������ � ����
    //  ofstream outf(ofname);
    //	if (!outf) {
    //		cerr << "���... �� ���� ������� ���� " << ofname << " ��� ������" << endl;
    //		return -1;
    //	}
    //
    //for (auto n : queryResult) {
    //    outf << n << endl;
    //}
    //ShellExecute(NULL, "open", "db_out.txt", NULL,NULL,1);
    ////////////////////////////////////////////////////////

    return records;
}
