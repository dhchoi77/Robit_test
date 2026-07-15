#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	int n;
	int r;
	printf("수열의 n과 r을 입력해 주세요(예시: 5 2): ");
	scanf("%d %d", &n,&r);
	int numerator = 1;		//분자
	int denominator = 1;	//분모
	
	for (int i = 1;i <= n;i++) {		//순열계산
		numerator *= i;
	}
	for (int i = 1;i <= (n - r);i++) {
		denominator *= i;
	}
	printf("순열(P): %d\n",numerator/denominator);

	numerator = 1; denominator = 1;		//분모, 분자 초기화
	for (int i = 1;i <= r;i++) {		//중복순열 계산
		numerator *= n;
	}
	printf("중복순열(π): %d\n", numerator / denominator);

	numerator = 1; denominator = 1;		//분모, 분자 초기화
	for (int i = 1;i <= n;i++) {		//조합 계산
		numerator *= i;
	}
	for (int i = 1;i <= (n - r);i++) {
		denominator *= i;
	}
	for (int i = 1;i <= r;i++) {
		denominator *= i;
	}
	printf("조합(C): %d\n", numerator / denominator);

	numerator = 1; denominator = 1;		//분모, 분자 초기화
	n = n + r - 1;						//중복조합에 맞게 n 설정
	for (int i = 1;i <= n;i++) {
		numerator *= i;
	}
	for (int i = 1;i <= (n - r);i++) {
		denominator *= i;
	}
	for (int i = 1;i <= r;i++) {
		denominator *= i;
	}
	printf("중복조합(H): %d\n", numerator / denominator);
	return 0;
}