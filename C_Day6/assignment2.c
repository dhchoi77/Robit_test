#include<stdio.h>
#include<stdlib.h>
typedef struct _Node{
    int data;       //저장할 정수 값
    struct _Node* next;         //다음 노드를 가리키는 포인터
}Node;

typedef struct {
    Node* top;      //스택의 최상단 노드 포인터
    int size;       //현재 스택에 저장된 원소의 개수
}Stack;

Node* push (Node* top,int value);       //스택값 추가
Node* pop (Stack stack);                //스택에서 값 제거
void size (int size);                   //스택 크기 출력
Node* top (Node* top);                  //스택의 최상단 값 출력
int isEmpty (Node* top);                //스택이 비어있는지 확인
Node* printStack (Node* top);           //스택의 모든 값 출력
Node* free_stack(Node* top);            //메모리 해제
int DIY_strcmp(char name1[30], char name2[30]);     //문자열 비교

int main (void){
    Stack stack;
    //스택 초기화
    stack.top=NULL;
    stack.size=0;
    //명령어 출력
    printf("명령어: push, pop, size, top, isEmpty,printStack,end\n");
    //무한루프 end입력시 종료
    while (1){
        printf("입력: ");
        char str[30];       //입력받을 명령어 저장소
        int value;          //입력받을 data 저장소
        scanf("%s",str);
        //push명령어 일때
        if (DIY_strcmp(str,"push")==0){
            scanf(" %d",&value);            //추가할 data 입력
            stack.top=push(stack.top,value);
            stack.size++;       //크기 증가
        }
        //pop명령어 일때
        else if (DIY_strcmp(str,"pop")==0){
            stack.top=pop(stack);
        }
        //size 명령어일때
        else if (DIY_strcmp(str,"size")==0){
            size(stack.size);
        }
        //top명령어 일때
        else if (DIY_strcmp(str,"top")==0){
            stack.top=top(stack.top);
        }
        // isEmpty 명령어 일때
        else if (DIY_strcmp(str,"isEmpty")==0){
            if (isEmpty(stack.top)==0){
                printf("true\n");
            }
            else {
                printf("false\n");
            }
        }
        //printStack명령어 일때
        else if (DIY_strcmp(str,"printStack")==0){
            printStack(stack.top);
            
        }
        //end명령어 일때 프로그램 종료
        else if (DIY_strcmp(str,"end")==0){ 
            stack.top=free_stack(stack.top);    
            stack.size=0;            //프로그램 종료
            printf("프로그램을 종료하겠습니다.\n");
            return 0;
        }
        else{
            printf("input ERROR\n");
        }
    }

    return 0;
}
Node* push (Node* top,int value){
    // 스택이 비어있는 경우 (첫 번째 노드 추가)
    if (top==NULL){
        Node* newnode;
        newnode=(Node*)malloc(sizeof(Node));  // 새로운 노드에 메모리 할당
        newnode->data=value;                  // 데이터 저장
        newnode->next=NULL;                   // 다음 노드가 없음
        top=newnode;
        return top;
    }
    
    // 스택에 이미 노드가 있는 경우
    Node* newnode;
    newnode=(Node*)malloc(sizeof(Node));      // 새로운 노드에 메모리 할당
    newnode->data=value;                      // 데이터 저장
    newnode->next=top;                        // 기존의 최상단 노드를 다음으로 연결
    top=newnode;                              // 새로운 노드가 최상단이 됨
    return top;
}
Node* pop (Stack stack){
    // 스택이 비어있는 경우 오류 메시지 출력
    if (stack.top==NULL){
        printf("stack is empty.\n");
        return NULL;
    }
    
    Node* del=stack.top;          // 제거할 노드 저장
    stack.size--;                 // 스택 크기 감소
    stack.top=stack.top->next;    // 최상단을 다음 노드로 변경
    free(del);                    // 제거된 노드의 메모리 해제
    return stack.top;             // 새로운 최상단 반환
}
void size (int size){
    printf("size: %d\n",size);
}
Node* top (Node* top){
    // 스택이 비어있는 경우 오류 메시지 출력
    if (top==NULL){
        printf("stack is empty.\n");
        return NULL;
    }
    
    printf("top: %d\n",top->data);  // 최상단 값 출력
    return top;
}
int isEmpty (Node* top){
    if (top==NULL){
        return 0;     // 스택이 비어있음
    }
    return 1;         // 스택에 요소가 있음
}
Node* printStack (Node* top){
    // 스택이 비어있는 경우 오류 메시지 출력
    if (top==NULL){
        printf("stack is empty.\n");
        return NULL;
    }
    
    // 최상단부터 끝까지 모든 노드의 값을 출력
    for (Node* p=top;p!=NULL;p=(p->next)){
        printf("%d ",p->data);
    }
    printf("\n");
    return top;
}
Node* free_stack(Node* top) {
    // 스택의 모든 노드를 순회하면서 메모리 해제
    while (top != NULL) {
        Node* next = top->next;   // 다음 노드 저장 (현재 노드 해제 후 필요)
        free(top);                // 현재 노드의 메모리 해제
        top=next;                 // 다음 노드로 이동
    }
    top = NULL;                   // 포인터를 NULL로 설정
    return top;
}
int DIY_strcmp(char name1[30], char name2[30]){
    int result = 0;  // 비교 결과를 저장할 변수
    
    // 30개 문자까지 비교 (배열의 최대 길이)
    for (int i=0;i<30;i++){
        // name1의 문자가 name2의 문자보다 작은 경우
        if (name1[i]<name2[i]){
            result = -1;
            return result;
        }
        // name1의 문자가 name2의 문자보다 큰 경우
        else if (name1[i]>name2[i]){
            result = 1;
            return result;
        }
        // 두 문자열이 모두 끝난 경우 (둘 다 null 문자)
        else if (name1[i]=='\0' && name2[i]=='\0'){
            return result;  // 0 반환 (문자열이 같음)
        }
    }
    return result;
}
 