#include <stdio.h>
#include <stdlib.h>

/* =========================================================
 *  과제 7 : 단순 연결 리스트 (Singly Linked List) 구현
 *  조건 : 화살표 연산자(->)를 사용하지 않는다.
 *         -> 대신 (*포인터).멤버  형태로 접근한다.
 *         ( p->data  는  (*p).data 와 완전히 같은 의미 )
 *  Data 자료형 : 자유 -> int 사용
 * ========================================================= */

/* 노드 구조체 : 데이터 + 다음 노드 주소 */
typedef struct Node {
    int data;
    struct Node* next;
} Node;

/* 리스트 구조체 : head(시작 노드)와 length(길이)를 함께 관리 */
typedef struct {
    Node* head;
    int length;
} List;

/* 프로그램 시작 시 가장 먼저 출력되는 사용설명서 */
void print_manual(void);

/* ---------- 기본 준비 함수 ---------- */

/* 리스트 초기화 */
void init(List* list);

/* 새 노드 생성 (동적 할당) */
Node* create_node(int value);


/* ---------- insert 계열 ---------- */

/* 맨 앞에 노드 추가 */
void insert_first(List* list, int value);

/* 맨 뒤에 노드 추가 */
void insert_back(List* list, int value);

/* 원하는 index 위치에 노드 추가 (0부터 시작) */
void insert_by_index(List* list, int index, int value);

/* 특정 data를 가진 노드 "뒤에" 새 노드 추가 */
void insert_by_data(List* list, int prev_data, int value);


/* ---------- delete 계열 ---------- */

/* 맨 앞 노드 삭제 */
void delete_first(List* list) ;

/* 맨 뒤 노드 삭제 */
void delete_back(List* list) ;

/* 원하는 index의 노드 삭제 */
void delete_by_index(List* list, int index) ;

/* 특정 data를 가진 노드 삭제 (처음 만난 것 하나) */
void delete_by_data(List* list, int value);

/* ---------- 조회 계열 ---------- */

/* data로 찾아 index 반환 (없으면 -1) */
int get_entry_by_data(List* list, int value);

/* index로 찾아 data 반환 */
int get_entry_by_index(List* list, int index);

/* 리스트 전체 길이 반환 */
int get_length(List* list);

/* 리스트의 모든 요소 출력 */
void print_list(List* list);

/* ---------- reverse ---------- */

/* 리스트를 역순으로 만들기 */
void reverse(List* list);

/* ---------- 마무리(메모리 정리) ---------- */

/* 리스트 전체 메모리 해제 */
void free_list(List* list);

//문자열 비교
int DIY_strcmp(char name1[30], char name2[30]);
/* =========================================================
 *  테스트용 main
 * ========================================================= */
