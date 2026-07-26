
/* ============================================================
   조류촬영자료 저장 프로그램 - Win32 네이티브 GUI 버전
   
   - 자료구조: LCRS 트리 + 관찰기록 연결리스트
   - GUI: 순수 Win32 API (windows.h)
   
   컴파일:
     cl bird_tree_win32.c /link user32.lib gdi32.lib
   ============================================================ */
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
 
#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "imm32.lib")
 
#define MAX_NAME_LEN 256
#define MAX_NODES 1000
#define SAVE_FILE "birdtree_data.txt"
 
//GUI 상수 
#define NODE_RADIUS 28
#define LEVEL_HEIGHT 110
#define NODE_GAP 30
 
//컨트롤 ID
#define ID_BTN_ADD_ROOT   1001
#define ID_BTN_ADD_CHILD  1002
#define ID_BTN_ADD_PARENT 1003
#define ID_BTN_DELETE     1004
#define ID_BTN_ADD_OBS    1005
#define ID_BTN_PREV_OBS   1006
#define ID_BTN_NEXT_OBS   1007
#define ID_BTN_DEL_OBS    1008
 
// 자료 구조
typedef struct Locate {
    int id;
    int year, month, day;
    char location[MAX_NAME_LEN];
    char memo[MAX_NAME_LEN];
    struct Locate *next;
} Locate;
 
typedef struct Node {
    int id;
    char name[MAX_NAME_LEN];
    Locate *observations;
    int obs_count;
    struct Node *parent;
    struct Node *first_child;
    struct Node *next_sibling;
    //화면 좌표 (렌더링 시 계산)
    int screen_x;
    int screen_y;
} Node;
 
typedef struct {
    Node *lookup[MAX_NODES];
    int node_count;
} TreeManager;
 
TreeManager tm;
static int locate_counter = 0;
 
//전역 상태
Node *g_selected = NULL;          //선택된 노드 
int g_current_obs_page = 0;       //현재 보고 있는 관찰기록 페이지 
HWND g_main_window = NULL;
int g_layout_x = 0;               //레이아웃 계산용 x 커서 

//트리 기본 연산
void init_tree_manager() {
    memset(&tm, 0, sizeof(TreeManager));
}
 
Node *create_node(const char *name) {
    if (tm.node_count >= MAX_NODES) return NULL;
 
    Node *node = (Node *)malloc(sizeof(Node));
    if (!node) return NULL;
 
    node->id = tm.node_count;
    strncpy_s(node->name, MAX_NAME_LEN, name, MAX_NAME_LEN - 1);
    node->observations = NULL;
    node->obs_count = 0;
    node->parent = NULL;
    node->first_child = NULL;
    node->next_sibling = NULL;
    node->screen_x = 0;
    node->screen_y = 0;
 
    tm.lookup[tm.node_count] = node;
    tm.node_count++;
    return node;
}
 
Node *find_node(int id) {
    if (id < 0 || id >= tm.node_count) return NULL;
    return tm.lookup[id];
}
 
void add_child(Node *parent, Node *child) {
    child->parent = parent;
    if (parent->first_child == NULL) {
        parent->first_child = child;
    } else {
        Node *sibling = parent->first_child;
        while (sibling->next_sibling != NULL) {
            sibling = sibling->next_sibling;
        }
        sibling->next_sibling = child;
    }
}
 
int add_root_node(const char *name) {
    Node *node = create_node(name);
    if (!node) return -1;
    return node->id;
}
 
int add_child_node(int parent_id, const char *name) {
    Node *parent = find_node(parent_id);
    if (!parent) return -1;
    Node *child = create_node(name);
    if (!child) return -1;
    add_child(parent, child);
    return child->id;
}
 
int add_parent_node(int target_id, const char *name) {
    Node *target = find_node(target_id);
    if (!target) return -1;
    Node *new_parent = create_node(name);
    if (!new_parent) return -1;
 
    Node *old_parent = target->parent;
    new_parent->first_child = target;
    new_parent->parent = old_parent;
 
    if (old_parent) {
        if (old_parent->first_child == target) {
            new_parent->next_sibling = target->next_sibling;
            old_parent->first_child = new_parent;
        } else {
            Node *prev = old_parent->first_child;
            while (prev && prev->next_sibling != target) {
                prev = prev->next_sibling;
            }
            if (prev) {
                new_parent->next_sibling = target->next_sibling;
                prev->next_sibling = new_parent;
            }
        }
    }
    target->parent = new_parent;
    target->next_sibling = NULL;
    return new_parent->id;
}
 
//노드 삭제 (DFS 재귀)
void free_subtree(Node *node) {
    if (!node) return;
 
    Node *child = node->first_child;
    while (child) {
        Node *next = child->next_sibling;
        free_subtree(child);
        child = next;
    }
 
    Locate *obs = node->observations;
    while (obs) {
        Locate *next_obs = obs->next;
        free(obs);
        obs = next_obs;
    }
 
    if (node->id >= 0 && node->id < MAX_NODES) {
        tm.lookup[node->id] = NULL;
    }
    free(node);
}
 
