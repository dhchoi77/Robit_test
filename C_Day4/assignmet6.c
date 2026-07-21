#include<stdio.h>
#include<stdlib.h>
//숫자 프린트 함수
void number_print(int *numbers,int len){        //숫자 무늬를 출력하는 함수
    int count=0;
    for (int i=0;i<3;i++){
        for (int j=0;j<4;j++){
            printf("%d",numbers[count]);
            count+=1;
            if (count>=len){    //배열의 인덱스가 범위를 벗어나면 다시 0번째로 돌아와서 출력하는 조건문
                count=0;
            }
        }
        printf("\n");
    }
};
int main (void){
    int capacity=1;                 //numbers배열의 사이즈
    int len=0;                      //numbers에 저장할 변수num의 인덱스
    int* numbers=(int*)malloc(sizeof(int)*capacity);
    while (1){          //입력오류가 아닌 이상 계속 반복
        if (capacity==len){     //배열 크기확인 부족하다면 +1씩 증가시키기
            capacity+=1;
            numbers=(int*)realloc(numbers,sizeof(int)*capacity);
        }
        int num;
        printf("입력: ");
        if (scanf("%d",&num)!=1){
            printf("ERROR");
            break;
        }                       //ERROR가 나지 않았다면 배열에 입력한 숫자 저장
        numbers[len++]=num;
        number_print(numbers,len);  //배열에 모든 숫자를 출력형식에 맞게 출력하는 함수에 전송



        
    }
    free(numbers);  //해제하기

    return 0;
}