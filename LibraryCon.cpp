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

// void _setcursortype(int type);
// ������������� ��� �������. ��������: _NOCURSOR (��� �������),
//  _SOLIDCURSOR (�������), _NORMALCURSOR (�������).

// int wherex(void);
// ���������� ���������� x ������� ������� ������� (������ �������� ���������� ����).
//
// int wherey(void);
// ���������� ���������� y ������� ������� ������� (������ �������� ���������� ����).

// int cscanf(const char *format[,��������,...]);
// ������ ������ � ������� cgets, � ����� ���������� format ��� ��������� ���������� � ���������.

#include "LibraryCon.h"
#include "LinsysDatabase.h"


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
    {"�����(�)",          "author",    "", 0}, // 1
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

//�������� ������ = issue
entity iiArr[] = {
    {"�����.�����",      "id",         "", 1}, // 0
    {"��� ��������",     "reader_id",  "", 1}, // 1
    {"��� ����������",   "book_id",    "", 1}, // 2 ??? !!!
    {"���� ������",      "date_order", "", 0}, // 3
    {"���� ������",      "date_issue", "", 0}, // 4
    {"���� ��������",    "date_backw", "", 0}  // 5
};

//�������� ���������� = reader
entity riArr[] = {
    {"�����.�����",       "id",         "", 1}, // 0
    {"�������",           "lname",      "", 0}, // 1
    {"���",               "fname",      "", 0}, // 2
    {"��������",          "mname",      "", 0}, // 3
    {"���� ��������",     "date_birth", "", 0}, // 4
    {"�������",           "phone",      "", 0}, // 5
    {"�������� �����",    "address",    "", 0}, // 6
    {"E-mail",            "email",      "", 0}, // 7

//    {"���������",         "role_id",    "", 1}, // 8
//    {"������ ������",     "abon_id",    "", 1}  // 9
};

