//
//  FileDescriptor.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#ifndef FileDescriptor_h
#define FileDescriptor_h
#define SET 1
#define UNSET 0

#include <iostream>
#include <fstream>
using namespace std;

class FileDescriptor{
    
private:
    
    fstream file;
    int lineNumber;     /* line number in the file */
    int charNumber;     /* character number in the line */
    string buffer;        /* buffer to store a line */
    string fileName;         /* file name */
    
    
public:
    
    FileDescriptor(string);
    string getFileName();
    bool isOpen() ;
    int getLineNum();
    int getCharNum();
    void close ();
    char getChar ();
    string getCurrLine();
    void ungetChar ();
    void reportError (string);
    void reportWarning (string);
    ~FileDescriptor();
    
};


#endif /* FileDescriptor_h */
