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

//void gotoxy(int x, int y);
void putsxy(int x, int y, char* str);
//void getsxy(char* txt, int x, int y, char* str);
//void clearRows(int a, int b);
void SetConsoleSize(int dx, int dy);
void GetConsoleSize(int &dw, int &dh);

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
void putsxy(int x, int y, char* str){
    gotoxy(x, y);
    cprintf("%s", str);
}

// =========================================================
void GetConsoleSize(int &dw, int &dh)
{
//    CONSOLE_SCREEN_BUFFER_INFO csbiData;
//    DWORD dwWidht, dwHeight;
//    HANDLE hWndCon = GetStdHandle(STD_OUTPUT_HANDLE);
//    if (FALSE != hWndCon)GetConsoleScreenBufferInfo(hWndCon, &csbiData);
//
//    dwWidht  = csbiData.dwSize.X; // ������ ������ / ����
//    dwHeight = csbiData.srWindow.Bottom + 1; // ������ ����
//    dw = (int)dwWidht;
//    dh = (int)dwHeight;

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

void handleBackspace(entity* arr, int searchInd){
    int len = arr[searchInd].vpar.size();
    if (len>0) {
        arr[searchInd].vpar.erase(len-1,1);
        putsxy(27+1+len,10+searchInd, " ");
    }
}

// =========================================================
//
int main()
{
    SetConsoleOutputCP(1251);
    SetConsoleCP(1251);
    SetConsoleSize(99, 37); // 65, 30 / 120, 50

    textbackground(BLUE);
    textcolor(YELLOW);
    clrscr();

    drawborder( 5,2,48,30,"-");
    drawborder(48,2,95,30,"-");
    drawborder(5,31,95,37,"-");

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
    clrarea( 6,3,47,29);

    putsxy( 7, 4, "**** ��� ������������ ������� ����� ****");
    putsxy(19, 7, "������ ������");
    putsxy( 7, 9, "========================================");
    putsxy(15,11, "[1] ����� ����������");  // menuGetBooks
    putsxy(15,12, "[2] ����� ����������");  // menuNewIssue
    putsxy(15,13, "[3] ����� �������");     // menuNewReader
    putsxy(15,14, "[4] ����� ���������");   // menuGetReaders
    putsxy(15,15, "[5] ������ � ������");   // menuGetIssues
    putsxy(15,16, "[6] ������, �������");   // menuMakeReports
    putsxy(15,17, "[7] ������������� ������");// menuExtraModes
        // "[1] ������ ���.�������");       // menuGetTickers()
        // "[2] ������ �������������");     // menuGetUsers()
        // "[3] ���� ����� �����");         // menuNewBook()
        // "[4] ���� ������ ���.������");   // menuNewTicker()
        // "[5] ���� ������ ������������"); // menuNewUser()
    putsxy(15,18, "[0] ����� ������");      // Exit
    putsxy( 7,20, "========================================");
    putsxy( 7,22, "������� ��������� �����:");

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
    struct text_info ti;// ��� �������������� ��������� ������
    gettextinfo(&ti);   // ����� ����������� �� ���� ���������

    clrarea( 6,3,47,29);

    putsxy( 9, 4, "**** �����  ������ ���������� ****");
    putsxy( 9, 6, "��������� ��������� ���� � �������");
    putsxy( 9, 7, "ENTER ��� ������ ���");
    putsxy( 9, 8, "ESC ��� �������� � �������� ����");
    putsxy( 9, 9, "==================================");

    for (int i=1; i<=LINSYS_BOOKS; i++)
        putsxy(9,10+i, (char*)biArr[i].par.c_str());

    putsxy( 9, 7+5+LINSYS_BOOKS, "==================================");// 21
    putsxy( 9,24, "  ����������� ������� �����/���� ");
    putsxy( 9,25, "   ��� ������ ��������� ������");
    putsxy( 9,26, "TAB ��� �������� �� ��������� � �����");

    window (1,1,48,30);// Ok!

    int searchInd = 1;
    int choice = 0;

    while( 1 )
    {
        putsxy(27,10+searchInd, "> ");
        cprintf("%s", biArr[searchInd].vpar.c_str());

        int choice = getch();

        if (!choice)// - ��� ����������� ��� ������
        {
            putsxy(27,10+searchInd, "  ");
            handle224key(searchInd, LINSYS_BOOKS);
        }
        else // ��������� ��-����������� ����� ������
        {
            if (choice == KEY_ESC) {
                clrarea( 6,3,47,29);// Ok!
                // ������� ����� ��������� ������
                for (int i=1; i<=LINSYS_BOOKS; i++)
                    biArr[i].vpar.clear();
                return 0;
            }
            if (choice == KEY_ENTER) {
                modePickBooks();
                settextinfo(&ti);// ������������ ��������� ������
            }
            if (choice == KEY_BACKSPACE) {
                handleBackspace(biArr, searchInd);
            }
            else { // �����
                biArr[searchInd].vpar += (char)choice;
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
int  modePickBooks()  {

    string sql = makeBookQuery();

    TestDatabase tdb;
    int records = tdb.testPickBooks(sql);
    int available = 0, ind = 0, maxind = records-2;;
    bool update = true;
    vector<string> queryResult = tdb.getQueryResult();
    gotoxy(9,28);
    cprintf("����� ������� %d, �� ��� � ������� %d", records, available);

    clrarea(49,3,90,29);// Ok!
    window (50,4,90,28);// Ok!

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
        if (biArr[i].vpar.size()>0) {
            sql_where.append(biArr[i].col);
            sql_where.append(biArr[i].sd?"=":" LIKE '%");
            sql_where.append(biArr[i].vpar);
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
            putsxy(27,10+searchInd, "  ");
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

int  modePickReaders()  {
    string sql_query = makeReaderQuery();
    putsxy( 8,26, (char*)sql_query.c_str());
    return 1;
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