//�������� �������� = abonent personal
entity aiArr[] = {
    {"�����.�����",      "id",        "", 1}, // 0
    {"��� ��������",     "reader_id", "", 1}, // 1
    {"��� ��������",     "role_id",   "", 1}, // 2
    {"�����",            "login",     "", 0}, // 3  1
    {"������",           "password",  "", 0}, // 4  2
    {"���. �����",       "ticket",    "", 0}, // 5  3
    {"���� �����������", "date_in",   "", 0}, // 6  4
    {"���� ��������",    "date_out",  "", 0}  // 7  5
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
    {"����� ����������",      "count_all", "", 1}, // 2
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


vector<tblBook> vecIssueReader; // ������ ������ ��������
vector<tblBook> vecIssueSystem; // ������ ������ ������������


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


// ===================================================================

// --- ��������� ������� � �.�. ------------------------------------
void indexControl(int choice, int maxind, int step, int &ind, bool& update)
{
    switch (choice - KEY_SPECIAL) {
        case KEY_UP:
            update = ind!=0; ind--; if (ind<0) ind=0;
            break;
        case KEY_DOWN:
            update = ind!=maxind; ind++; if (ind>maxind) ind=maxind;
            break;
        case KEY_PGUP:
            update = ind!=0; ind-=step; if (ind<1) ind=0;
            break;
        case KEY_PGDN:
            update = ind!=maxind; ind+=step; if (ind>=maxind) ind=maxind;
            break;
        case KEY_HOME:
            update = ind!=0; ind=0;
            break;
        case KEY_END:
            update = ind!=maxind; ind=maxind;
            break;
    }
}
// --- ���� ��������� ������� � �.�. ---------------------------------
int Handle224key(int& ind, int maxind) {
    int choice = getch();
    switch (choice) {
        case 72:// UpArrow:
            ind--; if (ind<1) ind=maxind;
        break;
        case 80:// DownArrow:
            ind++; if (ind>maxind) ind=1;
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

//  --- ��������� Backspace ------------------------------------
int HandleBackspace(entity* arr, int searchInd){
    int len = arr[searchInd].var.size();
    if (len>0)
    arr[searchInd].var.erase(len-1,1);
    return len;
}

void PrintTotalFound(int records)
{
    setWindow0;
    textcolor(WHITE);
    gotoxy(9,28);
    cprintf("����� ������� %6d", records);
}

// ----------------------------------------------
// �������� ��� ��������������� ������� / �������
int notExistsYet()
{
    textcolor(CYAN);
    setWindow0; putBorder2; clrWindow2; setWindow2;
    textcolor(WHITE);
    cputs("\r\n    ������ ������� ���� �� �����������");

    while (getch() != KEY_ENTER);//KEY_ESC

    textcolor(YELLOW);
    setWindow0; putBorder2;
    setWindow1;

    return 0;
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

// ---------------------------------------------
// ����������� ��������� �� ������ ������� �����
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
    while(1)
    {
        setWindow0; clrWindow1; clrWindow2; setWindow1;

        cputs("*** ��� ������������ ������� ����� ***\r\n\r\n\r\n"); // 4 5 6
        cputs("           ������ ������\r\n\r\n"); // 7 8
        cputs("======================================\r\n\r\n"); // 9 10
        cputs("      [1] ����� ����������\r\n");  // 11
        cputs("      [2] ���������� ������\r\n"); // 12
        cputs("      [3] ������ � ������\r\n\r\n"); // 13 14

        cputs("      [4] ����� ���������\r\n");   // 15
        cputs("      [5] ������, �������\r\n");   // 16
        cputs("      [6] ������������� ������\r\n\r\n");// 17 18

        cputs("      [0,Esc] ����� ������\r\n\r\n");      // 19 20
        cputs("======================================\r\n\r\n");// 21 22
        cputs("������� ��������� �����:"); // 24

        char choice = _getch();
        switch(choice)
        {
        case '1': case 1: menuSearchBooks(); break;
        case '2': case 2: menuCheckOrder(); break;
        case '3': case 3: menuIssuedOrders(); break;
        case '4': case 4: menuSearchReaders(); break;
        case '5': case 5: menuMakeReports(); break;
        case '6': case 6: menuExtraModes(); break;
        case '8': case 8: makeTestDatabase(); break;
        case '0': case 0: case KEY_ESC: return 0;
        }
    }
}


///===================================================================
///== [1] ����� ���������� == menuSearchBooks ========================
//
int  menuSearchBooks()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=7;

    cputs("****     �����  ����������    ****\r\n\r\n");// 4 5
    cputs("��������� ��������� ���� � �������\r\n"); // 6
    cputs("ENTER ��� ������ ���\r\n"); // 7
    cputs("ESC ��� �������� � �������� ����\r\n"); // 8
    cputs("==================================\r\n\r\n"); // 9 10
    for (int i=1; i<=LINSYS_BOOKS; i++) {// 11 - 21
        int len = biArr[i].par.size();
        if (len>parlen) parlen = len;
        cputs((char*)biArr[i].par.c_str()); cputs("\r\n");
    }
    cputs("\r\n==================================\r\n");// 22 23
    cputs("����������� ������� �����/����\r\n"); // 24
    cputs("   ��� ������ ��������� ������\r\n"); // 25
    cputs("ENTER ��� �������� ����� ������\r\n"); // 26

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
            Handle224key(searchInd, LINSYS_BOOKS);
        }
        else // ��������� ��-����������� ����� ������
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // ������������ ����
                clrscr();// ������� ������� ��������� ���� � ������������� ������ � ������� 1,1

                // ������� ������ ����� ��������� ������
                for (int i=1; i<=LINSYS_BOOKS; i++)
                    biArr[i].var.clear();
                return 0;
            } else
            if (choice == KEY_BACKSPACE) {
                int len = HandleBackspace(biArr, searchInd);
                putsxy(parlen+3+len,ypos+searchInd, " ");
            } else
            if (choice == KEY_ENTER) { // ����� � ������� � ����
                modeSearchBooks();
            }
            else { // �����
                biArr[searchInd].var += (char)choice;
            }
        }
    }
}
// ---------------------------------------------------------
int  modeSearchBooks()
{
    vector<tblBook> vecBook;// ������ ��� ���������

    LinsysDatabase tdb;
    string sql = tdb.makeBookQuery(0);
    int records = tdb.RequestBook(sql, vecBook);
    int issued = 0;
    int ind = 0, maxind = records-2;;
    bool update = true;

    PrintTotalFound(records);

    if (records == 0){
        setWindow1;
        //_setcursortype(_NORMALCURSOR);
        return records;
    }
    if (records == 1){
        clrWindow2; setWindow2;
        cputs(vecBook[ind].toString().c_str());
        setWindow1;
       // _setcursortype(_NORMALCURSOR);
        return records;
    }

    textcolor(CYAN);
    setWindow0; putBorder2;
    clrWindow2; setWindow2;
    _setcursortype(_NOCURSOR);

    textcolor(YELLOW); // ���������� ���� �������� ����������
    putsxy( 2,22, "����������� ��: ");
    putsxy(26,22, "�������� �����: 0");
    Button btn1( 1,24,"������");   btn1.draw();
    Button btn2(11,24,"��������"); btn2.draw();
    Button btn3(25,24,"�������� � �����"); btn3.draw();

    textcolor(WHITE); // ���������� ���� �������� ����������

    while (1)
    {
        if (update) {
            clrscr();// ������� ������� ��������� ���� � ������������� ������ � ������� 1,1
            cputs(vecBook[ind].toString().c_str());
        }

        int choice = getkbm();

        if (choice == KEY_ENTER) {
                textcolor(YELLOW);
                setWindow0; putBorder2;
                setWindow1;
                _setcursortype(_NORMALCURSOR);
                return records;
        }

        else if (choice == MOUSE_LCLICK)
        {
            int mx =_mousex - 49, my =_mousey - 3;
            if (btn1.clicked(mx, my)) { // ����������� �� �����
                sql = tdb.makeBookQuery(1);
                records = tdb.SearchBook(sql);
                ind = 0;
            }
            if (btn2.clicked(mx, my)) { // ����������� �� ��������
                sql = tdb.makeBookQuery(2);
                records = tdb.SearchBook(sql);
                ind = 0;
            }
            if (btn3.clicked(mx, my)) {  // �������� �����
                vecIssueReader.push_back(vecBook[ind]);
                char buf[5];
                sprintf(buf,"%2d", ++issued);
                putsxy(41,22, buf);
            }
        }
        else if (choice == MOUSE_WHEELUP)
            { update = ind!=0; if (--ind<0) ind=0; }
        else if (choice == MOUSE_WHEELDOWN)
            { update = ind!=maxind; if (++ind>maxind) ind=maxind;; }
        else if (choice - KEY_SPECIAL > 0)
            { indexControl(choice, maxind, 10, ind, update); }
    }
}


