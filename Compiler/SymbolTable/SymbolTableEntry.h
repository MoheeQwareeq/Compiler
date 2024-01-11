//
//  SymbolTableEntry.h
//  Compiler
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#ifndef SymbolTableEntry_h
#define SymbolTableEntry_h

#include <string>
#include <fstream>
using namespace std;

#define TYPE_SIZE 5

struct SteList;

enum J_TYPE{
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_FLOAT,
    TYPE_NONE
} ;

enum STE_ENTRY_TYPE{
    STE_VAR,
    STE_CONST,
    STE_ROUTINE,
    STE_UNDEFINED,
} ;

const static string TYPE_NAMES[] = {"integer", "string", "boolean", "float", "none"};
const static string ENTRY_TYPE_NAMES[] = { "var", "const", "routine", "undefined"};

class SymbolTableEntry{
    
public:
    
    string name;
    int offset;
    STE_ENTRY_TYPE type;
    SymbolTableEntry * next;
    SymbolTableEntry();
    SymbolTableEntry(string, STE_ENTRY_TYPE,J_TYPE = TYPE_NONE, int = 0);
    string toString();
    void print(ofstream &);
    J_TYPE getType();
    static int steConstValue(SymbolTableEntry *);   /*Return the value of the constant recorded in entry e.*/
    static string steName(SymbolTableEntry *);       /*Return the name of the object recorded in entry e*/
    static J_TYPE steVarType(SymbolTableEntry * );   /*Return the type of the variable recorded in entry e */
    
    union{
        struct{
            J_TYPE type;
        }var;
        
        struct{
            int value;
        } constant;
        
        struct{
            SteList * formals;
            J_TYPE resultType;
        } routine;
        
    };
    
};

#endif /* SymbolTableEntry_h */
