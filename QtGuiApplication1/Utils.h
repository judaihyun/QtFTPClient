#pragma once
#include "WinSockHeader.h"
#include <sstream>
#include <QString>
#include <qstringlist.h>
#define PRINT_BUFSIZE 10000



void replaceString(std::string & strCallId, const char * pszBefore, const char * pszAfter);

void portDecoder(std::string &source, std::string& ip, int &port);

std::string portEncoder(int, string);

void commandSeparator(std::vector<string> &, char *target);

string extractPath(string& input);

void printPerms(fs::perms p);

void printStatus(const fs::path& p, fs::file_status s);

QString variadicToQstring(const char*, ...);

QString sizeFormat(quint64 size);

void extractFileName(QStringList& input, QString& output);
/*
void DisplayText(const char *fmt, ...);

void DisplayDir(const char *fmt, ...);
*/