#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE_FILENAME 100        /* max length of file name */
#define MAX_SIZE_IDENT    20         /* max length of identifier */
#define MAX_SIZE_NUMBER   9          /* max digits of number */
#define MAX_SIZE_LINE     100		 /* max number of a line's character in input file */
#define MAX_SIZE_TABLE    100        /* max size of symbol table */
#define MAX_SIZE_CODE     1000       /* max number of intermidiate code */
#define MAX_SIZE_STACK    10000      /* max size of data stack */
#define MAX_SIZE_BREAK    1000       /* max number of 'break' statement */
#define MAX_SIZE_CONTINUE 1000       /* max number of 'continue' statement */
#define MAX_SIZE_BACKUP   100        /* max number of backups */
#define FCTCODE_NUM       16         /* number of function code */
#define TERMINAL_SYM_NUM  54         /* number of terminal symbol */
#define RESERVE_WORD_NUM  25         /* number of reserved word */

/* terminal symbol */
enum symbol
{
	nul, ident, mainsym, charsym, intsym, ifsym,
	elsesym, whilesym, readsym, writesym, plus, minus,
	times, slash, lparen, rparen, lbracket, rbracket, 
	lbrace, rbrace, semic, eql, great, less,
	greateql, lesseql, eqleql, neql, intnum, dblsym,
	andsym, orsym, forsym, modsym, oddsym, xorsym,
	incsym, decsym, notsym, brksym, ctnsym, exitsym,
	swtcsym, cassym, colonsym, defausym, dosym, reptsym,
	untlsym, bolsym, truesym, falsesym, doublenum, cstsym
};

/* the kind of object stored in the symbol table */
enum objectKind
{
	intVar, constIntVar, intArray,
	doubleVar, constDoubleVar, doubleArray,
	charVar, constCharVar, charArray,
	boolVar, constBoolVar, boolArray
};

/* object stored in the symbol table */
struct tableObject
{
	char name[MAX_SIZE_IDENT + 1];
	enum objectKind kind;
	int offset;	/* offset relative to current activity record */
	int size1;	/* the space distributed to variable is size1*size2 */
	int size2;
	double value;  /* only used by constant */
};

/* function code */
enum fctCode
{
	lit, opr, lod, lod2, lod3, sto, sto2, sto3,
	cal, ini, jmp, jpc, add, add2, add3, tad
};

/* intermidiate code */
struct instruction
{
	enum fctCode fct; /* function code */
	int operand1;
	int operand2;
	double operand3;
};

/* object stored in data stack */
struct stackObject
{
	int dataType; /* data-type, 1: int, 2: double, 3: char, 4: bool */
	int intData;
	double dblData;
};

/* backup object */
struct BackUp
{
	FILE* backupFin;                          /* backup the fin */
	enum symbol backupSym;                    /* backup the sym */
	char backupCh;                            /* backup the ch */
	int backupPosCh;                          /* backup the posCh */
	char backupLineCache[MAX_SIZE_LINE];      /* backup the lineCache */
	int backupChNum;                          /* backup the chNum */
	double backupDoubleNum;                   /* backup the doubleNum */
	int backupIntNum;                         /* backup the intNum */
	char backupId[MAX_SIZE_IDENT + 1];        /* backup the id */
	int backupIterCode;                       /* in addition to above, iterCode also need to be backuped */
};

extern char fileName[MAX_SIZE_FILENAME + 1];     /* file name */

/* 
 * variables used in lexical analysis
 */
extern struct BackUp backups[MAX_SIZE_BACKUP];   /* backups of lexical analysing's status */
extern int iterBackUp;                           /* iterator of backups */
extern FILE* fin;                                /* input file */
extern enum symbol sym;                          /* current terminal symbol read by getSym() */
extern char ch;                                  /* current character read by getCh() */
extern int posCh;								 /* counter using by getCh() */
extern char lineCache[MAX_SIZE_LINE];            /* line cache using by getCh() */
extern int chNum;								 /* counter using by line cache */
extern double doubleNum;                         /* current floating number */
extern int intNum;                               /* current number */
extern char id[MAX_SIZE_IDENT + 1];              /* current identifier */
extern char* reserveWord[RESERVE_WORD_NUM];      /* reserved word */
extern enum symbol ssym[256];                    /* symbol-value of single character */
extern enum symbol wsym[RESERVE_WORD_NUM];       /* symbol-value corresponding to reserved word */

/* 
 * variables used in syntactical analysis 
 */
extern int continueList[MAX_SIZE_CONTINUE];      /* continue statement to be backfilled */
extern int iterCtnList;                          /* the iterator of continueList */
extern int breakList[MAX_SIZE_BREAK];            /* break statement to be backfilled */
extern int iterBreakList;                        /* the iterator of breakList */
extern char* fctCode_string[FCTCODE_NUM];        /* strings corresponding to function code */
extern struct instruction code[MAX_SIZE_CODE];   /* intermidiate code */
extern int iterCode;                             /* the iterator of code */
extern struct tableObject table[MAX_SIZE_TABLE]; /* symbol table */
extern int iterTable;                            /* the iterator of table */
extern int expressionNum;                        /* store the value of 'expression' in 'expressionNum' 
												  * if 'expression' is a number */

/* 
 * lexical analyzer 
 */
void getSym ();
void backup ();  /* backup the status of lexical analysing */
void recover (); /* recover the status of lexical analysing from backups */

/* 
 * syntactical analyzer
 * tip: if no error occurs, sym should be the first symbol when a syntactical analyzer starts, and
 * sym should be the following symbol at the end of analysing.
 */
void varDeclarationList (int* ptr_offset);
void varDeclarationStat (int* ptr_offset);
void constDeclarationList (int* ptr_offset);
void constDeclarationStat (int* ptr_offset);
void statementList ();
void statement ();
void type ();
void ifStat ();
void whileStat ();
void readStat ();
void writeStat ();
void compoundStat ();
void expressionStat ();
void variable (enum objectKind* ptr_kind, int* ptr_offset, int* ptr_size1, int* ptr_size2, int* ptr_IncOrDec);
void simpleVariable (enum objectKind* ptr_kind, int* ptr_offset, int* ptr_size1, int* ptr_size2);
void expression ();
void simpleValue ();
void term ();
void factor ();
void additiveExpr ();
void forStat ();
void valueExpr ();
void breakStat ();
void exitStat ();
void continueStat ();
void switchStat ();
void dowhileStat ();
void repeatStat ();
void enter (enum objectKind k, int offset, int size1, int size2, double value);	/* add a new entry to symbol table */
int findPosition (char* identName);		/* search the position of an identifier in symbol table */
void error (int error_code);			/* error handler */
void gen (enum fctCode fct, int operand1, int operand2, double operand3);	/* generate an intermidiate code */

/* 
 * interpret and execute intermidiate code 
 */
void Interpret();