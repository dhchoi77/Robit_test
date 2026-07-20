#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main (void){
    int N;
    int result;         //예외처리 검증
    printf("행과 열이 될 N을 입력해주세요: ");
    result=scanf("%d",&N);
    if (result==1){
    int **arr = (int**) malloc(sizeof(int*) * N);           //N행 N열 이므로 동적할당으로 이차원 배열 생성
    for(int i = 0; i < N; i++){
        arr[i] = (int*) malloc(sizeof(int) * N);
        for(int j = 0; j < N; j++){                         //배열 초기화해주기
            arr[i][j] = 0;
        }
    }
    int num=1;                                              //배열에 들어갈 숫자
    int i=0;                                                //배열의 행과 열의 합
    while (num<=N*N){
        if (i<N){
            for (int m=0;m<=i;m++){                         //i<N조건일때는 왼쪽 아래로 내려가는 방향으로 배열에 숫자 채우기
                arr[m][i-m]=num;
                num+=1;
            }
        }
        else{ //i=5                                         //i가 N보다 커지거나 같아질 경우 arr[m][i-m]이 배열의 범위를 나가버릴 수도 있다.
            for (int m=i+1-N;m<N;m++){
                if(i-m >= 0 && i-m < N){                    // 범위를 조건문으로 설정해주고 아니면 다음으로 넘기기
                    arr[m][i-m] = num;
                    num++;
                }
            }
        }
        
        i+=1;
    }

    for(int i = 0; i < N; i++){                             //출력
        for(int j = 0; j < N; j++){
            printf("%2d ", arr[i][j]);
        }
        printf("\n");
    }
    for(int i = 0; i < N; i++){                             //동적할당 해제하기
        free(arr[i]);  // 각 행 먼저
    }
    free(arr);  // 배열 자체 해제
}
    else{
        printf("ERROR");
    }
}