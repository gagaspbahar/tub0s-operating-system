#include "../header/string_lib.h"

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

int splitParam(char *input, char param[8][64])
{
    int i = 0;
    int j = 0;
    int paramLen = 0;
    int inputLen = strlen(input);
    while (i < inputLen)
    {
        j = 0;
        while (input[i] != ' ' && input[i] != '\0')
        {
            param[paramLen][j] = input[i];
            i++;
            j++;
        }
        j++;
        i++;
        param[paramLen][j] = '\0';
        paramLen++;
    }
    return paramLen;
}

void memcpy(byte *dest, byte *src, unsigned int n) {
    // Mengcopy n bytes yang ditunjuk src ke dest
    int i;
    clear(dest, n);
    for(i = 0; i < n; i++){
        dest[i] = src[i];
    }
}