//
//  FileDescriptor.cpp
//  Scanner
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#include "FileDescriptor.h"

FileDescriptor::FileDescriptor(string fileName){
    streamFile.open(fileName,ios::in);
    if (streamFile.is_open()){
        file=fileName;
        cout<<"Start Runing ... "+file+"\n"<<endl;
        if (getline(streamFile, buffer)){
            buffer=buffer+"\n";
            lineNumber=SET;
            charNumber=UNSET;}
    }
    else cout<<"File not open \n";
}


string FileDescriptor::getFileName(){
    return file;
}


bool FileDescriptor::isOpen(){
    if (streamFile) return true;
    return false;
}


int FileDescriptor::getLineNum(){
    return lineNumber;
}

int FileDescriptor::getCharNum(){
    return charNumber;
}

void FileDescriptor::close(){
    streamFile.close();
}

char FileDescriptor::getChar (){
    const char * line = buffer.c_str();
    char curentChar=line[charNumber];
    if(charNumber < strlen(line))
        charNumber++;
    
    else {
        if (getline(streamFile, buffer)){
            buffer=buffer+"\n";
            lineNumber++;
            charNumber=UNSET;
        }
        
        else {
            buffer += EOF;
            curentChar=EOF;
        }
    }
    if(!curentChar )return getChar();
    return curentChar;
}


string FileDescriptor::getCurrLine(){
    return buffer;
}

void FileDescriptor::ungetChar (){
    charNumber--;
}

void FileDescriptor::reportError (string msg){
    cout <<getCurrLine();
    for (int i = 0; i < charNumber-3; i++) cout << " ";
    cout << "^"<< endl;
    cout<<msg+" on line ";
    cout<<getLineNum();
    cout<<" of "+file<<endl;
    cout<<endl;
}

void FileDescriptor::reportWarning(string msg){
    cout <<getCurrLine();
    for (int i = 0; i < charNumber-3; i++) cout << " ";
    cout << "^"<< endl;
    cout<<"Warnning: "+msg+" on line ";
    cout<<getLineNum();
    cout<<" of "+file<<endl;
    cout<<endl;
}

FileDescriptor::~FileDescriptor(){
    close();
}
