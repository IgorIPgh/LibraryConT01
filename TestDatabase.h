#ifndef TESTDATABASE_H_INCLUDED
#define TESTDATABASE_H_INCLUDED

#include <time.h>

#include <ctime>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <windows.h>

using namespace std;


// ---------------------------------------------------------
/*
// Enum to represent log levels
enum class LogLevel { DEBUG, INFO, WARNING, ERROR, CRITICAL };

class Logger {
public:
    // Constructor: Opens the log file in append mode
    Logger(const string& filename)
    {
        logFile.open(filename, ios::app);
        if (!logFile.is_open()) {
            cerr << "Error opening log file." << endl;
        }
    }

    // Destructor: Closes the log file
    ~Logger() { logFile.close(); }

    // Logs a message with a given log level
    void log(LogLevel level, const string& message)
    {
        // Get current timestamp
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char timestamp[20];
        strftime(timestamp, sizeof(timestamp),
                 "%Y-%m-%d %H:%M:%S", timeinfo);

        // Create log entry
        ostringstream logEntry;
        logEntry << "[" << timestamp << "] "
                 << levelToString(level) << ": " << message
                 << endl;

        // Output to console
        cout << logEntry.str();

        // Output to log file
        if (logFile.is_open()) {
            logFile << logEntry.str();
            logFile.flush(); // Ensure immediate write to file
        }
    }

private:
    ofstream logFile; // File stream for the log file

    // Converts log level to a string for output
    string levelToString(LogLevel level)
    {
        switch (level) {
        case DEBUG:
            return "DEBUG";
        case INFO:
            return "INFO";
        case WARNING:
            return "WARNING";
        case ERROR:
            return "ERROR";
        case CRITICAL:
            return "CRITICAL";
        default:
            return "UNKNOWN";
        }
    }
};
*/


// ---------------------------------------------------------
class TestDatabase
{
private:
    char buf[256];
    string dbname;
    string ofname;
    vector<string> queryResult;
    //sqlite3pp::database &dbase;

public:

    TestDatabase(){
        srand(time(NULL));
        dbname = "LInSys.db";
        ofname = "db_out.txt";
    }
    vector<string>& getQueryResult() { return queryResult; }

    // отладка: генераци€ целого числа в диапазоне
    char* genInt(int min, int max);

    // отладка: строка с числом
    char* StrInt(char* s, int i);

    // отладка: √енераци€ даты
    char* generateDate();

    // отладка: формируем INSERT'ы
    char *makeInsertBook  (int i);
    char *makeInsertReader(int i);
    char *makeInsertIssue (int i);
    char *makeInsertUser  (int i);
    char *makeInsertTicket(int i);

    // отладка: непосредственное создание Ѕƒ
    void makeNewDatabase(char* dbname);
    int  testPickBooks(string sql);

    // отладка: сохранить все данные в текстовый файл
    void saveSqlBook  (char* fname, int num);
    void saveSqlReader(char* fname, int num);
    void saveSqlIssue (char* fname, int num);
    void saveSqlUser  (char* fname, int num);
    void saveSqlTicker(char* fname, int num);

};


#endif // TESTDATABASE_H_INCLUDED
