#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

int main (void){
    int *arr;
    int num;
    int result=0;   //예외처리 검증
    printf("몇개의 원소를 할당하시겠습니까? : ");
    result=scanf("%d",&num);
    if (result==1){                                 //예외처리
    arr=(int*)malloc(sizeof(int)*num);              //원소의 개수를 입력받으므로 동적할당으로 배열 생성
    for (int i=0;i<num;i++){
        printf("정수형 데이터: ");
        scanf("%d",&arr[i]);
    }
    int max=arr[0], min=arr[0],sum=0;
    for (int i=0;i<num;i++){
        if (max<=arr[i]){                           //max판별
            max=arr[i];                         
        }
        if (min>=arr[i]){                           //min판별
            min=arr[i];
        }
        sum+=arr[i];                                //모든 요소를 sum에 저장
    }
    printf("최댓값: %d\n",max);
    printf("최솟값: %d\n",min);
    printf("전체합: %d\n",sum);
    printf("평균: %f",sum/(float)num);              //소숫점으로 나타내야함으로 분모나 분자에 float형으로 변환
    free(arr);  // 반드시 필요
    return 0;
}
    else{                                           //에러
        printf("ERROR");
    }
}