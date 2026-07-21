#include<stdio.h>
#include<stdlib.h>
int str_most_char(char* str,int len){
    //printf("\n%s %d",str,len);
    char most_char=str[0];           //가장 많이 나온 문자
    int most_number=0;              //가장 많이 나온 문자의 횟수

    char temp;  //임시문자;
    int temp_number;     //임시문자 등장 횟수;
    for (int i=0;i<len;i++){
        //임시문자 초기화
        temp=str[i];        
        temp_number=0;
        for (int j=0;j<len;j++){
            if (str[i]!=' ' && str[i]==str[j]){     //같은 문자면 등장 횟수 추가
                temp_number+=1;
            }
        }
        if (temp_number>=most_number){              //임시문자가 사전의 가장많이 나온 문자보다 많이 나왔다면 바꿔주기
            most_char=str[i];
            most_number=temp_number;
        }
    }
    printf("최다 등장 문자: %c",most_char);

    return 0;

}

int main (void){
    int capacity=1;     //동적할당할때 쓰는 배열의 크기
    int len =0;         //입력받는 배열의 크기
    char* str=(char*)malloc(sizeof(char)*capacity);
    char ch;
    printf("문자열 입력: ");
    //입력받으면서 실시간으로 배열의 크기를 증가 시켜서 메모리 최소화하기
    while (scanf("%c", &ch) == 1 && ch != EOF) {
        if (ch == '\n') break;
        //  공간 부족한지 체크
        if (len==capacity) {
            capacity +=1;
            str = (char *)realloc(str, capacity*sizeof(char));
        }
    
        //  글자 저장
        str[len++] = ch;
    }

    str[len] = '\0';  // 문자열 끝 표시


    for (int i=len-1;i>=0;i--){
        printf("%c",str[i]);
    }
    printf("\n");
    str_most_char(str,len);
}