int main(void) {
    List list;
    init(&list);
    print_manual();
    while(1){
        printf("입력: ");
        char gimic[30];
        scanf("%s",gimic);
        if (DIY_strcmp(gimic,"insert_back")==0){
            int gimic_data;
            scanf("%d",&gimic_data);
            insert_back(&list,gimic_data);
        }
        else if (DIY_strcmp(gimic,"insert_first")==0){
            int gimic_data;
            scanf("%d",&gimic_data);
            insert_first(&list,gimic_data);
        }
        else if (DIY_strcmp(gimic,"insert_by_index")==0){
            int gimic_index,gimic_data;
            scanf("%d %d",&gimic_index,&gimic_data);
            insert_by_index(&list,gimic_index,gimic_data);
        }
        else if (DIY_strcmp(gimic,"insert_by_data")==0){
            int gimic_data_before,gimic_data;       //gimic_data_before은 삽입할 데이터의 앞데이터 ++ 설명필요
            scanf("%d %d",&gimic_data_before,&gimic_data);
            insert_by_data(&list,gimic_data_before,gimic_data);
        }
        else if (DIY_strcmp(gimic,"get_length")==0){
            printf("현재 길이 : %d\n", get_length(&list));
        }
        else if (DIY_strcmp(gimic,"get_entry_by_index")==0){
            int idx;
            scanf("%d",&idx);
            printf("data : %d\n", get_entry_by_index(&list, idx));
        }
        else if (DIY_strcmp(gimic,"get_entry_by_data")==0){
            int val;
            scanf("%d",&val);
            printf("index : %d\n", get_entry_by_data(&list, val));
        }
        else if (DIY_strcmp(gimic,"delete_first")==0){
            delete_first(&list);
        }
        else if (DIY_strcmp(gimic,"delete_back")==0){
            delete_back(&list);
        }
        else if (DIY_strcmp(gimic,"delete_by_index")==0){
            int gimic_data_index;
            scanf("%d",&gimic_data_index);
            delete_by_index(&list, gimic_data_index);
        }
        else if (DIY_strcmp(gimic,"delete_by_data")==0){
            int gimic_data;
            scanf("%d",&gimic_data);
            delete_by_data(&list, gimic_data);
        }
        else if (DIY_strcmp(gimic,"reverse")==0){
            reverse(&list);
        }
        else if (DIY_strcmp(gimic,"print_list")==0){
            print_list(&list);
        }
        else{
            free_list(&list);
            return 0;
        }

    }
    
}
void print_manual(void) {
    printf("            단순 연결 리스트 (Singly Linked List) 사용설명서     \n");
    printf(" [ 추가 ]\n");
    printf("   insert_first (value)        : 맨 앞에 노드 추가\n");
    printf("   insert_back  (value)        : 맨 뒤에 노드 추가\n");
    printf("   insert_by_index(index,value): 원하는 index 위치에 추가\n");
    printf("   insert_by_data (data, value): 특정 data 노드 뒤에 추가\n");
    printf(" [ 삭제 ]\n");
    printf("   delete_first ()             : 맨 앞 노드 삭제\n");
    printf("   delete_back  ()             : 맨 뒤 노드 삭제\n");
    printf("   delete_by_index(index)      : 원하는 index 노드 삭제\n");
    printf("   delete_by_data (data)       : 특정 data 노드 삭제\n");
    printf(" [ 조회 ]\n");
    printf("   get_entry_by_data (data)    : data로 찾아 index 반환\n");
    printf("   get_entry_by_index(index)   : index로 찾아 data 반환\n");
    printf("   get_length()                : 리스트 전체 길이 반환\n");
    printf("   print_list()                : 모든 요소 출력\n");
    printf(" [ 기타 ]\n");
    printf("   reverse()                   : 리스트를 역순으로 만들기\n");
    printf("-------------------------------------------------------------\n");
    printf(" * index는 0부터 시작합니다. (첫 번째 노드 = index 0)\n");
    printf(" * Data 자료형은 int 입니다.\n");
    printf("=============================================================\n\n");
}

/* 리스트 초기화 */
void init(List* list) {
    (*list).head = NULL;
    (*list).length = 0;
}

/* 새 노드 생성 (동적 할당) */
Node* create_node(int value) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        printf("메모리 할당 실패\n");
        exit(1);
    }
    (*new_node).data = value;
    (*new_node).next = NULL;
    return new_node;
}


/* ---------- insert 계열 ---------- */

/* 맨 앞에 노드 추가 */
void insert_first(List* list, int value) {
    Node* new_node = create_node(value);
    (*new_node).next = (*list).head;   /* 새 노드가 기존 head를 가리킴 */
    (*list).head = new_node;           /* head를 새 노드로 갱신 */
    (*list).length++;
}

/* 맨 뒤에 노드 추가 */
void insert_back(List* list, int value) {
    Node* new_node = create_node(value);
    if ((*list).head == NULL) {        /* 리스트가 비어 있으면 */
        (*list).head = new_node;
    } else {
        Node* cur = (*list).head;
        while ((*cur).next != NULL) {  /* 마지막 노드까지 이동 */
            cur = (*cur).next;
        }
        (*cur).next = new_node;        /* 마지막 노드 뒤에 연결 */
    }
    (*list).length++;
}

/* 원하는 index 위치에 노드 추가 (0부터 시작) */
void insert_by_index(List* list, int index, int value) {
    if (index < 0 || index > (*list).length) {
        printf("잘못된 index 입니다.\n");
        return;
    }
    if (index == 0) {                  /* 맨 앞이면 insert_first 재사용 */
        insert_first(list, value);
        return;
    }
    Node* cur = (*list).head;
    int i;
    for (i = 0; i < index - 1; i++) {  /* 삽입 위치 바로 앞 노드까지 이동 */
        cur = (*cur).next;
    }
    Node* new_node = create_node(value);
    (*new_node).next = (*cur).next;
    (*cur).next = new_node;
    (*list).length++;
}

/* 특정 data를 가진 노드 "뒤에" 새 노드 추가 */
void insert_by_data(List* list, int prev_data, int value) {
    Node* cur = (*list).head;
    while (cur != NULL && (*cur).data != prev_data) {
        cur = (*cur).next;
    }
    if (cur == NULL) {                 /* 해당 data를 못 찾음 */
        printf("%d 데이터를 찾을 수 없습니다.\n", prev_data);
        return;
    }
    Node* new_node = create_node(value);
    (*new_node).next = (*cur).next;
    (*cur).next = new_node;
    (*list).length++;
}


