#include<stdio.h>
#include<stdlib.h>
struct Date {
    int year;
    int month;
    int day;
};
struct Time {
    int hour;
    int min;
    int sec;
};
struct Timestamp{           //중첩 구조체
    struct Date data;
    struct Time time;
    long long total_sec;
};
int ElapsedYear (int year){
    int count =0;   //윤년횟수;
    
    for (int i = 1; i < year; i++) {
        if ((i % 4 == 0 && i % 100 != 0) || i % 400 == 0) {     //윤년 판독
            count++;
        }
    }
    
    return count;
}

int main (void){
    //구조체 배열 선언
    struct Timestamp list[2];
    //입력부
    printf("날짜를 입력해주십시오.(ex. 2026 07 21 02 14 30): ");
    scanf("%d %d %d %d %d %d",&list[0].data.year,&list[0].data.month,&list[0].data.day,&list[0].time.hour,&list[0].time.min,&list[0].time.sec);
    printf("날짜를 입력해주십시오.(ex. 2026 07 21 02 14 30): ");
    scanf("%d %d %d %d %d %d",&list[1].data.year,&list[1].data.month,&list[1].data.day,&list[1].time.hour,&list[1].time.min,&list[1].time.sec);
    //초기화
    list[0].total_sec=0;            
    list[1].total_sec=0;
    list[0].total_sec+=list[0].data.year*365 * 24 * 60 * 60+ElapsedYear(list[0].data.year)*24 * 60 * 60;    //year의 전년도까지 second로 변환해 더하기
    list[1].total_sec+=list[1].data.year*365 * 24 * 60 * 60+ElapsedYear(list[1].data.year)*24 * 60 * 60;    //year의 전년도까지 second로 변환해 더하기

    //month의 전달까지 second로 변환해 더하기
    for (int i=0;i<2;i++){
        for (int j=1;j<list[i].data.month;j++){
            switch(j){              //month가 어떤 달이냐에 따라 더해지는 초 변경 예를 들면 1월은 31일까지 있고 9월은 30일까지 있는 것처럼
                case 1:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 2:
                    if ((list[i].data.year % 4 == 0 && list[i].data.year % 100 != 0) || list[i].data.year % 400 == 0){
                        list[i].total_sec+=29*24*60*60;
                    }
                    else{
                        list[i].total_sec+=28*24*60*60;
                    }
                    break;
                case 3:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 4:
                    list[i].total_sec+=30*24*60*60;
                    break;
                case 5:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 6:
                    list[i].total_sec+=30*24*60*60;
                    break;
                case 7:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 8:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 9:
                    list[i].total_sec+=30*24*60*60;
                    break;
                case 10:
                    list[i].total_sec+=31*24*60*60;
                    break;
                case 11:
                    list[i].total_sec+=30*24*60*60;
                    break;
                case 12:
                    list[i].total_sec+=31*24*60*60;
                    break;
            }
        }
        
    }
    //day의 전날까지 second로 변환해 더하기
    for (int i=0;i<2;i++){  
        for (int j=1;j<list[i].data.day;j++){
            list[i].total_sec+=24*60*60;
        }
    }
    //0~(hour-1)까지 second로 변환해 더하기
    for (int i=0;i<2;i++){
        for (int j=0;j<list[i].time.hour;j++){
            list[i].total_sec+=60*60;
        }
    }
    // 0~(min-1)까지 second로 변환해 더하기
    for (int i=0;i<2;i++){
        for (int j=0;j<list[i].time.min;j++){
            list[i].total_sec+=60;
        }
    }
    //0~sec 더하기
    for (int i=0;i<2;i++){
        for (int j=0;j<list[i].time.sec;j++){
            list[i].total_sec+=1;
        }
    }
    struct Timestamp minus={0,0,0,0,0,0,0}; //날짜차이 계산용 변수
    if (list[1].total_sec-list[0].total_sec>0){             //누가 큰지 판독
        minus.total_sec=list[1].total_sec-list[0].total_sec;
    }
    else{
        minus.total_sec=list[0].total_sec-list[1].total_sec;
    }                 
    minus.time.hour=minus.total_sec/(60*60);
    minus.time.min=(minus.total_sec%3600)/60;
    minus.time.sec=((minus.total_sec%3600)%60);
    printf("시간 차이: %d시 %d분 %d초",minus.time.hour,minus.time.min,minus.time.sec);
    
}