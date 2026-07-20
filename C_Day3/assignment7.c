#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int compareTag(char open[], char close[]) {
    int i = 1, j = 2;
    while (open[i] != '>' && close[j] != '>') {
        if (open[i] != close[j]) return 0;
        i++;
        j++;
    }
    return (open[i] == '>' && close[j] == '>');
}

int main (void){
    int capacity = 20;    // 초기 크기
    int len = 0;           // 현재까지 저장한 글자 수
    char *str = (char *)malloc(sizeof(char)*capacity);
    char ch;
    printf("입력: ");
    while (scanf("%c", &ch) == 1 && ch != EOF) {
        if (ch == '\n') break;
    // ① 공간 부족한지 체크
        if (len==capacity) {
            capacity *=2;
            str = (char *)realloc(str, capacity*sizeof(char));
        }
    
        // ② 글자 저장
        str[len++] = ch;
    }

    str[len] = '\0';  // 문자열 끝 표시
    

    char **words = (char**) malloc(sizeof(char*) * len);
    for(int i = 0; i < len; i++){
        words[i] = (char*) malloc(sizeof(char) * len);
    }
    int i = 0;
    int k = 0;  // ★ words[i] 안에서의 위치
    for (int j=0; j<len; j++) {
        if (str[j] == '>') {
            words[i][k++] = str[j];  // '>'까지 저장
            words[i][k] = '\0';      // 문자열 끝
            i++;                      // 다음 단어로
            k = 0;                    // 로컬 인덱스 리셋
        }
        else if(str[j]=='<'&&str[j-1]!='>'){
              // '>'까지 저장
            words[i][k++] = '\0';      // 문자열 끝
            
            i++;                      // 다음 단어로
            k = 0; 
            words[i][k++] = str[j];
        }
        else {
            words[i][k++] = str[j];
        }
    }
    words[i][k] = '\0';  // 마지막 단어 종결
    
    int stack_cap = 10;                                          // 스택 초기 크기
    int top = -1;                                                // 스택 top (비어있으면 -1)
    char **tag_stack = (char**)malloc(sizeof(char*) * stack_cap);
    int error = 0;                                               // 오류 발생 플래그

    // ★ 추가: 검증 루프 (words 배열 전체 순회)
    for (int m = 1; m <= i; m++) {
        // 여는 태그: <div>, <p>, <b>
        if (words[m][0] == '<' && words[m][1] != '/') {
            top++;
            // 스택 가득 차면 2배로 확장
            if (top >= stack_cap) {
                stack_cap *= 2;
                tag_stack = (char**)realloc(tag_stack, sizeof(char*) * stack_cap);
            }
            tag_stack[top] = words[m];   // push
        }
        // 닫는 태그: </div>, </p>, </b>
        else if (words[m][0] == '<' && words[m][1] == '/') {
            // 스택 비어있는데 닫는 태그 나옴 → 오류
            if (top == -1) {
                error = 1;
                break;
            }
            // 스택 top과 이름 비교
            if (!compareTag(tag_stack[top], words[m])) {
                error = 1;
                break;
            }
            top--;   // pop
        }
    }

    // ★ 추가: 끝났는데 스택에 남아있으면 오류 (안 닫힌 태그)
    if (top != -1) error = 1;

    // ═══════════════════════════════════════════════
    // ★ 추가: 오류 판정 후 출력
    // ═══════════════════════════════════════════════
    if (error) {
        printf("오류: 잘못된 태그 구조입니다.\n");
    }
    else {
        // 기존 탭 출력 로직 (검증 통과했을 때만 실행)
        int tab_stack = -1;
        int tab_switch = 1;
        for (int m = 1; m <= i; m++) {
            if (words[m][0] == '<' && words[m][1] != '/') {
                tab_switch = 1;
            }
            else if (words[m][0] == '<' && words[m][1] == '/') {
                tab_switch = -1;
            }
            tab_stack = tab_stack + tab_switch;
            for (int p = 0; p < tab_stack; p++) {
                printf("   ");
            }
            printf("%s\n", words[m]);
        }
    }

    // 메모리 해제
    for (int idx = 0; idx < len; idx++) {
        free(words[idx]);
    }
    free(words);
    free(str);
    free(tag_stack);   // ★ 추가: 스택 메모리 해제
    return 0;
}