int delete_node(int target_id) {
    Node *target = find_node(target_id);
    if (!target) return -1;
 
    Node *parent = target->parent;
    if (parent) {
        if (parent->first_child == target) {
            parent->first_child = target->next_sibling;
        } else {
            Node *prev = parent->first_child;
            while (prev && prev->next_sibling != target) {
                prev = prev->next_sibling;
            }
            if (prev) prev->next_sibling = target->next_sibling;
        }
    }
    target->next_sibling = NULL;
    free_subtree(target);
    return 0;
}
//관찰 기록 (연결리스트)
int add_observation(int target_id, int year, int month, int day,
                    const char *location, const char *memo) {
    Node *target = find_node(target_id);
    if (!target) return -1;
 
    Locate *obs = (Locate *)malloc(sizeof(Locate));
    if (!obs) return -1;
 
    obs->id = locate_counter++;
    obs->year = year;
    obs->month = month;
    obs->day = day;
    strncpy_s(obs->location, MAX_NAME_LEN, location ? location : "", MAX_NAME_LEN - 1);
    strncpy_s(obs->memo, MAX_NAME_LEN, memo ? memo : "", MAX_NAME_LEN - 1);
 
    /* 리스트 맨 앞에 추가 (최신이 위) */
    obs->next = target->observations;
    target->observations = obs;
    target->obs_count++;
    return obs->id;
}
 
int delete_observation(int target_id, int obs_id) {
    Node *target = find_node(target_id);
    if (!target) return -1;
 
    Locate *prev = NULL;
    Locate *curr = target->observations;
    while (curr) {
        if (curr->id == obs_id) {
            if (prev) prev->next = curr->next;
            else target->observations = curr->next;
            free(curr);
            target->obs_count--;
            return 0;
        }
        prev = curr;
        curr = curr->next;
    }
    return -1;
}
//파일 불러오기, 저장하기 
void save_node_recursive(FILE *fp, Node *node, int parent_id) {
    if (!node) return;
 
    fprintf(fp, "NODE|%d|%d|%s\n", node->id, parent_id, node->name);
 
    /* 관찰기록 역순 저장 -> 로드시 원래 순서 복원 */
    Locate *obs_arr[1000];
    int obs_cnt = 0;
    Locate *obs = node->observations;
    while (obs && obs_cnt < 1000) {
        obs_arr[obs_cnt++] = obs;
        obs = obs->next;
    }
    for (int i = obs_cnt - 1; i >= 0; i--) {
        fprintf(fp, "OBS|%d|%d|%d|%d|%d|%s|%s\n",
                node->id, obs_arr[i]->id,
                obs_arr[i]->year, obs_arr[i]->month, obs_arr[i]->day,
                obs_arr[i]->location, obs_arr[i]->memo);
    }
 
    Node *child = node->first_child;
    while (child) {
        save_node_recursive(fp, child, node->id);
        child = child->next_sibling;
    }
}
 
void save_to_file() {
    FILE *fp;
    if (fopen_s(&fp, SAVE_FILE, "w") != 0 || !fp) return;
 
    fprintf(fp, "BIRDTREE_V1\n");
    fprintf(fp, "NODE_COUNT|%d\n", tm.node_count);
    fprintf(fp, "OBS_COUNT|%d\n", locate_counter);
 
    for (int i = 0; i < tm.node_count; i++) {
        Node *node = tm.lookup[i];
        if (node && node->parent == NULL) {
            save_node_recursive(fp, node, -1);
        }
    }
    fclose(fp);
}
 
