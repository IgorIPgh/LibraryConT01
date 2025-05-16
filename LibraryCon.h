#ifndef LIBRARYCON_H_INCLUDED
#define LIBRARYCON_H_INCLUDED

////////////////////////////////////////////////////////////////////////////////
// Наименование Системы – «Информационная система БИБЛИОТЕКА».
// Краткое наименование - «ИнСиБ»

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

class Button {
    int x, y, n;
    const string s;
    struct text_info ti;
public:
    Button(int bx, int by, string bs) : x(bx), y(by), s(bs), n(bs.size()){};
    void draw(){
        gettextinfo(&ti);// сохраняем текущую видеоинформацию
        drawborder(x,y,x+n+2,y+1,"  ##"); // тень
        textattr(to_attr(WHITE,GREEN,0));
        clrarea(x,y,x+n+1,y);
        gotoxy(x+1,y);
        cputs(s.c_str());
        settextinfo(&ti);//// восстанавливаем видеоинформацию
    }
    bool clicked(int mx, int my){
        return (mx>=x && mx <=x+n+4 && my==y);
    }

};

// =========================================================
// Объявление функций

void putsxy(int x, int y, char* str);
void SetConsoleSize(int dx, int dy);
void GetConsoleSize(int &dw, int &dh);

int  userControl();
int  menuMain();
void indexControl(int choice, int maxind, int step, int &ind, bool& update);

// ---
int  notExistsYet();

int  menuSearchBooks();     // 1
int  menuCheckOrder();      // 2
int  menuIssuedOrders();    // 3
int  menuNewReader();       // 4
int  menuSearchReaders();   // 5
int  menuMakeReports();     // 6
int  menuExtraModes();      // 7

int  menuSearchUsers();     //
int  menuSearchTickers();   //
int  menuSearchBooks();     //

int  menuNewBook();
int  menuNewTicker();
int  menuNewUser();

// ---
int  modeSearchBooks();
int  modeSearchReaders();
int  modeSearchIssues();
int  modeSearchTickers();
int  modeSearchUsers();
int  modeMakeIssues();
int  modeMakeReports(int searchInd, string par, int parlen);

int  modeReportReaderDebts     (string par);
int  modeReportReaderRegistered(string par);
int  modeReportBooksIssued     (string par);

// ---
int  menuShowTickets();
int  menuAddBook();
int  menuEditBook();
int  menuDeleteBook();
int  menuAddReader();
int  menuEditReader();
int  menuDeleteReader();

// ---
int  makeTestDatabase();


#endif // LIBRARYCON_H_INCLUDED
