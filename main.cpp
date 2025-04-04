////////////////////////////////////////////////////////////////////////////////
// ������������ ������� � ��������������� ������� �����������.
// ������� ������������ - �������
//
// ������� ������� ������������� ��� ������������� ����� ����������� ����������
// � �������������� �������������� ������������ �������������/��������� ����������
// �� ������ � ������ ��������� ����������.

// void window(int left, int top, int right, int bottom);
// ������������ ������� ������.

// void clrarea(int left, int top, int right, int bottom);
// ������� ������� � ���������� ������������ � ������� ����

// int cputs(const char *string);
// cputs ������� ������������ ����� ������ string � ������� ��������� ����.
// � ������������ � �������� ���������� �������. ��� �������� �� ����� ������
// � ������� cputs ���������� ������������ ���� - "\r\n"

// void insline(void);
// insline - ��������� ������ ������ � ������� ������� ���������� ����,
// ��������� ��� ���� ������� ���� ����. ��� ������ ��� ������ ����������
// �� ���� ������ ����, � ��������� ������ � ��������� ���� ���������.

#include <stdio.h> // printf, scanf
#include <conio.h> // _getch
#include <ctype.h> // tolower
#include <string.h>// strcat
#include <windows.h>
#include <locale.h>

#include <iostream>
#include <fstream>

#include "sqlite3.h"
#include "coniow.h"
#include "logger.h"

#include "LinsysDatabase.h"

#define ESC 27
#define LINSYS_BOOKS   11
#define LINSYS_READERS 10
#define LINSYS_ISSUES  6
#define LINSYS_TICKETS 2
#define LINSYS_USERS   4
#define LINSYS_ROLES   2
#define LINSYS_BOOKINLIBR 4

#define putBorder1 drawborder( 5, 2,48,30,"-")
#define putBorder2 drawborder(48, 2,95,30,"-")
#define putBorder3 drawborder( 5,31,95,37,"-")

#define clrWindow1 clrarea( 6, 3,47,29)
#define clrWindow2 clrarea(49, 3,94,29)
#define clrWindow3 clrarea( 6,32,94,36)

#define setWindow0 window ( 1, 1,95,37)
#define setWindow1 window ( 9, 4,47,29)
#define setWindow2 window (50, 4,94,28)
#define setWindow3 window ( 7,33,94,36)

#define maxWin2H 24

using namespace std;

//Logger loggerM("logfile.txt"); // Create logger instance

// ---------------------------------------------------------


//typedef struct EntityItem {
//    string par; // ������������ ���������
//    string col; // ������������ �������
//    string var;// ����� ��� ������
//    int sd; // ��� ���������: 0-�����, 1-�����
//} entity;


//�������� Entity �������� = book
entity biArr[] = {
    {"�����.�����",       "id",        "", 1}, // 0
    {"�����, ������",     "author",    "", 0}, // 1
    {"��������",          "title",     "", 0}, // 2
    {"������������",      "publisher", "", 0}, // 3
    {"��� �������",       "year",      "", 1}, // 4
    {"����� ����",        "volume",    "", 1}, // 5
    {"��� (���������)",   "year_p",    "", 1}, // 6
    {"����� (���������)", "volume_p",  "", 1}, // 7
    {"����",              "genre",     "", 1}, // 8
    {"���",               "udk",       "", 0}, // 9
    {"���",               "bbk",       "", 0}, //10
    {"ISBN/ISSN",         "isbn",      "", 0}  //11
  //{"�-�� �����������",  "count",     ""}  //12
};

//�������� ���������� = reader
entity riArr[] = {
    {"�����.�����",       "id",         "", 1}, // 0
    {"�������",           "lname",      "", 0}, // 1
    {"���",               "fname",      "", 0}, // 2
    {"��������",          "mname",      "", 0}, // 3
    {"���� ��������",     "date_birth", "", 0}, // 4
    {"����� ��������",    "passport",   "", 0}, // 5
    {"�������",           "phone",      "", 0}, // 6
    {"�������� �����",    "address",    "", 0}, // 7
    {"E-mail",            "email",      "", 0}, // 8
    {"��� ������������",  "user_id",    "", 1}, // 9
    {"����������� �����", "ticket_id",  "", 1}  //10
};

