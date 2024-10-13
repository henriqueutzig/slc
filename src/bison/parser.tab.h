/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

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
     TK_PR_INT = 258,
     TK_PR_FLOAT = 259,
     TK_PR_IF = 260,
     TK_PR_ELSE = 261,
     TK_PR_WHILE = 262,
     TK_PR_RETURN = 263,
     TK_OC_LE = 264,
     TK_OC_GE = 265,
     TK_OC_EQ = 266,
     TK_OC_NE = 267,
     TK_OC_AND = 268,
     TK_OC_OR = 269,
     TK_IDENTIFICADOR = 270,
     TK_LIT_INT = 271,
     TK_LIT_FLOAT = 272,
     TK_ERRO = 273
   };
#endif
/* Tokens.  */
#define TK_PR_INT 258
#define TK_PR_FLOAT 259
#define TK_PR_IF 260
#define TK_PR_ELSE 261
#define TK_PR_WHILE 262
#define TK_PR_RETURN 263
#define TK_OC_LE 264
#define TK_OC_GE 265
#define TK_OC_EQ 266
#define TK_OC_NE 267
#define TK_OC_AND 268
#define TK_OC_OR 269
#define TK_IDENTIFICADOR 270
#define TK_LIT_INT 271
#define TK_LIT_FLOAT 272
#define TK_ERRO 273




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif

extern YYSTYPE yylval;

