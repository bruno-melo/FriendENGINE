#ifndef _LogObject_h
#define _LogObject_h

#include <string.h>
#include <fstream>
#include <sstream>

using namespace std;

class LogObject
{
public:
	stringstream buffer;
	fstream outputStream;
	string fileName;

	// create log file
	void initializeLogFile(char *filename);

	// closes the log file
	void closeLogFile();

	// writes the message in the log file
	void writeLog(int inScreen, const char * format, ...);
	LogObject() { fileName = ""; };
	~LogObject() { closeLogFile(); };
};
#endif