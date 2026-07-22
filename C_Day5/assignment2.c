#include<stdio.h>
#include<stdlib.h>
void arr_ij(int* sizeRow,int* sizeCol,int** pArr);      //배열에 숫자 저장하는 함수
void print(int* row,int* col,int** pArr);               //배열을 출력해주는 함수
int main (void){
    int **arr = NULL;
    int row,col,sizeRow,sizeCol;

    printf("열의 수를 입력하세요: ");
    scanf("%d",&sizeCol);
    printf("행의 수를 입력하세요: ");
    scanf("%d",&sizeRow);

    row=sizeRow;
    col=sizeCol;
    arr = (int**)malloc(sizeof(int*) * sizeRow);                //메모리 할당
    for (int i = 0; i < sizeRow; i++) {
        *(arr + i) = (int*)malloc(sizeof(int) * sizeCol);
    }
    
    arr_ij(&sizeRow,&sizeCol,arr);
    print(&row,&col,arr);

    for (int i=0;i<row;i++){                                    //메모리 해제하기
        free(arr[i]);
    }
    return 0;
}
void arr_ij(int* sizeRow, int* sizeCol, int** pArr){            
    int top=0,bottom=*sizeRow-1;                                //시작과 멈춤을 top,bottom,left,right로 관리하기
    int left=0, right=*sizeCol-1;      
    int num=1;
    while (top <= bottom && left <= right){                     //종료 조건 시작하는 위치가 top이 bottom보다 내려가거나 left가 right보다 오른쪽에 있거나
        //top print
        for (int t=left;t<=right;t++){                          //left에서 시작하여 right에서 멈추기
            *(*(pArr+top)+t)=num++;
            
        }
        top++;
        //right print
        for (int r=top;r<=bottom;r++){                          //top에서 시작하여 bottom에서 멈추기
            *(*(pArr+r)+right)=num++;
            
        }
        right--;
        //bottom print
        if (top<=bottom){                                       //스킵조건 추가
            for (int b=right;b>=left;b--){                      //right에서 시작하여 left에서 멈추기
                *(*(pArr+bottom)+b)=num++;
                
            }
            bottom--;
        }
        //left print
        if (left<=right){                                       //스킵조건 추가
            for (int l=bottom;l>=top;l--){                      //bottom에서 시작해 top에서 멈추기
                 *(*(pArr+l)+left)=num++;
            
            }
            left++;
        }
    }
}
void print(int* row,int* col, int** pArr){
    for (int i=0;i<*row;i++){
        for(int j=0;j<*col;j++){
            printf("%3d ",*(*(pArr+i)+j));                      //이중포인터 사용해서 해당 요소 출력하기
        }
        printf("\n");
    }
}