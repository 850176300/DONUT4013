//
//  Database.cpp
//  GIRLS_Framework_part
//
//  Created by renhong on 9/3/13.
//
//

#include "Database.h"
#include <iostream>
#pragma mark - Record 
Records::Records(sqlite3_stmt *pStmt)
{
    this->_stmt = pStmt;
    int lColumnCount = sqlite3_column_count(pStmt);
    for(int i = 0; i < lColumnCount; i++) {
    	string lFieldName = string(sqlite3_column_name(pStmt, i));
    	_fieldNames.push_back(lFieldName);
    }
    //cout << "that Name " << sqlite3_column_text(_stmt, 5) << "_stmt " << _stmt << endl;
}

Records::~Records()
{
    sqlite3_finalize(this->_stmt);
    this->_stmt = NULL;
}

inline int Records::fileIndex(string pFieldName)
{
    for(int i = 0; i < _fieldNames.size(); i++) {
		//char* ptr = (char*)wyArrayGet(m_columnNames, i);
        string lFieldName = _fieldNames.at(i);
		if(lFieldName == pFieldName)
			return i;
	}
    
	return -1;
}

int Records::intForField(string pFieldName)
{
    int lFieldIndex = this->fileIndex(pFieldName);
    assert(lFieldIndex >= 0);
    //cout << "_stmt " << _stmt <<endl;
    return sqlite3_column_int(this->_stmt, lFieldIndex);
}

float Records::floatForField(string pFieldName)
{
    int lFieldIndex = this->fileIndex(pFieldName);
    assert(lFieldIndex >= 0);
    return sqlite3_column_double(this->_stmt, lFieldIndex);
}

string Records::textForField(string pFieldName)
{
    int lFieldIndex = this->fileIndex(pFieldName);
    assert(lFieldIndex >= 0);
    const unsigned char *lTextChar = sqlite3_column_text(this->_stmt, lFieldIndex);
    //cout << "_stmt " << _stmt << "this Name " << lTextChar  << endl;
    string lText;
    if (NULL != lTextChar) {
        lText += (char *)lTextChar;
    }
    
    //cout  << "this Name " << lText  << endl;
    return lText;
}

const void* Records::blobForField(string pFieldName)
{
    int lFieldIndex = this->fileIndex(pFieldName);
    assert(lFieldIndex >= 0);
    return sqlite3_column_blob(this->_stmt, lFieldIndex);
}

bool Records::next()
{
    int rc = 0;
	if(NULL != this->_stmt) {
		bool retry;
		int numberOfRetries = 0;
		do {
			retry = false;
            
			rc = sqlite3_step(this->_stmt);
            
			if(SQLITE_BUSY == rc || SQLITE_LOCKED == rc) {
				// this will happen if the db is locked, like if we are doing an update or insert.
				// in that case, retry the step... and maybe wait just 10 milliseconds.
				retry = true;
				if(SQLITE_LOCKED == rc) {
					rc = sqlite3_reset(this->_stmt);
					if(rc != SQLITE_LOCKED) {
						cout << "Records::next: Unexpected result from sqlite3_reset (%d) rs" << rc << endl;;
					}
				}
#ifdef WIN32
                Sleep(2);
#else
                usleep(20);
#endif
                
                //time out
                if(numberOfRetries++ > 50) {
					cout << "wyResultSet::next: Database busy (%@)" << endl;
					break;
				}
			} else if(SQLITE_DONE == rc || SQLITE_ROW == rc) {
				// all is well, let's return.
			} else if(SQLITE_ERROR == rc) {
				cout << "Error calling sqlite3_step " << rc << endl;
				break;
			} else if(SQLITE_MISUSE == rc) {
				cout << "Error calling sqlite3_step " << rc << endl;
				break;
			} else {
				cout << "Unknown error calling sqlite3_step " << rc << endl;
				break;
			}
		} while(retry);
	}
    
    if(rc != SQLITE_ROW) {
		if (this->_stmt) {
            sqlite3_reset(this->_stmt);
        }
	}
    
	return (rc == SQLITE_ROW);
}

#pragma mark - Database
Database *Database::_sharedDatabase = NULL;
Database * Database::sharedDatabase()
{
    if (NULL == _sharedDatabase) {
        _sharedDatabase = new Database();
    }
    
    return _sharedDatabase;
}

Database::Database()
{
    
}

/*!
 @function
 @abstract   open database
 @discussion <#(description)#>
 @param
 adatabaseName database file name
 @result     
 */
