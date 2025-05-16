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
  "publisher TEXT NULL, "  // -- ������������
  "year   INTEGER NULL, "  // -- ��� �������
  "volume INTEGER NULL, "  // -- ����� ����
  "year_p INTEGER NULL, "  // -- ��� (���������)
  "volume_p INTEGER NULL, "// -- ����� (���������)
  "genre  TEXT NULL, " // -- ����
  "UDK    TEXT NULL, " // -- ���
  "BBK    TEXT NULL, " // -- ���
  "ISBN   TEXT NULL);";

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
// �������� ������� reader
char *sqlCreateTableReader =
  "CREATE TABLE IF NOT EXISTS reader("
  "id INTEGER PRIMARY KEY AUTOINCREMENT,  "
  "fname TEXT NOT NULL, "  // -- First name - ���
  "mname TEXT     NULL, "  // -- Middle name - ��������
  "lname TEXT NOT NULL, "  // -- Last name - �������
  "date_birth TEXT NULL, " // -- ���� ��������
  "address    TEXT NULL, " // -- �����
  "phone      TEXT NULL, " // -- �������
  "email      TEXT NULL);"; // -- �����

// ---------------------------------------------------------
// �������� ������� �������� = abonent
char *sqlCreateTableAbonent =
  "CREATE TABLE IF NOT EXISTS abonent("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- ��� ��������
  "role_id   INTEGER NOT NULL, " // -- ��� ������������
  "login     TEXT    NOT NULL, " // -- login - �����
  "password  TEXT    NOT NULL, " // -- ������
  "ticket    TEXT    NOT NULL, " // -- ����� ������
  "date_in   TEXT    NOT NULL, " // -- ���� �����������
  "date_out  TEXT        NULL);";// -- ���� �������

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

char *sqlInsertDataIssue =
  "INSERT INTO issue (reader_id, book_id, date_order, date_issue, date_backw) "
  "VALUES (%s, %s, %s, %s, %s);";

char *sqlInsertDataReader =
//  "INSERT INTO reader (fname, mname, lname, date_birth, address, phone, email, role_id, ticket_id) "
//  "VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s);";
  "INSERT INTO reader (fname, mname, lname, date_birth, address, phone, email) "
  "VALUES (%s, %s, %s, %s, %s, %s, %s);";

char *sqlInsertDataAbonent =
  "INSERT INTO abonent (reader_id, role_id, login, password, ticket, date_in, date_out) "
  "VALUES (%s,%s, %s,%s, %s, %s,%s);";

char *sqlInsertDataRole =
  "INSERT INTO role (descr) VALUES"
  "('��������'),"
  "('������������'),"
  "('�������'),"
  "('�������'),"
  "('������');";

char *sqlInsertDataGenre =
  "INSERT INTO genre (descr) VALUES"
  "('������'),"
  "('���������, �������'),"
  "('�������'),"
  "('���������'),"
  "('�������'),"
  "('�������, ����'),"
  "('����������� � �����'),"
  "('�����, �������������'),"
  "('�����'),"
  "('�������'),"
  "('�����'),"
  "('����������, �������'),"
  "('��������� � �������');";


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
    sprintf(txt[2], "'%s%03d'", "������������",i);
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

//"INSERT INTO reader (fname, mname, lname, date_birth, address, phone, email) "
//"VALUES (%s, %s, %s, %s, %s, %s, %s);";
char* LinsysDatabase::makeInsertReader(int i) {
    static char buf[512];
    char txt[10][32], ddt[14];
    sprintf(txt[0], "'%s%03d'", "���",i);
    sprintf(txt[1], "'%s%03d'", "��������",i);
    sprintf(txt[2], "'%s%03d'", "�������",i);
    sprintf(txt[3], "'%s'", generateDate(ddt,2020,2025));
    sprintf(txt[4], "'%s%03d'", "���",i);
    sprintf(txt[5], "'%s%03d'", "���",i);
    sprintf(txt[6], "'%s%03d'", "���",i);

    sprintf(buf, sqlInsertDataReader,
        txt[0], // 1 fname
        txt[1], // 2 mname
        txt[2], // 3 lname
        txt[3], // 4 date_birth
        txt[4], // 6 address
        txt[5], // 7 phone
        txt[6] // 8 email
    );
    return buf;
}

