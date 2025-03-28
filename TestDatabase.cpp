// Работа с базой данных
//

#include "logger.h"
#include "TestDatabase.h"

#include "sqlite3pp.h"
#include "sqlite3ppext.h"


// =========================================================

//Logger logger("logfile.txt"); // Create logger instance

// ---------------------------------------------------------
// Создание таблиц
//
// ---------------------------------------------------------
// Создание таблицы book
char *sqlCreateTableBook =
  "CREATE TABLE IF NOT EXISTS book("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "author TEXT NOT NULL, " // -- Автор
  "title  TEXT NOT NULL, " // -- Название
  "publisher TEXT NULL, " // -- Издательство
  "year   INTEGER NULL, " // -- Год издания
  "volume INTEGER NULL, " //-- Номер тома
  "year_p INTEGER NULL, " //-- Год (периодика)
  "volume_p INTEGER NULL, " // -- Номер (периодика)
  "genre  TEXT NULL, " //-- жанр
  "UDK    TEXT NULL, " // -- УДК
  "BBK    TEXT NULL, " // -- ББК
  "ISBN   TEXT NULL);";

// ---------------------------------------------------------
// Создание таблицы reader
char *sqlCreateTableReader =
  "CREATE TABLE IF NOT EXISTS reader("
  "id INTEGER PRIMARY KEY AUTOINCREMENT,  "
  "user_id INTEGER NOT NULL, " // -- Тип пользователя + login + psw
  "ticket_id INTEGER NOT NULL, " // -- Library card - читательский билет
  "fname TEXT NOT NULL, " // -- First name - Имя
  "mname TEXT     NULL, " // -- Middle name - Отчество
  "lname TEXT NOT NULL, " // -- Last name - Фамилия
  "date_birth TEXT NULL, " // -- Дата рождения
  "passport   TEXT NULL, " // -- Номер паспорта
  "address    TEXT NULL, " // -- Адрес
  "phone      TEXT NULL, " // -- Телефон
  "email      TEXT NULL);"; // -- Почта

// ---------------------------------------------------------
// Создание таблицы «Заказ» = issue
char *sqlCreateTableIssue =
  "CREATE TABLE IF NOT EXISTS issue("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- Код Читателя
  "book_id   INTEGER NOT NULL, " // -- Код Экземпляра
  "date_order   TEXT NOT NULL, " // -- Дата заказа
  "date_issue   TEXT     NULL, " // -- Дата выдачи
  "date_backw   TEXT     NULL);";// -- Дата возврата

// ---------------------------------------------------------
// Создание таблицы «Пользователь» = user
char *sqlCreateTableUser =
  "CREATE TABLE IF NOT EXISTS user("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "reader_id INTEGER NOT NULL, " // -- Код Читателя
  "role_id   INTEGER NOT NULL, " // -- Код пользователя
  "login     TEXT    NOT NULL, " // -- login - логин
  "password  TEXT    NOT NULL);";// -- пароль

// ---------------------------------------------------------
// Создание таблицы «Читательский билет» = The reader's ticket
char *sqlCreateTableTicket =
  "CREATE TABLE IF NOT EXISTS ticket("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "number   TEXT NOT NULL, " // -- Номер билета
  "date_in  TEXT NOT NULL, " // -- Дата регистрации
  "date_out TEXT     NULL);";// -- Дата выбытия

// ---------------------------------------------------------
// Создание таблицы «Роль» = role: сисадмин, библиотекарь, абонент, учитель, ученик
char *sqlCreateTableRole =
  "CREATE TABLE IF NOT EXISTS role("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "descr TEXT NOT NULL);"; // -- Наименование

// ---------------------------------------------------------
// Создание таблицы «Жанр» = genre
char *sqlCreateTableGenre =
  "CREATE TABLE IF NOT EXISTS genre("
  "id INTEGER PRIMARY KEY AUTOINCREMENT, "
  "descr TEXT NOT NULL);"; // -- Наименование

// ---------------------------------------------------------
// Вставка в таблицы
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
  "('сисадмин'),"
  "('библиотекарь'),"
  "('абонент'),"
  "('учитель'),"
  "('ученик');";

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

// отладка: формируем INSERT для book reader issue user ticket