//�������� ������ = issue
entity iiArr[] = {
    {"�����.�����",    "id",         "", 1}, // 0
    {"��� ��������",   "reader_id",  "", 1}, // 1
    {"��� ����������", "book_id",    "", 1}, // 2 ??? !!!
    {"���� ������",    "date_order", "", 0}, // 3
    {"���� ������",    "date_issue", "", 0}, // 4
    {"���� ��������",  "date_backw", "", 0}, // 5
    {"����������",     "returned",   "", 1}  // 6 ??? !!!
};

//�������� ������������� ����� = The reader's ticket
entity tiArr[] = {
    {"�����.�����",      "id",       "", 1}, // 0
    {"����� ������",     "number",   "", 1}, // 1
    {"���� �����������", "date_in",  "", 0}, // 2
    {"���� �������",     "date_out", "", 0}  // 2
};

//�������� �������������� = user
entity uiArr[] = {
    {"�����.�����",  "id",        "", 1}, // 0
    {"��� ��������", "reader_id", "", 1}, // 1
    {"��� ��������", "role_id",   "", 1}, // 2
    {"�����",        "login",     "", 0}, // 3
    {"������",       "password",  "", 0}  // 4
};

//�������� ������ = mission / role
//('��������', '������������', '�������', '�������', '������'
entity miArr[] = {
    {"�����.�����",  "id",    "", 1}, // 0
    {"��� ��������", "descr", "", 0}  // 1
};

//�������� ������ � ���������� = BookInLibr
entity ilArr[] = {
    {"�����.�����",           "id",        "", 1}, // 0
    {"��� �������� ��������", "book_id",   "", 1}, // 1
    {"����� ����������",      "count_al",  "", 1}, // 2
    {"������� ����������",    "count_cur", "", 1}  // 3
};

//�������� ������ = report
entity irArr[] = {
    {"", "", "", 0}, // 0
    {"������ ������ ���������(�������� IDs)","","", 6}, // 1  [id1 .. idN ��� ��� ��������]
    {"����������� ���������  (�������� ���)","","", 4}, // 2  [� .. �� .. ��� ��� ��������]
    {"������ �������� ����   (�������� ���)","","", 6}, // 3  [id1 .. idN ��� ��� ��������]
    {"������ ��������� ����",  "", "", 6}, // 4 -
    {"���������� �������",     "", "",24}  // 5 -
};



// =========================================================
// ���������� �������

void putsxy(int x, int y, char* str);
void SetConsoleSize(int dx, int dy);
void GetConsoleSize(int &dw, int &dh);

int  userControl();
int  menuMain();

// ---
int  menuSearchBooks();
int  menuSearchReaders();
int  menuSearchIssues();
int  menuSearchTickers();
int  menuSearchUsers();

int  menuNewBook();
int  menuNewReader();
int  menuNewIssue();
int  menuNewTicker();
int  menuNewUser();

int  menuMakeReports();
int  menuExtraModes();

// ---
int  modeSearchBooks();
int  modeSearchReaders();
int  modeSearchIssues();
int  modeSearchTickers();
int  modeSearchUsers();

int  modeMakeReports(int searchInd, string var, int parlen);

// ---
int     makeNewDatabase();
string  makeBookQuery();
string  makeReaderQuery();


// =========================================================
void putsxy(int x, int y, char* str){
    gotoxy(x, y);
    cprintf("%s", str);
}

// =========================================================
void GetConsoleSize(int &dw, int &dh)
{
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    dw = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    dh = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void SetConsoleSize(int dx, int dy)
{
    // �������� ���������� ���� ��� ������/������:
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);

    // ������ ��������� ����
    SetConsoleTitle("LInSys");

    // ������������� ��������� ������ ����
    SMALL_RECT windowSize = {0, 0, dx, dy};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);

    // ������������� ������ ������ ��������� ������:
    COORD bufferSize = {10, 10};// � ������� � ��������
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

COORD GetCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    // ��������� ���������� � �������� �������� ������ �������.
    GetConsoleScreenBufferInfo(hConsole, &consoleInfo);
    return consoleInfo.dwCursorPosition;
}


