#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
int main(void) {
	int num;
	printf("수를 입력해 주세요: ");
	scanf("%d", &num);
	for (int i = 1;i <= num-1;i++) {				//마지막줄은 빼고 n-1개의 줄을 먼저 찍는다.
		for (int j = 1;j <= num;j++) {				//과제5번을 참고해서 이번에는 행과 열을 더한것이 n+1인 자리만 *을 찍음
			if (j +i==num+1) {
				printf("*");
			}
			else {
				printf(" ");
			}

		}
		for (int j = num-1;j > 0;j--) {				//앞선 반복문의 y축대칭
			if (j + i==num+1) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");
	}
	for (int i = 1;i < 2 * num;i++) {			//마지막줄은 2n-1개만큼 *찍기
		printf("*");
	}

	return 0;
}