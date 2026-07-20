#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

int main (void){
    int arr[5][5];
    int num=1;
    for (int i=0;i<3;i++){                  //윗쪽 0,1,2행 
        for (int j=0;j<5;j++){
            if (i>j || 5-i<=j){
                arr[i][j]=0;
            }
            else{
                arr[i][j]=num;
                num+=1;
            }
        }
    }
    for (int i=0;i<2;i++){                  //3,4행
        for (int j=0;j<5;j++){
            int row = i+3;
            if (j>=4-row && j<=row){  
                arr[row][j]=num;
                num+=1;
            }
            else{
                arr[row][j]=0;
            }
        }
    }
    for (int i=0;i<5;i++){                   //출력  
        for (int j=0;j<5;j++){
            printf("%d ",arr[i][j]);

        }
        printf("\n");
    }
    return 0;
}