// =========================================================
//
int handle224key(int& ind, int maxind) {
    int choice = getch();
    switch (choice) {
        case 72:// UpArrow:
            ind--; if (ind<1) ind=maxind;//ind=1;
        break;
        case 80:// DownArrow:
            ind++; if (ind>maxind) ind=1;//ind=maxind;
        break;
        case 73:// PageUp:
            ind=1;
        break;
        case 81:// PageDown:
            ind=maxind;
        break;
    }
    return ind;
}

int handleBackspace(entity* arr, int searchInd){
    int len = arr[searchInd].var.size();
    if (len>0)
    arr[searchInd].var.erase(len-1,1);
    return len;
}

// =========================================================
//
int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    SetConsoleSize(99, 37);

    textbackground(BLUE);
    textcolor(YELLOW);
    clrscr();

    putBorder1;
    putBorder2;
    putBorder3;

    int rc = userControl();
    menuMain();

    return 0;
}

// -------------------------------------
//
int  checkPass(string login, string password){
    return 1;
}

int  userControl()
{
    string login, password;
    putsxy( 7, 4, "**** ����������� � ������� ����� ****");
    putsxy( 7, 6, "=====================================");
    putsxy(15, 8, "���� ���: ");
    cin >> login;
    putsxy(15, 9, "  ������: ");
    cin >> password;

    return checkPass(login, password);
}

// -------------------------------------
// ����� �������� ����
int menuMain()
{
    while(1) {
    setWindow0; clrWindow1;  clrWindow2; setWindow1;

    cputs("*** ��� ������������ ������� ����� ***\r\n"); // 4
    cputs("\r\n\r\n"); // 5 6
    cputs("           ������ ������\r\n"); // 7
    cputs("\r\n"); // 8
    cputs("======================================\r\n"); // 9
    cputs("\r\n"); // 10
    cputs("      [1] ����� ����������\r\n");  // menuSearchBooks
    cputs("      [2] ����� ����������\r\n");  // menuNewIssue
    cputs("      [3] ����� �������\r\n");     // menuNewReader
    cputs("      [4] ����� ���������\r\n");   // menuSearchReaders
    cputs("      [5] ������ � ������\r\n");   // menuSearchIssues
    cputs("      [6] ������, �������\r\n");   // menuMakeReports
    cputs("      [7] ������������� ������\r\n");// menuExtraModes
        // "[1] ������ ���.�������");       // menuSearchTickers()
        // "[2] ������ �������������");     // menuSearchUsers()
        // "[3] ���� ����� �����");         // menuNewBook()
        // "[4] ���� ������ ���.������");   // menuNewTicker()
        // "[5] ���� ������ ������������"); // menuNewUser()
    cputs("      [0] ����� ������\r\n");      // Exit 18
    cputs("\r\n"); // 19
    cputs("======================================\r\n\r\n");// 20 21 22
    cputs("������� ��������� �����:"); // 23

    char choice = _getch();
    switch(choice)
    {
    case '1': case 1: menuSearchBooks(); break; // menuGetBooks menuSearchBooks
//    case '2': case 2: menuNewIssue();    break;
//    case '3': case 3: menuNewReader();   break;
    case '4': case 4: menuSearchReaders();  break;
//    case '5': case 5: menuSearchIssues();   break;
    case '6': case 6: menuMakeReports(); break;
//    case '7': case 7: menuExtraModes();  break;
    case '8': case 8: makeNewDatabase();   break;
    case '0': case 0: case KEY_ESC: return 0;
    }
  }
}