bool Database::open(string pDatabasePath)
{
    if (SQLITE_OK != sqlite3_open(pDatabasePath.c_str(), &_database)) {
        return false;
    }
    
    return true;
}

/*!
 @function
 @abstract   close
 @discussion <#(description)#>
 @param      <#(name) (description)#>
 @result     <#(description)#>
 */
void Database::close()
{
    sqlite3_close(_database);
}


Records *Database::query(string pQuerySql)
{
    sqlite3_stmt *lStmt = NULL;
    assert(_database);
    if (SQLITE_OK != sqlite3_prepare_v2(_database, pQuerySql.c_str(), -1, &lStmt, NULL)) {
        cout << "QueryWithString.sqlite3_prepare error %s" << pQuerySql <<endl;
        return NULL;
    }
    
    Records *lRecords = new Records(lStmt);
    return lRecords;
}

Records *Database::query(vector<string> pFields,map<string, string>pConditions, string pTableName)
{
    string lConditions;
    map<string, string>::iterator lIterator;
    for(lIterator = pConditions.begin(); lIterator != pConditions.end(); lIterator++)
    {
        assert(lIterator->first.length() > 0);
        if (lIterator->first.length() > 0) {//query fileds must > 0
            lConditions = lConditions + lIterator->first + "='" + lIterator->second + "' AND ";
        }
    }
    //assert(lConditions.length() > 0);
    if (lConditions.length() > 0) {
        lConditions = lConditions.substr(0, lConditions.length() - 5);
    }
    string lFields;
    for (int i = 0; i < pFields.size(); ++i) {
        lFields = lFields + pFields.at(i) + ",";
    }
    assert(lFields.length() > 0);
    if (lFields.length() > 0) {
        lFields = lFields.substr(0, lFields.length() - 1);
    }
    
    string lSqlString = "SELECT " + lFields + " FROM " + pTableName;
    if (lConditions.size() > 0) {
        lSqlString = lSqlString + " WHERE " + lConditions;
    }
    return this->query(lSqlString);
}

/*!
 @function
 @abstract   insert a new record
 @discussion 
 @param
 values:
 colNames:
 tableName:
 @result     
 */
int Database::insert(string pFields, string pValues, string pTableName)//insert a new record，pValues，pFields must be SQL format
{
    string lSqlString = "INSERT INTO " + pTableName + "(" + pFields + ") " + "VALUES(" + pValues + ")";
    return this->insert(lSqlString);
}

int Database::insert(map<string, string> pFieldAndValues, string pTableName)
{
    string lFields;
    string lValues;
    map<string, string>::iterator lIterator;
    for(lIterator = pFieldAndValues.begin(); lIterator != pFieldAndValues.end(); lIterator++)
    {
        assert(lIterator->first.length() > 0);
        if (lIterator->first.length() > 0) {//query fileds must > 0
            lFields = lFields + "'" + lIterator->first + "',";
            lValues = lValues + "'" + lIterator->second + "',";
        }
    }
    assert(lFields.length() > 0);
    if (lFields.length() > 0) {
        lFields = lFields.substr(0, lFields.length() - 1);
    }
    if (lValues.length() > 0) {
        lValues = lValues.substr(0, lValues.length() - 1);
    }
    return this->insert(lFields, lValues, pTableName);
}

int Database::insert(string pInsertSql)//insert one record via SQL
{
    sqlite3_stmt *lStmt = NULL;
    assert(_database);
    if (SQLITE_OK != sqlite3_prepare(_database, pInsertSql.c_str(), -1, &lStmt, NULL)) {
        cout << "Database::insert(string pInsertSql) sqlite3_prepare error, sql string " + pInsertSql << endl;
        return -1;
    }
    if (SQLITE_DONE != sqlite3_step(lStmt)) {
        cout << "Insert new values error, sql string " + pInsertSql << endl;
        return -1;
    }
    sqlite3_finalize(lStmt);
    return sqlite3_last_insert_rowid(_database);
}

/*!
 @function
 @abstract   delete a record
 @discussion 
 @param
 aRowID 
 tableName 
 @result     
 */
bool Database::remove(int pRowID, string pTableName)
{
    ostringstream lOs;
    lOs << pRowID;
    string lConditions = "ROWID = " + lOs.str();
    lOs.str("");
    return this->remove(lConditions, pTableName);
}

/*!
 @function
 @abstract   delete a record
 @discussion 
 @param
 conditions 
 tablename 
 @result    
 */
