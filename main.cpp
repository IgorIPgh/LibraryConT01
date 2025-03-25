// Наименование Системы – «Информационная система БИБЛИОТЕКА».
// Краткое наименование - «ИнСиБ»
//
// Система «ИнСиБ» предназначена для автоматизации труда сотрудников библиотеки
// и предоставлении дополнительных возможностей пользователям/абонентам библиотеки
// по поиску и заказу требуемой литературы.

// void window(int left, int top, int right, int bottom);
// Ограничивает область вывода.

// void clrarea(int left, int top, int right, int bottom);
// Очищает область с указанными координатами в текущем окне

// int cputs(const char *string);
// cputs выводит ограниченную нулем строку string в текущее текстовое окно.
// в соответствии с текущими выбранными цветами. Для перехода на новую строку
// в функции cputs необходимо использовать пару - "\r\n"


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
    string par; // наименование параметра
    string col; // наименование столбца
    string vpar;// текст для поиска
    int sd; // тип параметра: 0-текст, 1-цифра
} entity;


//Сущность Entity «Элемент» = book
entity biArr[] = {
    {"Внутр.номер",       "id",        "", 1}, // 0
    {"Автор, авторы",     "author",    "", 0}, // 1
    {"Заглавие",          "title",     "", 0}, // 2
    {"Издательство",      "publisher", "", 0}, // 3
    {"Год издания",       "year",      "", 1}, // 4
    {"Номер тома",        "volume",    "", 1}, // 5
    {"Год (периодика)",   "year_p",    "", 1}, // 6
    {"Номер (периодика)", "volume_p",  "", 1}, // 7
    {"Жанр",              "genre",     "", 1}, // 8
    {"УДК",               "udk",       "", 0}, // 9
    {"ББК",               "bbk",       "", 0}, //10
    {"ISBN/ISSN",         "isbn",      "", 0}  //11
  //{"К-во экземпляров",  "count",     ""}  //12
};

//Сущность «Читатель» = reader
entity riArr[] = {
    {"Внутр.номер",       "id",         "", 1}, // 0
    {"Фамилия",           "lname",      "", 0}, // 1
    {"Имя",               "fname",      "", 0}, // 2
    {"Отчество",          "mname",      "", 0}, // 3
    {"Дата рождения",     "date_birth", "", 0}, // 4
    {"Номер паспорта",    "passport",   "", 0}, // 5
    {"Телефон",           "phone",      "", 0}, // 6
    {"Домашний адрес",    "address",    "", 0}, // 7
    {"E-mail",            "email",      "", 0}, // 8
    {"Тип пользователя",  "user_id",    "", 1}, // 9
    {"Читателький билет", "ticket_id",  "", 1}  //10
};

//Сущность «Заказ» = issue
entity iiArr[] = {
    {"Внутр.номер",    "id",         "", 1}, // 0
    {"Код абонента",   "reader_id",  "", 1}, // 1
    {"Код экземпляра", "book_id",    "", 1}, // 2 ??? !!!
    {"Дата заказа",    "date_order", "", 0}, // 3
    {"Дата выдачи",    "date_issue", "", 0}, // 4
    {"Дата возврата",  "date_backw", "", 0}, // 5
    {"Возвращена",     "returned",   "", 1}  // 6 ??? !!!
};

//Сущность «Читательский билет» = The reader's ticket
entity tiArr[] = {
    {"Внутр.номер",      "id",       "", 1}, // 0
    {"Номер билета",     "number",   "", 1}, // 1
    {"Дата регистрации", "date_in",  "", 0}, // 2
    {"Дата выбытия",     "date_out", "", 0}  // 2
};

//Сущность «Пользователь» = user
entity uiArr[] = {
    {"Внутр.номер",  "id",        "", 1}, // 0
    {"Код абонента", "reader_id", "", 1}, // 1
    {"Тип абонента", "role_id",   "", 1}, // 2
    {"Логин",        "login",     "", 0}, // 3
    {"Пароль",       "password",  "", 0}  // 4
};

