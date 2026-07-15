#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main(void) {
	float arr[5];	//실수를 저장할 배열

	for (int i = 0;i < sizeof(arr) / sizeof(float);i++) {	//배열의 사이즈를 자료형으로 나누어 i를 배열의 인덱스로 사용
		printf("%d번째 실수를 입력하시오: ",i+1);
		scanf_s("%f", &arr[i]);								//입력받은 실수를 arr에 i번째 인덱스에 저장
		

	}
	float mean=0;		
	float max = arr[0];	//초기값을 배열의 첫번째 값으로 지정
	float min = arr[0];	//초기값을 배열의 첫번째 값으로 지정
	for (int j = 0;j < sizeof(arr) / sizeof(float);j++) {
		mean += arr[j];			//요소를 다 더하고 나중에 배열의 크기로 나눌예정
		if (max <= arr[j]) {	//max와 배열의 요소와 비교후 조건에 맞다면 max에 배열의 요소 저장
			max = arr[j];
		}
		if (min >= arr[j]) {	//조건에 맞다면 min에 배열의 요소 저장
			min = arr[j];
		}
	}
	printf("------------결과----------\n");
	printf("평균: %f\n", mean/(sizeof(arr)/sizeof(float)));	//배열의 크기로 나누기
	printf("최대값: %f\n", max);
	printf("최솟값: %f\n", min);
    return 0;
}