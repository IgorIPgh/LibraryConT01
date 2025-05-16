////////////////////////////////////////////////////////////////////////////////
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

// void insline(void);
// insline - вставляет пустую строку в позиции курсора текстового окна,
// используя при этом текущий цвет фона. Все строки под пустой сдвигаются
// на одну строку вниз, а последняя строка в текстовом окне пропадает.

// void _setcursortype(int type);
// Устанавливает вид курсора. Варианты: _NOCURSOR (без курсора),
//  _SOLIDCURSOR (большой), _NORMALCURSOR (обычный).

// int wherex(void);
// возвращает координату x текущей позиции курсора (внутри текущего текстового окна).
//
// int wherey(void);
// возвращает координату y текущей позиции курсора (внутри текущего текстового окна).

// int cscanf(const char *format[,аргумент,...]);
// вводит строку с помощью cgets, а затем использует format для занесения информации в аргументы.

#include "LibraryCon.h"
#include "LinsysDatabase.h"


// ---------------------------------------------------------

//typedef struct EntityItem {
//    string par; // наименование параметра
//    string col; // наименование столбца
//    string var;// текст для поиска
//    int sd; // тип параметра: 0-текст, 1-цифра
//} entity;

//Сущность Entity «Элемент» = book
entity biArr[] = {
    {"Внутр.номер",       "id",        "", 1}, // 0
    {"Автор(ы)",          "author",    "", 0}, // 1
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

//Сущность «Заказ» = issue
entity iiArr[] = {
    {"Внутр.номер",      "id",         "", 1}, // 0
    {"Код абонента",     "reader_id",  "", 1}, // 1
    {"Код экземпляра",   "book_id",    "", 1}, // 2 ??? !!!
    {"Дата заказа",      "date_order", "", 0}, // 3
    {"Дата выдачи",      "date_issue", "", 0}, // 4
    {"Дата возврата",    "date_backw", "", 0}  // 5
};

//Сущность «Читатель» = reader
entity riArr[] = {
    {"Внутр.номер",       "id",         "", 1}, // 0
    {"Фамилия",           "lname",      "", 0}, // 1
    {"Имя",               "fname",      "", 0}, // 2
    {"Отчество",          "mname",      "", 0}, // 3
    {"Дата рождения",     "date_birth", "", 0}, // 4
    {"Телефон",           "phone",      "", 0}, // 5
    {"Домашний адрес",    "address",    "", 0}, // 6
    {"E-mail",            "email",      "", 0}, // 7

//    {"Категория",         "role_id",    "", 1}, // 8
//    {"Другие данные",     "abon_id",    "", 1}  // 9
};

//Сущность «Абонент» = abonent personal
entity aiArr[] = {
    {"Внутр.номер",      "id",        "", 1}, // 0
    {"Код абонента",     "reader_id", "", 1}, // 1
    {"Тип абонента",     "role_id",   "", 1}, // 2
    {"Логин",            "login",     "", 0}, // 3  1
    {"Пароль",           "password",  "", 0}, // 4  2
    {"Чит. билет",       "ticket",    "", 0}, // 5  3
    {"Дата регистрации", "date_in",   "", 0}, // 6  4
    {"Дата закрытия",    "date_out",  "", 0}  // 7  5
};

//Сущность «Роль» = mission / role
//('сисадмин', 'библиотекарь', 'абонент', 'учитель', 'ученик'
entity miArr[] = {
    {"Внутр.номер",  "id",    "", 1}, // 0
    {"Тип абонента", "descr", "", 0}  // 1
};

//Сущность «Книга В Библиотеке» = BookInLibr
entity ilArr[] = {
    {"Внутр.номер",           "id",        "", 1}, // 0
    {"Код элемента хранения", "book_id",   "", 1}, // 1
    {"Общее количество",      "count_all", "", 1}, // 2
    {"Текущее количество",    "count_cur", "", 1}  // 3
};

//Сущность «Отчет» = report
entity irArr[] = {
    {"", "", "", 0}, // 0
    {"Список долгов читателей(интервал IDs)","","", 6}, // 1  [id1 .. idN или все читатели]
    {"Регистрация читателей  (интервал дат)","","", 4}, // 2  [с .. по .. или все читатели]
    {"Список выданных книг   (интервал дат)","","", 6}, // 3  [id1 .. idN или все читатели]
    {"Список свободных книг",  "", "", 6}, // 4 -
    {"Статистика заказов",     "", "",24}  // 5 -
};


vector<tblBook> vecIssueReader; // список заказа читателя
vector<tblBook> vecIssueSystem; // список заказа библиотекаря


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


// ===================================================================

// --- обработка стрелок и т.п. ------------------------------------
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
// --- тоже обработка стрелок и т.п. ---------------------------------
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

//  --- обработка Backspace ------------------------------------
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
    cprintf("Всего найдено %6d", records);
}

