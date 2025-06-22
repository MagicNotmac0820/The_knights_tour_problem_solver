#ifndef _IO_HANDLER_H_
#define _IO_HANDLER_H_

#if __cplusplus
extern "C" {
#endif

void clearInputBuffer(void) ;

#if __cplusplus
}
#endif

#if __cplusplus
#include <iostream>
#include <limits>

void clearInputBuffer(void){
    std::cin.clear() ;
    std::cin.ignore( std::numeric_limits<std::streamsize>::max() , '\n' ) ;
    return ;
}
#else
#include <stdio.h>

void clearInputBuffer(void){
    char ch ;
    while( ( ch = getchar() ) != EOF && ch != '\n' ) ;

    return ;
}
#endif

#endif  //_IO_HANDLER_H_