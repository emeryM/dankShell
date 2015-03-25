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
     LS = 261,
     CD = 262,
     EXIT = 263,
     PIPE = 264,
     QUOTE = 265,
     OPEN_CARAT = 266,
     CLOSE_CARAT = 267,
     BACKSLASH = 268,
     AMPERSAND = 269,
     PLUS = 270,
     SEMICOLON = 271,
     OPEN_PAREN = 272,
     CLOSE_PAREN = 273,
     TWO_PERIODS = 274
   };
#endif
/* Tokens.  */
#define INTEGER 258
#define DECIMAL 259
#define WORD 260
#define LS 261
#define CD 262
#define EXIT 263
#define PIPE 264
#define QUOTE 265
#define OPEN_CARAT 266
#define CLOSE_CARAT 267
#define BACKSLASH 268
#define AMPERSAND 269
#define PLUS 270
#define SEMICOLON 271
#define OPEN_PAREN 272
#define CLOSE_PAREN 273
#define TWO_PERIODS 274




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