///===================================================================
///== [2] ���������� ������ == menuCheckOrder ========================
//
void sayNoIssues(){
    setWindow0; clrWindow2; setWindow2;
    cputs("\r\n������ ������ ���������� ����.\r\n\r\n"); //
    cputs("��������� � �������� ���� ����� ESC.\r\n\r\n"); //
    cputs("��������� � ����� ������ ����������,\r\n"); //
    cputs("����������� ������ ������������� ����,\r\n"); //
    cputs("����� ���� ��������� � ����� ������.\r\n"); //
    while (getch() != KEY_ESC);
    textcolor(YELLOW);
    setWindow0; putBorder2; setWindow1;
    _setcursortype(_NORMALCURSOR);
}
int  menuCheckOrder()
{
    setWindow0; clrWindow1; setWindow1; textcolor(YELLOW);

    cputs("****     ������  �  ������    ****\r\n\r\n");// 4 5
    cputs("�������������� ����� ����������\r\n"); // 6
    cputs("� ���� ������ ����������� � �����\r\n\r\n"); // 7 8

    cputs("==================================\r\n\r\n"); // 9 10

    cputs("��������� ������ ��������� ����,\r\n");  // 11
    cputs("������������ � ������ ������.\r\n\r\n"); // 12 13

    cputs("��� ������������� ������� ������,\r\n"); // 14
    cputs("��� ���� ������������ ������ \r\n"); // 15
    cputs("������� � �������� ����. \r\n"); // 16
    cputs("������������ � ������ �������� �����\r\n"); // 17
    cputs("����������! (������ ����� �����)\r\n"); // 18
    cputs("��������� ������ ��������� ��������\r\n"); // 19
    cputs("������� ������ ��� ������\r\n\r\n"); // 20 21

    cputs("==================================\r\n\r\n");// 22 23
    cputs("��������� �� ������ ����������� \r\n"); // 24
    cputs("��������� �������, � ����� PageUp,\r\n"); // 25
    cputs("PageDown, Home � END. ����� - ESC\r\n"); // 26

    return notExistsYet();
}


///===================================================================
///== [3] ������ � ������ == menuIssuedOrders ========================
/// Orders to be issued
int  menuIssuedOrders()
{
    return notExistsYet();
}