void load_from_file() {
    typedef struct { Node *node; int parent_id; } UnlinkedNode;
    UnlinkedNode unlinked_nodes[MAX_NODES];
    int unlinked_count = 0;
 
    FILE *fp;
    if (fopen_s(&fp, SAVE_FILE, "r") != 0 || !fp) return;
 
    char line[2048];
    int saved_obs_count = 0;
 
    if (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strcmp(line, "BIRDTREE_V1") != 0) { fclose(fp); return; }
    }
    if (fgets(line, sizeof(line), fp)) { /* NODE_COUNT */ }
    if (fgets(line, sizeof(line), fp)) { sscanf_s(line, "OBS_COUNT|%d", &saved_obs_count); }
 
    while (fgets(line, sizeof(line), fp)) {
        line[strcspn(line, "\r\n")] = 0;
        if (strlen(line) == 0) continue;
 
        if (strncmp(line, "NODE|", 5) == 0) {
            int id, parent_id;
            char name[MAX_NAME_LEN];
            memset(name, 0, MAX_NAME_LEN);
 
            char *p = line + 5;
            id = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            parent_id = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            strncpy_s(name, MAX_NAME_LEN, p, MAX_NAME_LEN - 1);
 
            Node *node = create_node(name);
            if (!node) continue;
            if (node->id != id) {
                tm.lookup[node->id] = NULL;
                node->id = id;
                tm.lookup[id] = node;
            }
 
            if (parent_id >= 0) {
                Node *parent = find_node(parent_id);
                if (parent) {
                    add_child(parent, node);
                } else {
                    unlinked_nodes[unlinked_count].node = node;
                    unlinked_nodes[unlinked_count].parent_id = parent_id;
                    unlinked_count++;
                }
            }
        }
        else if (strncmp(line, "OBS|", 4) == 0) {
            int node_id, obs_id, year, month, day;
            char location[MAX_NAME_LEN], memo[MAX_NAME_LEN];
            memset(location, 0, MAX_NAME_LEN);
            memset(memo, 0, MAX_NAME_LEN);
 
            char *p = line + 4;
            node_id = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            obs_id = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            year = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            month = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
            day = atoi(p);
            p = strchr(p, '|'); if (!p) continue; p++;
 
            char *np = strchr(p, '|');
            if (np) {
                int len = (int)(np - p);
                if (len > MAX_NAME_LEN - 1) len = MAX_NAME_LEN - 1;
                strncpy_s(location, MAX_NAME_LEN, p, len);
                p = np + 1;
                strncpy_s(memo, MAX_NAME_LEN, p, MAX_NAME_LEN - 1);
            } else {
                strncpy_s(location, MAX_NAME_LEN, p, MAX_NAME_LEN - 1);
            }
 
            Node *target = find_node(node_id);
            if (target) {
                Locate *obs = (Locate *)malloc(sizeof(Locate));
                if (obs) {
                    obs->id = obs_id;
                    obs->year = year; obs->month = month; obs->day = day;
                    strncpy_s(obs->location, MAX_NAME_LEN, location, MAX_NAME_LEN - 1);
                    strncpy_s(obs->memo, MAX_NAME_LEN, memo, MAX_NAME_LEN - 1);
                    obs->next = target->observations;
                    target->observations = obs;
                    target->obs_count++;
                }
            }
        }
    }
 
    for (int i = 0; i < unlinked_count; i++) {
        Node *parent = find_node(unlinked_nodes[i].parent_id);
        if (parent) add_child(parent, unlinked_nodes[i].node);
    }
 
    if (saved_obs_count > locate_counter) locate_counter = saved_obs_count;
    fclose(fp);
}
 
/* ============================================================
   트리 레이아웃 계산 (DFS)
   각 노드의 screen_x, screen_y를 계산
   ============================================================ */
 
int count_leaves(Node *node) {
    if (!node) return 0;
    if (!node->first_child) return 1;
    int count = 0;
    Node *child = node->first_child;
    while (child) {
        count += count_leaves(child);
        child = child->next_sibling;
    }
    return count;
}
 
//DFS로 x좌표 배치: 리프는 순서대로, 부모는 자식 중앙
int layout_node(Node *node, int depth, int *leaf_cursor) {
    if (!node) return 0;
 
    node->screen_y = 60 + depth * LEVEL_HEIGHT;
 
    if (!node->first_child) {
        //리프 노드: 커서 위치에 배치 
        node->screen_x = 60 + (*leaf_cursor) * (NODE_RADIUS * 2 + NODE_GAP);
        (*leaf_cursor)++;
        return node->screen_x;
    }
 
    //자식들 먼저 배치 (재귀)
    int first_x = -1, last_x = 0;
    Node *child = node->first_child;
    while (child) {
        int cx = layout_node(child, depth + 1, leaf_cursor);
        if (first_x < 0) first_x = cx;
        last_x = cx;
        child = child->next_sibling;
    }
 
    //부모는 자식들의 중앙
    node->screen_x = (first_x + last_x) / 2;
    return node->screen_x;
}
 
void layout_tree() {
    int leaf_cursor = 0;
    for (int i = 0; i < tm.node_count; i++) {
        Node *node = tm.lookup[i];
        if (node && node->parent == NULL) {
            layout_node(node, 0, &leaf_cursor);
        }
    }
}

