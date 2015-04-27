//
//  DatabaseHelper.cpp
//  GIRLS_Framework_part
//
//  Created by renhong on 9/4/13.
//
//

#include "DatabaseHelper.h"
#include <fstream>
#include <cocos2d.h>
#include <iostream>
#include <cocos2d.h>
using namespace cocos2d;

#pragma mark - DatabaseHelper
DatabaseHelper *DatabaseHelper::_sharedDatabaseHelper = NULL;
DatabaseHelper * DatabaseHelper::sharedDatabaseHelper()
{
    if (NULL == _sharedDatabaseHelper) {
        _sharedDatabaseHelper = new DatabaseHelper();
    }
    
    return _sharedDatabaseHelper;
}

string DatabaseHelper::fileName(string pDatabaseName)
{
    int nend=0;
    int nbegin=0;
    vector<string> lPaths;
    while(nend != -1)
    {
        nend = pDatabaseName.find("/", nbegin);
        if(nend == -1)
            lPaths.push_back(pDatabaseName.substr(nbegin, pDatabaseName.length()-nbegin));
        else
            lPaths.push_back(pDatabaseName.substr(nbegin, nend-nbegin));
        nbegin = nend + strlen("/");
    }
    string lDatabaseName;
    if (lPaths.size() > 0) {
        lDatabaseName = lPaths.at(lPaths.size() - 1);
    }
    else{
        lDatabaseName = pDatabaseName;
    }
    return lDatabaseName;
}

bool DatabaseHelper::createDatabase(string pDatabaseName)
{

    bool lRes = false;
    string lDatabaseName = this->fileName(pDatabaseName);
    string lDatabasePath = FileUtils::getInstance()->getWritablePath() + lDatabaseName;
    string lDatabaseSrcPath = FileUtils::getInstance()->fullPathFromRelativeFile(lDatabaseName.c_str(), pDatabaseName.c_str());
    if (FileUtils::getInstance()->isFileExist(lDatabaseSrcPath)) {
        if (!FileUtils::getInstance()->isFileExist(lDatabasePath)) {
            long len = 0;
            unsigned char *data = NULL;
            
            data = FileUtils::getInstance()->getFileData(lDatabaseSrcPath.c_str(),"r",&len);
            if (NULL != data && len > 0) {
                std::string destPath = FileUtils::getInstance()->getWritablePath();
                destPath += lDatabaseName;
                FILE *fp = fopen(destPath.c_str(),"w+");
                if(NULL != fp){
                    fwrite(data,sizeof(char),len,fp);
                    fclose(fp);
                    lRes = true;
                }
                delete []data;
                data = NULL;
            }
        }
        else{
            lRes = true;
        }
    }
    
    return lRes;
}

string DatabaseHelper::databasePath(string pDatabaseName)
{
    string lDatabaseName = this->fileName(pDatabaseName);
    string lDatabasePath = FileUtils::getInstance()->getWritablePath() + lDatabaseName;
    bool lDatabaseExist = FileUtils::getInstance()->isFileExist(lDatabasePath);
    assert(lDatabaseExist);
    if (lDatabaseExist) {
        return lDatabasePath;
    }
    return "";
}