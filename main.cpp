// ������������ ������� � ��������������� ������� �����������.
// ������� ������������ - �������
//
// ������� ������� ������������� ��� ������������� ����� ����������� ����������
// � �������������� �������������� ������������ �������������/��������� ����������
// �� ������ � ������ ��������� ����������.

#include <stdio.h> // printf, scanf
#include <conio.h> // _getch
#include <ctype.h> // tolower
#include <string.h>// strcat
#include <windows.h>

#include <iostream>
#include <fstream>

#include "sqlite3.h"

//#include "sqlite3pp.h"
//#include "sqlite3ppext.h"

#include "TestDatabase.h"

#define ESC 27
#define LINSYS_BOOKS   11
#define LINSYS_READERS 10
#define LINSYS_ISSUES  6
#define LINSYS_TICKETS 2
#define LINSYS_USERS   4
#define LINSYS_ROLES   2
#define LINSYS_BOOKINLIBR 4

using namespace std;

typedef struct EntityItem {
    string par; // ������������ ���������
    string col; // ������������ �������
    string vpar;// ����� ��� ������
    int sd; // ��� ���������: 0-�����, 1-�����
} entity;


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
//('������������', '�������', '������', '�������', '��������'
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


// =========================================================
// ���������� �������

void gotoxy(int x, int y);
void putsxy(int x, int y, char* str);
void getsxy(char* txt, int x, int y, char* str);
void clearRows(int a, int b);
void SetConsoleSize(int dx, int dy);

int  userControl();
int  menuMain();

// ---
int  menuGetBooks();
int  menuGetReaders();
int  menuGetIssues();
int  menuGetTickers();
int  menuGetUsers();

int  menuNewBook();
int  menuNewReader();
int  menuNewIssue();
int  menuNewTicker();
int  menuNewUser();

int  menuMakeReports();
int  menuExtraModes();

// ---
int  modePickBooks();
int  modePickReaders();
int  modePickIssues();
int  modePickTickers();
int  modePickUsers();

int  modeSaveNewBook();
int  modeSaveNewReader();
int  modeSaveNewIssue();
int  modeSaveNewTicket();
int  modeSaveNewUser();

int  modeMakeReports();

// ---
int     makeDebugData();
int     makeNewDatabase();
string  makeBookQuery();
string  makeReaderQuery();


// =========================================================
void gotoxy(int x, int y){
    COORD coord = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void putsxy(int x, int y, char* str){
    gotoxy(x, y);
    printf("%s", str);
}

void getsxy(char* txt, int x, int y, char* str){
    gotoxy(x, y);
    printf("%s", txt);
    fgets(str, sizeof(str), stdin);
    int l = strlen(str);
    if(str[l-1] == '\n') str[l-1] = 0;
}

void clearRows(int a, int b){
    for(int r=a;r<=b;r++) {
      gotoxy(1,r); for(int c=0;c<90;c++) putchar(32);
    }
}

// =========================================================
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
    putsxy(27, 8+ind, "  ");

    switch (choice) {
        case 72:// UpArrow:
            ind--; if (ind<1) ind=1;
        break;

        case 80:// DownArrow:
            ind++; if (ind>maxind) ind=maxind;
        break;

        case 73:// PageUp:
            ind=1;
        break;

        case 81:// PageDown:
            ind=maxind;
        break;
    }
}

void handleBackspace(entity* arr, int searchInd){
    int len = arr[searchInd].vpar.size();
    if (len>0) {
        arr[searchInd].vpar.erase(len-1,1);
        putsxy(27+1+len, 8+searchInd, " ");
    }
}

// =========================================================
//
int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    SetConsoleSize(65, 30);

    int rc = userControl();
    menuMain();

    return 0;
}

/**
    sqlite3pp::database db("LInSys.db");

    ofstream outf("db_out.txt");
	// ���� �� �� ����� ������� ���� ���� ��� ������ ������ � ����
	if (!outf)
	{
		// �� ������� ��������� �� ������ � ��������� exit()
		cerr << "Uh oh, SomeText.txt could not be opened for writing!" << endl;
		exit(1);
	}

    sqlite3pp::query query(db, sql01.c_str());

    for (int i = 0; i < query.column_count(); ++i) {
      outf << query.column_name(i) << endl;// "\t";
    }

    string author, title, fname, lname, date_issue;
    for (sqlite3pp::query::iterator i = query.begin(); i != query.end(); ++i) {
      (*i).getter() >> author >> title >> fname >> lname >> date_issue;
      outf << "author:" << author << " title:" << title <<
        " fname:" << fname << " lname:" << lname <<
        " date_issue:" << date_issue << endl;
    }
*/

// -------------------------------------
//
int  checkPass(string login, string password){
    return 1;
}

int  userControl(){
    string login, password;
    putsxy( 8, 2, "**** ����������� � ������� ����� ****");
    putsxy( 8, 4, "=====================================");
    putsxy(16, 6, "���� ���: ");
    cin >> login;
    putsxy(16, 7, "  ������: ");
    cin >> password;

    return checkPass(login, password);
}

