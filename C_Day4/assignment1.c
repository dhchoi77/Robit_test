#include <stdio.h>
struct student {
    int grade;  //학년
    float score;  //학점
    char name[30];  //이름
};
int DIY_strcmp(char name1[30], char name2[30]){
    int result =1;  //누가 큰지 판별, result가 name1이 크거나 같으면 1, 다르면 -1 반환
    for (int i=0;i<30;i++){
        if (name1[i]<name2[i]){
            result =-1;
            return result;
        }
        else if (name1[i]=='\0' && name2[i]=='\0'){
            return result;
        }
    }
    return result;

    
}

int main(void){
    struct student list[5];    //학생정보 리스트
    for (int i=0;i<5;i++){  //학생정보 입력
        printf("%d번 학생의 학년, 학점, 이름을 입력해주세요(ex. 1 3.5 홍길동): ",i+1);
        if (scanf("%d %f %s",&list[i].grade,&list[i].score,&list[i].name)!=3){  //예외처리
            printf("ERROR");
            return 0;
        }
        
    }
    //학생학년순으로 내림차순 정렬(버블정렬)
    struct student change_temp;    //정렬때 쓸 변경 공간
    for (int i=0;i<5;i++){  
        for (int j=0;j<4-i;j++){
            if (list[j].grade>list[j+1].grade){
                change_temp=list[j+1];
                list[j+1]=list[j];
                list[j]=change_temp;
            }
        }

    }
    for (int i=0;i<5;i++){      //점수 정렬
        for (int j=0;j<4-i;j++){
            if (list[j].grade==list[j+1].grade && list[j].score>list[j+1].score){
                change_temp=list[j+1];
                list[j+1]=list[j];
                list[j]=change_temp;
            }
        }

    }
    for (int i=0;i<5;i++){      //이름 정렬
        for (int j=0;j<4-i;j++){
            if (list[j].grade==list[j+1].grade && list[j].score==list[j+1].score && 0<DIY_strcmp(list[j].name,list[j+1].name)){
                change_temp=list[j+1];
                list[j+1]=list[j];
                list[j]=change_temp;
            }
        }

    }
    printf("\n출력\n");
    for (int i=0;i<5;i++){
        printf("%d %.1f %s\n",list[i].grade, list[i].score, list[i].name);
    }


    return 0;
}