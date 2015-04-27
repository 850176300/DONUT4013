//
//  DatabaseHelper.h
//  GIRLS_Framework_part
//
//  Created by renhong on 9/4/13.
//
//

#ifndef __GIRLS_Framework_part__DatabaseHelper__
#define __GIRLS_Framework_part__DatabaseHelper__

#include <iostream>
#include <string>
//#include "CCFileUtils.h"

using namespace std;

class DatabaseHelper {
private:
    static DatabaseHelper *_sharedDatabaseHelper;
    string fileName(string pDatabaseName);
protected:
    int copyfile(string pSource, string pDest);
public:
    static DatabaseHelper *sharedDatabaseHelper();
    bool createDatabase(string pDatabaseName);
    string databasePath(string pDatabaseName);
};

#endif /* defined(__GIRLS_Framework_part__DatabaseHelper__) */
