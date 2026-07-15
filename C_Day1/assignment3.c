#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	printf("년도를 입력하세요 : ");
	int year;
	scanf("%d", &year); //년도 입력
	if (year % 4 == 0) {					
		if (year % 100 != 0 || year % 400 == 0) {				//4의 배수이면서, 100의 배수가 아닐 때 또는 400의 배수라면
			printf("윤년입니다.");
		}
		else {								//4의 배수지만 100의 배수이거나 400의 배수가 아니라면
			printf("윤년이 아닙니다.");
		}

	}
	else {									//조건에 맞지 않는다면
		printf("윤년이 아닙니다.");
	}
	return 0;
}