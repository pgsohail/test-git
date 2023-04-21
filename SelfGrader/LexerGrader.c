// CAutoGrader.cpp : Defines the entry point for the console application.
//


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define JsonStrSize 5000
#define Presubmission 1
char* JsonStr;

// remove this before releasing template
#define NumberTestFiles 15


Token tokenArray[2024];
int numberTokens;

char* TokenTypeString (TokenType t);
// a list of files to be used in testing the lexer
char* testFiles[NumberTestFiles] = {"IdentifiersOnly.jack" , "KeywordsOnly.jack" , "IntegersOnly.jack", "Main.jack", "Ball.jack" , "Fraction.jack", "List.jack", "Square.jack" , "SquareGame.jack", "Empty.jack", "OnlyComments.jack", "EofInComment.jack", "EofInStr.jack", "NewLineInStr.jack", "IllegalSymbol.jack"};


int InitGraderString ()
{
	JsonStr = (char *) malloc (sizeof (char) * JsonStrSize);
	strcpy (JsonStr, "{\n");   // let's start, bismillah
	strcat (JsonStr, "\t\"output\": \"Graded by CAutoGrader\",\n");
	strcat (JsonStr, "\t\"tests\":\n");  // let's start the tests
	strcat (JsonStr, "\t[\n");
	return 1;
}

int AddTestString (int score, int max_score, char* output, int is_final)
{
	char temp[320];

	strcat (JsonStr , "\t\t{\n");
	sprintf (temp, "%i", score);
	strcat (JsonStr , "\t\t\t\"score\": ");
	strcat (JsonStr , temp);
	strcat (JsonStr , ",\n");

	sprintf (temp, "%i", max_score);
	strcat (JsonStr , "\t\t\t\"max_score\": ");
	strcat (JsonStr , temp);
	strcat (JsonStr , ",\n");


	strcat (JsonStr , "\t\t\t\"output\": ");
	strcat (JsonStr , "\"");
	strcat (JsonStr , output);
	strcat (JsonStr , "\"");
	strcat (JsonStr , "\n");

	strcat (JsonStr , "\t\t}");
	if (!is_final)
		strcat (JsonStr, ",");
	strcat (JsonStr, "\n");

	return 1;
}

int CloseGraderString ()
{
	strcat (JsonStr, "	]\n"); // end of tests
	strcat (JsonStr, "}\n");
	return 1;
}

char* TokenTypeString (TokenType t)
{
	switch (t)
	{
		case RESWORD: return "RESWORD";
		case ID: return "ID";
		case INT: return "INT";
		case SYMBOL: return "SYMBOL";
		case STRING: return "STRING";
		case EOFile: return "EOFile";
		case ERR: return "ERR";
		default: return "Not a recognised token type";
	}

}

void PrintToken (Token t)
{
	printf ("<%s, %i, %s, %s>\n", t.fl, t.ln , t.lx, TokenTypeString (t.tp));
}


// test the lexer
int t_lexer ()
{
	int r;
	int m=20;
	char s[100];
	int fail = 0;
	char binFileName[128];

	printf ("Calling your InitLexer function for a non-existent file (zeezo.jack)\n");
	r = InitLexer ("zeezo.jack");
	if (r != 0)
	{
		printf ("function should have returned 0; 1 mark deducted\n");
		m--;
	}
	printf ("Calling your InitLexer function for an existing file (Main.jack)\n");
	r = InitLexer ("Main.jack");
	if (r != 1)
	{
		printf ("function should have returned 1; 1 mark deducted\n");
		m--;
	}
	printf ("\nTesting your GetNextToken function on various source files (1 mark each)\n");
	for (int j = 0 ; j < NumberTestFiles ; j++) // for each test file
	{
		// create bin file name
		strcpy (binFileName , testFiles[j]);
		strcat (binFileName , "_tokens.bin");
		// load the tokens from my file
		printf ("\nloading tokens from %s file\n", binFileName);
		FILE* tfile = fopen (binFileName, "rb");
		if (tfile == 0)
		{
			printf ("Fatal grader error: could not open tokens' file %s , will quit, good bye\n", binFileName );
			system ("pause");
			exit (0);
		}
		fread (&numberTokens , sizeof (int), 1, tfile);
		fread (tokenArray, sizeof (Token), numberTokens , tfile);
		fclose (tfile);
		printf ("There are %i tokens in this file\n", numberTokens);
		printf ("Calling your InitLexer function for file %s \n", testFiles[j]);
		InitLexer (testFiles[j]);
		printf ("Checking that your GetNextToken function does return the correct tokens\n");
		Token t = GetNextToken ();
		int i = 0;
		while (t.tp != EOFile && i < numberTokens)
		{
			if ( (t.tp != tokenArray[i].tp) || (t.ln != tokenArray[i].ln) || (strcmp (t.lx , tokenArray[i].lx)))
			{
				printf ("Error in token\n");
				printf ("Your function returned this token:\n");
				PrintToken (t);
				printf ("Actual token should be:\n");
				PrintToken (tokenArray[i]);
				m--;
				break;
			}
			i++;
			t  = GetNextToken();
		}
		if (t.tp == EOFile && i < numberTokens && tokenArray[i].tp != EOFile) // function returned EOFile early (there are more tokens in the file)
		{
			printf ("Error in token\n");
			printf ("Your function returned this token:\n");
			PrintToken (t);
			printf ("Actual token should be:\n");
			PrintToken (tokenArray[i]);
			m--;
		}
		printf ("Calling your StopLexer function\n");
		StopLexer ();
	}

	printf ("\nTesting your PeekNextToken function using file Main.jack (3 marks)\n");
	printf ("Repeatedly calling PeekNextToken followed by GetNextToken. They should return the same token\n");
	InitLexer ("Main.jack");
	Token q = PeekNextToken ();
	Token t = GetNextToken ();
	while (t.tp != EOFile)
	{
		if ( (t.tp != q.tp) || (t.ln != q.ln) || (strcmp (t.lx , q.lx)))
		{
			printf ("Error in token\n");
			printf ("PeekNextToken returned this token:\n");
			PrintToken (q);
			printf ("GetNextToken returned this token:\n");
			PrintToken (t);
			m=m-3;
			break;
		}
		q = PeekNextToken ();
		t = GetNextToken();
	}
	StopLexer ();

	sprintf (s,"%i/20 for the lexer", m);
	if (!Presubmission)
		AddTestString (m, 20, s, 1);
	return m;
}


#ifdef TEST
int main (int argc, char* argv[])
{
	FILE* jsonFile;
	int tot = 0;


	if (!Presubmission)
		InitGraderString ();

	printf ("\t$$$ Checking your lexer, behold $$$\n");
	printf ("\t=========================================\n");
	printf ("Started ...\n");

	tot += t_lexer ();

	//if (Presubmission)
	//{
		printf ("\n---------------------------------------------------\n");
		printf ("\t\tTotal mark = %i/20\n", tot);
		printf ("---------------------------------------------------\n\n");
	//}
	printf ("Finished\n");


	if (!Presubmission)  // create the results.json file
	{
		CloseGraderString ();
		//printf ("%s",JsonStr);
		jsonFile = fopen ("/autograder/results/results.json", "w");
		fprintf (jsonFile, "%s", JsonStr);
		fclose (jsonFile);
	}




	return 0;
}
#endif
