#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 
int main(void) {
	int num;
	printf("수를 입력해 주세요: ");
	scanf("%d", &num);
	//윗별
	for (int i = 1;i <= num;i++) {
		for (int j = 1;j <= num;j++) {			//한줄을 2개로 쪼갬
			if (j<=i) {							//n번째 줄일때 n번째칸까지만 *찍기
				printf("*");
			}
			else {
				printf(" ");
			}
			
		}
		for (int j = num;j>0;j--) {
			if (j <= i) {						//y축대칭으로 *찍기
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");							//줄바꿈
	}
	//아랫별
	for (int i = num-1;i >=1;i--) {				//윗별보다 1줄 부족한걸 감안하여 x축대칭으로 *찍기
		for (int j = 1;j <= num;j++) {			 
			if (j <= i) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		for (int j = num;j>0;j--) {
			if (j <= i) {
				printf("*");
			}
			else {
				printf(" ");
			}
		}
		printf("\n");							//줄바꿈
	}	
}