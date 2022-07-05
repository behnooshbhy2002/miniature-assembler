#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#include<stdbool.h>
#define NORTYPE 5
#define NOITYPE 8
#define NOJTYPE 2
#define RTYPE
#define ITYPE
#define JTYPE
struct symbolTable
{
    int value;
    char *symbol;
};
struct instruction
{
    size_t instType;
    int Instdecimal;
    int op;
    char Hex[9];
    int Binary[33];
    int rs;
    int rt;
    int rd;
    int imm;
};
int findSymbolTableLen(FILE *);
int fillSymTab(struct symbolTable * , FILE * ,FILE *);
int FindImmValue(char * , struct symbolTable* , int , FILE *);
int HexToInt(char *);
int IsNumber(char *);
void decimalToHexa(int , char *);
void IntToHex(struct instruction *);
void check_labels(int , struct symbolTable * , FILE *);
void Print_Binary(struct instruction *);
#endif // HEADER_H_INCLUDED
