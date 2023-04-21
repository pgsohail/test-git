// header file for the lexer module

#define TEST		// uncomment this line to run the self-grader

// the TokenType enumerated data type represents all possible token types in a JACK program, ERR is used to indicate a lexical error
typedef enum {RESWORD, ID , INT , SYMBOL, STRING, EOFile, ERR} TokenType;
// the LexErrCodes enumerated data type represent error codes for all possible lexical errors that can occur during tokenisation
// EofInCom: End of file in comment
// NewLnInStr: New line in string literal
// EofInStr: End of file in string literal
// IllSym: Illegal symbol in source file
typedef enum {EofInCom , NewLnInStr , EofInStr , IllSym} LexErrCodes;

// a structure for representing tokens
typedef struct {
  TokenType tp;		// the type of this token, e.g. INT
  char lx[128];		// the lexeme (string) of the token, e.g. "34". If the lexer encounters an error this string is set to an error message
  int ec;			// If the lexer encounters an error this value is set to the proper error code (see the above enumerated list of errors)
  int ln;			// the line number of the source file where the token exists
  char fl[32];		// the file (name) in which this token exists
} Token;


int InitLexer (char* file);
Token GetNextToken ();
Token PeekNextToken ();
int StopLexer ();