// -------------------------------------
// ����� �������� ����
int menuMain()
{
  while(1) {
    system("cls");

    putsxy( 8, 2, "**** ��� ������������ ������� ����� ****");
    putsxy(20, 5, "������ ������");
    putsxy( 8, 7, "========================================");
    putsxy(16, 9, "[1] ����� ����������");  // menuGetBooks
    putsxy(16,10, "[2] ����� ����������");  // menuNewIssue
    putsxy(16,11, "[3] ����� �������");     // menuNewReader
    putsxy(16,12, "[4] ����� ���������");   // menuGetReaders
    putsxy(16,13, "[5] ������ � ������");   // menuGetIssues
    putsxy(16,14, "[6] ������, �������");   // menuMakeReports
    putsxy(16,15, "[7] ������������� ������");// menuExtraModes
        // "[1] ������ ���.�������");       // menuGetTickers()
        // "[2] ������ �������������");     // menuGetUsers()
        // "[3] ���� ����� �����");         // menuNewBook()
        // "[4] ���� ������ ���.������");   // menuNewTicker()
        // "[5] ���� ������ ������������"); // menuNewUser()
    putsxy(16,16, "[0] ����� ������");      // Exit
    putsxy( 8,18, "========================================");
    putsxy( 8,20, "������� ��������� �����:");

    char choice = _getch();
    switch(choice)
    {
    case '1': case 1: menuGetBooks();    break;
//    case '2': case 2: menuNewIssue();    break;
//    case '3': case 3: menuNewReader();   break;
    case '4': case 4: menuGetReaders();  break;
//    case '5': case 5: menuGetIssues();   break;
//    case '6': case 6: menuMakeReports(); break;
//    case '7': case 7: menuExtraModes();  break;
    case '8': case 8: makeNewDatabase();   break;
    case '0': case 0: case ESC: return 0;
    }
  }
}


// -------------------------------------
// ���� ������ ������ ����������
int  menuGetBooks()
{
    system("cls");
    putsxy( 8, 2, "**** �����  ������ ���������� ****");
    putsxy( 8, 4, "��������� ��������� ���� � �������");
    putsxy(16, 5, "Ctrl+F ��� ������ ���");
    putsxy(16, 6, "Ctrl+X ��� ESC ��� ������");
    putsxy( 8, 7, "==================================");

    for (int i=1; i<=LINSYS_BOOKS; i++)
        putsxy(8, 8+i, (char*)biArr[i].par.c_str());

    putsxy( 8, 7+3+LINSYS_BOOKS, "==================================");// 21
    putsxy( 8,22, "  ����������� ������� �����/���� ");
    putsxy( 8,23, "   ��� ������ ��������� ������");

    int searchInd = 1;
    int choice = 0;

    while( 1 ) {

        putsxy(27, 8+searchInd, "> ");
        printf("%s", biArr[searchInd].vpar.c_str());

        int choice = getch();

        if (choice == VK_ESCAPE || choice == 24)
            return 0;
        if (choice == 8) { // Backspace
            handleBackspace(biArr, searchInd);
            continue;
        }
        if (choice == 224) {
            handle224key(searchInd, LINSYS_BOOKS);
        }
        else if (choice == 6) {// CTRL+F
            modePickBooks();
            //gotoxy(8,25); printf("����� ������� %d, �� ��� � ������� %d", 111, 99);
        }
        else {// �����
            biArr[searchInd].vpar += (char)choice;
        }
    }
}

// -------------------------------------
// ���� ������ ������ ���������
int  menuGetReaders()
{
    system("cls");
    putsxy( 8, 2, "**** �����  ������  ��������� ****");
    putsxy( 8, 4, "��������� ��������� ���� � �������");
    putsxy(16, 5, "Ctrl+F ��� ������ ���");
    putsxy(16, 6, "Ctrl+X ��� ESC ��� ������");
    putsxy( 8, 7, "==================================");

    for (int i=1; i<=LINSYS_READERS; i++)
        putsxy(8, 8+i, (char*)riArr[i].par.c_str());

    putsxy( 8, 7+3+LINSYS_READERS, "==================================");// 21
    putsxy( 8,22, "  ����������� ������� �����/���� ");
    putsxy( 8,23, "   ��� ������ ��������� ������");

    int searchInd = 1;
    int choice = 0;

    while( 1 ) {

        putsxy(27, 8+searchInd, "> ");
        printf("%s", riArr[searchInd].vpar.c_str());

        int choice = getch();

        if (choice == VK_ESCAPE || choice == 24)
            return 0;
        if (choice == 8) { // Backspace
            handleBackspace(riArr, searchInd);
            continue;
        }
        if (choice == 224) {
            handle224key(searchInd, LINSYS_READERS);
        }
        else if (choice == 6) {// CTRL+F
            modePickReaders();
            //gotoxy(8,25); printf("����� ������� %d, �� ��� � ������� %d", 111, 99);
        }
        else {// �����
            riArr[searchInd].vpar += (char)choice;
        }
    }
}

