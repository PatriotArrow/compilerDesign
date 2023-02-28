//test string
//
#include <stdio.h>

#include <ctype.h>

#include <string.h>

#include <stdbool.h>

#include <stdlib.h>


#define MAX_IDENTIFIER_LENGTH 31
#define MAX_NUMBER_LENGTH 31
#define MAX_STRING_LENGTH 100

typedef enum {
    TOKEN_IDENTIFIER,
    TOKEN_KEYWORD,
    TOKEN_STRING,
    TOKEN_NUMBER,
    TOKEN_PUNCTUATOR,
    TOKEN_ERROR,
    TOKEN_EOF
}
TokenType;

typedef struct {
    TokenType type;
    char * attribute;
}
Token;

bool isKeyword(char * str) {
    // list of keywords
    char* keywords[] =
    {
        "auto", "break", "case", "char", "const", "continue","default", "do", "double", 
        "else", "enum", "extern","float", "for", "goto", "if", "int", "long", "register",
        "return", "short", "signed", "sizeof", "static", "struct","switch", "typedef",
        "union", "unsigned", "void", "volatile","while","include","define"
    };

    //size of pointer array / size of a pointer = number of pointers ;
    //int numKeywords = sizeof(keywords) / sizeof(keywords[0]);
    int numKeywords = div(sizeof(keywords), sizeof(keywords[0])).quot;

    for (int i = 0; i < numKeywords; i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return true;
        }
    }
    return false;
}

void skip_single_line_comments(FILE * fp) {
    char c;
    do {
        c = fgetc(fp);

    } while (c != '\n');

}

void skip_comments(FILE * fp) {
    char c;
    printf("test2");
    while (1) {
        printf("test");
        c = fgetc(fp);
        if (c == '*') {
            c = fgetc(fp);
            if (c == '/')
                break;
            else
                ungetc(c, fp);
        }
    }
}

void strip_white_and_comment(char c, FILE * fp) {
    while (isspace(c)) {
        c = fgetc(fp);
    }

    if (c == '/') {
        c = fgetc(fp);
        //printf("%c\n",c);
        if (c == '/') {
            skip_single_line_comments(fp);
            c = fgetc(fp);
            strip_white_and_comment(c, fp);
        } else if (c == '*') {
            skip_comments(fp);
            c = fgetc(fp);
            strip_white_and_comment(c, fp);
        } else {
            ungetc(c, fp);
            return;
        }
    }

}
Token read_string(FILE * fp) {
    Token token;
    char string[MAX_STRING_LENGTH];
    int i = 0;

    char c = fgetc(fp);

    while (c != 34) {
        string[i++] = c;
        c = fgetc(fp);
    }
    string[i] = '\0';

    token.type = TOKEN_STRING;
    token.attribute = strdup(string);
    return token;

}

Token read_identifier(char c, FILE * fp) {
    Token token;
    char identifier[MAX_IDENTIFIER_LENGTH];
    int i = 0;
    identifier[i++] = c;

    c = fgetc(fp);

    while (isalnum(c) || c == '_') {
        //if i < max_id_length ?
        identifier[i++] = c;
        c = fgetc(fp);
    }
    ungetc(c, fp);

    identifier[i] = '\0';
    if (isKeyword(identifier)) {
        token.type = TOKEN_KEYWORD;
    } else {
        token.type = TOKEN_IDENTIFIER;
    }

    token.attribute = strdup(identifier);

    return token;
}

Token read_number(char c, FILE * fp) {
    Token token;
    char number[MAX_NUMBER_LENGTH];
    int i = 0;
    number[i++] = c;

    c = fgetc(fp);
    while (isdigit(c)) {
        //if i < max_number_length ?
        number[i++] = c;
        c = fgetc(fp);

    }

    ungetc(c, fp);

    number[i] = '\0';
    token.type = TOKEN_NUMBER;
    token.attribute = strdup(number);
    return token;
}

//  skip over whitespace and comments
void skip_whitespace_and_comments(FILE * fp) {
    char c = fgetc(fp);
    while (isspace(c) || c == '/') {
        if (c == '/') {
            c = fgetc(fp);
            if (c == '/') {
                // single-line comment
                while ((c = fgetc(fp)) != '\n' && c != EOF);
            } else if (c == '*') {
                // multi-line comment
                int prev_c = '\0';
                while ((c = fgetc(fp)) != EOF) {
                    if (prev_c == '*' && c == '/') {
                        break;
                    }
                    prev_c = c;
                }
            } else {
                ungetc(c, fp);
                break;
            }
        } else {
            c = fgetc(fp);
        }
    }
    ungetc(c, fp);
}

//comments
Token read_token(FILE * fp) {
    Token token;
    //char c = fgetc(fp);    
    //strip_white_and_comment(c,fp);
    skip_whitespace_and_comments(fp);
    char c = fgetc(fp);
    if (c == EOF) {
        token.type = TOKEN_EOF;
        token.attribute = "";
        return token;
    }

    if (ispunct(c)) {
        if (c == 34) {
            token = read_string(fp);
            return token;
        } else {
            token.type = TOKEN_PUNCTUATOR;
            char char_to_string[] = {
                c,
                '\0'
            };
            token.attribute = strdup(char_to_string);
            return token;
        }
    } else
        switch (c) {
        case '/':
            token.type = TOKEN_PUNCTUATOR;
            token.attribute = "/";
            break;

        case '\n':
            printf("NEWLINEFOUND\n");
            token.type = TOKEN_PUNCTUATOR;
            token.attribute = "";
            break;
        case EOF:
            token.type = TOKEN_EOF;
            token.attribute = "";
            break;
        default:
            if (isalpha(c) || c == '_') {
                token = read_identifier(c, fp);
            } else if (isdigit(c)) {

                token = read_number(c, fp);
            } else {
                token.type = TOKEN_ERROR;
                token.attribute = "Invalid token";
                printf("%c\n", c);

            }
            break;
        }
    return token;
}

void print_token(Token token, FILE * fp) {
    switch (token.type) {
    case TOKEN_IDENTIFIER:
        fprintf(fp, "IDENTIFIER %s\n", token.attribute);
        break;
    case TOKEN_KEYWORD:
        fprintf(fp, "KEYWORD %s\n", token.attribute);
        break;
    case TOKEN_NUMBER:
        fprintf(fp, "NUMBER %s\n", token.attribute);
        break;
    case TOKEN_STRING:
        fprintf(fp, "STRING %s\n", token.attribute);
        break;
    case TOKEN_ERROR:
        fprintf(fp, "ERROR %s\n", token.attribute);
        break;
    case TOKEN_EOF:
        fprintf(fp, "EOF\n");
        break;
    default:
        fprintf(fp, "%s\n", token.attribute);
        break;

    }
}

void main() {
    FILE * source_fp = fopen("test.c", "r");
    FILE * output_file = fopen("tokens2.txt", "w");

    Token token;
    do {
        token = read_token(source_fp);
        //printf("%s\n" , token.attribute);
        print_token(token, output_file);
    } while (token.type != TOKEN_EOF);

}