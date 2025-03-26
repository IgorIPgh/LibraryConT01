
// C++ program to implement a basic logging system.

#include "logger.h"

using namespace std;

void Logger::log(LOGlevel level, const string& message)
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

    // Output to console !!! ???
    //cout << logEntry.str();

    // Output to log file
    if (logFile.is_open()) {
        logFile << logEntry.str();
        logFile.flush(); // Ensure immediate write to file
    }
}
