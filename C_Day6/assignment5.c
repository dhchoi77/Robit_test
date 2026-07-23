#include <stdio.h>
#include <stdlib.h>

#define STR_LEN 30

/* ===== 구조체 정의 ===== */
/* 과제 요구 format : 나라, 도, 시, 구 -> 4개 필드 */
typedef struct {
    char country[STR_LEN];   /* 나라 */
    char province[STR_LEN];  /* 도   */
    char city[STR_LEN];      /* 시   */
    char district[STR_LEN];  /* 구   */
} Adress;

typedef struct {
    int   student_id;        /* 번호 */
    char  name[STR_LEN];     /* 이름 */
    Adress adress;           /* 주소 */
    float score;             /* 성적 */
} Student;

/* 정렬 / 검색 기준 */
enum {
    KEY_ID = 1, KEY_NAME, KEY_COUNTRY, KEY_PROVINCE,
    KEY_CITY, KEY_DISTRICT, KEY_SCORE
};

/* ===== 함수 원형 ===== */
/* 문자열 직접 구현 */
int  DIY_strcmp(const char s1[], const char s2[]);
void DIY_strcpy(char dest[], const char src[]);

/* 입력 예외처리 */
void clear_buffer(void);
int  input_int(const char* msg);
float input_float(const char* msg);
void input_str(const char* msg, char out[], int len);

/* 기능 */
int  cmp_student(const Student* a, const Student* b, int key);
int  select_key(void);
void stu_print(const Student* list, int size);
void stu_sort(Student* list, int size, int key);
void stu_search(const Student* list, int size);
void stu_add(Student** list, int* size, int* capacity);
void stu_del(Student* list, int* size);
void stu_save(const Student* list, int size);
void stu_read(Student** list, int* size, int* capacity);

/* ===== main ===== */
int main(void)
{
    int capacity = 4;                 /* 할당된 칸 수 */
    int size = 0;                     /* 실제 학생 수 */
    Student* list = (Student*)malloc(sizeof(Student) * capacity);
    char cmd[STR_LEN];

    if (list == NULL) {
        printf("메모리 할당 실패\n");
        return 1;
    }

    while (1) {
        printf("\n[명령] stu_add / stu_del / stu_sort / stu_search / "
               "stu_print / stu_save / stu_read / exit\n");
        input_str("입력: ", cmd, STR_LEN);

        if (DIY_strcmp(cmd, "stu_add") == 0) {
            stu_add(&list, &size, &capacity);
        }
        else if (DIY_strcmp(cmd, "stu_del") == 0) {
            stu_del(list, &size);
        }
        else if (DIY_strcmp(cmd, "stu_sort") == 0) {
            int key = select_key();
            if (key != 0) {
                stu_sort(list, size, key);
                stu_print(list, size);
            }
        }
        else if (DIY_strcmp(cmd, "stu_search") == 0) {
            stu_search(list, size);
        }
        else if (DIY_strcmp(cmd, "stu_print") == 0) {
            stu_print(list, size);
        }
        else if (DIY_strcmp(cmd, "stu_save") == 0) {
            stu_save(list, size);
        }
        else if (DIY_strcmp(cmd, "stu_read") == 0) {
            stu_read(&list, &size, &capacity);
        }
        else if (DIY_strcmp(cmd, "exit") == 0) {
            break;
        }
        else {
            printf("ERROR : 없는 명령입니다.\n");
        }
    }

    free(list);                       /* 프로그램 종료 시 반드시 해제 */
    return 0;
}

/* ===== 문자열 함수 직접 구현 ===== */
/* s1 > s2 : 1,  s1 == s2 : 0,  s1 < s2 : -1 */
int DIY_strcmp(const char s1[], const char s2[])
{
    int i = 0;
    while (s1[i] != '\0' && s2[i] != '\0') {
        if ((unsigned char)s1[i] < (unsigned char)s2[i]) return -1;
        if ((unsigned char)s1[i] > (unsigned char)s2[i]) return 1;
        i++;
    }
    if (s1[i] == '\0' && s2[i] == '\0') return 0;
    return (s1[i] == '\0') ? -1 : 1;      /* 짧은 쪽이 앞 */
}

void DIY_strcpy(char dest[], const char src[])
{
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

/* ===== 입력 예외처리 ===== */
void clear_buffer(void)
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF)
        ;
}

/* 정수만 받을 때까지 반복 (문자 입력 방지) */
int input_int(const char* msg)
{
    int value;
    while (1) {
        printf("%s", msg);
        if (scanf("%d", &value) == 1) {
            clear_buffer();
            return value;
        }
        printf("ERROR : 숫자를 입력하세요.\n");
        clear_buffer();
    }
}

