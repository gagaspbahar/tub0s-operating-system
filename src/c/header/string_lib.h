#ifndef STRING_H
#define STRING_H
#include "std_type.h"

unsigned int strlen(char *string);
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2);
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src);
// Melakukan penyalinan null terminated string

#endif