//그리기 (GDI) - DFS 재귀
void draw_node_recursive(HDC hdc, Node *node) {
    if (!node) return;
 
    //부모-자식 연결선 먼저 그리기
    Node *child = node->first_child;
    while (child) {
        MoveToEx(hdc, node->screen_x, node->screen_y + NODE_RADIUS, NULL);
        LineTo(hdc, child->screen_x, child->screen_y - NODE_RADIUS);
        draw_node_recursive(hdc, child);    // 재귀    
        child = child->next_sibling;
    }
 
    //노드 원 그리기
    HBRUSH brush;
    HPEN pen;
    if (node == g_selected) {
        brush = CreateSolidBrush(RGB(74, 144, 226));   //선택: 파란 배경
        pen = CreatePen(PS_SOLID, 3, RGB(46, 92, 138));
    } else {
        brush = CreateSolidBrush(RGB(255, 255, 255));   //기본: 흰 배경
        pen = CreatePen(PS_SOLID, 2, RGB(74, 144, 226));
    }
    HBRUSH old_brush = (HBRUSH)SelectObject(hdc, brush);
    HPEN old_pen = (HPEN)SelectObject(hdc, pen);
 
    Ellipse(hdc,
            node->screen_x - NODE_RADIUS, node->screen_y - NODE_RADIUS,
            node->screen_x + NODE_RADIUS, node->screen_y + NODE_RADIUS);
 
    //노드 이름 텍스트 
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, node == g_selected ? RGB(255, 255, 255) : RGB(51, 51, 51));
 
    RECT text_rect;
    text_rect.left = node->screen_x - NODE_RADIUS;
    text_rect.right = node->screen_x + NODE_RADIUS;
    text_rect.top = node->screen_y - 10;
    text_rect.bottom = node->screen_y + 10;
    DrawTextA(hdc, node->name, -1, &text_rect,
              DT_CENTER | DT_VCENTER | DT_SINGLELINE | DT_WORD_ELLIPSIS);
 
    //관찰 횟수 표시 (원 아래) 
    if (node->obs_count > 0) {
        char obs_text[64];
        sprintf_s(obs_text, sizeof(obs_text), "%d회 관찰", node->obs_count);
        SetTextColor(hdc, RGB(102, 102, 102));
        RECT obs_rect;
        obs_rect.left = node->screen_x - 50;
        obs_rect.right = node->screen_x + 50;
        obs_rect.top = node->screen_y + NODE_RADIUS + 2;
        obs_rect.bottom = node->screen_y + NODE_RADIUS + 20;
        DrawTextA(hdc, obs_text, -1, &obs_rect, DT_CENTER | DT_SINGLELINE);
    }
 
    SelectObject(hdc, old_brush);
    SelectObject(hdc, old_pen);
    DeleteObject(brush);
    DeleteObject(pen);
}
 
void draw_tree(HDC hdc) {
    for (int i = 0; i < tm.node_count; i++) {
        Node *node = tm.lookup[i];
        if (node && node->parent == NULL) {
            draw_node_recursive(hdc, node);
        }
    }
}
//클릭한 좌표에서 노드 찾기 (DFS)
Node *hit_test(Node *node, int x, int y) {
    if (!node) return NULL;
 
    int dx = x - node->screen_x;
    int dy = y - node->screen_y;
    if (dx * dx + dy * dy <= NODE_RADIUS * NODE_RADIUS) {
        return node;
    }
 
    Node *child = node->first_child;
    while (child) {
        Node *hit = hit_test(child, x, y);
        if (hit) return hit;
        child = child->next_sibling;
    }
    return NULL;
}
 
Node *find_node_at(int x, int y) {
    for (int i = 0; i < tm.node_count; i++) {
        Node *node = tm.lookup[i];
        if (node && node->parent == NULL) {
            Node *hit = hit_test(node, x, y);
            if (hit) return hit;
        }
    }
    return NULL;
}

//텍스트 입력 창 (Edit 컨트롤을 직접 생성하는 모달 입력)
HWND g_edit_hwnd = NULL;
char g_prompt_text[256] = "";
int g_dialog_result = 0;
char g_dialog_buffer[MAX_NAME_LEN] = "";
 