// ----------------------------------------------
// Заглушка для нереализованных функций / режимов
int notExistsYet()
{
    textcolor(CYAN);
    setWindow0; putBorder2; clrWindow2; setWindow2;
    textcolor(WHITE);
    cputs("\r\n    Данная функция пока не реализована");

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
// регистрацию реализуем во второй очереди ИнСиБ
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
    while(1)
    {
        setWindow0; clrWindow1; clrWindow2; setWindow1;

        cputs("*** Вас приветствует система ИнСиБ ***\r\n\r\n\r\n"); // 4 5 6
        cputs("           РЕЖИМЫ РАБОТЫ\r\n\r\n"); // 7 8
        cputs("======================================\r\n\r\n"); // 9 10
        cputs("      [1] Поиск литературы\r\n");  // 11
        cputs("      [2] Оформление заказа\r\n"); // 12
        cputs("      [3] Заказы к выдаче\r\n\r\n"); // 13 14

        cputs("      [4] Поиск абонентов\r\n");   // 15
        cputs("      [5] Отчеты, справки\r\n");   // 16
        cputs("      [6] Дополнителные режимы\r\n\r\n");// 17 18

        cputs("      [0,Esc] Конец работы\r\n\r\n");      // 19 20
        cputs("======================================\r\n\r\n");// 21 22
        cputs("Введите требуемый режим:"); // 24

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
///== [1] Поиск литературы == menuSearchBooks ========================
//
int  menuSearchBooks()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=7;

    cputs("****     ВЫБОР  ЛИТЕРАТУРЫ    ****\r\n\r\n");// 4 5
    cputs("Заполните требуемые поля и нажмите\r\n"); // 6
    cputs("ENTER для поиска или\r\n"); // 7
    cputs("ESC для возврата в основное меню\r\n"); // 8
    cputs("==================================\r\n\r\n"); // 9 10
    for (int i=1; i<=LINSYS_BOOKS; i++) {// 11 - 21
        int len = biArr[i].par.size();
        if (len>parlen) parlen = len;
        cputs((char*)biArr[i].par.c_str()); cputs("\r\n");
    }
    cputs("\r\n==================================\r\n");// 22 23
    cputs("Используйте стрелки вверх/вниз\r\n"); // 24
    cputs("   для выбора параметра поиска\r\n"); // 25
    cputs("ENTER для перехода между окнами\r\n"); // 26

    int searchInd=1, choice=0;

    textcolor(WHITE); // установили цвет вводимых параметров
    while( 1 )
    {
        putsxy(parlen+2,ypos+searchInd, "> "); // 27
        cprintf("%s", biArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - это расширенный код клавиш
        {
            putsxy(parlen+2,ypos+searchInd, "  ");
            Handle224key(searchInd, LINSYS_BOOKS);
        }
        else // Обработка не-расширенных кодов клавиш
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // восстановили цвет
                clrscr();// очищает текущее текстовое окно и устанавливает курсор в позицию 1,1

                // очистка данных после окончания поиска
                for (int i=1; i<=LINSYS_BOOKS; i++)
                    biArr[i].var.clear();
                return 0;
            } else
            if (choice == KEY_BACKSPACE) {
                int len = HandleBackspace(biArr, searchInd);
                putsxy(parlen+3+len,ypos+searchInd, " ");
            } else
            if (choice == KEY_ENTER) { // найти и вывести в окно
                modeSearchBooks();
            }
            else { // текст
                biArr[searchInd].var += (char)choice;
            }
        }
    }
}
// ---------------------------------------------------------
int  modeSearchBooks()
{
    vector<tblBook> vecBook;// список для просмотра

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

    textcolor(YELLOW); // установили цвет вводимых параметров
    putsxy( 2,22, "Сортировать по: ");
    putsxy(26,22, "Заказано всего: 0");
    Button btn1( 1,24,"АВТОРУ");   btn1.draw();
    Button btn2(11,24,"ЗАГЛАВИЮ"); btn2.draw();
    Button btn3(25,24,"ДОБАВИТЬ В ЗАКАЗ"); btn3.draw();

    textcolor(WHITE); // установили цвет вводимых параметров

    while (1)
    {
        if (update) {
            clrscr();// очищает текущее текстовое окно и устанавливает курсор в позицию 1,1
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
            if (btn1.clicked(mx, my)) { // сортировать по имени
                sql = tdb.makeBookQuery(1);
                records = tdb.SearchBook(sql);
                ind = 0;
            }
            if (btn2.clicked(mx, my)) { // сортировать по заглавию
                sql = tdb.makeBookQuery(2);
                records = tdb.SearchBook(sql);
                ind = 0;
            }
            if (btn3.clicked(mx, my)) {  // заказать книгу
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
///== [2] Оформление заказа == menuCheckOrder ========================
//
void sayNoIssues(){
    setWindow0; clrWindow2; setWindow2;
    cputs("\r\nСписок заказа литературы пуст.\r\n\r\n"); //
    cputs("Вернитесь в основное меню через ESC.\r\n\r\n"); //
    cputs("Перейдите в режим поиска литературы,\r\n"); //
    cputs("подготовьте список запрашиваемых книг,\r\n"); //
    cputs("после чего вернитесь в режим заказа.\r\n"); //
    while (getch() != KEY_ESC);
    textcolor(YELLOW);
    setWindow0; putBorder2; setWindow1;
    _setcursortype(_NORMALCURSOR);
}
int  menuCheckOrder()
{
    setWindow0; clrWindow1; setWindow1; textcolor(YELLOW);

    cputs("****     ЗАКАЗЫ  К  ВЫДАЧЕ    ****\r\n\r\n");// 4 5
    cputs("Подготовленный набор литературы\r\n"); // 6
    cputs("в этом режиме оформляется в заказ\r\n\r\n"); // 7 8

    cputs("==================================\r\n\r\n"); // 9 10

    cputs("Проверьте список выбранных книг,\r\n");  // 11
    cputs("составленный в режиме поиска.\r\n\r\n"); // 12 13

    cputs("При необходимости удалите лишнее,\r\n"); // 14
    cputs("для чего используется кнопка \r\n"); // 15
    cputs("УДАЛИТЬ в соседнем окне. \r\n"); // 16
    cputs("Восстановить в списке удалённые книги\r\n"); // 17
    cputs("невозможно! (только через поиск)\r\n"); // 18
    cputs("Используя кнопку СОХРАНИТЬ передать\r\n"); // 19
    cputs("готовый список для заказа\r\n\r\n"); // 20 21

    cputs("==================================\r\n\r\n");// 22 23
    cputs("Навигация по списку выполняется \r\n"); // 24
    cputs("клавишами стрелок, а также PageUp,\r\n"); // 25
    cputs("PageDown, Home и END. Выход - ESC\r\n"); // 26

    return notExistsYet();
}


///===================================================================
///== [3] Заказы к выдаче == menuIssuedOrders ========================
/// Orders to be issued
int  menuIssuedOrders()
{
    return notExistsYet();
}


///===================================================================
///== [4] Поиск абонентов == menuSearchReaders =======================
//
int  menuSearchReaders()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=7;

    cputs("****     ПОИСК   АБОНЕНТОВ    ****\r\n\r\n");// 4 5
    cputs("Заполните требуемые поля и нажмите\r\n"); // 6
    cputs("ENTER для поиска или\r\n"); // 7
    cputs("ESC для возврата в основное меню\r\n"); // 8
    cputs("==================================\r\n\r\n"); // 9 10
    for (int i=1; i<=LINSYS_READERS; i++) {// 11 - 17
        int len = riArr[i].par.size();
        if (len>parlen) parlen = len;
        cputs((char*)riArr[i].par.c_str()); cputs("\r\n");
    }
    cputs("\r\n==================================\r\n");// 22 23
    cputs("Используйте стрелки вверх/вниз\r\n"); // 24
    cputs("   для выбора параметра поиска\r\n"); // 25
    cputs("ENTER для перехода между окнами\r\n"); // 26

    int searchInd=1, choice=0;

    textcolor(WHITE); // установили цвет вводимых параметров
    while( 1 )
    {
        putsxy(parlen+2,ypos+searchInd, "> "); // 27
        cprintf("%s", riArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - это расширенный код клавиш
        {
            putsxy(parlen+2,ypos+searchInd, "  ");
            Handle224key(searchInd, LINSYS_READERS);
        }
        else // Обработка не-расширенных кодов клавиш
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // восстановили цвет
                // очистка данных после окончания поиска
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
            else { // текст
                riArr[searchInd].var += (char)choice;
            }
        }
    }
}

// ---------------------------------------------------------
int  modeSearchReaders()
{
    // реализовать по аналогии с modeSearchBooks()

    return notExistsYet();
}


///===================================================================
///== [5] Отчеты, справки == menuMakeReports  ========================
///
//    Список долгов читателей(интервал IDs) // 1  [id1 .. idN или все читатели]
//    Регистрация читателей  (интервал дат) // 2  [с .. по .. или все читатели]
//    Список выданных книг   (интервал дат) // 3  [id1 .. idN или все читатели]
//    Список свободных книг",  "", "", 6},  // 4 -
//    Статистика заказов",     "", "",24}   // 5 -
//
int  menuMakeReports()
{
    setWindow0; clrWindow1; setWindow1;
    int parlen=0, ypos=9;

    cputs("****   ФОРМИРОВАНИЕ  ОТЧЕТОВ  ****\r\n\r\n");// 4
    cputs("Заполните требуемые поля и нажмите\r\n"); // 6
    cputs("ENTER для получения отчета\r\n"); // 7
    cputs("ESC для возврата в основное меню\r\n"); // 8
    cputs("==================================\r\n\r\n\r\n"); // 9 10 11 12
    for (int i=1; i<=LINSYS_REPORT; i++) {// 13 - 17
        int n = irArr[i].par.size();
        if (n>parlen) parlen = n;
        cputs((char*)irArr[i].par.c_str()); cputs("\r\n\r\n");
    }
    cputs("\r\n");// 18 19
    cputs("==================================\r\n");// 20
    cputs("Используйте стрелки вверх/вниз\r\n"); // 21
    cputs("   для выбора параметра поиска,\r\n"); // 22
    cputs("ENTER для перехода между окнами\r\n"); // 23

    int searchInd=1, choice=0;

    textcolor(WHITE); // установили цвет вводимых параметров
    while( 1 )
    {
        putsxy(7,ypos+(searchInd*2-1), "> "); // 27
        cprintf("%s", irArr[searchInd].var.c_str());

        int choice = getch();

        if (choice==0)// =0 - это расширенный код клавиш
        {
            putsxy(7,ypos+(searchInd*2-1), "  ");
            Handle224key(searchInd, LINSYS_REPORT);
        }
        else // Обработка не-расширенных кодов клавиш
        {
            if (choice == KEY_ESC) {
                textcolor(YELLOW); // восстановили желтый
                // очистка данных после окончания поиска
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
            else { // текст
                irArr[searchInd].var += (char)choice;
            }
        }
    }
}


//----------------------------------------------------------
// Формирование отчета и его вывод в окно. Имеется скроллинг
// var - вх. параметры формируемого отчёта
int  modeReportReaderDebts(string var)
{
    vector<tblDebts> vecDebts;// список для просмотра

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

    textcolor(WHITE); // установили цвет выводимых данных

    while (1)
    {
        if (update) {
            clrscr();// очищает текущее текстовое окно и устанавливает курсор в позицию 1,1
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
//    // maxWin2H - число строк в окне 2 = 24
//    int recwin = maxWin2H/parlen; // записей в окне
//    int maxind = records-recwin;  // индекс 1й записи в последнем окне
//    bool update = true;
//    vector<string> &queryResult = tdb.getQueryResult();
//
//    PrintTotalFound(records);
//
//    clrWindow2; setWindow2;
//
//    if (records == 0){
//        cputs("\r\n    Данная функция пока не реализована");
//        return records;
//    }
//    if (maxind <= 0){ // всё помещается в 1 окне
//        for (int i=0; i<records; i++)
//            cputs(queryResult[ind+i].c_str());
//        return records;
//    }
//
//    while (1)
//    {
//        if (update) {
//            clrscr();// очищает текущее текстовое окно и устанавливает курсор в позицию 1,1
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
///== [6] Дополнителные режимы == menuExtraModes =====================
//
int menuExtraModes()
{
    while(1) {
    setWindow0; clrWindow1;  clrWindow2; setWindow1;
    textcolor(YELLOW);

    cputs("****     ДОПОЛНИТЕЛНЫЕ  РЕЖИМЫ    ****\r\n\r\n"); // 7 8
    cputs("======================================\r\n\r\n"); // 9 10
    cputs("      [1] Список чит.билетов\r\n\r\n"); // 11 12
    cputs("      [2] Добавить описание книги\r\n"); // 13
    cputs("      [3] Изменить описание книги\r\n"); // 14
    cputs("      [4] Удалить  описание книги\r\n\r\n");// 15 16
    cputs("      [5] Добавить абонента\r\n"); // 17
    cputs("      [6] Изменить абонента\r\n"); // 18
    cputs("      [7] Удалить  абонента\r\n\r\n"); // 19 20
    cputs("      [0,Esc] Конец работы\r\n\r\n");      // 21 22
    cputs("======================================\r\n\r\n");// 23 24
    cputs("Введите требуемый режим:"); // 25

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
///== [6-1] Список чит.билетов ===
int menuShowTickets() {
    vector<tblAbonent> vecAbonent;// список для просмотра

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
    textcolor(WHITE); // установили цвет вводимых параметров

    while (1)
    {
        if (update) {
            clrscr();// очищает текущее текстовое окно и устанавливает курсор в позицию 1,1
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

///== [6-2] Добавить описание книги ===
int menuAddBook() {
    return notExistsYet();
}

///== [6-3] Изменить описание книги ===
int menuEditBook() {
    return notExistsYet();
}

///== [6-4] Удалить  описание книги ===
int menuDeleteBook() {
    return notExistsYet();
}

///== [6-5] Добавить абонента ==
int  menuAddReader(){
    return notExistsYet();
}

///== [6-6] Изменить абонента ==
int  menuEditReader(){
    return notExistsYet();
}

///== [6-7] Удалить  абонента ==
int  menuDeleteReader(){
    return notExistsYet();
}


///===================================================================
///== [8] Тестирование. Создание БД == makeTestDatabase ===============
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
