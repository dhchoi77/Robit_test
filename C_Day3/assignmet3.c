#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main (void){
    int height, width;
    printf("행과 열을 입력해주세요(ex. 5 7): ");
    int result=0;           //예외처리 검증
    result=scanf("%d %d", &height, &width);

    if (result==2){
        int **arr = (int**) malloc(sizeof(int*) * height);      //동적할당과 이중포인터를 이용해서 이차원배열을 생성
        for(int i = 0; i < height; i++){
            arr[i] = (int*) malloc(sizeof(int) * width);
        }

        int num = 1;
        int top = 0, bottom = height - 1, left = 0, right = width - 1;

            // 나선형 채우기
        while(top <= bottom && left <= right){
            // 오른쪽으로
             for(int j = left; j <= right; j++){
                arr[top][j] = num++;
            }
            top++;

            // 아래로
            for(int i = top; i <= bottom; i++){
                arr[i][right] = num++;
            }
            right--;

            // 왼쪽으로 (역순)
            if(top <= bottom){
                for(int j = right; j >= left; j--){
                    arr[bottom][j] = num++;
                }
                bottom--;
            }

            // 위로 (역순)
            if(left <= right){
                for(int i = bottom; i >= top; i--){
                    arr[i][left] = num++;
                }
                left++;
            }
        }

        // 출력
        for(int i = 0; i < height; i++){
            for(int j = 0; j < width; j++){
                printf("%2d ", arr[i][j]);
            }
            printf("\n");
        }

        // 메모리 해제
        for(int i = 0; i < height; i++){
            free(arr[i]);
        }
        free(arr);

        return 0;
    }
    else if (result==0){
        printf("정수를 입력하세요.");
    }
    else{
        printf("입력오류");
    }
    
}