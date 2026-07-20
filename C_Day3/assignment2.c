#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main (void){
    int arr[4][3]={
        {1,2,3},{4,5,6},{7,8,9},{10,11,12}
    };
    for (int i=0;i<4;i++){
        for (int j=0;j<3;j++){
            printf("%d ",arr[i][j]);

        }
        printf("\n");
    }
    printf("\n");
    for (int i=0;i<3;i++){
        for (int j=0;j<4;j++){
            printf("%d ",arr[j][i]);        //입력할때와 i와 j의 위치를 다르게 하여 arr출력, 즉 배열자체를 바꾼것이 아닌 출력형식을 바꾼것
        }
        printf("\n");
    }
    return 0;
}