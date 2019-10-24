#pragma once
typedef struct Node{
        char data[5]; //mnemonic 
        char value[3];//the hexadecimal value

        char locctr[10]; //for symtab
        char lab[10]; //for symtab

        //pointers for double linked-list
        struct Node *next;
        struct Node *prev; 
} node;

typedef struct List{
        node *head;
        node *tail;
        int s;
} list;

typedef struct arr{
    char data[5]; //mnemonic 
    char value[3];//the hexadecimal value
} arr; 

static arr optable[] = {{"ADD", "18"},  {"RSUB", "4C"},
                {"AND", "58"},  {"STA", "0C"},
                {"COMP", "28"}, {"STCH", "54"},
                {"DIV", "24"},  {"STL", "14"},
                {"J", "3C"},    {"STX", "10"},
                {"JEQ", "30"},  {"SUB", "1C"},
                {"JGT", "34"},  {"TD", "E0"},
                {"JLT", "38"},  {"TIX", "2C"},
                {"JSUB", "48"}, {"WD", "DC"},
                {"LDA", "00"},  {"LDCH", "50"},
                {"LDL", "08"},  {"LDX", "04"},
                {"MUL", "20"},  {"OR", "44"},
                {"RD", "D8"}};

extern list*opHash;
extern list*symHash;

extern char prog_len[10];
extern int START_LOC;

void printList(int i);
void printHash();
unsigned int hash(char *s);
void insert(char *d, char *v);
void OPTAB(arr *opt);
bool search(char *x);
void insertSy(char *d, char*v);
bool search2(char *x);
int get_loc(char*x);
void printHash2(FILE*inter);
void split(char*str, char*lab, char*opc,char*oper);
void validLabel(char*lab, char*err);
void validOpr(char*oper, char*err);
void split_inter(char*str, char*loc, char*lab, char*opc, char*oper, char*err);
void get_val(char*opc, char*val);
void get_addr(char*oper, char*addr);
bool index_oper(char*oper);
void init_header(FILE*obj, char*oper, char *prog, char*start, char*length);
void begin_textR(FILE*obj, char *startaddr, char*object);
void dump1(int start, int end);
void load_prog(char*filen);