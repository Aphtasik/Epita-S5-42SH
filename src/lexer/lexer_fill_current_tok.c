#include "lexer.h"

static struct token *lexer_tokenize(char *token_str, enum token_type type)
{
    // We don't want to tokenize these useless things
    if (strlen(token_str) == 0 || *token_str == ' '
        || *token_str == '\t') // TODO: == '#' ???
        return NULL;

    // Allocate the token and fill his struct
    if (type == 0)
        type = token_get_type(token_str);
    struct token *t = token_new(type);

    t->value = malloc(sizeof(char) * (strlen(token_str) + 1));
    strcpy(t->value, token_str);

    return t;
}

static char *lexer_make_string(char *begin, char *end)
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

static int lexer_last_token(struct lexer *lexer, struct my_string *mystr,
                            struct token *t)
{
    // Handle last Token still in mystr
    if (mystr->len == 0)
    {
        // If we are here, it's an EOF
        t = token_new(TOKEN_EOF);
        lexer->current_tok = t;
        my_string_free(mystr);
        return 0;
    }
    else
    {
        t = lexer_tokenize(mystr->str, 0);
        my_string_free(mystr);
        lexer->current_tok = t;
        return 1;
    }
}

// Fill lexer_curr_token and return the pos of the beginning of the next tok
char *lexer_fill_current_tok(struct lexer *lexer)
{
    char *str = lexer->input;
    struct token *t = NULL;
    struct my_string *mystr = my_string_init();
    char *end_separator = NULL;
    int is_special;
    enum token_type type;

    while (*str != '\0')
    {
        is_special = get_special_token_end(str, &end_separator, &type);
        if (is_special && mystr->len == 0)
        {
            char *diff = lexer_make_string(str, end_separator);
            t = lexer_tokenize(diff, type);
            free(diff);
            if (!t)
            {
                // We have a space, nothing or \t
                // just skip it and free the created token
                token_free(t);
                str++;
                continue;
            }
            else
            {
                my_string_free(mystr);
                lexer->current_tok = t;
                return end_separator;
            }
        }
        else if (is_special && mystr->len != 0)
        {
            // End of the curr token
            // Give this tok to lexer
            t = lexer_tokenize(mystr->str, 0);
            my_string_free(mystr);
            lexer->current_tok = t;
            return str;
        }
        else
        {
            // We are in a token, copy next char in mystr
            my_string_push(mystr, *str);
            str++;
        }
    }

    // last_token = 0 if NULL 1 if there is still smth in it
    return lexer_last_token(lexer, mystr, t) ? str : NULL;
}
