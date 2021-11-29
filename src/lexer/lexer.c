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

struct lexer *lexer_new(char *input)
{
    struct lexer *new = malloc(sizeof(struct lexer));
    if (!new)
        errx(EXIT_FAILURE, "Lexer new error");

    new->input = input;
    new->current_tok = NULL;
        
    return new;
}

void lexer_free(struct lexer *lexer)
{
    if (lexer)
    {
        if (lexer->current_tok)
            token_free(lexer->current_tok);

        if (lexer->input)
            free(lexer->input);

        free(lexer);
    }
}

struct my_string
{
  char *str;
  int max_len;
  int len;
};

struct my_string *my_string_init(void)
{
    struct my_string *mystr = malloc(sizeof (struct my_string));
    if (!mystr)
        return NULL;

    mystr->max_len = 2;
    mystr->len = 0;
    mystr->str = malloc(mystr->max_len);
    mystr->str[0] = '\0';

    return mystr;
}

void my_string_resize(struct my_string *mystr)
{
    if (mystr == NULL)
        return;

    mystr->max_len *= 2;
    mystr->str = realloc(mystr->str, mystr->max_len);
}

void my_string_push(struct my_string *mystr, char c)
{
    if (!mystr)
        return;

    if (mystr->len + 1 == mystr->max_len)
        my_string_resize(mystr);

    mystr->str[mystr->len] = c;
    mystr->str[mystr->len + 1] = '\0';
    mystr->len++;
}

void my_string_free(struct my_string *mystr)
{
    free(mystr->str);
    free(mystr);
}

char *lexer_get_separator(char *str)
{
    if (*str == '(' || *str == ')' || *str == '{' || *str == '}' ||
            *str == '\n' || *str == ';' || *str == ' ' || *str == '\t' ||
            *str == '<' || *str == '>')
        return (str + 1);
    return str;
}

int get_special_token_end(char *str, char **end)
{
  int res = 0;
// TODO: Handle: quotes/arithmetics/vars/comments
  res = ((*end = lexer_get_separator(str)) != str);
  return res;
}

enum token_type token_get_type(char *token_str)
{
    for (size_t i = 0; token_pairs[i].value; i++)
    {
        if (!strcmp(token_pairs[i].value, token_str))
        {
            return token_pairs[i].type;
        }
    }
    return TOKEN_WORD;
}

struct token *lexer_tokenize(char *token_str)
{
    if (strlen(token_str) == 0 || *token_str == ' ' || *token_str == '\t') // TODO: == '#' ???
    {
        return NULL;
    }

    enum token_type type = token_get_type(token_str);
    struct token *t = token_new(type);

    t->value = malloc(sizeof(char) * (strlen(token_str) + 1));
    strcpy(t->value, token_str);

    return t;
}

char *lexer_make_string(char *begin, char *end)
{
    char *res = malloc(sizeof(char) * (end - begin + 1));
    char *pos = res;

    for (; begin != end; begin++)
    {
        *pos = *begin;
        pos++;
    }
    *pos = '\0';
    return res;
}

// Fill lexer_curr_token and return the pos of the beginning of the next tok
char *lexer_fill_current_tok(struct lexer *lexer)
{
    char *str = lexer->input;
    struct token *t = NULL;
    struct my_string *mystr = my_string_init();
    char *end_separator = NULL;
    int is_special;

    while (*str != '\0')
    {
        is_special = get_special_token_end(str, &end_separator);
        if (is_special && mystr->len == 0)
        {
            t = lexer_tokenize(lexer_make_string(str, end_separator));
            if (!t)
            {
                // We have a space, nothing or \t
                // just skip it until we have something interesting
                str++;
                continue;
            }
            else
            {
                // Copy from str to end_separator
                t = lexer_tokenize(lexer_make_string(str, end_separator));
                my_string_free(mystr);
                lexer->current_tok = t;
                return end_separator;
            }
        }
        else if (is_special && mystr->len != 0)
        {
            // end of curr token
            // create and return token
            t = lexer_tokenize(mystr->str);
            my_string_free(mystr);
            lexer->current_tok = t;
            return str;
        }
        else
        {
            my_string_push(mystr, *str);
            str++;
        }
    }

    if (mystr->len == 0)
    {
        // If we are here, it's an EOF
        t = token_new(TOKEN_EOF);
        lexer->current_tok = t;
        my_string_free(mystr);
        return NULL;
    }
    else
    {
        t = lexer_tokenize(mystr->str);
        my_string_free(mystr);
        lexer->current_tok = t;
        return str;
    }
}

// return curr token
struct token *lexer_peek(struct lexer *lexer)
{
    if (!lexer->current_tok)
        lexer_fill_current_tok(lexer);

    return lexer->current_tok;
}

// incr pos and return token
struct token *lexer_pop(struct lexer *lexer)
{
    if (lexer->current_tok)
        token_free(lexer->current_tok);

    char *next_pos = lexer_fill_current_tok(lexer);
    lexer->input = next_pos;

    struct token *t = token_new(lexer->current_tok->type);

    if (lexer->current_tok->value)
    {
        t->value = calloc(strlen(lexer->current_tok->value) + 1, sizeof(char));
        strcpy(t->value, lexer->current_tok->value);
    }

    if (lexer->current_tok)
        token_free(lexer->current_tok);
    lexer->current_tok = NULL;
    return t;
}