/* 실수만 받을 때까지 반복 */
float input_float(const char* msg)
{
    float value;
    while (1) {
        printf("%s", msg);
        if (scanf("%f", &value) == 1) {
            clear_buffer();
            return value;
        }
        printf("ERROR : 숫자를 입력하세요.\n");
        clear_buffer();
    }
}

void input_str(const char* msg, char out[], int len)
{
    printf("%s", msg);
    if (scanf("%29s", out) != 1) {
        out[0] = '\0';
    }
    clear_buffer();
    (void)len;
}

/* ===== 기준 선택 메뉴 ===== */
int select_key(void)
{
    int key;
    printf("기준 선택 [1]번호 [2]이름 [3]나라 [4]도 [5]시 [6]구 [7]성적\n");
    key = input_int("번호 입력: ");
    if (key < 1 || key > 7) {
        printf("ERROR : 1~7 사이로 입력하세요.\n");
        return 0;
    }
    return key;
}

/* ===== 비교 함수 (정렬 기준별) ===== */
int cmp_student(const Student* a, const Student* b, int key)
{
    switch (key) {
    case KEY_ID:
        if (a->student_id < b->student_id) return -1;
        if (a->student_id > b->student_id) return 1;
        return 0;
    case KEY_NAME:     return DIY_strcmp(a->name, b->name);
    case KEY_COUNTRY:  return DIY_strcmp(a->adress.country,  b->adress.country);
    case KEY_PROVINCE: return DIY_strcmp(a->adress.province, b->adress.province);
    case KEY_CITY:     return DIY_strcmp(a->adress.city,     b->adress.city);
    case KEY_DISTRICT: return DIY_strcmp(a->adress.district, b->adress.district);
    case KEY_SCORE:
        if (a->score < b->score) return -1;
        if (a->score > b->score) return 1;
        return 0;
    }
    return 0;
}

/* ===== 출력 ===== */
void stu_print(const Student* list, int size)
{
    if (size == 0) {
        printf("등록된 학생이 없습니다.\n");
        return;
    }
    printf("\n%-6s %-10s %-8s %-8s %-8s %-8s %6s\n",
           "번호", "이름", "나라", "도", "시", "구", "성적");
    for (int i = 0; i < size; i++) {
        printf("%-6d %-10s %-8s %-8s %-8s %-8s %6.2f\n",
               list[i].student_id, list[i].name,
               list[i].adress.country, list[i].adress.province,
               list[i].adress.city, list[i].adress.district,
               list[i].score);
    }
}

/* ===== 1. 정렬 (선택 정렬) ===== */
void stu_sort(Student* list, int size, int key)
{
    for (int i = 0; i < size - 1; i++) {
        int min = i;
        for (int j = i + 1; j < size; j++) {
            if (cmp_student(&list[j], &list[min], key) < 0)
                min = j;
        }
        if (min != i) {                       /* 구조체는 통째로 교환 가능 */
            Student tmp = list[i];
            list[i] = list[min];
            list[min] = tmp;
        }
    }
    printf("정렬 완료.\n");
}

/* ===== 2. 찾기 : 조건에 맞는 모든 학생 이름 출력 ===== */
void stu_search(const Student* list, int size)
{
    int key = select_key();
    int found = 0;
    if (key == 0) return;

    if (key == KEY_ID) {
        int target = input_int("찾을 번호: ");
        for (int i = 0; i < size; i++)
            if (list[i].student_id == target) {
                printf("%s\n", list[i].name);
                found++;
            }
    }
    else if (key == KEY_SCORE) {
        float target = input_float("찾을 성적: ");
        for (int i = 0; i < size; i++) {
            float d = list[i].score - target;
            if (d < 0) d = -d;
            if (d < 0.0001f) {                 /* 실수는 오차 범위로 비교 */
                printf("%s\n", list[i].name);
                found++;
            }
        }
    }
    else {
        char target[STR_LEN];
        input_str("찾을 값: ", target, STR_LEN);
        for (int i = 0; i < size; i++) {
            const char* field;
            if      (key == KEY_NAME)     field = list[i].name;
            else if (key == KEY_COUNTRY)  field = list[i].adress.country;
            else if (key == KEY_PROVINCE) field = list[i].adress.province;
            else if (key == KEY_CITY)     field = list[i].adress.city;
            else                          field = list[i].adress.district;

            if (DIY_strcmp(field, target) == 0) {
                printf("%s\n", list[i].name);
                found++;
            }
        }
    }

    if (found == 0) printf("해당하는 학생이 없습니다.\n");
    else            printf("총 %d명 찾음.\n", found);
}

