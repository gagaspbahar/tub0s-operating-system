#include "header/std_lib.h"

// Opsional : Tambahkan implementasi kode C

// TODO
int div(int a, int b) {
    int ans = 0;
	while (ans * b <= a){
		ans++;
	}
	return ans - 1;
}

int mod(int a, int n) {
    while (a >= n){
		a = a - n;
	}
	return a;
}

// Operasi standar bahasa C
// Implementasikan
void memcpy(byte *dest, byte *src, unsigned int n) {}
// Mengcopy n bytes yang ditunjuk src ke dest

unsigned int strlen(char *string) {}
// Mengembalikan panjang suatu null terminated string

bool strcmp(char *s1, char *s2) {}
// Mengembalikan true jika string sama

void strcpy(char *dst, char *src) {}
// Melakukan penyalinan null terminated string

void clear(byte *ptr, unsigned int n) {
    // Mengosongkan byte array yang memiliki panjang n
    int i;
	for (i = 0; i < n; i++){
		ptr[i] = 0x00;
	}
}
