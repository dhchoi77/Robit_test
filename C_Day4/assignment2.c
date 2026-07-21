#include <stdio.h>
#include <stdlib.h>
#include <math.h>
struct position {
    int x;
    int y;
    float center_distance;      //중앙으로부터 멀어진 거리
    float max_distance;         //가장 떨어진 좌표값으로부터 거리
};

int main (void){
    int len;
    printf("좌표 갯수 입력: ");     //배열의 사이즈 입력
    scanf("%d",&len);
    struct position* list = (struct position*)malloc(sizeof(struct position)*len);  //동적할당으로 배열만들기
    for (int i=0;i<len;i++){  //학생정보 입력
        if (scanf("%d %d",&list[i].x,&list[i].y)!=2){  //예외처리
            printf("ERROR");
            return 0;
        }
        
    }
    float center_x=0;       //중앙 좌표의 x값
    float center_y=0;       //중앙 좌표의 y값
    //중앙좌표값 구하기
    for (int i=0;i<len;i++){        
        center_x+=list[i].x;        
        center_y+=list[i].y;
    }
    center_x/=len;
    center_y/=len;

    for (int i=0;i<len;i++){                        //중앙으로부터 떨어진 거리 구하기
        list[i].center_distance=sqrt((center_x-list[i].x )* (center_x-list[i].x) + (center_y-list[i].y)*(center_y-list[i].y));
    }
    
    float max_distance=list[0].center_distance;          //중앙값으로부터 가장 떨어진 거리 및 초기화
    int max_distance_index=0;                   //초기화
    for (int i=0;i<len;i++){
        if (max_distance<list[i].center_distance){
            max_distance=list[i].center_distance;
            max_distance_index=i;
        }
    }
    for (int i=0;i<len;i++){                        //max_distance 구하기
        if (i!=max_distance_index){
            list[i].max_distance=sqrt((list[max_distance_index].x-list[i].x)*(list[max_distance_index].x-list[i].x)+(list[max_distance_index].y-list[i].y)*(list[max_distance_index].y-list[i].y));
        }
        
    }
    float max_distance_sum=0;                           //가장 떨어진 좌표값으로부터의 거리합
    for (int i=0;i<len;i++){
        if (i!=max_distance_index){
            max_distance_sum=max_distance_sum+list[i].max_distance;
        }
        
    }
    printf("가장 거리가 먼 좌표는 (%d,%d)이며, 다른 좌표의 거리 총합은 약%.1f입니다.",list[max_distance_index].x,list[max_distance_index].y,max_distance_sum);

    free(list);

    return 0;
}