LRESULT CALLBACK input_wnd_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND edit, ok_btn, cancel_btn, label;
    switch (msg) {
        case WM_CREATE:
            label = CreateWindowA("STATIC", g_prompt_text,
                WS_CHILD | WS_VISIBLE,
                20, 15, 300, 20, hwnd, NULL, NULL, NULL);
            edit = CreateWindowA("EDIT", "",
                WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL | WS_TABSTOP,
                20, 40, 300, 25, hwnd, (HMENU)200, NULL, NULL);
            ok_btn = CreateWindowA("BUTTON", "확인",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
                160, 75, 75, 28, hwnd, (HMENU)IDOK, NULL, NULL);
            cancel_btn = CreateWindowA("BUTTON", "취소",
                WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                245, 75, 75, 28, hwnd, (HMENU)IDCANCEL, NULL, NULL);
 
            /* 입력창에 IME(한글 입력기) 명시적 연결 */
            {
                HIMC himc = ImmGetContext(edit);
                if (!himc) {
                    ImmAssociateContext(edit, ImmCreateContext());
                } else {
                    ImmReleaseContext(edit, himc);
                }
            }
 
            g_edit_hwnd = edit;
            SetFocus(edit);
            return 0;
        case WM_COMMAND:
            if (LOWORD(wParam) == IDOK) {
                GetWindowTextA(edit, g_dialog_buffer, MAX_NAME_LEN);
                g_dialog_result = 1;
                DestroyWindow(hwnd);
                return 0;
            } else if (LOWORD(wParam) == IDCANCEL) {
                g_dialog_result = 0;
                DestroyWindow(hwnd);
                return 0;
            }
            break;
        case WM_CLOSE:
            g_dialog_result = 0;
            DestroyWindow(hwnd);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
 
//모달 입력 창 표시
int prompt_text(HWND parent, const char *prompt, char *out, int out_size) {
    strncpy_s(g_prompt_text, sizeof(g_prompt_text), prompt, sizeof(g_prompt_text) - 1);
    g_dialog_result = 0;
    g_dialog_buffer[0] = '\0';
 
    static int registered = 0;
    if (!registered) {
        WNDCLASSA wc = {0};
        wc.lpfnWndProc = input_wnd_proc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.lpszClassName = "InputDialogClass";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassA(&wc);
        registered = 1;
    }
 
    EnableWindow(parent, FALSE);
 
    HWND dlg = CreateWindowA("InputDialogClass", prompt,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 360, 150,
        parent, NULL, GetModuleHandleA(NULL), NULL);
 
    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);
 
    /* 모달 메시지 루프
       Enter 처리는 input_wnd_proc의 IDOK(기본 버튼)가 담당하므로
       여기서 WM_KEYDOWN을 직접 가로채지 않는다.
       (가로채면 IME 한글 조합 확정용 Enter까지 삼켜서 입력이 꼬임) */
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(dlg)) break;
        if (!IsDialogMessage(dlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
 
    EnableWindow(parent, TRUE);
    SetForegroundWindow(parent);
 
    if (g_dialog_result) {
        strncpy_s(out, out_size, g_dialog_buffer, out_size - 1);
        return 1;
    }
    return 0;
}

//관찰기록 패널 그리기 (오른쪽 사이드)
#define PANEL_WIDTH 320
 
void draw_obs_panel(HDC hdc, RECT *client) {
    if (!g_selected) return;
 
    int panel_x = client->right - PANEL_WIDTH;
 
    //패널 배경
    RECT panel = { panel_x, 0, client->right, client->bottom };
    HBRUSH bg = CreateSolidBrush(RGB(248, 249, 250));
    FillRect(hdc, &panel, bg);
    DeleteObject(bg);
 
    //왼쪽 경계선
    HPEN border = CreatePen(PS_SOLID, 1, RGB(221, 221, 221));
    HPEN old = (HPEN)SelectObject(hdc, border);
    MoveToEx(hdc, panel_x, 0, NULL);
    LineTo(hdc, panel_x, client->bottom);
    SelectObject(hdc, old);
    DeleteObject(border);
 
    int x = panel_x + 20;
    int y = 20;
 
    //노드 이름 제목
    SetBkMode(hdc, TRANSPARENT);
    SetTextColor(hdc, RGB(51, 51, 51));
    HFONT title_font = CreateFontA(22, 0, 0, 0, FW_BOLD, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, 0, 0, "맑은 고딕");
    HFONT old_font = (HFONT)SelectObject(hdc, title_font);
    char title[300];
    sprintf_s(title, sizeof(title), "%s", g_selected->name);
    TextOutA(hdc, x, y, title, (int)strlen(title));
    SelectObject(hdc, old_font);
    DeleteObject(title_font);
    y += 35;
 
    HFONT normal_font = CreateFontA(16, 0, 0, 0, FW_NORMAL, 0, 0, 0,
        DEFAULT_CHARSET, 0, 0, 0, 0, "맑은 고딕");
    old_font = (HFONT)SelectObject(hdc, normal_font);
 
    int total = g_selected->obs_count;
 
    if (total == 0) {
        SetTextColor(hdc, RGB(153, 153, 153));
        TextOutA(hdc, x, y, "관찰 기록이 없습니다.", (int)strlen("관찰 기록이 없습니다."));
        y += 30;
    } else {
        /* 페이지 정보 */
        if (g_current_obs_page >= total) g_current_obs_page = total - 1;
        if (g_current_obs_page < 0) g_current_obs_page = 0;
 
        char page_info[64];
        sprintf_s(page_info, sizeof(page_info), "관찰 기록 %d / %d",
                  g_current_obs_page + 1, total);
        SetTextColor(hdc, RGB(136, 136, 136));
        TextOutA(hdc, x, y, page_info, (int)strlen(page_info));
        y += 28;
 
        /* 현재 페이지의 관찰기록 찾기 */
        Locate *obs = g_selected->observations;
        for (int i = 0; i < g_current_obs_page && obs; i++) obs = obs->next;
 
        if (obs) {
            SetTextColor(hdc, RGB(51, 51, 51));
 
            char buf[512];
            sprintf_s(buf, sizeof(buf), "날짜: %d년 %d월 %d일",
                      obs->year, obs->month, obs->day);
            TextOutA(hdc, x, y, buf, (int)strlen(buf));
            y += 26;
 
            sprintf_s(buf, sizeof(buf), "장소: %s",
                      strlen(obs->location) ? obs->location : "(없음)");
            TextOutA(hdc, x, y, buf, (int)strlen(buf));
            y += 26;
 
            sprintf_s(buf, sizeof(buf), "메모: %s",
                      strlen(obs->memo) ? obs->memo : "(없음)");
            RECT memo_rect = { x, y, client->right - 20, y + 60 };
            DrawTextA(hdc, buf, -1, &memo_rect, DT_LEFT | DT_WORDBREAK);
            y += 60;
        }
    }
 
    SelectObject(hdc, old_font);
    DeleteObject(normal_font);
}
//버튼 위치 재배치
 
HWND g_btn_add_root, g_btn_add_child, g_btn_add_parent, g_btn_delete;
HWND g_btn_add_obs, g_btn_prev_obs, g_btn_next_obs, g_btn_del_obs;
 
void reposition_controls(HWND hwnd) {
    RECT rc;
    GetClientRect(hwnd, &rc);
    int panel_x = rc.right - PANEL_WIDTH;
 
    //상단 툴바 버튼
    MoveWindow(g_btn_add_root, 10, 10, 110, 32, TRUE);
    MoveWindow(g_btn_add_child, 125, 10, 90, 32, TRUE);
    MoveWindow(g_btn_add_parent, 220, 10, 90, 32, TRUE);
    MoveWindow(g_btn_delete, 315, 10, 70, 32, TRUE);
 
    //패널 하단 관찰기록 버튼
    int by = rc.bottom - 90;
    MoveWindow(g_btn_prev_obs, panel_x + 20, by, 60, 30, TRUE);
    MoveWindow(g_btn_next_obs, panel_x + 90, by, 60, 30, TRUE);
    MoveWindow(g_btn_del_obs, panel_x + 160, by, 90, 30, TRUE);
    MoveWindow(g_btn_add_obs, panel_x + 20, by + 40, 230, 32, TRUE);
}
 
void update_button_states() {
    BOOL has_sel = (g_selected != NULL);
    EnableWindow(g_btn_add_child, has_sel);
    EnableWindow(g_btn_add_parent, has_sel);
    EnableWindow(g_btn_delete, has_sel);
    EnableWindow(g_btn_add_obs, has_sel);
    EnableWindow(g_btn_prev_obs, has_sel);
    EnableWindow(g_btn_next_obs, has_sel);
    EnableWindow(g_btn_del_obs, has_sel && g_selected->obs_count > 0);
}
 
//새 관찰기록 입력 - 통합 폼 (한 창에서 모든 필드 입력)
 
//폼 결과 전역 변수
struct {
    char year[32], month[32], day[32];
    char location[MAX_NAME_LEN], memo[MAX_NAME_LEN];
    int result;      /* 1=확인, 0=취소 */
} g_obs_form;
 
//폼 안의 EDIT 핸들
static HWND g_ef_year, g_ef_month, g_ef_day, g_ef_loc, g_ef_memo;
 
LRESULT CALLBACK obs_form_proc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            int lx = 20, ex = 90, w = 250, h = 26, y = 20, gap = 36;
 
            CreateWindowA("STATIC", "년도:", WS_CHILD | WS_VISIBLE,
                lx, y + 4, 60, 20, hwnd, NULL, NULL, NULL);
            g_ef_year = CreateWindowA("EDIT", g_obs_form.year,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER,
                ex, y, 80, h, hwnd, NULL, NULL, NULL);
 
            CreateWindowA("STATIC", "월:", WS_CHILD | WS_VISIBLE,
                180, y + 4, 30, 20, hwnd, NULL, NULL, NULL);
            g_ef_month = CreateWindowA("EDIT", g_obs_form.month,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER,
                210, y, 50, h, hwnd, NULL, NULL, NULL);
 
            CreateWindowA("STATIC", "일:", WS_CHILD | WS_VISIBLE,
                270, y + 4, 30, 20, hwnd, NULL, NULL, NULL);
            g_ef_day = CreateWindowA("EDIT", g_obs_form.day,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_NUMBER,
                300, y, 50, h, hwnd, NULL, NULL, NULL);
            y += gap;
 
            CreateWindowA("STATIC", "장소:", WS_CHILD | WS_VISIBLE,
                lx, y + 4, 60, 20, hwnd, NULL, NULL, NULL);
            g_ef_loc = CreateWindowA("EDIT", g_obs_form.location,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
                ex, y, w, h, hwnd, NULL, NULL, NULL);
            y += gap;
 
            CreateWindowA("STATIC", "메모:", WS_CHILD | WS_VISIBLE,
                lx, y + 4, 60, 20, hwnd, NULL, NULL, NULL);
            g_ef_memo = CreateWindowA("EDIT", g_obs_form.memo,
                WS_CHILD | WS_VISIBLE | WS_BORDER | WS_TABSTOP | ES_AUTOHSCROLL,
                ex, y, w, h, hwnd, NULL, NULL, NULL);
            y += gap;
 
            /* 확인 / 취소 */
            CreateWindowA("BUTTON", "저장",
                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | WS_TABSTOP,
                ex, y, 110, 30, hwnd, (HMENU)IDOK, NULL, NULL);
            CreateWindowA("BUTTON", "취소",
                WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                ex + 120, y, 110, 30, hwnd, (HMENU)IDCANCEL, NULL, NULL);
 
            SetFocus(g_ef_year);
            return 0;
        }
 
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            if (id == IDOK) {
                GetWindowTextA(g_ef_year, g_obs_form.year, 32);
                GetWindowTextA(g_ef_month, g_obs_form.month, 32);
                GetWindowTextA(g_ef_day, g_obs_form.day, 32);
                GetWindowTextA(g_ef_loc, g_obs_form.location, MAX_NAME_LEN);
                GetWindowTextA(g_ef_memo, g_obs_form.memo, MAX_NAME_LEN);
                g_obs_form.result = 1;
                DestroyWindow(hwnd);
                return 0;
            }
            else if (id == IDCANCEL) {
                g_obs_form.result = 0;
                DestroyWindow(hwnd);
                return 0;
            }
            break;
        }
 
        case WM_CLOSE:
            g_obs_form.result = 0;
            DestroyWindow(hwnd);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
 
