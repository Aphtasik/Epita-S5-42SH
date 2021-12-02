#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdlib.h>

struct my_string
{
    char *str;
    int max_len;
    int len;
};

// Init my_string struct with a max_len of 2
struct my_string *my_string_init(void);

// Resize my_string if full
void my_string_resize(struct my_string *mystr);

// Push one char c to the end of mystr
void my_string_push(struct my_string *mystr, char c);

// Free the struct
void my_string_free(struct my_string *mystr);
#endif
