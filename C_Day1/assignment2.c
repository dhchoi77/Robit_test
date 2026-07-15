#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h> 

int main() {
    int num;
    printf("N번째 피보나치수열 출력 프로그램\n");
    printf("N: ");
    scanf("%d", &num);
    if (num == 1) {     //입력 숫자 1의 예외처리
        printf("0");
        
    }
    else if (num == 2) { //입력 숫자 2의 예외처리
        printf("1");
    }
    else if (num >= 3) {
        int p1 = 0;
        int p2 = 1;
        int next;   //N번째 피보나치수
        for (int i = 3;i <= num;i++) {  //0,1이 이미 있기 때문에 i는 3번부터 출발
            next = p1 + p2;
            p1 = p2;
            p2 = next;

        }
        printf("%d ", next);        //출력
    }
    
    return 0;
}