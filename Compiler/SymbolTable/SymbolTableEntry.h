//
//  SymbolTableEntry.h
//  Symbol Table
//
//  Created by MOHEE QWAREEQ on 05/08/2023.
//

#ifndef SymbolTableEntry_h
#define SymbolTableEntry_h
#include <string>
#include <fstream>

#define TYPE_SIZE 5

using namespace std;

struct ste_list;

enum j_type{
    TYPE_INTEGER,
    TYPE_STRING,
    TYPE_BOOLEAN,
    TYPE_FLOAT,
    TYPE_NONE
} ;

enum ste_entry_type{
    STE_VAR,
    STE_CONST,
    STE_ROUTINE,
    STE_UNDEFINED,
} ;

static string type_names[] = {"integer", "string", "boolean", "float", "none"};
static string entry_type_names[] = { "var", "const", "routine", "undefined"};
static string str;

class SymbolTableEntry
{
public:
    string name;
    int offset;
    ste_entry_type type;
    SymbolTableEntry * next;
    SymbolTableEntry();
    SymbolTableEntry(string entryName, ste_entry_type Type, j_type jType = TYPE_NONE);
    SymbolTableEntry(string entryName, ste_entry_type Type, int constValue = 0);
    string toString();
    void print(ofstream &fout);
    j_type getType();
    static int ste_const_value(SymbolTableEntry* e);   /*Return the value of the constant recorded in entry e.*/
    static string ste_name(SymbolTableEntry* e);       /*Return the name of the object recorded in entry e*/
    static j_type ste_var_type(SymbolTableEntry* e);   /*Return the type of the variable recorded in entry e */
    
    union{
        struct{
            j_type type;
        } var;
        
        struct{
            int value;
        } constant;
        
        struct{
            ste_list * formals;
            j_type result_type;
        } routine;
        
    };
    
};

#endif /* SymbolTableEntry_h */
