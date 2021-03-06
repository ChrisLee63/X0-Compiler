#include "global.h"

char fileName[MAX_LEN_FILENAME + 1];     /* file name */
FILE* fin;                               /* file pointer of input file */

/* 
 * variables used in lexical analysis 
 */
BackUp backups[MAX_NUM_BACKUP];		     /* backups of lexical analysis's status */
int backUpNum = 0;                       /* number of backups */
Symbol sym;                              /* current terminal symbol read by ReadSymbol() */
char ch = ' ';                           /* current character read by ReadCh() */
int posCh;								 /* position of the character to be read in line cache */
char lineCache[MAX_NUM_LINE];            /* line cache using by ReadCh() */
int chNum;								 /* number of character in line cache */
double doubleNum;                        /* current floating number */
int intNum;                              /* current integer */
char id[MAX_LEN_IDENT + 1];              /* name of current identifier */

/* reserved word */
char reserveWord[RESERVE_WORD_NUM][MAX_LEN_RESERVE_WORD] = { "bool", "break", "case",
															 "char", "const", "continue",
															 "default", "do", "double",
															 "else", "exit", "false",
															 "for", "if", "int",
															 "main", "odd", "read",
															 "repeat", "return", "switch",
															 "true", "until", "void",
															 "while", "write", "xor" };

/* symbol-value corresponding to reserved word */
Symbol wsym[RESERVE_WORD_NUM] = { bolsym, brksym, cassym, charsym, cstsym, ctnsym, 
								  defausym, dosym, dblsym, elsesym, exitsym, falsesym,
								  forsym, ifsym, intsym, mainsym, oddsym, readsym, 
								  reptsym, retsym, swtcsym, truesym, untlsym, voidsym, 
								  whilesym, writesym, xorsym };


/* 
 * variables used in syntactical analysis 
 */
int continueList[MAX_NUM_CONTINUE];					      /* 'continue' statement to be backfilled */
int ctnNum = 0;									          /* number of 'continue' statement to be backfilled */
int breakList[MAX_NUM_BREAK];							  /* 'break' statement to be backfilled */
int brkNum = 0;									          /* number of 'break' statement to be backfilled */
Instruction code[MAX_NUM_CODE];						      /* intermediate code */
int codeNum = 0;										  /* number of intermediate code */
FunctionInfo fctInfo[MAX_NUM_FUNCTION];					  /* information of each function */
int fctNum = 0;										      /* number of function */

/* strings corresponding to function code */
char fctCodeString[FCTCODE_NUM][MAX_LEN_FCTCODE] = { "lit", "opr", "lod", "sto", "cal", 
													 "ini", "jmp", "jpc", "add", "sub", "tad" };