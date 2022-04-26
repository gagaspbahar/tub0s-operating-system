#include "header/string.h"

unsigned int strlen(char *string) {
    // Mengembalikan panjang suatu null terminated string
	int ans = 0;
	while(string[ans] != '\0'){
		ans++;
	}
	return ans;
}

bool strcmp(char *s1, char *s2) {
    // Mengembalikan true jika string sama
    int i = 0;
    while(s1[i] == s2[i] && s1[i] != '\0' && s2[i] != '\0'){
        i++;
    }
    return (s1[i] == '\0' && s2[i] == '\0');
}

void strcpy(char *dst, char *src) {
    // Melakukan penyalinan null terminated string
    int i = 0;
    clear(dst, strlen(dst));
    while(src[i] != '\0'){
        dst[i] = src[i];
        i++;
    }
}