// ---------------------------------------------------------
int  modePickBooks()  {
    string sql = makeBookQuery();
    clearRows(26,27);
    putsxy(8,26, (char*)sql.c_str());
    gotoxy(1,28);

    TestDatabase tdb;
    tdb.testPickBooks(sql);

    return 1;
}

int  modePickReaders()  {
    string sql_query = makeReaderQuery();
    putsxy( 8,26, (char*)sql_query.c_str());
    return 1;
}

string  makeBookQuery()  {
    string sql_query = "SELECT * FROM book WHERE ";
    for(int i=1; i<=LINSYS_BOOKS; i++) {
        if (biArr[i].vpar.size()>0) {
            sql_query.append(biArr[i].col);
            sql_query.append(biArr[i].sd?"=":" LIKE '%");
            sql_query.append(biArr[i].vpar);
            sql_query.append(biArr[i].sd?" ":"%' ");
            sql_query.append("AND ");
        }
    }
    // ��������� ��������� ���������
    size_t position = sql_query.rfind("AND ");
    sql_query[position] = 0;
    return sql_query;
}

string  makeReaderQuery()  {
    string sql_query = "SELECT * FROM reader WHERE ";
    for(int i=1; i<=LINSYS_READERS; i++) {
        if (riArr[i].vpar.size()>0) {
            sql_query.append(riArr[i].col);
            sql_query.append(riArr[i].sd?"=":" LIKE '%");
            sql_query.append(riArr[i].vpar);
            sql_query.append(riArr[i].sd?" ":"%' ");
            sql_query.append("AND ");
        }
    }
    // ��������� ��������� ���������
    size_t position = sql_query.rfind("AND ");
    sql_query[position] = 0;
    return sql_query;
}


int makeNewDatabase()
{
    TestDatabase tdb;
    tdb.makeNewDatabase("LInSys.db");

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

                [7] ������������� ������
                [0] ����� ������

        ========================================

        ������� ��������� �����:
------------------------------------------------------------
[1] ����� ����������
        **** �����  ������ ���������� ****

        ��������� ��������� ���� � �������
                Ctrl+F ��� ������ ���
                Ctrl+X ��� ESC ��� ������
        ==================================

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

        ==================================

          ����������� ������� �����/����
           ��� ������ ��������� ������

        ����� ������� 111, �� ��� � ������� 99
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

        =============================================


------------------------------------------------------------
[3] ����� �������

        **** �����  �����  ������� ****

        ��������� ��������� ���� � �������
                Ctrl+S ��� ������ ��������
                Ctrl+X ��� ESC ��� ������
        ==================================

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

        ==================================

          ����������� ������� �����/����
           ��� ������ ��������� ������
------------------------------------------------------------
[4] ����� ���������

        **** �����  ������  ��������� ****

        ��������� ��������� ���� � �������
                Ctrl+F ��� ������ ���
                Ctrl+X ��� ESC ��� ������
        ==================================

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

        ==================================

          ����������� ������� �����/����
           ��� ������ ��������� ������
------------------------------------------------------------
[5] ������ � ������

        **** �����  ������ �  ������  ****

        ��������� ��������� ���� � �������
                Ctrl+F ��� ������ ���
                Ctrl+X ��� ESC ��� ������
        ==================================

        ��� ��������       >
        ��� ����������
        ���� ������
        ���� ������
        ���� ��������
        ����������

        ==================================

          ����������� ������� �����/����
           ��� ������ ��������� ������
------------------------------------------------------------
[6] ������, ������� - �� ������ ������, - �����������

              ������������ ������� � �������
	����� �� ��������:
		- ������ ������ ��������
		- ������� �-�� ���� � ����� ������ � ������� ��������
	����� �� ������ ����:
		- ������ �������� ����
		- ������ ��������� ����
		- ������ ��������, ����������� � ������������ ������
		- ������ ��������, ����������� �� ����� �������
		- ������ ��������, ����������� � �������� ������������� ��������
	����� - �������������� ������
        - �-�� ����, �������� �� ���������� ������� (�� ����������)
        - �-�� �����������, ������������������ � ���������� �������
        - �-�� ��������� �������� �� ������ (� ������������)
        - �-�� ��������� �������� �� �������� (� ������������)
        - �-�� ��������� �������� �� ������ ��������� (� ������������)

------------------------------------------------------------
[7]
              �������������� ������ ������

        ========================================

                [1] ������ ���.�������
                [2] ������ �������������
                [3] ���� ����� �����
                [4] ���� ������ ���.������
                [5] ���� ������ ������������
                [0] ������� � �������� ����

        ========================================

        ������� ��������� �����:
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