///===================================================================
//  ("      [1] ����� ����������\r\n");  // menuSearchBooks
// -------------------------------------
// ���� ������ ������ ����������
int  menuSearchBooks()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=7;

    cputs("**** �����  ������ ���������� ****\r\n\r\n");// 4
    cputs("��������� ��������� ���� � �������\r\n"); // 6
    cputs("ENTER ��� ������ ���\r\n"); // 7
    cputs("ESC ��� �������� � �������� ����\r\n"); // 7
    cputs("==================================\r\n\r\n"); // 9
    for (int i=1; i<=LINSYS_BOOKS; i++) {// 10 - 20
        int len = biArr[i].par.size();
        if (len>parlen) parlen = len;
        cputs((char*)biArr[i].par.c_str()); cputs("\r\n");
    }
    cputs("                                  \r\n");// 20
    cputs("==================================\r\n");// 21
    cputs("����������� ������� �����/����\r\n"); // 24
    cputs("   ��� ������ ��������� ������\r\n"); // 25
    cputs("�� TAB �������: �������� -- �����\r\n"); // 26

    int searchInd=1, choice=0;

    textcolor(WHITE); // ���������� ���� �������� ����������
    while( 1 )
    {
        putsxy(parlen+2,ypos+searchInd, "> "); // 27
        cprintf("%s", biArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - ��� ����������� ��� ������
        {
            putsxy(parlen+2,ypos+searchInd, "  ");
            handle224key(searchInd, LINSYS_BOOKS);
        }
        else // ��������� ��-����������� ����� ������
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // ������������ ����
                // ������� ������ ����� ��������� ������
                for (int i=1; i<=LINSYS_BOOKS; i++)
                    biArr[i].var.clear();
                return 0;
            } else
            if (choice == KEY_BACKSPACE) {
                int len = handleBackspace(biArr, searchInd);
                putsxy(parlen+3+len,ypos+searchInd, " ");
            } else
            if (choice == KEY_ENTER) {
                modeSearchBooks();
                setWindow1;
            }
            else { // �����
                biArr[searchInd].var += (char)choice;
            }
        }
    }
}

// ---------------------------------------------------------
//			KEY_UP, KEY_DOWN,
//			KEY_PGUP, KEY_PGDN,
//			KEY_HOME, KEY_END,
//			KEY_TAB, KEY_ENTER
//          MOUSE_LCLICK, MOUSE_RCLICK
//          MOUSE_WHEELUP, MOUSE_WHEELDOWN
int  modeSearchBooks()  {

    string sql = makeBookQuery();

    LinsysDatabase tdb;
    int records = tdb.SearchBook(sql);
    int available = 0, ind = 0, maxind = records-2;;
    bool update = true;
    vector<string> queryResult = tdb.getQueryResult();

    setWindow0;
    gotoxy(9,28);
    cprintf("����� ������� %d, �� ��� � ������� %d", records, available);

    clrWindow2; setWindow2;

    if (records == 0){
        return records;
    }
    if (records == 1){
        cputs(queryResult[ind].c_str());
        return records;
    }

    while (1)
    {
        if (update) {
        cputs(queryResult[ind].c_str());
        cputs(queryResult[ind+1].c_str());
        }

        int choice = getkbm();

        if (choice == KEY_TAB || choice == KEY_ENTER) { return records; }

        else if (choice == MOUSE_LCLICK || choice == MOUSE_WHEELUP)
            { update = ind!=0; if (--ind<0) ind=0; }
        else if (choice == MOUSE_RCLICK || choice == MOUSE_WHEELDOWN)
            { update = ind!=maxind; if (++ind>maxind) ind=maxind;; }

        else if (choice - KEY_SPECIAL > 0)
        {
            switch (choice - KEY_SPECIAL) {
                case KEY_UP:
                    update = ind!=0; ind--; if (ind<0) ind=0;
                    break;
                case KEY_DOWN:
                    update = ind!=maxind; ind++; if (ind>maxind) ind=maxind;
                    break;
                case KEY_PGUP:
                    update = ind!=0; ind-=2; if (ind<1) ind=0;
                    break;
                case KEY_PGDN:
                    update = ind!=maxind; ind+=2; if (ind>=maxind) ind=maxind;
                    break;
                case KEY_HOME:
                    update = ind!=0; ind=0;
                    break;
                case KEY_END:
                    update = ind!=maxind; ind=maxind;
                    break;
            }
        }
    }
}

string  makeBookQuery()  {
    string sql_query = "SELECT * FROM book "; //;
    string sql_where;
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
    return sql_query;
}





///===================================================================
//  ("      [4] ����� ���������\r\n");   // menuSearchReaders

// -------------------------------------
// ���� ������ ������ ���������
int  menuSearchReaders()
{
    return 0;
}

