#include<stdio.h>
#include<stdlib.h>
void arr_ij(int* sizeRow,int* sizeCol,int** pArr);
void print(int* row,int* col,int** pArr);
int main (void){
    int **arr = NULL;
    int row,col,sizeRow,sizeCol;

    printf("열의 수를 입력하세요: ");
    scanf("%d",&sizeCol);
    printf("행의 수를 입력하세요: ");
    scanf("%d",&sizeRow);

    row=sizeRow;
    col=sizeCol;
    arr = (int**)malloc(sizeof(int*) * sizeRow);
    for (int i = 0; i < sizeRow; i++) {
        *(arr + i) = (int*)malloc(sizeof(int) * sizeCol);
    }
    
    arr_ij(&sizeRow,&sizeCol,arr);
    print(&row,&col,arr);

    for (int i=0;i<row;i++){
        free(arr[i]);
    }
    return 0;
}
void arr_ij(int* sizeRow, int* sizeCol, int** pArr){
    int position[4]={0,*sizeCol-1,*sizeRow-1,0};
    int top=0,bottom=*sizeRow-1;
    int left=0, right=*sizeCol-1;      //{top,right,bottom,left}
    int num=1;
    while (top <= bottom && left <= right){
        //top print
        for (int t=left;t<=right;t++){
            *(*(pArr+top)+t)=num++;
            
        }
        top++;
        //right print
        for (int r=top;r<=bottom;r++){
            *(*(pArr+r)+right)=num++;
            
        }
        right--;
        //bottom print
        if (top<=bottom){
            for (int b=right;b>=left;b--){
                *(*(pArr+bottom)+b)=num++;
                
            }
            bottom--;
        }
        //left print
        if (left<=right){
            for (int l=bottom;l>=top;l--){
                 *(*(pArr+l)+left)=num++;
            
            }
            left++;
        }
    }
}
void print(int* row,int* col, int** pArr){
    for (int i=0;i<*row;i++){
        for(int j=0;j<*col;j++){
            printf("%3d ",*(*(pArr+i)+j));
        }
        printf("\n");
    }
}