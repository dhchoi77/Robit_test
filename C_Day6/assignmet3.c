#include<stdio.h>
#include<stdlib.h>

// 큐의 각 노드를 나타내는 구조체
typedef struct _Node{
    int data;              // 저장할 정수 값
    struct _Node* next;    // 다음 노드를 가리키는 포인터
}Node;

// 큐 전체를 관리하는 구조체
typedef struct {
    Node* front;   // 큐의 앞부분 (Dequeue할 위치)
    Node* rear;    // 큐의 뒷부분 (Enqueue할 위치)
    int size;      // 현재 큐에 저장된 원소의 개수
}Queue;

// 함수 선언부
Queue* Enqueue (Queue* que,int value);      // 큐에 값 추가
Node* Dequeue (Queue* que);                 // 큐에서 값 제거
void size (int size);                       // 큐 크기 출력
void front (Node* front);                   // 큐의 front 값 출력
void rear (Node* rear);                     // 큐의 rear 값 출력
void isEmpty (int size);                    // 큐가 비어있는지 확인
void printQueue (Node* front);              // 큐의 모든 값 출력
void free_Queue(Queue* que);                // 큐의 모든 노드 메모리 해제
int DIY_strcmp(char name1[30], char name2[30]); // 문자열 비교 함수

int main (void){
    Queue que;
    que.front=NULL;    // 초기 상태: 큐가 비어있음
    que.rear=NULL;
    que.size=0;        // 초기 크기: 0
    
    printf("명령어: Enqueue, Dequeue, size, front, rear, isEmpty, printQueue, end\n");
    
    // 무한 루프: 사용자가 'end'를 입력할 때까지 반복
    while (1){
        printf("입력: ");
        char str[30];        // 명령어를 입력받을 문자열
        int value;           // Enqueue 명령어의 값
        
        scanf("%s",str);     // 사용자로부터 명령어 입력
        
        // Enqueue 명령어: 큐의 rear에 새로운 값을 추가
        if (DIY_strcmp(str,"Enqueue")==0){
            scanf(" %d",&value);             // 추가할 값 입력
            que=(*Enqueue(&que,value));      // 큐에 값 추가
            que.size++;                      // 크기 증가
        }
        // Dequeue 명령어: 큐의 front에서 값 제거
        else if (DIY_strcmp(str,"Dequeue")==0){
            que.front=Dequeue(&que);         // 큐에서 front 값 제거 및 반환
        }
        // size 명령어: 현재 큐의 크기 출력
        else if (DIY_strcmp(str,"size")==0){
            size(que.size);
        }
        // front 명령어: 큐의 front 값 출력 (제거하지 않음)
        else if (DIY_strcmp(str,"front")==0){
            front(que.front);
        }
        // rear 명령어: 큐의 rear 값 출력 (제거하지 않음)
        else if (DIY_strcmp(str,"rear")==0){
            rear(que.rear);
        }
        // isEmpty 명령어: 큐가 비어있는지 확인
        else if (DIY_strcmp(str,"isEmpty")==0){
            isEmpty(que.size);
        }
        // printQueue 명령어: 큐의 모든 값을 front부터 rear 방향으로 출력
        else if (DIY_strcmp(str,"printQueue")==0){
            printQueue(que.front);
        }
        // end 명령어: 프로그램 종료
        else if (DIY_strcmp(str,"end")==0){
            free_Queue(&que);                // 모든 노드의 메모리 해제
            printf("프로그램을 종료하겠습니다.\n");
            return 0;
        }
        // 위의 명령어 중 하나가 아니면 오류 출력
        else {
            printf("input ERROR\n");
        }
    }
    return 0;
}
Queue* Enqueue (Queue* que,int value){
    // 큐가 비어있는 경우 (첫 번째 노드 추가)
    if (que->front==NULL){
        Node* newnode=(Node*)malloc(sizeof(Node));  // 새로운 노드에 메모리 할당
        newnode->data=value;                        // 데이터 저장
        newnode->next=NULL;                         // 다음 노드가 없음
        que->front=newnode;                         // 이 노드가 front가 됨
        que->rear=newnode;                          // 이 노드가 rear가 됨
        return que;
    }
    
    // 큐에 이미 노드가 있는 경우
    Node* newnode=(Node*)malloc(sizeof(Node));      // 새로운 노드에 메모리 할당
    newnode->data=value;                            // 데이터 저장
    newnode->next=NULL;                             // 새로운 노드가 끝이 됨
    que->rear->next=newnode;                        // 기존 rear의 다음에 연결
    que->rear=newnode;                              // 새로운 노드가 rear가 됨
    return que;
}
Node* Dequeue (Queue* que){
    // 큐가 비어있는 경우 오류 메시지 출력
    if (que->front==NULL){
        printf("Queue is enpty\n");
        return NULL;
    }
    
    printf("data: %d\n",que->front->data);   // 제거할 값 출력
    Node* del=que->front;                    // 제거할 노드 저장
    que->front=que->front->next;             // front를 다음 노드로 이동
    free(del);                               // 제거된 노드의 메모리 해제
    que->size--;                             // 큐의 크기 감소
    
    // 큐가 완전히 비어진 경우 rear도 NULL로 설정
    if (que->front == NULL){
        que->rear = NULL;
    }
    
    return que->front;                       // 새로운 front 반환
}
void size (int size){
    printf("size: %d\n",size);
}
void front (Node* front){
    if (front!=NULL){
        printf("front: %d\n",front->data);   // front 값 출력
    }
    else{
        printf("Queue is empty.\n");         // 큐가 비어있으면 오류 메시지
    }
}
void rear (Node* rear){
    if (rear!=NULL){
        printf("rear: %d\n",rear->data);     // rear 값 출력
    }
    else{
        printf("Queue is empty.\n");         // 큐가 비어있으면 오류 메시지
    }
}
void isEmpty (int size){
    if (size==0){
        printf("true\n");       // 크기가 0이면 큐가 비어있음
    }
    else{
        printf("flase\n");      // ※ 오타: "flase" → "false"로 수정 필요
    }
}
void printQueue (Node* front){
    // 큐가 비어있는 경우 오류 메시지 출력
    if (front==NULL){
        printf("Queue is empty");
        return;
    }
    
    // front부터 rear까지 모든 노드의 값을 순서대로 출력
    for(Node* p=front;p!=NULL;p=p->next){
        printf("%d ",p->data);
    }
    printf("\n");
}
void free_Queue(Queue* que){
    // 큐에 남아있는 모든 노드의 메모리를 해제
    for (int i=0;i<(que->size);i++){
        Node* next=que->front->next;   // 다음 노드 저장 (현재 노드 해제 후 필요)
        free(que->front);              // 현재 노드의 메모리 해제
        que->front=next;               // front를 다음 노드로 이동
    }
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