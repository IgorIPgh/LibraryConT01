// ������ � ����� ������
//

#include "logger.h"
#include "LinsysDatabase.h"

#include "sqlite3pp.h"
#include "sqlite3ppext.h"


// =========================================================


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
  "date_backw   TEXT     NULL);";// -- ���� ��������

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
  "number   TEXT NOT NULL, " // -- ����� ������
  "date_in  TEXT NOT NULL, " // -- ���� �����������
  "date_out TEXT     NULL);";// -- ���� �������

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
  "INSERT INTO issue (reader_id, book_id, date_order, date_issue, date_backw) "
  "VALUES (%s, %s, %s, %s, %s);";

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
char* LinsysDatabase::makeInsertBook(int i) {
    static char buf[512];
    char txt[11][32];
    sprintf(txt[0], "'%s%03d'", "�����",i);
    sprintf(txt[1], "'%s%03d'", "��������",i);
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
char* LinsysDatabase::makeInsertReader(int i) {
    static char buf[512];
    char txt[10][32], ddt[14];
    //int id = genInt(1, 100);
    sprintf(txt[0], "%d", i+1);
    sprintf(txt[1], "%d", i+1);
    sprintf(txt[2], "'%s%03d'", "���",i);
    sprintf(txt[3], "'%s%03d'", "��������",i);
    sprintf(txt[4], "'%s%03d'", "�������",i);
    sprintf(txt[5], "'%s'", generateDate(ddt,2020,2025));
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

//  "INSERT INTO issue (reader_id, book_id, date_order, date_issue, date_backw) "
//  "VALUES (%s, %s, %s, %s, %s);";
char* LinsysDatabase::makeInsertIssue(int i)
{
    static char buf[512] = {0};
    char txt[5][32], ddt[14];
    int val;

    val = genint(1,99);
    sprintf(txt[0], "%d", val);
    val = genint(1,999);
    sprintf(txt[1], "%d", val);
    sprintf(txt[2], "'%s'", generateDate(ddt,2020,2025)); // �����

    if (i%3 == 0) // �� ������ ������ ������
    {   // ���� �� ������, �� � �� ����������
        strcpy(txt[3], "''");
        strcpy(txt[4], "''");
    }
    else // ����� �����
    {
        // ������: ����� 0-4 ���� �� ������ / ���� � ddt
        val = genint(0,4);
        sprintf(txt[3], "'%s'", addDaysToDate(ddt, val));
        // ���������� ������ ����� �����
        // �������: ����� 1-14 ���� �� ������
        val = genint(1,11);
        const char* sday = (i%5==0 ? addDaysToDate(ddt, val) : "");
        sprintf(txt[4], "'%s'", sday );
    }
    sprintf(buf, sqlInsertDataIssue,
        txt[0], // 1 reader_id
        txt[1], // 2 book_id
        txt[2], // 3 date_order
        txt[3], // 4 date_issue
        txt[4]  // 5 date_backw
    );
    return buf;
}

//  "INSERT INTO ticket (number, date_in, date_out) "
//  "VALUES (%s, %s, %s);";
char* LinsysDatabase::makeInsertTicket(int i)
{
    static char buf[512];
    char txt[3][32], ddt[14];
    sprintf(txt[0], "'%s%03d'", "�����", i);
    sprintf(txt[1], "'%s'", generateDate(ddt,2020,2025));

    // ������ 25-� ��������� �� ����������
    int val = genint(100,1000);
    const char* sday = (i%25==0 ? addDaysToDate(ddt, val) : "");
    sprintf(txt[2], "'%s'", sday );

    sprintf(buf, sqlInsertDataTicket,
        txt[0], // 1 number
        txt[1], // 3 date_in
        txt[2]  // 4 date_out
    );
    return buf;
}

//  "INSERT INTO user (reader_id, role_id, login, password) "
//  "VALUES (%d, %d, %s, %s);";
char* LinsysDatabase::makeInsertUser(int i)
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

// ---------------------------------------------------------
// �������� �� � ����:
//    ������������ � ���������� ������ ��������� �������
//
void LinsysDatabase::makeNewDatabase(char* dbname)
{
    Logger logger(logfile);
    try
    {
        sqlite3pp::database dbase(dbname);
        {
            sqlite3pp::transaction xct(dbase);
            {
//                dbase.execute("DROP TABLE IF EXISTS book;");
//                dbase.execute(sqlCreateTableBook);
//                for(int i=0; i<1000; i++){
//                    dbase.execute(makeInsertBook(i));
//                }
//                logger.log(LOGlevel::INF, "������� book ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS reader;");
//                dbase.execute(sqlCreateTableReader);
//                for(int i=0; i<200; i++){
//                    dbase.execute(makeInsertReader(i));
//                }
//                logger.log(LOGlevel::INF, "������� reader ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS issue;");
//                dbase.execute(sqlCreateTableIssue);
//                for(int i=0; i<400; i++){
//                    dbase.execute(makeInsertIssue(i));//makeInsertIssue(i);
//                }
//                logger.log(LOGlevel::INF, "������� issue ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS user;");
//                dbase.execute(sqlCreateTableUser);
//                for(int i=0; i<200; i++){
//                    dbase.execute(makeInsertUser(i));
//                }
//                logger.log(LOGlevel::INF, "������� user ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS ticket;");
//                dbase.execute(sqlCreateTableTicket);
//                for(int i=0; i<200; i++){
//                    dbase.execute(makeInsertTicket(i));
//                }
//                logger.log(LOGlevel::INF, "������� ticket ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS role;");
//                dbase.execute(sqlCreateTableRole);
//                dbase.execute(sqlInsertDataRole);
//                logger.log(LOGlevel::INF, "������� role ������� � ���������");
//
//                dbase.execute("DROP TABLE IF EXISTS genre;");
//                dbase.execute(sqlCreateTableGenre);
//                logger.log(LOGlevel::INF, "������� genre �������, �� ���������");
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


void LinsysDatabase::createDatabaseSql(char* fname)
{
    Logger logger(logfile);
    ofstream out(fname, ios::out);

    if (!out.is_open()) {
        logger.log(LOGlevel::INF, "������ �������� �����");
        return;
    }

    out << "DROP TABLE book;" << endl;
    out << "DROP TABLE reader;" << endl;
    out << "DROP TABLE issue;" << endl;
    out << "DROP TABLE user;" << endl;
    out << "DROP TABLE ticket;" << endl;
    out << "DROP TABLE role;" << endl;
    out << "DROP TABLE genre;" << endl;
    logger.log(LOGlevel::INF, "������������ ������� ��� �������� ������");

    out << sqlCreateTableBook << endl;
    out << sqlCreateTableReader << endl;
    out << sqlCreateTableIssue << endl;
    out << sqlCreateTableUser << endl;
    out << sqlCreateTableTicket << endl;
    out << sqlCreateTableRole << endl;
    out << sqlCreateTableGenre << endl;
    logger.log(LOGlevel::INF, "������������ ������� ��� �������� ������");

    for(int i=0; i<1000; i++)
        out << makeInsertBook(i) << endl;
    for(int i=0; i<200; i++)
        out << makeInsertReader(i) << endl;
    for(int i=0; i<400; i++)
        out << makeInsertIssue(i) << endl;
    for(int i=0; i<200; i++)
        out << makeInsertUser(i) << endl;
    for(int i=0; i<200; i++)
        out << makeInsertTicket(i) << endl;
    out << sqlInsertDataRole << endl;
    logger.log(LOGlevel::INF, "������������ ������ ��� ��������");

    out.close();
}



// ---------------------------------------------------------
// �������: ��������� ��� ������ � ��������� ����

void LinsysDatabase::saveSqlBook(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertBook(i));
    }
    fclose(fpDump);
}
void LinsysDatabase::saveSqlReader(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertReader(i));
    }
    fclose(fpDump);
}
void LinsysDatabase::saveSqlIssue(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertIssue(i));
    }
    fclose(fpDump);
}
void LinsysDatabase::saveSqlUser(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertUser(i));
    }
    fclose(fpDump);
}
void LinsysDatabase::saveSqlTicker(char* fname, int num){
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
int LinsysDatabase::genint(int min, int max) {
    return min + rand()%(max-min+1);
}

char* LinsysDatabase::genInt(int min, int max) {
    static char text[10];
    sprintf(text, "%d", min + rand()%(max-min+1));
    return text;
}

// �������: ��������� ���� � ���������
char* LinsysDatabase::generateDate(char* buf, int i1, int i2){
    sprintf(buf, "%4d-%02d-%02d",
        genint(i1, i2), genint(1, 12), genint(1, 31));
    return buf;
}

// �������: ��������� ����, ��������� �� days ����
char* LinsysDatabase::addDaysToDate(char* date, int days)
{
   	int y, m, d;
   	sscanf(date, "%d-%d-%d", &y, &m, &d);

    struct tm u = {0}, *w;
	u.tm_year = y - 1900;
	u.tm_mon  = m - 1;
	u.tm_mday = d + days;

	time_t next = mktime(&u);
	w = localtime(&next);
	strftime(date, 12, "%Y-%m-%d", w);

	return date;
}
string LinsysDatabase::addDaysToDate(string date, int days)
{
   	int y, m, d; char ddd[12];
   	sscanf(date.c_str(), "%d-%d-%d", &y, &m, &d);

    struct tm u = {0}, *w;
	u.tm_year = y - 1900;
	u.tm_mon  = m - 1;
	u.tm_mday = d + days;

	time_t next = mktime(&u);
	w = localtime(&next);
	strftime(ddd, 12, "%Y-%m-%d", w);

	return string(ddd);
}

//////////////////////////////////////////////////////////////////////
// + SearchBook
int  LinsysDatabase::SearchBook(string sql)
{
    int records=0;

    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                queryResult.clear();

                sqlite3pp::query query(dbase, sql.c_str());

//                string id, author, title, publisher, year, volume, year_p, volume_p, genre, udk, bbk, isbn;
                string col[LINSYS_BOOKS+1];

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string bookinfo;
//                    (*i).getter() >> id >> author >> title >> publisher >> year >> volume
//                                >> year_p >> volume_p >> genre >> udk >> bbk >> isbn;// >> count;
//                    bookinfo.append("---"+id+"------------------"+"\r\n");
//                    bookinfo.append("�����: "+author+"\r\n");
//                    bookinfo.append("��������: "+title+"\r\n");
//                    bookinfo.append("������������: "+publisher+"\r\n");
//                    bookinfo.append("��� �������: "+year+"\r\n");
//                    bookinfo.append("����� ����: "+volume+"\r\n");
//                    bookinfo.append("��� (���������): "+year_p+"\r\n");
//                    bookinfo.append("����� (���������): "+volume_p+"\r\n");
//                    bookinfo.append("����: "+genre+"\r\n");
//                    bookinfo.append("���: "+udk+"\r\n");
//                    bookinfo.append("���: "+bbk+"\r\n");
//                    bookinfo.append("ISBN: "+isbn+"\r\n");

                    (*i).getter() >> col[0] >> col[1] >> col[2] >> col[3] >> col[ 4] >> col[ 5]
                                  >> col[6] >> col[7] >> col[8] >> col[9] >> col[10] >> col[11];
                    bookinfo.append("---"+col[0]+"------------------\r\n");
                    for (int k=1; k<=LINSYS_BOOKS; k++)
                        bookinfo.append(biArr[k].par + ": " + col[k] + "\r\n");

                    queryResult.push_back(bookinfo);
                    records++;
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    // ������ ��� �������:
    SaveAndShowResult("SearchBook.out");

    return records;
}

//////////////////////////////////////////////////////////////////////
// - SearchReader
int  LinsysDatabase::SearchReader(string sql)
{
    int records=0;

    return records;
}

// ---------------------------------------------------------
//
// == ������ ===
//
// ����� �� ���������:
//     + ������ ������ ��������;
//     + ������ ���������, ������������������ � ���������� �������.
// ����� �� ������ ����:
//     + ������ �������� ����;
//     - ������ ��������� ����;
// ����������:
//     - ������� �-�� ���� � ����� ������ � ������� ��������;
//     + ������ ����, �������� �� ���������� ������� (�� ����������).
//
// (�����)
// (�����) (�����)
// (�����)
//
// ---------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// + ������ ������ ��������;
// list of reader's debts
int  LinsysDatabase::ListOfReadersDebts(string par)
{
    char * fs1 = "AND reader.id=%d";
    char * fs2 = "AND reader.id BETWEEN %d AND %d";
    char sql[256], ddd[36];

    int v1, v2;
    int rc = sscanf(par.c_str(), "%d%*c%d", &v1, &v2);
    if (rc==1)
        sprintf(ddd, fs1, v1);
    else if (rc==2)
        sprintf(ddd, fs2, v1,v2);
    else ddd[0] = 0;

    char tsql[] =
    "SELECT issue.date_issue, reader.id, reader.lname, reader.fname, book.author, book.title "
    "FROM issue "
    "INNER JOIN reader ON reader.id = issue.reader_id %s "
    "INNER JOIN book ON book.id = issue.book_id AND book.year!='' "
    "WHERE issue.date_backw='' AND issue.date_issue!='' "
    "ORDER BY reader.id;";

    sprintf(sql, tsql, ddd);

    int days=15, records=0;
    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                queryResult.clear();

                sqlite3pp::query query(dbase, sql);

                string date_issue, id, lname, fname, author, title, reader;

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string record;
                    (*i).getter() >> date_issue >> id >> lname >> fname >> author >> title;
                    reader = lname + " " + fname;
                    record.append("---"+id+"---(�����)--------\r\n");
                    record.append("���� ������: "+date_issue+"\r\n");
                    record.append("�������: "+reader+"\r\n");
                    record.append("�����(�): "+author+"\r\n");
                    record.append("��������: "+title+"\r\n");
                    record.append("�.�. ����������: "+addDaysToDate(date_issue, days)+"\r\n");
                    queryResult.push_back(record);
                    records++;
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    // ������ ��� �������:
    SaveAndShowResult("ListOfReadersDebts.out");
    return records;
}

//////////////////////////////////////////////////////////////////////
// + ������ ���������, ������������������ � ���������� �������
// list of readers registered during the time period
int LinsysDatabase::ListOfReadersRegistered(string par)//string date1, string date2
{
    char * fs1 = "WHERE ticket.date_in = '%d-%02d-%02d'";
    char * fs2 = "WHERE ticket.date_in BETWEEN '%d-%02d-%02d' AND '%d-%02d-%02d'";
    char sql[256], ddd[128];

    int y1,y2, m1,m2, d1,d2;
    int rc = sscanf(par.c_str(), "%d%*c%d%*c%d %d%*c%d%*c%d", &y1,&m1,&d1, &y2,&m2,&d2);
    if (rc==3)
        sprintf(ddd, fs1, y1,m1,d1);
    else if (rc==6)
        sprintf(ddd, fs2, y1,m1,d1, y2,m2,d2);
    else ddd[0] = 0;

    int records=0;
    char tsql[] =
        "SELECT reader.id, reader.lname, reader.fname, ticket.number, ticket.date_in "
        "FROM reader "
        "INNER JOIN ticket ON reader.ticket_id = ticket.id "
        "%s "
        "ORDER BY ticket.date_in;";

    sprintf(sql, tsql, ddd);

    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                queryResult.clear();

                sqlite3pp::query query(dbase, sql);

                string id, lname, fname, number, date_in, reader;

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string record;
                    (*i).getter() >> id >> lname >> fname >> number >> date_in;
                    reader = lname + " " + fname;
                    record.append("---"+id+"---(�����)--------\r\n");
                    record.append("�������: "+reader+"\r\n");
                    record.append("������������ �����: "+number+"\r\n");
                    record.append("���� ������: "+date_in+"\r\n");
                    queryResult.push_back(record);
                    records++;
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    // ������ ��� �������:
    SaveAndShowResult("ListOfReadersRegistered.out");
    return records;
}


//////////////////////////////////////////////////////////////////////
// + ������ �������� ����;  list of books issued;
// + ������ ����, �������� �� ���������� ������� (�� ����������).
int LinsysDatabase::ListOfBooksIssued(string par)//string date1, string date2date1, string date2)
{
    char * fs1 = "WHERE issue.date_issue = '%d-%02d-%02d'";
    char * fs2 = "WHERE issue.date_issue BETWEEN '%d-%02d-%02d' AND '%d-%02d-%02d'";
    char sql[256], ddd[128];

    int y1,y2, m1,m2, d1,d2;
    int rc = sscanf(par.c_str(), "%d%*c%d%*c%d %d%*c%d%*c%d", &y1,&m1,&d1, &y2,&m2,&d2);
    if (rc==3)
        sprintf(ddd, fs1, y1,m1,d1);
    else if (rc==6)
        sprintf(ddd, fs2, y1,m1,d1, y2,m2,d2);
    else ddd[0] = 0;

    int records=0;
    char tsql[] =
        "SELECT book.id, book.author, book.title, book.publisher, book.genre, issue.date_issue "
        "FROM issue "
        "INNER JOIN book ON issue.book_id = book.id AND book.year!='' "
        "%s "
        "AND issue.date_backw='' AND issue.date_issue!='' "
        "ORDER BY book.author;";

    sprintf(sql, tsql, ddd);

    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                queryResult.clear();

                sqlite3pp::query query(dbase, sql);

                string id, author, title, publisher, genre, date_issue;

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string record;
                    (*i).getter() >> id >> author >> title >> publisher >> genre >> date_issue;
                    record.append("---"+id+"---(�����)--------\r\n");
                    record.append("�����: "+author+"\r\n");
                    record.append("��������: "+title+"\r\n");
                    record.append("������������: "+publisher+"\r\n");
                    record.append("����: "+genre+"\r\n");
                    record.append("���� ������: "+date_issue+"\r\n");
                    queryResult.push_back(record);
                    records++;
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    // ������ ��� �������:
    SaveAndShowResult("ListOfBooksIssued.out");
    return records;
}

//////////////////////////////////////////////////////////////////////
// ������ ��������� ����;  list of available books;
int LinsysDatabase::ListOfBooksAvailabled(string par)//string date1, string date2date1, string date2)
{
    int records=0;
    return records;
}

// ������� �-�� ���� � ����� ������ � ������� ��������;
// the average number of books per order and the frequency of requests;
int LinsysDatabase::CalcIssueStatistics(string par)
{
    int records=0;
    return records;
}


////////////////////////////////////////////////////////////
// ������ ��� �������:
int LinsysDatabase::SaveAndShowResult(string file)
{
    if (file.empty()) file = ofname;
    ofstream outf(file);
    if (!outf) {
        cerr << "���... �� ���� ������� ���� " << file << " ��� ������" << endl;
        return -1;
    }
    for (auto n : queryResult) {
        outf << n << endl;
    }
    ShellExecute(NULL, "open", file.c_str(), NULL,NULL,1);
    return 0;
}