///===================================================================
///== [4] ����� ��������� == menuSearchReaders =======================
//
int  menuSearchReaders()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=7;

    cputs("****     �����   ���������    ****\r\n\r\n");// 4 5
    cputs("��������� ��������� ���� � �������\r\n"); // 6
    cputs("ENTER ��� ������ ���\r\n"); // 7
    cputs("ESC ��� �������� � �������� ����\r\n"); // 8
    cputs("==================================\r\n\r\n"); // 9 10
    for (int i=1; i<=LINSYS_READERS; i++) {// 11 - 17
        int len = riArr[i].par.size();
        if (len>parlen) parlen = len;
        cputs((char*)riArr[i].par.c_str()); cputs("\r\n");
    }
    cputs("\r\n==================================\r\n");// 22 23
    cputs("����������� ������� �����/����\r\n"); // 24
    cputs("   ��� ������ ��������� ������\r\n"); // 25
    cputs("ENTER ��� �������� ����� ������\r\n"); // 26

    int searchInd=1, choice=0;

    textcolor(WHITE); // ���������� ���� �������� ����������
    while( 1 )
    {
        putsxy(parlen+2,ypos+searchInd, "> "); // 27
        cprintf("%s", riArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - ��� ����������� ��� ������
        {
            putsxy(parlen+2,ypos+searchInd, "  ");
            Handle224key(searchInd, LINSYS_READERS);
        }
        else // ��������� ��-����������� ����� ������
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // ������������ ����
                // ������� ������ ����� ��������� ������
                for (int i=1; i<=LINSYS_READERS; i++)
                    riArr[i].var.clear();
                return 0;
            } else
            if (choice == KEY_BACKSPACE) {
                int len = HandleBackspace(riArr, searchInd);
                putsxy(parlen+3+len,ypos+searchInd, " ");
            } else
            if (choice == KEY_ENTER) {
                modeSearchReaders();
                setWindow1;
            }
            else { // �����
                riArr[searchInd].var += (char)choice;
            }
        }
    }
}

// ---------------------------------------------------------
int  modeSearchReaders()
{
    // ����������� �� �������� � modeSearchBooks()

    return notExistsYet();
}


///===================================================================
///== [5] ������, ������� == menuMakeReports  ========================
///
//    ������ ������ ���������(�������� IDs) // 1  [id1 .. idN ��� ��� ��������]
//    ����������� ���������  (�������� ���) // 2  [� .. �� .. ��� ��� ��������]
//    ������ �������� ����   (�������� ���) // 3  [id1 .. idN ��� ��� ��������]
//    ������ ��������� ����",  "", "", 6},  // 4 -
//    ���������� �������",     "", "",24}   // 5 -
//
int  menuMakeReports()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=9;

    cputs("****   ������������  �������  ****\r\n\r\n");// 4
    cputs("��������� ��������� ���� � �������\r\n"); // 6
    cputs("ENTER ��� ��������� ������\r\n"); // 7
    cputs("ESC ��� �������� � �������� ����\r\n"); // 8
    cputs("==================================\r\n\r\n\r\n"); // 9 10 11 12
    for (int i=1; i<=LINSYS_REPORT; i++) {// 13 - 17
        int n = irArr[i].par.size();
        if (n>parlen) parlen = n;
        cputs((char*)irArr[i].par.c_str()); cputs("\r\n\r\n");
    }
    cputs("\r\n");// 18 19
    cputs("==================================\r\n");// 20
    cputs("����������� ������� �����/����\r\n"); // 21
    cputs("   ��� ������ ��������� ������,\r\n"); // 22
    cputs("ENTER ��� �������� ����� ������\r\n"); // 23

    int searchInd=1, choice=0;

    textcolor(WHITE); // ���������� ���� �������� ����������
    while( 1 )
    {
        putsxy(7,ypos+(searchInd*2-1), "> "); // 27
        cprintf("%s", irArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - ��� ����������� ��� ������
        {
            putsxy(7,ypos+(searchInd*2-1), "  ");
            Handle224key(searchInd, LINSYS_REPORT);
        }
        else // ��������� ��-����������� ����� ������
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // ������������ ������
                // ������� ������ ����� ��������� ������
                for (int i=1; i<=LINSYS_REPORT; i++)
                    irArr[i].var.clear();
                return 0;
            } else
            if (choice == KEY_BACKSPACE) {
                int len = HandleBackspace(irArr, searchInd);
                putsxy(7+1+len,ypos+(searchInd*2-1), " ");
            } else
            if (choice == KEY_ENTER) {
                //modeMakeReports(searchInd, irArr[searchInd].var, irArr[searchInd].sd);
                if (searchInd==0) modeReportReaderDebts(irArr[searchInd].var);
                if (searchInd==1) modeReportReaderRegistered(irArr[searchInd].var);
                if (searchInd==2) modeReportBooksIssued(irArr[searchInd].var);
                setWindow1;
            }
            else { // �����
                irArr[searchInd].var += (char)choice;
            }
        }
    }
}


