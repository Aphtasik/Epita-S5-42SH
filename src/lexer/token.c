#include "token.h"

// Struct that allows us to avoid a huge switch
static struct token token_pairs[] = {
    { TOKEN_IF, "if" },       { TOKEN_THEN, "then" }, { TOKEN_ELIF, "elif" },
    { TOKEN_ELSE, "else" },   { TOKEN_FI, "fi" },     { TOKEN_SEMICOL, ";" },
    { TOKEN_PIPE, "|" },      { TOKEN_BACKN, "\n" },  { TOKEN_EOF, "\0" },
    { TOKEN_WHILE, "while" }, { TOKEN_DO, "do" },     { TOKEN_AND, "&&" },
    { TOKEN_OR, "||" },       { TOKEN_NOT, "!" },     { TOKEN_AMPERS, "&" },
    { TOKEN_DONE, "done" },   { TOKEN_FOR, "for" },   { TOKEN_UNTIL, "until" },
    { TOKEN_CASE, "case" },   { TOKEN_ESAC, "esac" }, { TOKEN_UNKNOWN, NULL },
};

struct token *token_new(enum token_type type)
{
    struct token *new = malloc(sizeof(struct token));
    new->type = type;
    new->value = NULL;
    return new;
}

void token_free(struct token *token)
{
    if (token)
    {
        if (token->value)
            free(token->value);

        free(token);
        token = NULL;
    }
}

enum token_type token_get_type(char *token_str)
{
    // Iterate throught the token_pairs struct
    // Compare string and get the token type
    for (size_t i = 0; token_pairs[i].value; i++)
    {
        if (!strcmp(token_pairs[i].value, token_str))
            return token_pairs[i].type;
    }
    return TOKEN_WORD;
}
