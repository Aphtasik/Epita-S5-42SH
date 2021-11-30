project(
  'example',
  'c',
  version : '1.0',
  default_options: [
    'c_std=c99',
    'warning_level=3',
    'werror=true',
  ],
)

cflags = ['-D_GNU_SOURCE', '-D_POSIX_C_SOURCE=200809L']
add_project_arguments(cflags, language: 'c')

incdirs = [
  include_directories('utils'),
  include_directories('lexer'),
]

common = [
  'lexer/lexer_basics.c',
  'lexer/lexer_fill_current_tok.c',
  'lexer/lexer_peek_pop.c',
  'lexer/lexer_special_token.c',
  'lexer/my_string.c',
  'lexer/token.c',
]

token_printer = executable(
    # name of the output executable
    'token_printer',
    # source files for the executable
    common + [ 'lexer_main_test.c' ],
    # these are passed as -I
    include_directories: incdirs
)