//  "INSERT INTO abonent (reader_id, role_id, login, password, ticket, date_in, date_out) "
//  "VALUES (%s,%s, %s,%s, %s, %s,%s);";
char* LinsysDatabase::makeInsertAbonent(int i)
{
    static char buf[512];
    char txt[7][32], ddt[14];

    sprintf(txt[0], "%d", i+1); // reader_id
    sprintf(txt[1], "%s", genInt(1,5));  // role_id
    sprintf(txt[2], "'%s%03d'", "�����",i);  // login
    sprintf(txt[3], "'%s%03d'", "������",i); // password
    sprintf(txt[4], "'%s%03d'", "�����", i); // ticket
    sprintf(txt[5], "'%s'", generateDate(ddt,2020,2025)); // date_in
    // ������ 25-� ��������� �� ����������
    int val = genint(100,1000);
    const char* sday = (i%25==0 ? addDaysToDate(ddt, val) : "");
    sprintf(txt[6], "'%s'", sday ); // date_out

    sprintf(buf, sqlInsertDataAbonent,
        txt[0], // 1 reader_id
        txt[1], // 2 role_id
        txt[2], // 3 login
        txt[3], // 4 password
        txt[4], // 1 ticket
        txt[5], // 3 date_in
        txt[6]  // 4 date_out
    );
    return buf;
}

