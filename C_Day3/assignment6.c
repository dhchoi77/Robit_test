#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main (void){
    int height, width;
    int result;
    printf("행과 열을 입력해주세요 홀수만(ex. 5 7): ");
    result=scanf("%d %d",&height, &width);
    if (result==2){
        if (width%2==0 || height%2==0 || width>0 || height>0){              //행과열의 짝수와 0,음수일때 예외처리
            printf("홀수만 입력하세요.");
        }
        else{

    int **arr = (int**) malloc(sizeof(int*) * height);
    for(int i = 0; i < height; i++){
        arr[i] = (int*) malloc(sizeof(int) * width);
        for(int j = 0; j < width; j++){
            arr[i][j] = 0;
        }
    }
    int num=1;                                              //배열에 들어갈 숫자
    int cycle=1;                                            //마름모의 변의 크기==사이클
    arr[height/2][width/2]=num;                             //가장 가운데 숫자 입력
    num+=1;
    int x=width/2,y=height/2;                               //요소의 위치를 좌표값으로 관리
    while (num<=height*width){
        y+=1;
        if (0 <= x && x < width && 0 <= y && y < height){   //경계 체크   
            arr[y][x] = num;                                //사이클이 돌았을때 아래로 한칸 아래로 이동
            num++;
        }
        for (int m=0;m<cycle-1;m++){                        //우측 아래방향으로 내려가기 + 다른 변보다 1칸 덜 내려가기
            x+=1;y+=1;
            
            if (0<=x && x<width && 0<=y && y<height){       //좌표값이 범위에 있으면 숫자 입력 아니면 통과
                arr[y][x]=num;
                num+=1;
            }
            else{
                continue;
            }
        }
        for (int m=0;m<cycle;m++){                          //우측 위방향으로 올라가기
            y-=1;x+=1;
            if (0<=x && x<width && 0<=y && y<height){
                arr[y][x]=num;
                num+=1;
            }
            else{
                continue;
            }
        }
        for (int m=0;m<cycle;m++){                          //좌측 위방향으로 내려가기
            y-=1;x-=1;
            if (0<=x && x<width && 0<=y && y<height){
                arr[y][x]=num;
                num+=1;
            }
            else{
                continue;
            }
        }
        for (int m=0;m<cycle;m++){                          //좌측 아래방향으로 내려가기
            x-=1;y+=1;
            
            if (0<=x && x<width && 0<=y && y<height){
                arr[y][x]=num;
                num+=1;
            }
            else{
                continue;
            }
        }
        
        cycle+=1;                                           //싸이클 1 더하기
    }

    for(int i = 0; i < height; i++){            //출력
        for(int j = 0; j < width; j++){
            printf("%3d ", arr[i][j]);
        }
        printf("\n");
    }

    for(int i = 0; i < height; i++){            //메모리 해제
        free(arr[i]);
    }
    free(arr);
    return 0;
}
}
    else{
        printf("ERROR");
    }
}