void add_observation_flow(HWND hwnd) {
    if (!g_selected) return;
 
    // 폼 초기화
    memset(&g_obs_form, 0, sizeof(g_obs_form));
 
    static int registered = 0;
    if (!registered) {
        WNDCLASSA wc = {0};
        wc.lpfnWndProc = obs_form_proc;
        wc.hInstance = GetModuleHandleA(NULL);
        wc.lpszClassName = "ObsFormClass";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        RegisterClassA(&wc);
        registered = 1;
    }
 
    EnableWindow(hwnd, FALSE);
 
    char title[300];
    sprintf_s(title, sizeof(title), "'%s' 관찰 기록 추가", g_selected->name);
 
    HWND dlg = CreateWindowA("ObsFormClass", title,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU,
        CW_USEDEFAULT, CW_USEDEFAULT, 400, 210,
        hwnd, NULL, GetModuleHandleA(NULL), NULL);
 
    ShowWindow(dlg, SW_SHOW);
    UpdateWindow(dlg);
 
    //모달 루프
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        if (!IsWindow(dlg)) break;
        if (!IsDialogMessage(dlg, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
 
    EnableWindow(hwnd, TRUE);
    SetForegroundWindow(hwnd);
 
    if (!g_obs_form.result) return;  //취소
 
    add_observation(g_selected->id,
                    atoi(g_obs_form.year), atoi(g_obs_form.month), atoi(g_obs_form.day),
                    g_obs_form.location, g_obs_form.memo);
    save_to_file();
 
    g_current_obs_page = 0;
    update_button_states();
    InvalidateRect(hwnd, NULL, TRUE);
}
 
 
// 메인 윈도우 프로시저
 
LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE: {
            g_btn_add_root = CreateWindowA("BUTTON", "+ 새 트리",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ADD_ROOT, NULL, NULL);
            g_btn_add_child = CreateWindowA("BUTTON", "+ 하위",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ADD_CHILD, NULL, NULL);
            g_btn_add_parent = CreateWindowA("BUTTON", "+ 상위",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ADD_PARENT, NULL, NULL);
            g_btn_delete = CreateWindowA("BUTTON", "삭제",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_DELETE, NULL, NULL);
            g_btn_prev_obs = CreateWindowA("BUTTON", "◀ 이전",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_PREV_OBS, NULL, NULL);
            g_btn_next_obs = CreateWindowA("BUTTON", "다음 ▶",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_NEXT_OBS, NULL, NULL);
            g_btn_del_obs = CreateWindowA("BUTTON", "기록 삭제",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_DEL_OBS, NULL, NULL);
            g_btn_add_obs = CreateWindowA("BUTTON", "+ 새 관찰 기록 추가",
                WS_CHILD | WS_VISIBLE,
                0, 0, 0, 0, hwnd, (HMENU)ID_BTN_ADD_OBS, NULL, NULL);
 
            reposition_controls(hwnd);
            update_button_states();
            return 0;
        }
 
        case WM_SIZE:
            reposition_controls(hwnd);
            InvalidateRect(hwnd, NULL, TRUE);
            return 0;
 
        case WM_COMMAND: {
            int id = LOWORD(wParam);
            char name[MAX_NAME_LEN];
 
            if (id == ID_BTN_ADD_ROOT) {
                if (prompt_text(hwnd, "새 트리(루트) 이름:", name, sizeof(name)) && strlen(name)) {
                    add_root_node(name);
                    layout_tree();
                    save_to_file();
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_ADD_CHILD && g_selected) {
                if (prompt_text(hwnd, "하위 노드 이름:", name, sizeof(name)) && strlen(name)) {
                    add_child_node(g_selected->id, name);
                    layout_tree();
                    save_to_file();
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_ADD_PARENT && g_selected) {
                if (prompt_text(hwnd, "상위 노드 이름:", name, sizeof(name)) && strlen(name)) {
                    add_parent_node(g_selected->id, name);
                    layout_tree();
                    save_to_file();
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_DELETE && g_selected) {
                char msg_buf[300];
                sprintf_s(msg_buf, sizeof(msg_buf),
                    "'%s' 노드를 삭제하시겠습니까?%s",
                    g_selected->name,
                    g_selected->first_child ? "\n\n하위 노드도 모두 삭제됩니다!" : "");
                if (MessageBoxA(hwnd, msg_buf, "삭제 확인",
                                MB_YESNO | MB_ICONWARNING) == IDYES) {
                    delete_node(g_selected->id);
                    g_selected = NULL;
                    layout_tree();
                    save_to_file();
                    update_button_states();
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_ADD_OBS && g_selected) {
                add_observation_flow(hwnd);
            }
            else if (id == ID_BTN_PREV_OBS && g_selected) {
                if (g_current_obs_page > 0) {
                    g_current_obs_page--;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_NEXT_OBS && g_selected) {
                if (g_current_obs_page < g_selected->obs_count - 1) {
                    g_current_obs_page++;
                    InvalidateRect(hwnd, NULL, TRUE);
                }
            }
            else if (id == ID_BTN_DEL_OBS && g_selected && g_selected->obs_count > 0) {
                if (MessageBoxA(hwnd, "이 관찰 기록을 삭제하시겠습니까?", "삭제",
                                MB_YESNO | MB_ICONWARNING) == IDYES) {
                    Locate *obs = g_selected->observations;
                    for (int i = 0; i < g_current_obs_page && obs; i++) obs = obs->next;
                    if (obs) {
                        delete_observation(g_selected->id, obs->id);
                        save_to_file();
                        if (g_current_obs_page > 0) g_current_obs_page--;
                        update_button_states();
                        InvalidateRect(hwnd, NULL, TRUE);
                    }
                }
            }
            return 0;
        }
 
        case WM_LBUTTONDOWN: {
            int x = LOWORD(lParam);
            int y = HIWORD(lParam);
 
            RECT rc;
            GetClientRect(hwnd, &rc);
            /* 패널 영역이 아니면 노드 선택 처리 */
            if (x < rc.right - PANEL_WIDTH) {
                Node *hit = find_node_at(x, y);
                g_selected = hit;
                g_current_obs_page = 0;
                update_button_states();
                InvalidateRect(hwnd, NULL, TRUE);
            }
            return 0;
        }
 
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
 
            RECT rc;
            GetClientRect(hwnd, &rc);
 
            /* 더블 버퍼링 */
            HDC memdc = CreateCompatibleDC(hdc);
            HBITMAP membm = CreateCompatibleBitmap(hdc, rc.right, rc.bottom);
            HBITMAP oldbm = (HBITMAP)SelectObject(memdc, membm);
 
            /* 배경 */
            HBRUSH bg = CreateSolidBrush(RGB(250, 250, 250));
            FillRect(memdc, &rc, bg);
            DeleteObject(bg);
 
            /* 트리 그리기 */
            draw_tree(memdc);
 
            /* 관찰기록 패널 */
            draw_obs_panel(memdc, &rc);
 
            /* 화면에 복사 */
            BitBlt(hdc, 0, 0, rc.right, rc.bottom, memdc, 0, 0, SRCCOPY);
 
            SelectObject(memdc, oldbm);
            DeleteObject(membm);
            DeleteDC(memdc);
 
            EndPaint(hwnd, &ps);
            return 0;
        }
 
        case WM_DESTROY:
            save_to_file();
            PostQuitMessage(0);
            return 0;
    }
    return DefWindowProcA(hwnd, msg, wParam, lParam);
}
 
   //WinMain
 
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrev, LPSTR cmdLine, int nShow) {
    init_tree_manager();
    load_from_file();
    layout_tree();
 
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "BirdTreeWindow";
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    RegisterClassA(&wc);
 
    g_main_window = CreateWindowA("BirdTreeWindow",
        "조류촬영자료 저장 - 트리 관리",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 1100, 700,
        NULL, NULL, hInstance, NULL);
 
    ShowWindow(g_main_window, nShow);
    UpdateWindow(g_main_window);
 
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}
 
