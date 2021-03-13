#include "9cc.h"

void error(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

static char* current_input;
static void verror_at(char* loc, char* fmt, va_list ap) {
    int pos = loc - current_input;
    fprintf(stderr, "%s\n", current_input);
    fprintf(stderr, "%*s", pos, "");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(EXIT_FAILURE);
}

void error_at(char* loc, char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verror_at(loc, fmt, ap);
}

void error_tok(Token* tok, char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    verror_at(tok->loc, fmt, ap);
}

bool equal(Token* tok, char* op) {
    return memcmp(tok->loc, op, tok->len) == 0 && op[tok->len] == '\0';
}

Token* skip(Token* tok, char* s) {
    if (!equal(tok, s)) {
        error_tok(tok, "expected '%s'", s);
    }
    return tok->next;
}

static Token* new_token(TokenKind kind, char* start, char* end) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind  = kind;
    tok->loc   = start;
    tok->len   = end - start;
    return tok;
}

static bool startswith(char* p, char* q) {
    return strncmp(p, q, strlen(q)) == 0;
}

static bool is_ident1(char c) {
    return ('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z') || c == '_';
}

static bool is_ident2(char c) {
    return is_ident1(c) || ('0' <= c && c <= '9');
}

static int read_punct(char* p) {
    if (startswith(p, "==") || startswith(p, "!=") || startswith(p, "<=") || startswith(p, ">=")) {
        return 2;
    }

    return ispunct(*p) ? 1 : 0;
}

static void convert_keyword(Token* tok) {
    for (Token* t = tok; t->kind != TK_EOF; t = t->next) {
        if (equal(t, "return")) {
            t->kind = TK_KEYWORD;
        }
    }
}

Token* tokenize(char* p) {
    current_input = p;
    Token head    = {};
    Token* cur    = &head;

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (isdigit(*p)) {
            cur->next = new_token(TK_NUM, p, p);
            cur       = cur->next;

            char* q  = p;
            cur->val = strtol(p, &p, 10);
            cur->len = q - p;
            continue;
        }

        if (is_ident1(*p)) {
            char* start = p;

            do {
                p++;
            } while (is_ident2(*p));

            cur->next = new_token(TK_IDENT, start, p);
            cur       = cur->next;
            continue;
        }

        int punct_len = read_punct(p);
        if (punct_len) {
            cur->next = new_token(TK_PUNCT, p, p + punct_len);
            cur       = cur->next;
            p += cur->len;
            continue;
        }

        error_at(p, "invalid token");
    }

    cur->next = new_token(TK_EOF, p, p);
    cur       = cur->next;

    convert_keyword(head.next);
    return head.next;
}