//Сущность «Роль» = mission / role
//('библиотекарь', 'абонент', 'ученик', 'учитель', 'сисадмин'
entity miArr[] = {
    {"Внутр.номер",  "id",    "", 1}, // 0
    {"Тип абонента", "descr", "", 0}  // 1
};

//Сущность «Книга В Библиотеке» = BookInLibr
entity ilArr[] = {
    {"Внутр.номер",           "id",        "", 1}, // 0
    {"Код элемента хранения", "book_id",   "", 1}, // 1
    {"Общее количество",      "count_al",  "", 1}, // 2
    {"Текущее количество",    "count_cur", "", 1}  // 3
};



// =========================================================
// Объявление функций

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
//    dwWidht  = csbiData.dwSize.X; // Ширина буфера / окна
//    dwHeight = csbiData.srWindow.Bottom + 1; // Высота окна
//    dw = (int)dwWidht;
//    dh = (int)dwHeight;

    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
    dw = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    dh = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

void SetConsoleSize(int dx, int dy)
{
    // Получаем дескриптор окна для чтения/записи:
    HANDLE wHnd = GetStdHandle(STD_OUTPUT_HANDLE);

    // Меняем заголовок окна
    SetConsoleTitle("LInSys");

    // Устанавливаем требуемый размер окна
    SMALL_RECT windowSize = {0, 0, dx, dy};
    SetConsoleWindowInfo(wHnd, 1, &windowSize);

    // Устанавливаем размер нового экранного буфера:
    COORD bufferSize = {10, 10};// в строках и столбцах
    SetConsoleScreenBufferSize(wHnd, bufferSize);
}

COORD GetCursorPosition() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
    // Извлекаем информацию о заданном экранном буфере консоли.
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
    putsxy( 7, 4, "**** РЕГИСТРАЦИЯ В СИСТЕМЕ ИнСиБ ****");
    putsxy( 7, 6, "=====================================");
    putsxy(15, 8, "Ваше имя: ");
    cin >> login;
    putsxy(15, 9, "  Пароль: ");
    cin >> password;

    return checkPass(login, password);
}