// ---------------------------------------------------------
// �������� �� � ����:
//    ������������ � ���������� ������ ��������� �������
//    ��������� CP1251
void LinsysDatabase::makeTestDatabase(char* dbname)
{
    Logger logger(logfile);
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
                logger.log(LOGlevel::INF, "������� book ������� � ���������"); Beep(400,500);

                dbase.execute("DROP TABLE IF EXISTS reader;");
                dbase.execute(sqlCreateTableReader);
                for(int i=0; i<200; i++){
                    dbase.execute(makeInsertReader(i));
                }
                logger.log(LOGlevel::INF, "������� reader ������� � ���������"); Beep(500,500);

                dbase.execute("DROP TABLE IF EXISTS issue;");
                dbase.execute(sqlCreateTableIssue);
                for(int i=0; i<400; i++){
                    dbase.execute(makeInsertIssue(i));
                }
                logger.log(LOGlevel::INF, "������� issue ������� � ���������"); Beep(600,500);

                dbase.execute("DROP TABLE IF EXISTS abonent;");
                dbase.execute(sqlCreateTableAbonent);
                for(int i=0; i<200; i++){
                    dbase.execute(makeInsertAbonent(i));
                }
                logger.log(LOGlevel::INF, "������� abonent ������� � ���������");  Beep(700,500);

                dbase.execute("DROP TABLE IF EXISTS role;");
                dbase.execute(sqlCreateTableRole);
                dbase.execute(sqlInsertDataRole);
                logger.log(LOGlevel::INF, "������� role ������� � ���������"); Beep(800,500);

                dbase.execute("DROP TABLE IF EXISTS genre;");
                dbase.execute(sqlCreateTableGenre);
                dbase.execute(sqlInsertDataGenre);
                logger.log(LOGlevel::INF, "������� genre �������, �� ���������"); Beep(900,500);
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
    out << "DROP TABLE issue;" << endl;
    out << "DROP TABLE reader;" << endl;
    out << "DROP TABLE abonent;" << endl;
    out << "DROP TABLE role;" << endl;
    out << "DROP TABLE genre;" << endl;
    logger.log(LOGlevel::INF, "������������ ������� ��� �������� ������");

    out << sqlCreateTableBook << endl;
    out << sqlCreateTableIssue << endl;
    out << sqlCreateTableReader << endl;
    out << sqlCreateTableAbonent << endl;
    out << sqlCreateTableRole << endl;
    out << sqlCreateTableGenre << endl;
    logger.log(LOGlevel::INF, "������������ ������� ��� �������� ������");

    for(int i=0; i<1000; i++)
        out << makeInsertBook(i) << endl;
    for(int i=0; i<400; i++)
        out << makeInsertIssue(i) << endl;
    for(int i=0; i<200; i++)
        out << makeInsertReader(i) << endl;
    for(int i=0; i<1000; i++)
        out << makeInsertAbonent(i) << endl;
    out << sqlInsertDataRole << endl;
    out << sqlInsertDataGenre << endl;
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
void LinsysDatabase::saveSqlAbonent(char* fname, int num){
    FILE *fpDump;
    if((fpDump=fopen(fname, "w"))==NULL) {
        return;
    }
    for(int i=0; i<num; i++) {
        fprintf(fpDump,"%s\n", makeInsertAbonent(i));
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

////////////////////////////////////////////////////////////////////
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

                string col[LINSYS_BOOKS+1];

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    string bookinfo;
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
    //SaveAndShowResult("SearchBook.out");
    return records;
}

//
string LinsysDatabase::makeBookQuery(int sortby)  {
    string sql_query = "SELECT * FROM book ";
    string sql_where, sql_sort = " ORDER BY ";
    for(int i=1; i<=LINSYS_BOOKS; i++) {
        if (biArr[i].var.size()>0) {
            sql_where.append(biArr[i].col);
            sql_where.append(biArr[i].sd?"=":" LIKE '%");
            sql_where.append(biArr[i].var);
            sql_where.append(biArr[i].sd?" ":"%' ");
            sql_where.append("AND ");
        }
    }
    // ��������� ��������� ���������
    size_t position = sql_where.rfind("AND ");
    sql_where[position] = 0;
    // ������ �������� �������
    if(sql_where.size() > 0) {
        sql_query.append("WHERE "+sql_where);
    }
    sql_sort = sortby==0?"id":sortby==1?"author":"title";
    sql_query.append(sql_sort);

    return sql_query;
}


//////////////////////////////////////////////////////////////////////
// + RequestBook
int LinsysDatabase::RequestBook(string sql, vector<tblBook> & vec)
{
    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                vec.clear();

                sqlite3pp::query query(dbase, sql.c_str());

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    tblBook tbook;
                    (*i).getter()
                        >> tbook.id >> tbook.author >> tbook.title >> tbook.publisher
                        >> tbook.year >> tbook.volume >> tbook.year_p >> tbook.volume_p
                        >> tbook.genre >> tbook.udk >> tbook.bbk >> tbook.isbn;
                    vec.push_back(tbook);
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    return vec.size();
}

///-------------------------------------------------------------------
// �������� ���������� = reader
//
string  LinsysDatabase::makeReaderQuery()  {
    string sql_query =
    "SELECT reader.id as read_id, abonent.id as abon_id, abonent.role_id, "
    "lname,fname,mname, date_birth, address,phone,email, "
    "role.descr, abonent.ticket "
    "FROM reader, abonent "
    "INNER JOIN role ON abonent.role_id = role.id "
    "AND abonent.reader_id = reader.id ";

    string sql_where;
    for(int i=1; i<=LINSYS_READERS; i++) {
        if (riArr[i].var.size()>0) {
            sql_where.append(riArr[i].col);
            sql_where.append(riArr[i].sd?"=":" LIKE '%");
            sql_where.append(riArr[i].var);
            sql_where.append(riArr[i].sd?" ":"%' ");
            sql_where.append("AND ");
        }
    }
    // ������� ��������� ��������� AND
    size_t position = sql_where.rfind("AND ");
    sql_where[position] = 0;
    // ������ �������� �������
    if(sql_where.size() > 0) {
        sql_query.append("WHERE "+sql_where);
    }
    return sql_query;
}

//////////////////////////////////////////////////////////////////////
// + RequestReader
int LinsysDatabase::RequestReader(string sql, vector<tblReader> &vec)
{
    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                vec.clear();

                sqlite3pp::query query(dbase, sql.c_str());

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    tblReader trdr;
                    (*i).getter()
                        >> trdr.read_id >> trdr.abon_id >> trdr.role_id
                        >> trdr.fname >> trdr.mname >> trdr.lname
                        >> trdr.date_birth >> trdr.address >> trdr.phone
                        >> trdr.email >> trdr.srole >> trdr.sticket;
                    vec.push_back(trdr);
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    return vec.size();
}

//////////////////////////////////////////////////////////////////////
// �������� �������� = abonent
//
string  LinsysDatabase::makeAbonentQuery()  {
    string sql_query ="SELECT abonent.id as abon_id, "
        "reader.id as read_id, role.id as role_id, "
        "login, password, ticket, date_in, date_out, "
        "reader.lname, reader.mname, reader.fname, role.descr "
        "FROM abonent, reader INNER JOIN role ON abonent.role_id = role.id "
        "AND abon_id = read_id";

    string sql_where;
    for(int i=1; i<=LINSYS_ABONENTS; i++) {
        if (riArr[i].var.size()>0) {
            sql_where.append(aiArr[i].col);
            sql_where.append(aiArr[i].sd?"=":" LIKE '%");
            sql_where.append(aiArr[i].var);
            sql_where.append(aiArr[i].sd?" ":"%' ");
            sql_where.append("AND ");
        }
    }
    // ������� ��������� ��������� AND
    size_t position = sql_where.rfind("AND ");
    sql_where[position] = 0;
    // ������ �������� �������
    if(sql_where.size() > 0) {
        sql_query.append("WHERE "+sql_where);
    }
    return sql_query;
}


//////////////////////////////////////////////////////////////////////
// + vectorAbonent
int LinsysDatabase::RequestAbonent(string sql, vector<tblAbonent> &vec)
{
    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                vec.clear();

                sqlite3pp::query query(dbase, sql.c_str());

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    tblAbonent trdr;
                    string ln,mn,fn;
                    (*i).getter()
                        >> trdr.abon_id >> trdr.read_id >> trdr.role_id
                        >> trdr.login >> trdr.password >> trdr.ticket
                        >> trdr.date_in >> trdr.date_out >> ln >> mn >> fn
                        >> trdr.srole;
                    trdr.sreader=ln+' '+fn+' '+mn;
                    vec.push_back(trdr);
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    return vec.size();
}

// ---------------------------------------------------------
// == ������ ===
//    ������ ������ ���������(�������� IDs) 1  [id1 .. idN ��� ��� ��������]
//    ����������� ���������  (�������� ���) 2  [� .. �� .. ��� ��� ��������]
//    ������ �������� ����   (�������� ���) 3  [id1 .. idN ��� ��� ��������]
//    ������ ��������� ����
//    ���������� �������
// ---------------------------------------------------------

//////////////////////////////////////////////////////////////////////
// + ������ ������ ��������;
// list of reader's debts
string  LinsysDatabase::makeReaderDebtsQuery(string par)  {
    string sql_query =
    "SELECT reader.id, book.id, reader.lname, reader.fname, reader.mname, reader.address, reader.phone, "
    "book.author, book.title, book.year, book.volume, book.year_p, book.volume_p, issue.date_issue, "
    "date(issue.date_issue, '+15 days') "
    "FROM issue "
    "INNER JOIN reader ON issue.reader_id=reader.id AND issue.date_backw='' AND issue.date_issue!='' "
    "INNER JOIN book ON issue.book_id=book.id ";

    int r1=-1, r2=-1;
    int rc=sscanf(par.c_str(), "%d%*c%d", &r1, &r2);
    string where="";

    switch(rc) {
        case 1: where="reader.id="+to_string(r1); break;
        case 2: where="reader.id>="+to_string(r1);
                where.append(" AND reader.id<="+to_string(r2)); break;
    }

    // ������ �������� �������
    if(where.size() > 0) {
        sql_query.append("WHERE "+where);
    }
    Logger logger(logfile);
    logger.log(LOGlevel::INF, sql_query);
    return sql_query;
}

int LinsysDatabase::RequestReaderDebts(string sql, vector<tblDebt> &vec)
{
    try {
        sqlite3pp::database dbase(dbname.c_str());
        {
            sqlite3pp::transaction xct(dbase);
            {
                vec.clear();

                sqlite3pp::query query(dbase, sql.c_str());

                for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
                    tblDebt tdbt;
                    (*i).getter()
                        >> tdbt.read_id >> tdbt.book_id >> tdbt.lname >> tdbt.fname >> tdbt.mname >> tdbt.address
                        >> tdbt.phone >> tdbt.author >> tdbt.title >> tdbt.year >> tdbt.volume >> tdbt.year_p
                        >> tdbt.volume_p >> tdbt.date_issue >> tdbt.date_wait
                        ;
                    vec.push_back(tdbt);
                }
            }
        }
        dbase.disconnect();

    } catch (exception & ex) {
        Logger logger(logfile);
        logger.log(LOGlevel::ERR, ex.what());
    }

    return vec.size();
}


//////////////////////////////////////////////////////////////////////
// + ������ ���������, ������������������ � ���������� �������
// list of readers registered during the time period
//
string  LinsysDatabase::makeReaderRegisteredQuery(string par)  {
    string sql_query =
    "select reader.id, abonent.id, abonent.role_id,"
    "lname,fname,mname, date_birth, address,phone,email,"
    "abonent.ticket, abonent.date_in from reader"
    "inner join abonent on reader.id=abonent.reader_id"
    "where abonent.date_in ";

    string sql_where;
    string p1="", p2="";
    sscanf(par.c_str(), "%s%*c%s", &p1, &p2);
    if (p2=="") sql_query.append("= "+p1);
    else sql_query.append("between "+p1+" and "+p2);
    // ������ �������� �������
    if(sql_where.size() > 0) {
        sql_query.append(sql_where);
    }
    return sql_query;
}
int LinsysDatabase::RequestReaderRegistered(string sql, vector<tblRgstrd> & vec){
    int records=0;
    return records;
}

//////////////////////////////////////////////////////////////////////
// + ������ �������� ����;  list of books issued;
// + ������ ����, �������� �� ���������� ������� (�� ����������).
string LinsysDatabase::makeBooksIssuedQuery(string par){
    return "";
}
int LinsysDatabase::RequestBooksIssued(string sql, vector<tblIssued> & vec){
    return 0;
}

//////////////////////////////////////////////////////////////////////
// - ������ ��������� ����;  list of available books; ������ ��������� ����
int LinsysDatabase::ListOfBooksAvailabled(string par)//string date1, string date2
{
    int records=0;
    return records;
}
//
// - ������� �-�� ���� � ����� ������ � ������� ��������;
// the average number of books per order and the frequency of requests;
int LinsysDatabase::CalcIssueStatistics(string par)
{
    int records=0;
    return records;
}


////////////////////////////////////////////////////////////
// ������ ��� �������: ����� � ���� ���������� queryResult
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
