#include <err.h>
#include <io/cstream.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <utils/vec.h>

#include "ast/ast.h"
#include "execution/exec.h"
#include "lexer/lexer.h"
#include "parser/parse.h"

/**
 * \brief Parse the command line arguments
 * \return A character stream
 */
static struct cstream *parse_args(int argc, char *argv[])
{
    // If launched without argument, read the standard input
    if (argc == 1)
    {
        if (isatty(STDIN_FILENO))
            return cstream_readline_create();
        return cstream_file_create(stdin, /* fclose_on_free */ false);
    }

    // 42sh FILENAME
    if (argc == 2)
    {
        FILE *fp = fopen(argv[1], "r");
        if (fp == NULL)
        {
            warn("failed to open input file");
            return NULL;
        }

        return cstream_file_create(fp, /* fclose_on_free */ true);
    }

    if (argc >= 2 && (strcmp(argv[1], "-c") == 0))
    {
        return cstream_file_create(stdin, /* fclose_on_free */ false);
        ;
    }

    fprintf(stderr, "Usage: %s [COMMAND]\n", argv[0]);
    return NULL;
}

int exec_command(struct vec *line)
{
    char *input = calloc(line->size + 1, sizeof(char));
    size_t i = 0;
    for (i = 0; i < line->size; i++)
        input[i] = line->data[i];
    input[i] = '\0';

    if (!input)
    {
        free(input);
        return 0;
    }

    if (*input == '\0')
    {
        free(input);
        return 0;
    }

    // Init lexer
    struct lexer *lex = lexer_new(input);
    if (lex == NULL)
    {
        free(input);
        return 1;
    }

    // Init ast
    skip_newline(lex);
    struct ast *ast = parse(lex);
    if (ast == NULL)
    {
        lexer_free(lex);
        free(input);
        return 1;
    }

    lexer_free(lex);
    ast_free(ast);
    free(input);

    return 0;
}

/**
 * \brief Read and print lines on newlines until EOF
 * \return An error code
 */
enum error read_print_loop(struct cstream *cs, struct vec *line, int is_file)
{
    enum error err;

    while (true)
    {
        // Read the next character
        int c;
        if ((err = cstream_pop(cs, &c)))
            return err;

        // If the end of file was reached, stop right there
        if (c == EOF)
        {
            exec_command(line); // TODO: leaks + err
            break;
        }

        // If a newline was met, print the line
        if (is_file == 0 && c == '\n')
        {
            exec_command(line); // TODO: leaks + err
            vec_reset(line);
            continue;
        }

        // Otherwise, add the character to the line
        vec_push(line, c);
    }

    return NO_ERROR;
}

int main(int argc, char *argv[])
{
    int rc;

    // Parse command line arguments and get an input stream
    struct cstream *cs;
    if ((cs = parse_args(argc, argv)) == NULL)
    {
        rc = 1;
        goto err_parse_args;
    }

    // Create a vector to hold the current line
    struct vec line;
    vec_init(&line);

    // Run the test loop
    if (argc == 1)
    {
        if (read_print_loop(cs, &line, 0) != NO_ERROR)
        {
            rc = 1;
            goto err_loop;
        }
    }
    if (argc == 2)
    {
        if (read_print_loop(cs, &line, 1) != NO_ERROR)
        {
            rc = 1;
            goto err_loop;
        }
    }
    else
    {
        size_t count = 0;
        int i = 2;
        int j = 0;
        for (i = 2; i < argc; i++)
        {
            for (j = 0; argv[i][j] != '\0'; j++)
                ;
            count += j;
        }

        char *input = malloc(count + 1 + argc);
        input[0] = '\0';
        for (i = 2; i < argc; i++)
        {
            input = strcat(input, argv[i]);
            input = strcat(input, " ");
        }

        // Init lexer
        struct lexer *lex = lexer_new(input);

        // Init ast
        skip_newline(lex);
        struct ast *ast = parse(lex);

        lexer_free(lex);
        ast_free(ast);
        free(input);
    }

    // Success
    rc = 0;

err_loop:
    cstream_free(cs);
    vec_destroy(&line);
err_parse_args:
    return rc;
}
