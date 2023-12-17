//
//  FileDescriptor.cpp
//  Compiler
//
//  Created by MOHEE QWAREEQ on 09/08/2023.
//

#include "FileDescriptor.h"


FileDescriptor::FileDescriptor(string fileName){
    file.open(fileName,ios::in);
    if (file.is_open()){
        this->fileName = fileName;
        cout<<"Start Runing ... "+fileName+"\n"<<endl;
        if (getline(file, buffer)){
            buffer = buffer + "\n";
            lineNumber = SET;
            charNumber = UNSET;
        }
    }
    else
        cout<<"File not open \n";
}


string FileDescriptor::getFileName(){
    return fileName;
}


bool FileDescriptor::isOpen(){
    if (file)
        return true;
    return false;
}


int FileDescriptor::getLineNum(){
    return lineNumber;
}

int FileDescriptor::getCharNum(){
    return charNumber;
}

void FileDescriptor::close(){
    file.close();
}

char FileDescriptor::getChar (){
    const char * line = buffer.c_str();
    char curentChar = line[charNumber];
    if(charNumber < strlen(line))
        charNumber++;
    
    else {
        if (getline(file, buffer)){
            buffer = buffer+"\n";
            lineNumber++;
            charNumber = UNSET;
        }
        
        else {
            buffer += EOF;
            curentChar = EOF;
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
    cout<<" of " + fileName<<endl;
    cout<<endl;
}

void FileDescriptor::reportWarning(string msg){
    cout <<getCurrLine();
    for (int i = 0; i < charNumber-3; i++) cout << " ";
    cout << "^"<< endl;
    cout<< "Warnning: " + msg + " on line ";
    cout<<getLineNum();
    cout<<" of " + fileName<<endl;
    cout<<endl;
}

FileDescriptor::~FileDescriptor(){
    file.close();
}
