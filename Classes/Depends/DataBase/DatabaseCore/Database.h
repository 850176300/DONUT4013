//
//  Database.h
//  GIRLS_Framework_part
//
//  Created by renhong on 9/3/13.
//
//

#ifndef __GIRLS_Framework_part__Database__
#define __GIRLS_Framework_part__Database__

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include "cocos2d.h"
#include "sqlite3.h"
#include <unistd.h>
#include <time.h>
#include <errno.h>
using namespace std;

class Records {
private:
    sqlite3_stmt *_stmt;
    vector<string> _fieldNames;
private:
    inline string fieldName(int pFieldIndex);
    inline int fileIndex(string pFieldName);
public:
    Records(sqlite3_stmt *_stmt);
    ~Records();
    
    int intForField(string pFieldName);
    float floatForField(string pFieldName);
    string textForField(string pFieldName);
    const void *blobForField(string pFieldName);
    bool next();
};

class Database {
private:
    sqlite3 *_database;
    Database();
//protected:
    static Database *_sharedDatabase;
public:
    static Database *sharedDatabase();
    
    bool open(string pDatabasePath);
    void close();
    
    Records *query(string pQuerySql);
    Records *query(vector<string> pFields, map<string, string>pConditions, string pTableName);

    int insert(string pFields, string pValues, string pTableName);
    int insert(map<string, string>, string pTableName);
    int insert(string pInsertSql);
    bool remove(int pRowID, string pTableName);
    bool remove(string pConditions, string pTableName);
    bool update(int pRowID, string pFieldAndValues, string pTableName);
    bool update(string pSql);
    bool update(string pConditions, string pFieldAndValues, string pTableName);
    bool update(string pConditions, map<string, string>pFieldAndValues, string pTableName);
    bool update(map<string, string> pConditions, map<string, string>pFieldAndValues, string pTableName);
    bool update(int pRowID, map<string, string>pFieldAndValues, string pTableName);
};

#endif /* defined(__GIRLS_Framework_part__Database__) */
