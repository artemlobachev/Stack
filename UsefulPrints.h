#ifndef CFP_H
#define CFP_H
   
#include <stdio.h>
#include <stdio.h>
#include <stdarg.h>

#define RED "\033[31m"
#define GREEN "\033[32m"
#define ORANGE "\033[33m"
#define BLUE "\033[34m"
#define PURPLE "\033[35m"
#define CYAN "\033[36m"
#define GREY "\033[37m"
#define RESET "\033[0m"


#define COLOR_PRINTF(COLOR, ...)    \
do                                   \
 {                                    \
    printf(COLOR __VA_ARGS__);         \
    printf(RESET);                      \
 } while(0)                             

#define PRINT_STARS puts("******************************************")

#endif