// -------------------------------------
// Вывод главного меню
int menuMain()
{
    while(1) {
    clrarea( 6,3,47,29);

    putsxy( 7, 4, "**** Вас приветствует система ИнСиБ ****");
    putsxy(19, 7, "РЕЖИМЫ РАБОТЫ");
    putsxy( 7, 9, "========================================");
    putsxy(15,11, "[1] Поиск литературы");  // menuGetBooks
    putsxy(15,12, "[2] Заказ литературы");  // menuNewIssue
    putsxy(15,13, "[3] Новый абонент");     // menuNewReader
    putsxy(15,14, "[4] Поиск абонентов");   // menuGetReaders
    putsxy(15,15, "[5] Заказы к выдаче");   // menuGetIssues
    putsxy(15,16, "[6] Отчеты, справки");   // menuMakeReports
    putsxy(15,17, "[7] Дополнителные режимы");// menuExtraModes
        // "[1] Список чит.билетов");       // menuGetTickers()
        // "[2] Список пользователей");     // menuGetUsers()
        // "[3] Ввод новой книги");         // menuNewBook()
        // "[4] Ввод нового чит.билета");   // menuNewTicker()
        // "[5] Ввод нового пользователя"); // menuNewUser()
    putsxy(15,18, "[0] Конец работы");      // Exit
    putsxy( 7,20, "========================================");
    putsxy( 7,22, "Введите требуемый режим:");

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
// Меню режима поиска литературы
int  menuGetBooks()
{
    struct text_info ti;// для восстановления состояния экрана
    gettextinfo(&ti);   // после возвращения из окна просмотра

    clrarea( 6,3,47,29);

    putsxy( 9, 4, "**** РЕЖИМ  ПОИСКА ЛИТЕРАТУРЫ ****");
    putsxy( 9, 6, "Заполните требуемые поля и нажмите");
    putsxy( 9, 7, "ENTER для поиска или");
    putsxy( 9, 8, "ESC для возврата в основное меню");
    putsxy( 9, 9, "==================================");

    for (int i=1; i<=LINSYS_BOOKS; i++)
        putsxy(9,10+i, (char*)biArr[i].par.c_str());

    putsxy( 9, 7+5+LINSYS_BOOKS, "==================================");// 21
    putsxy( 9,24, "  Используйте стрелки вверх/вниз ");
    putsxy( 9,25, "   для выбора параметра поиска");
    putsxy( 9,26, "TAB для возврата из просмотра в поиск");

    window (1,1,48,30);// Ok!

    int searchInd = 1;
    int choice = 0;

    while( 1 )
    {
        putsxy(27,10+searchInd, "> ");
        cprintf("%s", biArr[searchInd].vpar.c_str());

        int choice = getch();

        if (!choice)// - Это расширенный код клавиш
        {
            putsxy(27,10+searchInd, "  ");
            handle224key(searchInd, LINSYS_BOOKS);
        }
        else // Обработка не-расширенных кодов клавиш
        {
            if (choice == KEY_ESC) {
                clrarea( 6,3,47,29);// Ok!
                // очистка после окончания поиска
                for (int i=1; i<=LINSYS_BOOKS; i++)
                    biArr[i].vpar.clear();
                return 0;
            }
            if (choice == KEY_ENTER) {
                modePickBooks();
                settextinfo(&ti);// восстановили параметры экрана
            }
            if (choice == KEY_BACKSPACE) {
                handleBackspace(biArr, searchInd);
            }
            else { // текст
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
    cprintf("Всего найдено %d, из них в наличии %d", records, available);

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
    // последнее вхождение подстроки
    size_t position = sql_where.rfind("AND ");
    sql_where[position] = 0;
    // полный оператор запроса
    if(sql_where.size() > 0) {
        sql_query.append("WHERE "+sql_where);
    }
    return sql_query;
}







// -------------------------------------
// Меню режима поиска абонентов
int  menuGetReaders()
{
    system("cls");
    putsxy( 8, 2, "**** РЕЖИМ  ПОИСКА  АБОНЕНТОВ ****");
    putsxy( 8, 4, "Заполните требуемые поля и нажмите");
    putsxy(16, 5, "Ctrl+F для поиска или");
    putsxy(16, 6, "Ctrl+X или ESC для выхода");
    putsxy( 8, 7, "==================================");

    for (int i=1; i<=LINSYS_READERS; i++)
        putsxy(8, 8+i, (char*)riArr[i].par.c_str());

    putsxy( 8, 7+3+LINSYS_READERS, "==================================");// 21
    putsxy( 8,22, "  Используйте стрелки вверх/вниз ");
    putsxy( 8,23, "   для выбора параметра поиска");

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
            //gotoxy(8,25); printf("Всего найдено %d, из них в наличии %d", 111, 99);
        }
        else {// текст
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
    // последнее вхождение подстроки
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
        **** РЕГИСТРАЦИЯ В СИСТЕМЕ ИнСиБ ****

        =====================================

                Ваше имя: 1
                  Пароль: 1

------------------------------------------------------------
        **** Вас приветствует система ИнСиБ ****

                    РЕЖИМЫ РАБОТЫ

        ========================================
                [1] Поиск литературы
                [2] Заказ литературы
                [3] Новый абонент
                [4] Поиск абонентов
                [5] Заказы к выдаче

                [7] Дополнителные режимы
                [0] Конец работы

        ========================================

        Введите требуемый режим:
------------------------------------------------------------
[1] Поиск литературы
        **** РЕЖИМ  ПОИСКА ЛИТЕРАТУРЫ ****

        Заполните требуемые поля и нажмите
                Ctrl+F для поиска или
                Ctrl+X или ESC для выхода
        ==================================

        Автор, авторы      >
        Заглавие
        Издательство
        Год издания
        Номер тома
        Год (периодика)
        Номер (периодика)
        Жанр
        УДК
        ББК
        ISBN/ISSN

        ==================================

          Используйте стрелки вверх/вниз
           для выбора параметра поиска

        Всего найдено 111, из них в наличии 99
------------------------------------------------------------
[2] Заказ литературы

                **** ЗАКАЗ ЛИТЕРАТУРЫ ****

        Заполните требуемые поля и нажмите Ctrl+F
        В полученном списке для требуемого издания
        найдите его внутренний номер, нажмите Ctrl+S,
        после чего введите этот номер после запроса
                Ctrl+X или ESC для выхода
        =============================================

        Автор, авторы      >
        Заглавие
        Издательство
        Год издания
        Номер тома
        Год (периодика)
        Номер (периодика)
        Жанр
        УДК
        ББК
        ISBN/ISSN

        =============================================


------------------------------------------------------------
[3] Новый абонент

        **** РЕЖИМ  НОВЫЙ  АБОНЕНТ ****

        Заполните требуемые поля и нажмите
                Ctrl+S для записи абонента
                Ctrl+X или ESC для выхода
        ==================================

        Фамилия            >
        Имя
        Отчество
        Дата рождения
        Номер паспорта
        Телефон
        Домашний адрес
        E-mail
        Тип пользователя
        Читателький билет

        ==================================

          Используйте стрелки вверх/вниз
           для выбора параметра поиска
------------------------------------------------------------
[4] Поиск абонентов

        **** РЕЖИМ  ПОИСКА  АБОНЕНТОВ ****

        Заполните требуемые поля и нажмите
                Ctrl+F для поиска или
                Ctrl+X или ESC для выхода
        ==================================

        Фамилия            >
        Имя
        Отчество
        Дата рождения
        Номер паспорта
        Телефон
        Домашний адрес
        E-mail
        Тип пользователя
        Читателький билет

        ==================================

          Используйте стрелки вверх/вниз
           для выбора параметра поиска
------------------------------------------------------------
[5] Заказы к выдаче

        **** РЕЖИМ  ЗАКАЗЫ К  ВЫДАЧЕ  ****

        Заполните требуемые поля и нажмите
                Ctrl+F для поиска или
                Ctrl+X или ESC для выхода
        ==================================

        Код абонента       >
        Код экземпляра
        Дата заказа
        Дата выдачи
        Дата возврата
        Возвращена

        ==================================

          Используйте стрелки вверх/вниз
           для выбора параметра поиска
------------------------------------------------------------
[6] Отчеты, справки - НЕ ФОРМАТ ЭКРАНА, - НАПОМИНАНИЕ

              ФОРМИРОВАНИЕ ОТЧЕТОВ И СПРАВОК
	Отчет по читателю:
		- список долгов читателя
		- среднее к-во книг в одном заказе и частота запросов
	Отчет по выдаче книг:
		- список выданных книг
		- список свободных книг
		- список операций, совершенных с определенной книгой
		- список операций, совершенных со всеми книгами
		- список операций, совершенных с участием определенного читателя
	Отчет - статистические данные
        - к-во книг, выданных за промежуток времени (по категориям)
        - к-во посетителей, зарегистрированным в промежуток времени
        - к-во поисковых запросов по автору (с расшифровкой)
        - к-во поисковых запросов по названию (с расшифровкой)
        - к-во поисковых запросов по другим критериям (с расшифровкой)

------------------------------------------------------------
[7]
              ДОПОЛНИТЕЛЬНЫЕ РЕЖИМЫ РАБОТЫ

        ========================================

                [1] Список чит.билетов
                [2] Список пользователей
                [3] Ввод новой книги
                [4] Ввод нового чит.билета
                [5] Ввод нового пользователя
                [0] Возврат в основное меню

        ========================================

        Введите требуемый режим:
------------------------------------------------------------
[7] - [1] Список чит.билетов

------------------------------------------------------------
[7] - [2] Список пользователей

------------------------------------------------------------
[7] - [3] Ввод новой книги

------------------------------------------------------------
[7] - [4] Ввод нового чит.билета

------------------------------------------------------------
[7] - [5] Ввод нового пользователя

------------------------------------------------------------


------------------------------------------------------------
*/
