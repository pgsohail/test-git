/************************************************************************
University of Bradford

*************************************************************************/


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"


// YOU CAN ADD YOUR OWN FUNCTIONS, DECLARATIONS AND VARIABLES HERE

FILE* f;
char* Gfile_name;

const char* Keywords[21] = {"class", "constructor", "function", "method",
                          "field", "static", "var", "int", "char", "do",
                          "boolean", "void", "true", "false", "null",
                          "this", "let", "if", "else", "while", "return"}; //an array of Keywords

const char* Symbols[19] = {"(", ")", "{", "}", "[", "]", ".", ",", ";", "+",
                         "-", "*", "/", "&", "|", "<", ">", "=", "~"};

int isKeyword(char* a)
{
  int i;
  for (i = 0; i < 21; i++)
  {
    if (strcmp(a, Keywords[i]) == 0)
    {
      return 1; //keyword found
    }
  }
  return 0; //not a keyword
}

int isSymbol(char* b)
{
  int i;
  for (i = 0; i < 19; i++)
  {
    if (Symbols[i] == b)
    {
      return 1; //symbol found
    }
  }
  return 0; //not a symbol
}




// IMPLEMENT THE FOLLOWING functions
//***********************************

// Initialise the lexer to read from source file
// file_name is the name of the source file
// This requires opening the file and making any necessary initialisations of the lexer
// If an error occurs, the function should return 0
// if everything goes well the function should return 1
int InitLexer (char* file_name)
{
  f = fopen(file_name, "r");
  if (f == NULL)
  {
    printf("Error:can't open file\n");
    return 0;
  }
  Gfile_name = file_name;
  return 1; //success

}



// Get the next token from the source file
Token GetNextToken ()
{
	Token t;
  strcpy(t.fl, Gfile_name);

  int c;
  c = getc(f);

  if (c == EOF)
  {
    t.tp = EOFile;
    strcpy(t.lx, "End of file");
    return t;
  }

  if (isspace(c))
  {
    while (isspace(c = getc(f)))
    {}
    ungetc(c, f);
    return(GetNextToken(f));
  }

  if (c == '/')
  {
    c = getc(f);
    if(c == '*')
    {
      while ((c = getc(f)) != EOF)
      {
        if (c == '*')
        {
          if ((c = getc(f)) == '/')
          {
            return GetNextToken(f);
          }
          ungetc(c, f);
        }
        else if (c == '\n')
        {
          t.tp = ERR;
          strcpy(t.lx, "Error: unexpected eof in comment");
          return t;
        }
      }
    }
    else
    {
      ungetc(c, f);
      return t;
    }
  }

  if (c == '"')
  {
    while ((c = getc(f)) != EOF)
    {
      if (c == '\\')
      {
        c = getc(f);
      }
      else if(c == '\n')
      {
        t.tp = ERR;
        strcpy(t.lx, "Error: new line in string constant");
        t.ec = NewLnInStr;
        return t;
      }
      else if(c == '"')
      {
        return t;
      }
    }
    t.tp = ERR;
    strcpy(t.lx, "Error: unexpected eof in string constant");
    return t;
  }

  char temp[128];
  int i = 0;

  if (isalpha(c))
  {
    while (isalnum(c))
    {
      temp[i++] = c;
      c = getc(f);
    }

    temp[i] = '\0';

    if (isKeyword(temp) == 1)
    {
      t.tp = RESWORD;
      strcpy(t.lx, temp);
      return t;
    }

    else
    {
       t.tp = ID;
       strcpy(t.lx, temp);
       return t;
     }
  }
  else if (isdigit(c))
  {
    i = 0;
    while (isdigit(c))
    {
      temp[i++] = c;
      c = getc(f);
      t.tp = INT;
      strcpy(t.lx, temp);
      return t;
    }
  }

  else
  {

    //it is an illegal symbol

    if (isSymbol(temp) == 1)
    {
      t.tp = SYMBOL;
      strcpy(t.lx, temp);
      return t;
    }


    else
    {
      t.lx[0] = c;
      t.lx[1] = '\0';
      t.tp = ERR;
      strcpy(t.lx, "IllSym: Illegal symbol in source file");
      return t;
    }
  }

}

// peek (look) at the next token in the source file without removing it from the stream
Token PeekNextToken ()
{
  Token t;
  t.tp = ERR;
  return t;

}

// clean out at end, e.g. close files, free memory, ... etc
int StopLexer ()
{
  fclose(f);
	return 0;
}

// do not remove the next line
#ifndef TEST
int main ()
{
	// implement your main function here
  // NOTE: the autograder will not use your main function
  int r = InitLexer(file_name);
  if (r == 0)
  {
    return 0;
  }
  Token t;
  do
  {
    token = GetNextToken();
    if (t.tp == ERR)
    {
      printf("Error at line %d in file %s: %s\n", t.ln, t.fl, t.lx);
      break;
    }
    printf("%s\n", t.lx);
  } while (t.tp != EOFile);

}
// do not remove the next line
#endif

