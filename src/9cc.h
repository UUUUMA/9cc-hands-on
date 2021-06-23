#define _POSIX_C_SOURCE 200809L
#include <assert.h>
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * Tokenizer
 */

typedef enum {
    TK_IDENT,
    TK_PUNCT,
    TK_KEYWORD,
    TK_NUM,
    TK_EOF,
} TokenKind;

// Token
typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token* next;
    int val;
    char* loc;
    int len;
};

void error_at(char* loc, char* fmt, ...);
void error_tok(Token* tok, char* fmt, ...);
bool equal(Token* tok, char* op);
Token* skip(Token* tok, char* s);
bool consume(Token** rest, Token* tok, char* str);
Token* tokenize(char* p);

/*
 * Type
 */

typedef struct Type Type;
typedef struct Node Node;

typedef enum {
    TY_CHAR,
    TY_INT,
    TY_PTR,
    TY_FUNC,
    TY_ARRAY,
} TypeKind;

struct Type {
    TypeKind kind;

    // sizeof() value
    int size;

    Type* base;
    Token* name;

    int array_len;

    // For Function
    Type* return_ty;
    Type* params;
    Type* next;
};

extern Type* ty_char;
extern Type* ty_int;

bool is_integer(Type* ty);
Type* copy_type(Type* ty);
Type* pointer_to(Type* ty);
Type* func_type(Type* return_ty);
Type* array_of(Type* base, int size);
void add_type(Node* node);

/*
 * Parser
 */

typedef enum {
    ND_ADD,
    ND_SUB,
    ND_MUL,
    ND_DIV,
    ND_NEG,
    ND_EQ,
    ND_NE,
    ND_LT,
    ND_LE,
    ND_ASSIGN,
    ND_ADDR,
    ND_DEREF,
    ND_RETURN,
    ND_IF,
    ND_FOR,
    ND_BLOCK,
    ND_FUNCALL,
    ND_EXPR_STMT,
    ND_VAR,
    ND_NUM,
} NodeKind;

// Local variable
typedef struct Obj Obj;
struct Obj {
    Obj* next;
    char* name;
    Type* ty;
    bool is_local;

    // offset from BSP
    int offset;

    bool is_function;

    // Function
    Obj* params;
    Node* body;
    Obj* locals;
    int stack_size;
};

// AST node
struct Node {
    NodeKind kind;
    Node* next;
    Node* lhs;
    Node* rhs;
    Type* ty;

    // representative token
    Token* tok;

    // for "if" or "for" statement
    Node* cond;
    Node* then;
    Node* els;

    // for "for" statement
    Node* init;
    Node* inc;

    // for "block" statement
    Node* body;

    // for function call
    char* funcname;
    Node* args;

    // for variables
    Obj* var;

    // for integer
    int val;
};

Obj* parse(Token* tok);

/*
 * Code generator
 */

void codegen(Obj* prog);