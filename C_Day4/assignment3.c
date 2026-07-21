#include<stdio.h>
#include<stdlib.h>
struct product{
    char name[30];  //제품의 이름
    int price;      //제품의 가격
};
//문자열 비교함수 과제1번에서 쓴 함수 응용하기
int DIY_strcmp(char name1[30], char name2[30]){
    int result =0;  //result가 name1이 name2보다 크면 1,같으면 0, 다르면 -1 반환
    for (int i=0;i<30;i++){
        if (name1[i]<name2[i]){
            result =-1;
            return result;
        }
        else if (name1[i]>name2[i]){
            result =1;
            return result;
        }
        else if (name1[i]=='\0' && name2[i]=='\0'){
            return result;
        }
    }
    return result;

    
}
int main (void){
    int len;
    printf("구매한 제품 갯수: ");
    scanf("%d",&len);
    struct product* list = (struct product*)malloc(sizeof(struct product)*len);     //동적할당
    for (int i=0;i<len;i++){  //제품 정보 입력
        if (scanf("%s %d",&list[i].name,&list[i].price)!=2){  //예외처리
            printf("ERROR");
            return 0;
        }
        
    }
    char want_product[30];
    scanf("%s",&want_product);
    int want_product_total_price=0;
    //원하는 제품과 입력한제품의 이름이 같다면 가격을 더해주기
    for (int i=0;i<len;i++){
        if (0==DIY_strcmp(want_product,list[i].name)){  //문자열 비교 같다면 0이 나온다.
            want_product_total_price+=list[i].price;
        }
    }
    if (want_product_total_price>0){        //입력한 제품이 있는지 확인
        printf("%s의 총 가격은 %d입니다.",want_product,want_product_total_price);
    }
    else{       //없다면 0출력
        printf("0");
    }
    free(list);
}