bool Database::remove(string pConditions, string pTableName)//delete a record, pConditions must be SQL format
{
    sqlite3_stmt *lStmt = NULL;
    string lSqlString = "DELETE FROM " + pTableName + " WHERE " + pConditions;
    assert(_database);
    if (SQLITE_OK != sqlite3_prepare(_database, lSqlString.c_str(), -1, &lStmt, NULL)) {
        cout << "removeRows:withTableName:.sqlite3_prepare error, sql string " + lSqlString << endl;
        return false;
    }
    if (SQLITE_DONE != sqlite3_step(lStmt)) {
        cout << "DELETE  values with conditions error, sql string " + lSqlString << endl;
        return false;
    }
    
    sqlite3_finalize(lStmt);
    
    return true;
}

/*!
 @function
 @abstract   update a record
 @discussion 
 @param
 conNamesandValues 
 conditions 
 @result     
 */
bool Database::update(int pRowID, string pFieldAndValues, string pTableName)//pFieldAndValues must be SQL format
{
    ostringstream lOs;
    lOs << pRowID;
    string lConditions = "ROWID = " + lOs.str();
    lOs.str("");
    return this->update(lConditions, pFieldAndValues, pTableName);
}

bool Database::update(string pSql){
    sqlite3_stmt *lStmt = NULL;
    assert(_database);
    if (SQLITE_OK != sqlite3_prepare(_database, pSql.c_str(), -1, &lStmt, NULL)) {
        cout << "updateRows:withConditions:withTableName:.sqlite3_prepare error, sql string " + pSql << endl;
        return false;
    }
    if (SQLITE_DONE != sqlite3_step(lStmt)) {
        cout << "update  values with conditions error, sql string " + pSql << endl;
        return false;
    }
    sqlite3_finalize(lStmt);
    
    return true;
}

/*!
 @function
 @abstract   
 @discussion 
 @param
 conNamesandValues 
 conditions 
 @result    
 */
bool Database::update(string pConditions, string pFieldAndValues, string pTableName)
{
    
    string lSqlString = "UPDATE " + pTableName + " SET " + pFieldAndValues + " WHERE " + pConditions;
    assert(_database);
    return this->update(lSqlString);
}

/*!
 @function
 @abstract   
 @discussion 
 @param
 conNamesandValues
 conditions
 FieldAndValues
 @result
 */
bool Database::update(string pConditions, map<string, string>pFieldAndValues, string pTableName)
{
    string lFieldAndValues;
    map<string, string>::iterator lIterator;
    for(lIterator = pFieldAndValues.begin(); lIterator != pFieldAndValues.end(); lIterator++)
    {
        assert(lIterator->first.length() > 0);
        if (lIterator->first.length() > 0) {//filed must > 0
            lFieldAndValues = lFieldAndValues + lIterator->first + "='" + lIterator->second + "',";
        }
    }
    assert(lFieldAndValues.length() > 0);
    if (lFieldAndValues.length() > 0) {
        lFieldAndValues = lFieldAndValues.substr(0, lFieldAndValues.length() - 1);
    }
    
    return this->update(pConditions, lFieldAndValues, pTableName);
}

bool Database::update(map<string, string> pConditions, map<string, string>pFieldAndValues, string pTableName)
{
    string lFieldAndValues;
    map<string, string>::iterator lIterator;
    for(lIterator = pFieldAndValues.begin(); lIterator != pFieldAndValues.end(); lIterator++)
    {
        assert(lIterator->first.length() > 0);
        if (lIterator->first.length() > 0) {//filed must > 0
            lFieldAndValues = lFieldAndValues + lIterator->first + "='" + lIterator->second + "',";
        }
    }
    assert(lFieldAndValues.length() > 0);
    if (lFieldAndValues.length() > 0) {
        lFieldAndValues = lFieldAndValues.substr(0, lFieldAndValues.length() - 1);
    }
    
    
    
    string lConditions;
    for(lIterator = pConditions.begin(); lIterator != pConditions.end(); lIterator++)
    {
        assert(lIterator->first.length() > 0);
        if (lIterator->first.length() > 0) {//query fileds must > 0
            lConditions = lConditions + lIterator->first + "='" + lIterator->second + "' AND ";
        }
    }
    //assert(lConditions.length() > 0);
    if (lConditions.length() > 0) {
        lConditions = lConditions.substr(0, lConditions.length() - 5);
    }
    
    return this->update(lConditions, lFieldAndValues, pTableName);
}

bool Database::update(int pRowID, map<string, string>pFieldAndValues, string pTableName)
{
    ostringstream lOs;
    lOs << pRowID;
    string lConditions = "ROWID = " + lOs.str();
    lOs.str("");
    return this->update(lConditions, pFieldAndValues, pTableName);
}