//"INSERT INTO book (author, title, publisher, year, volume, year_p, volume_p, genre, UDK, BBK, ISBN) "
//"VALUES (%s, %s, %s, %s, %s, %s, %s, %s, %s, %s, %s)";
char* TestDatabase::makeInsertBook(int i) {
    static char buf[512];
    char txt[11][32];
    sprintf(txt[0], "'%s%03d'", "Автор",i);
    sprintf(txt[1], "'%s%03d'", "Заголовок",i);
    sprintf(txt[2], "'%s%03d'", "Изд-во",i);
    sprintf(txt[3], "%s", i%2==0 ? genInt(2000, 2025) : "NULL");
    sprintf(txt[4], "%s", i%2==0 ? genInt(1, 5) : "NULL");
    sprintf(txt[5], "%s", i%2==1 ? genInt(2000, 2025) : "NULL");
    sprintf(txt[6], "%s", i%2==1 ? genInt(1, 9) : "NULL");
    sprintf(txt[7], "%s", genInt(1, 5)); // genre
    sprintf(txt[8], "'%s%03d'", "УДК",i);
    sprintf(txt[9], "'%s%03d'", "ББК",i);
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
    char txt[10][32], ddt[14];
    //int id = genInt(1, 100);
    sprintf(txt[0], "%d", i+1);
    sprintf(txt[1], "%d", i+1);
    sprintf(txt[2], "'%s%03d'", "Имя",i);
    sprintf(txt[3], "'%s%03d'", "Отчество",i);
    sprintf(txt[4], "'%s%03d'", "Фамилия",i);
    sprintf(txt[5], "'%s'", generateDate(ddt,2020,2025));
    sprintf(txt[6], "'%s%03d'", "Паспорт",i);
    sprintf(txt[7], "'%s%03d'", "Адр",i);
    sprintf(txt[8], "'%s%03d'", "Тел",i);
    sprintf(txt[9], "'%s%03d'", "Пчт",i);

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
char* TestDatabase::makeInsertIssue(int i)
{
    static char buf[512] = {0};
    char txt[5][32], ddt[14];
    int val;

    val = genint(1,99);
    sprintf(txt[0], "%d", val);
    val = genint(1,999);
    sprintf(txt[1], "%d", val);
    sprintf(txt[2], "'%s'", generateDate(ddt,2020,2025)); // заказ

    if (i%3 == 0) // не выдана каждая третья
    {   // если не выдана, то и не возвращена
        strcpy(txt[3], "''");
        strcpy(txt[4], "''");
    }
    else // заказ выдан
    {
        // выдача: сдвиг 0-4 дней от заказа / дата в ddt
        val = genint(0,4);
        sprintf(txt[3], "'%s'", addDaysToDate(ddt, val));
        // возвращена каждая пятая книга
        // возврат: сдвиг 1-14 дней от выдачи
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
char* TestDatabase::makeInsertTicket(int i)
{
    static char buf[512];
    char txt[3][32], ddt[14];
    sprintf(txt[0], "'%s%03d'", "Билет", i);
    sprintf(txt[1], "'%s'", generateDate(ddt,2020,2025));

    // каждый 25-й выписался из библиотеки
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
char* TestDatabase::makeInsertUser(int i)
{
    static char buf[512];
    char txt[4][32];
    sprintf(txt[0], "%s", genInt(1,99));
    sprintf(txt[1], "%s", genInt(1,5));
    sprintf(txt[2], "'%s%03d'", "Логин",i);
    sprintf(txt[3], "'%s%03d'", "Пароль",i);

    sprintf(buf, sqlInsertDataUser,
        txt[0], // 1 reader_id
        txt[1], // 2 role_id
        txt[2], // 3 login
        txt[3]  // 4 password
    );
    return buf;
}

// ---------------------------------------------------------
// Создание БД с нуля:
//    формирование и заполнение таблиц тестовыми данными
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
//                dbase.execute("DROP TABLE IF EXISTS book;");
//                dbase.execute(sqlCreateTableBook);
//                for(int i=0; i<1000; i++){
//                    dbase.execute(makeInsertBook(i));
//                }
//                logger.log(LOGlevel::INF, "таблица book создана и заполнена");
//
//                dbase.execute("DROP TABLE IF EXISTS reader;");
//                dbase.execute(sqlCreateTableReader);
//                for(int i=0; i<200; i++){
//                    dbase.execute(makeInsertReader(i));
//                }
//                logger.log(LOGlevel::INF, "таблица reader создана и заполнена");
//
//                dbase.execute("DROP TABLE IF EXISTS issue;");
//                dbase.execute(sqlCreateTableIssue);
//                for(int i=0; i<400; i++){
//                    dbase.execute(makeInsertIssue(i));//makeInsertIssue(i);
//                }
//                logger.log(LOGlevel::INF, "таблица issue создана и заполнена");
//
//                dbase.execute("DROP TABLE IF EXISTS user;");
//                dbase.execute(sqlCreateTableUser);
//                for(int i=0; i<200; i++){
//                    dbase.execute(makeInsertUser(i));
//                }
//                logger.log(LOGlevel::INF, "таблица user создана и заполнена");
//
                dbase.execute("DROP TABLE IF EXISTS ticket;");
                dbase.execute(sqlCreateTableTicket);
                for(int i=0; i<200; i++){
                    dbase.execute(makeInsertTicket(i));
                }
                logger.log(LOGlevel::INF, "таблица ticket создана и заполнена");
//
//                dbase.execute("DROP TABLE IF EXISTS role;");
//                dbase.execute(sqlCreateTableRole);
//                dbase.execute(sqlInsertDataRole);
//                logger.log(LOGlevel::INF, "таблица role создана и заполнена");
//
//                dbase.execute("DROP TABLE IF EXISTS genre;");
//                dbase.execute(sqlCreateTableGenre);
//                logger.log(LOGlevel::INF, "таблица genre создана, не заполнена");
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


void TestDatabase::createDatabaseSql(char* fname)
{
    Logger logger("logfile.txt"); // Create logger instance
    ofstream out(fname, ios::out);

    if (!out.is_open()) {
        logger.log(LOGlevel::INF, "ошибка открытия файла");
        return;
    }

    out << "DROP TABLE book;" << endl;
    out << "DROP TABLE reader;" << endl;
    out << "DROP TABLE issue;" << endl;
    out << "DROP TABLE user;" << endl;
    out << "DROP TABLE ticket;" << endl;
    out << "DROP TABLE role;" << endl;
    out << "DROP TABLE genre;" << endl;
    logger.log(LOGlevel::INF, "сформированы скрипты для удаления таблиц");

    out << sqlCreateTableBook << endl;
    out << sqlCreateTableReader << endl;
    out << sqlCreateTableIssue << endl;
    out << sqlCreateTableUser << endl;
    out << sqlCreateTableTicket << endl;
    out << sqlCreateTableRole << endl;
    out << sqlCreateTableGenre << endl;
    logger.log(LOGlevel::INF, "сформированы скрипты для создания таблиц");

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
    logger.log(LOGlevel::INF, "сформированы данные для загрузки");

    out.close();
}



// ---------------------------------------------------------
// отладка: сохранить все данные в текстовый файл

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

// отладка: генерация целого числа в диапазоне
int TestDatabase::genint(int min, int max) {
    return min + rand()%(max-min+1);
}

char* TestDatabase::genInt(int min, int max) {
    static char text[10];
    sprintf(text, "%d", min + rand()%(max-min+1));
    return text;
}

// отладка: Генерация даты
char* TestDatabase::generateDate(char* buf, int i1, int i2){
    sprintf(buf, "%4d-%02d-%02d",
        genint(i1, i2), genint(1, 12), genint(1, 31));
    return buf;
}

// отладка: получение даты, смещённой на days дней
char* TestDatabase::addDaysToDate(char* date, int days)
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
                    bookinfo.append("Автор: "+author+"\r\n");
                    bookinfo.append("Название: "+title+"\r\n");
                    bookinfo.append("Издательство: "+publisher+"\r\n");
                    bookinfo.append("Год издания: "+year+"\r\n");
                    bookinfo.append("Номер тома: "+volume+"\r\n");
                    bookinfo.append("Год (периодика): "+year_p+"\r\n");
                    bookinfo.append("Номер (периодика): "+volume_p+"\r\n");
                    bookinfo.append("Жанр: "+genre+"\r\n");
                    bookinfo.append("УДК: "+udk+"\r\n");
                    bookinfo.append("ББК: "+bbk+"\r\n");
                    bookinfo.append("ISBN: "+isbn+"\r\n");
                    //bookinfo.append("Экземпляров: "+count+"\r\n");
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
    // только для отладки:
  	// Если мы не можем открыть этот файл для записи данных в него
    //  ofstream outf(ofname);
    //	if (!outf) {
    //		cerr << "Упс... не могу открыть файл " << ofname << " для записи" << endl;
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