int  modeSearchReaders()  {
    string sql_query = makeReaderQuery();
    putsxy( 8,26, (char*)sql_query.c_str());
    return 1;
}

string  makeReaderQuery()  {
    string sql_query = "SELECT * FROM reader WHERE ";
    for(int i=1; i<=LINSYS_READERS; i++) {
        if (riArr[i].var.size()>0) {
            sql_query.append(riArr[i].col);
            sql_query.append(riArr[i].sd?"=":" LIKE '%");
            sql_query.append(riArr[i].var);
            sql_query.append(riArr[i].sd?" ":"%' ");
            sql_query.append("AND ");
        }
    }
    // ��������� ��������� ���������
    size_t position = sql_query.rfind("AND ");
    sql_query[position] = 0;
    return sql_query;
}

int  menuMakeReports()
{
    return 0;
}

int makeNewDatabase()
{
    LinsysDatabase tdb;
    tdb.makeNewDatabase("LInSys.db");

//    tdb.createDatabaseSql("linsys.sql");
//    saveSqlBook("insert_book.sql", 1000);
//    saveSqlReader("insert_reader.sql", 200);
//    saveSqlIssue("insert_issue.sql", 400);
//    saveSqlUser("insert_user.sql", 200);
//    saveSqlTicker("insert_ticket.sql", 200);

    return 0;
}


/*
------------------------------------------------------------
        **** ����������� � ������� ����� ****
        =====================================
                ���� ���: 1
                  ������: 1

------------------------------------------------------------
        **** ��� ������������ ������� ����� ****
                    ������ ������
        ========================================
                [1] ����� ����������
                [2] ����� ����������
                [3] ����� �������
                [4] ����� ���������
                [5] ������ � ������
                [6] ������, �������
                [7] ������������� ������
                [0] ����� ������

------------------------------------------------------------
[1] ����� ����������

------------------------------------------------------------
[2] ����� ����������

                **** ����� ���������� ****

        ��������� ��������� ���� � ������� Ctrl+F
        � ���������� ������ ��� ���������� �������
        ������� ��� ���������� �����, ������� Ctrl+S,
        ����� ���� ������� ���� ����� ����� �������
                Ctrl+X ��� ESC ��� ������
        =============================================

        �����, ������      >
        ��������
        ������������
        ��� �������
        ����� ����
        ��� (���������)
        ����� (���������)
        ����
        ���
        ���
        ISBN/ISSN

------------------------------------------------------------
[3] ����� �������

        **** �����  �����  ������� ****
        �������            >
        ���
        ��������
        ���� ��������
        ����� ��������
        �������
        �������� �����
        E-mail
        ��� ������������
        ����������� �����

----------------
[4] ����� ���������

        **** �����  ������  ��������� ****
        �������            >
        ���
        ��������
        ���� ��������
        ����� ��������
        �������
        �������� �����
        E-mail
        ��� ������������
        ����������� �����

------------------------------------------------------------
[5] ������ � ������

        **** �����  ������ �  ������  ****
        ��� ��������       >
        ��� ����������
        ���� ������
        ���� ������
        ���� ��������
        ����������

------------------------------------------------------------
[6] ������, �������

        ****   ������������  �������  ****
        ������ ������ ��������      [id ��� ��� ��������]
        ������������������ �������� [� .. �� .. ��� ��� ��������]
        ����� ������ �������� ����  [id ��� ��� ��������]
        ����� ������ ��������� ����
        ���������� �������

------------------------------------------------------------
[7]
              �������������� ������ ������
        ========================================
                [1] ������ ���. �������
                [2] ������ �������������
                [3] ���� ����� �����
                [4] ���� ������ ���.������
                [5] ���� ������ ������������
                [0] ������� � �������� ����

------------------------------------------------------------
[7] - [1] ������ ���.�������

------------------------------------------------------------
[7] - [2] ������ �������������

------------------------------------------------------------
[7] - [3] ���� ����� �����

------------------------------------------------------------
[7] - [4] ���� ������ ���.������

------------------------------------------------------------
[7] - [5] ���� ������ ������������

------------------------------------------------------------


------------------------------------------------------------
*/