//----------------------------------------------------------
// ������������ ������ � ��� ����� � ����. ������� ���������
// var - ��. ��������� ������������ ������
int  modeReportReaderDebts(string var)
{
    vector<tblDebts> vecDebts;// ������ ��� ���������

    LinsysDatabase tdb;
    string sql = tdb.makeReaderDebtsQuery(var);
    int records = tdb.RequestReaderDebts(sql, vecDebts);
    int issued = 0;
    int ind = 0, maxind = records-2;;
    bool update = true;

    PrintTotalFound(records);

    if (records == 0){
        setWindow1;
        //_setcursortype(_NORMALCURSOR);
        return records;
    }
    if (records == 1){
        clrWindow2; setWindow2;
        cputs(vecDebts[ind].toString().c_str());
        setWindow1;
        // _setcursortype(_NORMALCURSOR);
        return records;
    }

    textcolor(CYAN);
    setWindow0; putBorder2;
    clrWindow2; setWindow2;
    _setcursortype(_NOCURSOR);

    textcolor(WHITE); // ���������� ���� ��������� ������

    while (1)
    {
        if (update) {
            clrscr();// ������� ������� ��������� ���� � ������������� ������ � ������� 1,1
            cputs(vecDebts[ind].toString().c_str());
        }

        int choice = getkbm();

        if (choice == KEY_ENTER) {
                textcolor(YELLOW);
                setWindow0; putBorder2;
                setWindow1;
                _setcursortype(_NORMALCURSOR);
                return records;
        }

        else if (choice == MOUSE_WHEELUP)
            { update = ind!=0; if (--ind<0) ind=0; }
        else if (choice == MOUSE_WHEELDOWN)
            { update = ind!=maxind; if (++ind>maxind) ind=maxind;; }
        else if (choice - KEY_SPECIAL > 0)
            { indexControl(choice, maxind, 10, ind, update); }
    }
}


int  modeReportReaderRegistered(string par){
    return 0;
}
int  modeReportBooksIssued     (string par){
    return 0;
}



//int  modeMakeReports(int searchInd, string var, int parlen)
//{
//    LinsysDatabase tdb;
//    int records;
//    switch(searchInd){
//    case 1: records = tdb.ListOfReadersDebts(var); break;
//    case 2: records = tdb.ListOfReadersRegistered(var); break;
//    case 3: records = tdb.ListOfBooksIssued(var); break;
//    case 4: records = tdb.ListOfBooksAvailabled(var); break;
//    case 5: records = tdb.CalcIssueStatistics(var); break;
//    }
//    int available = 0, ind = 0;
//    // maxWin2H - ����� ����� � ���� 2 = 24
//    int recwin = maxWin2H/parlen; // ������� � ����
//    int maxind = records-recwin;  // ������ 1� ������ � ��������� ����
//    bool update = true;
//    vector<string> &queryResult = tdb.getQueryResult();
//
//    PrintTotalFound(records);
//
//    clrWindow2; setWindow2;
//
//    if (records == 0){
//        cputs("\r\n    ������ ������� ���� �� �����������");
//        return records;
//    }
//    if (maxind <= 0){ // �� ���������� � 1 ����
//        for (int i=0; i<records; i++)
//            cputs(queryResult[ind+i].c_str());
//        return records;
//    }
//
//    while (1)
//    {
//        if (update) {
//            clrscr();// ������� ������� ��������� ���� � ������������� ������ � ������� 1,1
//            for (int i=0; i<recwin; i++)
//                cputs(queryResult[ind+i].c_str());
//        }
//        int choice = getkbm();
//
//        if (choice == KEY_TAB || choice == KEY_ENTER)
//            { return records; }
//        else if (choice == MOUSE_LCLICK || choice == MOUSE_WHEELUP)
//            { update = ind!=0; if (--ind<0) ind=0; }
//        else if (choice == MOUSE_RCLICK || choice == MOUSE_WHEELDOWN)
//            { update = ind!=maxind; if (++ind>maxind) ind=maxind;; }
//        else if (choice - KEY_SPECIAL > 0)
//            { indexControl(choice, maxind, recwin, ind, update); }
//    }
//}


