#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 

int main(void) {
	printf("Input Arithmetic Operation\n");
	printf("ex) 5 + 3 \n    5 - 3 \n    5 * 3 \n    5 / 3 \n    5 ^ 3\n");
	float num1, num2;
	char ao;								//부호
	printf("input: ");
	scanf("%f %c %f", &num1, &ao, &num2);	// 띄어쓰기를 해야만 구분가능
	
	if (ao == '+') {
		printf("%.2f + %.2f = %.2f", num1, num2, num1 + num2);
	}
	else if (ao == '-') {
		printf("%.2f - %.2f = %.2f", num1, num2, num1 - num2);
	}
	else if (ao == '*') {
		printf("%.2f * %.2f = %.2f", num1, num2, num1 * num2);
	}
	else if (ao == '/') {
		printf("%.2f / %.2f = %.2f", num1, num2, num1 / num2);
	}
	else if (ao == '^') {	//거듭제곱은 연산자가 없으므로 직접 계산
		float result=num1;
		for (int i = 1;i < num2;i++) {
			result *= num1;
		}
		printf("%.2f ^ %.2f = %.2f", num1, num2, result);
	}
}