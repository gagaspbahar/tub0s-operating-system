#include "../header/math.h"

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