///===================================================================
///== [6] ������������� ������ == menuExtraModes =====================
//
int menuExtraModes()
{
    while(1) {
    setWindow0; clrWindow1;  clrWindow2; setWindow1;
    textcolor(YELLOW);

    cputs("****     �������������  ������    ****\r\n\r\n"); // 7 8
    cputs("======================================\r\n\r\n"); // 9 10
    cputs("      [1] ������ ���.�������\r\n\r\n"); // 11 12
    cputs("      [2] �������� �������� �����\r\n"); // 13
    cputs("      [3] �������� �������� �����\r\n"); // 14
    cputs("      [4] �������  �������� �����\r\n\r\n");// 15 16
    cputs("      [5] �������� ��������\r\n"); // 17
    cputs("      [6] �������� ��������\r\n"); // 18
    cputs("      [7] �������  ��������\r\n\r\n"); // 19 20
    cputs("      [0,Esc] ����� ������\r\n\r\n");      // 21 22
    cputs("======================================\r\n\r\n");// 23 24
    cputs("������� ��������� �����:"); // 25

    char choice = _getch();
    switch(choice)
    {
    case '1': case 1: menuShowTickets();  break;// user ticket reader
    case '2': case 2: menuAddBook();      break;
    case '3': case 3: menuEditBook();     break;
    case '4': case 4: menuDeleteBook();   break;
    case '5': case 5: menuAddReader();    break;// reader role ticket
    case '6': case 6: menuEditReader();   break;
    case '7': case 7: menuDeleteReader(); break;
    case '0': case 0: case KEY_ESC: return 0;
    }
  }
}

///===================================================================
///== [6-1] ������ ���.������� ===
int menuShowTickets() {
    vector<tblAbonent> vecAbonent;// ������ ��� ���������

    LinsysDatabase tdb;
    string sql = tdb.makeAbonentQuery();
    int records = tdb.RequestAbonent(sql, vecAbonent);
    int ind = 0, maxind = records-1;
    bool update = true;

    PrintTotalFound(records);

    if (records == 0){
        setWindow1;
        //_setcursortype(_NORMALCURSOR);
        return records;
    }
    if (records == 1){
        clrWindow2; setWindow2;
        cputs(vecAbonent[ind].toString().c_str());
        setWindow1;
        // _setcursortype(_NORMALCURSOR);
        return records;
    }

    textcolor(CYAN);
    setWindow0; putBorder2; clrWindow2; setWindow2;
    _setcursortype(_NOCURSOR);
    textcolor(WHITE); // ���������� ���� �������� ����������

    while (1)
    {
        if (update) {
            clrscr();// ������� ������� ��������� ���� � ������������� ������ � ������� 1,1
            cputs(vecAbonent[ind].toString().c_str());
        }

        int choice = getkbm();

        if (choice == KEY_ENTER) {
                textcolor(YELLOW);
                setWindow0; putBorder2;
                setWindow1;
                _setcursortype(_NORMALCURSOR);
                return records;
        }
        else if (choice == MOUSE_WHEELUP)
            { update = ind!=0; if (--ind<0) ind=0; }
        else if (choice == MOUSE_WHEELDOWN)
            { update = ind!=maxind; if (++ind>maxind) ind=maxind;; }
        else if (choice - KEY_SPECIAL > 0)
            { indexControl(choice, maxind, 10, ind, update); }
    }
}

///== [6-2] �������� �������� ����� ===
int menuAddBook() {
    return notExistsYet();
}

///== [6-3] �������� �������� ����� ===
int menuEditBook() {
    return notExistsYet();
}

///== [6-4] �������  �������� ����� ===
int menuDeleteBook() {
    return notExistsYet();
}

///== [6-5] �������� �������� ==
int  menuAddReader(){
    return notExistsYet();
}

///== [6-6] �������� �������� ==
int  menuEditReader(){
    return notExistsYet();
}

///== [6-7] �������  �������� ==
int  menuDeleteReader(){
    return notExistsYet();
}


///===================================================================
///== [8] ������������. �������� �� == makeTestDatabase ===============
int makeTestDatabase()
{
    LinsysDatabase tdb;
    tdb.makeTestDatabase("LInSys.db");

//    tdb.createDatabaseSql("linsys.sql");
//    saveSqlBook("insert_book.sql", 1000);
//    saveSqlReader("insert_reader.sql", 200);
//    saveSqlIssue("insert_issue.sql", 400);

    return 0;
}
