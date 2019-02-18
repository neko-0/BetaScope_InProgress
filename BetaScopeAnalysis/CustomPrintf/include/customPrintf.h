#ifndef CUSTOM_PRINTF_H
#define CUSTOM_PRINTF_H
#include <stdio.h>
#include <string>


struct CUSTOM_PRINTF
{
  static void warning_printf( std::string input_string )
  {
    printf("\033[1;31m");
    printf("%s\n", input_string.c_str() );
    printf("\033[0m");
  }

  static void help_printf( std::string input_string )
  {
    printf("\033[01;33m");
    printf("%s\n", input_string.c_str() );
    printf("\033[0m");
  }
};

#endif //BETASCOPE_SRC_CUSTOMPRINTF_HPP_CUMSTOMPRINTF_H_
