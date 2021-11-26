#include "lexer.h"

#include <err.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

// Struct that allows us to avoid a huge switch
static struct token token_pairs[] = 
{
    {TOKEN_IF, "if"},
    {TOKEN_THEN, "then"},
    {TOKEN_ELIF, "elif"},
    {TOKEN_ELSE, "else"},
    {TOKEN_FI, "fi"},
    {TOKEN_SEMICOL, ";"},
    {TOKEN_PIPE, "|"},
    {TOKEN_BACKN, "\n"},
    {TOKEN_EOF, "\0"},
    {TOKEN_UNKNOWN, NULL},
};

struct lexer *lexer_new(const char *input)
{
    struct lexer *new = malloc(sizeof(struct lexer));
    if (!new)
        errx(EXIT_FAILURE, "Lexer new error");

    new->input = input;
    new->pos = 0;
    new->current_tok = NULL;
        
    return new;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer->current_tok)
        token_free(lexer->current_tok);

    free(lexer);
}

int is_delimitor(char c)
{
    if (c == '\0' || c == ' ' || c == '\t' || c == '\n' || c == ';')
        return 1;
    return 0;
}

size_t get_len_next_token(const char *input)
{
    size_t len = 0;
    while (!is_delimitor(input[len]))
        len++;

    // If we are on a ; or \n, we need return 1
    if (len == 0 && (input[len] == ';' || input[len] == '\n'))
        len++;
    return len;
}

// Find the token corresponding to the currect str in input
struct token *fill_token(const char *input)
{
    size_t tok_len = get_len_next_token(input);
    char *str_token = calloc(tok_len + 1, sizeof(char));
    str_token = strncpy(str_token, input, tok_len);

    for (int i = 0; token_pairs[i].value; i++)
    {
        if (!strcmp(str_token, token_pairs[i].value))
        {
            free(str_token);
            return token_new(token_pairs[i].type);
        }
    }

    // Quote case
    if (str_token[0] == '\'')
    {
        struct token *t = token_new(TOKEN_SQUOTE);
        char *str_token_quote = calloc(tok_len - 1, sizeof(char));
        str_token_quote = strncpy(str_token_quote, input + 1, tok_len - 2);
        t->value = str_token_quote;
        free(str_token);
        return t;
    }

    // Word case
    struct token *t = token_new(TOKEN_WORD);
    t->value = str_token;
    free(str_token);
    return t;
}

// return curr token
struct token *lexer_peek(struct lexer *lexer)
{
    if (lexer->current_tok)
    {
        return lexer->current_tok;
    }

    struct token *new = fill_token(lexer->input + lexer->pos);
    lexer->current_tok = new;
    return new;
}

// incr pos and return token
struct token *lexer_pop(struct lexer *lexer)
{
    // If we are on a \n or ;, we need to do +1 in order to let the while skip the spaces
    if (lexer->input[lexer->pos] == '\n' || lexer->input[lexer->pos] == ';')
        lexer->pos++;

    while(!is_delimitor(lexer->input[lexer->pos]))
        lexer->pos++;

    if (lexer->input[lexer->pos] != '\0' && lexer->input[lexer->pos] != ';' && lexer->input[lexer->pos] != '\n')
        lexer->pos++;

    struct token *t = lexer->current_tok;
    lexer->current_tok = NULL;
    return t;
}
