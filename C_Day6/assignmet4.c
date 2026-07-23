#include<stdio.h>
#include<stdlib.h>

// 노드 구조체 (스택과 큐에서 공통으로 사용)
typedef struct _Node{
    char data;             // 저장할 문자 값
    struct _Node* next;    // 다음 노드를 가리키는 포인터
}Node;

// 스택 구조체
typedef struct{
    Node* top;   // 스택의 최상단 노드 포인터
    int size;    // 현재 스택에 저장된 원소의 개수
}Stack;

// 큐 구조체
typedef struct{
    Node* front;  // 큐의 앞부분 (Dequeue할 위치)
    Node* rear;   // 큐의 뒷부분 (Enqueue할 위치)
    int size;     // 현재 큐에 저장된 원소의 개수
}Queue;

// 함수 선언부
Node* push (Node* top, char value);        // 스택에 문자 추가
char pop(Node** top);                      // 스택에서 문자 제거 및 반환

Queue* Enqueue (Queue* que, char value);   // 큐에 문자 추가
char deq (Node** front);                   // 큐에서 문자 제거 및 반환

int main (void){
    // 스택 초기화
    Stack stack;
    stack.top=NULL;
    stack.size=0;
    
    // 큐 초기화
    Queue que;
    que.front=NULL;
    que.rear=NULL;
    que.size=0;
    
    printf("입력: ");
    char str;
    //입력받기: 한 글자씩 받아서 스택과 큐에 저장
    while(scanf("%c",&str)){
        if (str=='\n'){  // 엔터키 입력 시 루프 탈출
            break;
        }
        
        if (str!=' '){   // 공백을 제외한 문자만 저장
            // 스택에 push: 최신 문자가 위에 쌓인다 (역순 저장)
            stack.top=push(stack.top,str);
            
            // 큐에 Enqueue: 먼저 들어온 문자가 front에 있다 (정순 저장)
            que=(*Enqueue(&que,str));
        }
    }
    //회문 판별 로직
    while(que.front!=NULL && stack.top!=NULL){
        if (deq(&que.front) != pop(&stack.top)){
            printf("회문이 아니다.\n");
            return 0;
        }
    }
    
    printf("회문입니다.\n");
    return 0;
}
//push 함수
//스택의 최상단에 새로운 문자를 추가하는 함수
Node* push (Node* top, char value){
    // 스택이 비어있는 경우 (첫 번째 노드 추가)
    if (top==NULL){
        Node* newnode;
        newnode=(Node*)malloc(sizeof(Node));  // 새로운 노드에 메모리 할당
        newnode->data=value;                  // 문자 저장
        newnode->next=NULL;                   // 다음 노드가 없음
        top=newnode;
        return top;
    }
    // 스택에 이미 노드가 있는 경우
    Node* newnode;
    newnode=(Node*)malloc(sizeof(Node));      // 새로운 노드에 메모리 할당
    newnode->data=value;                      // 문자 저장
    newnode->next=top;                        // 기존의 최상단 노드를 다음으로 연결
    top=newnode;                              // 새로운 노드가 최상단이 됨
    return top;
}
//pop 함수
//스택의 최상단 문자를 제거하고 반환하는 함수
char pop(Node** top){
    char data = (*top)->data;    // 현재 top의 문자 데이터 저장
    Node* del = (*top);          // 제거할 노드 저장
    (*top) = (*top)->next;       // top을 다음 노드로 이동
    free(del);                   // 제거된 노드의 메모리 해제
    return data;                 // 저장된 문자 반환
}

// Enqueue 함수
//큐의 rear에 새로운 문자를 추가하는 함수
Queue* Enqueue (Queue* que, char value){
    // 큐가 비어있는 경우 (첫 번째 노드 추가)
    if (que->front==NULL){
        Node* newnode=(Node*)malloc(sizeof(Node));  // 새로운 노드에 메모리 할당
        newnode->data=value;                        // 문자 저장
        newnode->next=NULL;                         // 다음 노드가 없음
        que->front=newnode;                         // 이 노드가 front가 됨
        que->rear=newnode;                          // 이 노드가 rear가 됨
        return que;
    }
    
    // 큐에 이미 노드가 있는 경우
    Node* newnode=(Node*)malloc(sizeof(Node));      // 새로운 노드에 메모리 할당
    newnode->data=value;                            // 문자 저장
    newnode->next=NULL;                             // 새로운 노드가 끝이 됨
    que->rear->next=newnode;                        // 기존 rear의 다음에 연결
    que->rear=newnode;                              // 새로운 노드가 rear가 됨
    return que;
}

//deq 함수
//큐의 front에서 문자를 제거하고 반환하는 함수
char deq (Node** front){
    char data=(*front)->data;   // 현재 front의 문자 데이터 저장
    Node* del=(*front);         // 제거할 노드 저장
    (*front)=(*front)->next;    // front를 다음 노드로 이동
    free(del);                  // 제거된 노드의 메모리 해제
    return data;                // 저장된 문자 반환
}