/* ---------- delete 계열 ---------- */

/* 맨 앞 노드 삭제 */
void delete_first(List* list) {
    if ((*list).head == NULL) {
        printf("리스트가 비어 있습니다.\n");
        return;
    }
    Node* target = (*list).head;
    (*list).head = (*target).next;     /* head를 다음 노드로 */
    free(target);                      /* 메모리 해제 */
    (*list).length--;
}

/* 맨 뒤 노드 삭제 */
void delete_back(List* list) {
    if ((*list).head == NULL) {
        printf("리스트가 비어 있습니다.\n");
        return;
    }
    if ((*(*list).head).next == NULL) { /* 노드가 하나뿐이면 */
        free((*list).head);
        (*list).head = NULL;
        (*list).length--;
        return;
    }
    Node* cur = (*list).head;
    while ((*(*cur).next).next != NULL) { /* 마지막 바로 앞 노드까지 이동 */
        cur = (*cur).next;
    }
    free((*cur).next);                 /* 마지막 노드 해제 */
    (*cur).next = NULL;
    (*list).length--;
}

/* 원하는 index의 노드 삭제 */
void delete_by_index(List* list, int index) {
    if (index < 0 || index >= (*list).length) {
        printf("잘못된 index 입니다.\n");
        return;
    }
    if (index == 0) {
        delete_first(list);
        return;
    }
    Node* cur = (*list).head;
    int i;
    for (i = 0; i < index - 1; i++) {  /* 삭제 대상 앞 노드까지 이동 */
        cur = (*cur).next;
    }
    Node* target = (*cur).next;
    (*cur).next = (*target).next;      /* 대상을 건너뛰고 연결 */
    free(target);
    (*list).length--;
}

/* 특정 data를 가진 노드 삭제 (처음 만난 것 하나) */
void delete_by_data(List* list, int value) {
    if ((*list).head == NULL) {
        printf("리스트가 비어 있습니다.\n");
        return;
    }
    if ((*(*list).head).data == value) { /* head가 삭제 대상이면 */
        delete_first(list);
        return;
    }
    Node* cur = (*list).head;
    while ((*cur).next != NULL && (*(*cur).next).data != value) {
        cur = (*cur).next;
    }
    if ((*cur).next == NULL) {
        printf("%d 데이터를 찾을 수 없습니다.\n", value);
        return;
    }
    Node* target = (*cur).next;
    (*cur).next = (*target).next;
    free(target);
    (*list).length--;
}


/* ---------- 조회 계열 ---------- */

/* data로 찾아 index 반환 (없으면 -1) */
int get_entry_by_data(List* list, int value) {
    Node* cur = (*list).head;
    int index = 0;
    while (cur != NULL) {
        if ((*cur).data == value) {
            return index;
        }
        cur = (*cur).next;
        index++;
    }
    return -1;
}

/* index로 찾아 data 반환 */
int get_entry_by_index(List* list, int index) {
    if (index < 0 || index >= (*list).length) {
        printf("잘못된 index 입니다.\n");
        return -1;
    }
    Node* cur = (*list).head;
    int i;
    for (i = 0; i < index; i++) {
        cur = (*cur).next;
    }
    return (*cur).data;
}

/* 리스트 전체 길이 반환 */
int get_length(List* list) {
    return (*list).length;
}

/* 리스트의 모든 요소 출력 */
void print_list(List* list) {
    Node* cur = (*list).head;
    printf("[");
    while (cur != NULL) {
        printf("%d", (*cur).data);
        if ((*cur).next != NULL) {
            printf(" -> ");            /* 문자열일 뿐, 연산자 아님 */
        }
        cur = (*cur).next;
    }
    printf("]  (길이: %d)\n", (*list).length);
}


/* ---------- reverse ---------- */

/* 리스트를 역순으로 만들기 */
void reverse(List* list) {
    Node* prev = NULL;
    Node* cur = (*list).head;
    Node* next = NULL;
    while (cur != NULL) {
        next = (*cur).next;   /* 다음 노드 미리 저장 */
        (*cur).next = prev;   /* 화살표 방향 뒤집기 */
        prev = cur;           /* prev 한 칸 전진 */
        cur = next;           /* cur 한 칸 전진 */
    }
    (*list).head = prev;      /* 마지막에 도달한 노드가 새 head */
}


/* ---------- 마무리(메모리 정리) ---------- */

/* 리스트 전체 메모리 해제 */
void free_list(List* list) {
    Node* cur = (*list).head;
    while (cur != NULL) {
        Node* next = (*cur).next;
        free(cur);
        cur = next;
    }
    (*list).head = NULL;
    (*list).length = 0;
}
//문자열 비교
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