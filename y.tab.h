/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.
   
   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     INTEGER = 258,
     DECIMAL = 259,
     WORD = 260,
     FLAG = 261,
     EOLN = 262,
     LS = 263,
     CD = 264,
     EXIT = 265,
     PATH = 266,
     SETENV = 267,
     HOME_PATH = 268,
     HOME = 269,
     ROOT = 270,
     PIPE = 271,
     QUOTE = 272,
     OPEN_CARAT = 273,
     CLOSE_CARAT = 274,
     BACKSLASH = 275,
     AMPERSAND = 276,
     PLUS = 277,
     SEMICOLON = 278,
     OPEN_PAREN = 279,
     CLOSE_PAREN = 280,
     TWO_PERIODS = 281,
     UNSETENV = 282,
     PRINTENV = 283
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define DECIMAL 259
#define WORD 260
#define FLAG 261
#define EOLN 262
#define LS 263
#define CD 264
#define EXIT 265
#define PATH 266
#define SETENV 267
#define HOME_PATH 268
#define HOME 269
#define ROOT 270
#define PIPE 271
#define QUOTE 272
#define OPEN_CARAT 273
#define CLOSE_CARAT 274
#define BACKSLASH 275
#define AMPERSAND 276
#define PLUS 277
#define SEMICOLON 278
#define OPEN_PAREN 279
#define CLOSE_PAREN 280
#define TWO_PERIODS 281
#define UNSETENV 282
#define PRINTENV 283




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