/* ===== 3-1. 추가 ===== */
void stu_add(Student** list, int* size, int* capacity)
{
    Student* s;

    /* 자리가 부족하면 용량을 2배로 확장 */
    if (*size >= *capacity) {
        int new_cap = (*capacity == 0) ? 4 : (*capacity) * 2;
        Student* tmp = (Student*)realloc(*list, sizeof(Student) * new_cap);
        if (tmp == NULL) {
            printf("메모리 재할당 실패\n");
            return;                            /* 기존 데이터는 그대로 유지 */
        }
        *list = tmp;
        *capacity = new_cap;
    }

    s = &(*list)[*size];                       /* 새로 채울 칸 */

    s->student_id = input_int("번호: ");
    input_str("이름: ", s->name, STR_LEN);
    input_str("나라: ", s->adress.country,  STR_LEN);
    input_str("도  : ", s->adress.province, STR_LEN);
    input_str("시  : ", s->adress.city,     STR_LEN);
    input_str("구  : ", s->adress.district, STR_LEN);
    s->score = input_float("성적: ");

    (*size)++;
    printf("추가 완료. (현재 %d명)\n", *size);
}

/* ===== 3-2. 삭제 : 중복이면 선택해서 삭제 ===== */
void stu_del(Student* list, int* size)
{
    int idx[100];
    int found = 0;
    int target_id;
    int del;

    if (*size == 0) {
        printf("등록된 학생이 없습니다.\n");
        return;
    }

    target_id = input_int("삭제할 학생 번호: ");

    for (int i = 0; i < *size && found < 100; i++)
        if (list[i].student_id == target_id)
            idx[found++] = i;

    if (found == 0) {
        printf("해당 번호의 학생이 없습니다.\n");
        return;
    }

    if (found == 1) {
        del = idx[0];
    }
    else {                                     /* 중복 -> 목록 보여주고 선택 */
        int pick;
        printf("중복된 학생이 %d명 있습니다.\n", found);
        for (int i = 0; i < found; i++) {
            int k = idx[i];
            printf("[%d] %d %s %s %s %s %s %.2f\n", i + 1,
                   list[k].student_id, list[k].name,
                   list[k].adress.country, list[k].adress.province,
                   list[k].adress.city, list[k].adress.district,
                   list[k].score);
        }
        pick = input_int("삭제할 항목 번호: ");
        if (pick < 1 || pick > found) {
            printf("ERROR : 범위를 벗어났습니다.\n");
            return;
        }
        del = idx[pick - 1];
    }

    for (int i = del; i < *size - 1; i++)      /* 뒤 원소를 한 칸씩 당김 */
        list[i] = list[i + 1];
    (*size)--;
    printf("삭제 완료. (현재 %d명)\n", *size);
}

/* ===== 4-1. 파일 저장 ===== */
void stu_save(const Student* list, int size)
{
    char filename[STR_LEN];
    FILE* fp;

    input_str("저장할 파일 이름: ", filename, STR_LEN);
    fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("ERROR : 파일을 열 수 없습니다.\n");
        return;
    }

    fprintf(fp, "%d\n", size);                 /* 첫 줄에 학생 수 */
    for (int i = 0; i < size; i++) {
        fprintf(fp, "%d %s %s %s %s %s %.2f\n",
                list[i].student_id, list[i].name,
                list[i].adress.country, list[i].adress.province,
                list[i].adress.city, list[i].adress.district,
                list[i].score);
    }
    fclose(fp);
    printf("%s 에 저장 완료. (%d명)\n", filename, size);
}

/* ===== 4-2. 파일 불러오기 ===== */
void stu_read(Student** list, int* size, int* capacity)
{
    char filename[STR_LEN];
    FILE* fp;
    int n;

    input_str("불러올 파일 이름: ", filename, STR_LEN);
    fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("ERROR : 파일을 열 수 없습니다.\n");
        return;
    }

    if (fscanf(fp, "%d", &n) != 1 || n < 0) {  /* 파일 형식 예외처리 */
        printf("ERROR : 파일 형식이 잘못되었습니다.\n");
        fclose(fp);
        return;
    }

    if (n > *capacity) {                       /* 필요하면 확장 */
        Student* tmp = (Student*)realloc(*list, sizeof(Student) * n);
        if (tmp == NULL) {
            printf("메모리 재할당 실패\n");
            fclose(fp);
            return;
        }
        *list = tmp;
        *capacity = n;
    }

    for (int i = 0; i < n; i++) {
        Student* s = &(*list)[i];
        if (fscanf(fp, "%d %29s %29s %29s %29s %29s %f",
                   &s->student_id, s->name,
                   s->adress.country, s->adress.province,
                   s->adress.city, s->adress.district,
                   &s->score) != 7) {
            printf("ERROR : %d번째 줄이 손상되었습니다.\n", i + 1);
            *size = i;
            fclose(fp);
            return;
        }
    }

    *size = n;
    fclose(fp);
    printf("%s 에서 %d명 불러옴.\n", filename, n);
}