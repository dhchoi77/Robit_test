"""
지뢰찾기 (Minesweeper) - pygame 구현

조작법
  좌클릭      : 칸 열기
  우클릭      : 깃발 표시 / 해제
  좌+우클릭   : 코딩(chording) - 숫자 주변 깃발 수가 맞으면 나머지 칸 한 번에 열기
  스마일 버튼 : 재시작
  1 / 2 / 3   : 난이도 변경 (초급 / 중급 / 고급)
  ESC         : 종료
"""

import random
import sys

import pygame

# ---------------------------------------------------------------------------
# 상수 정의
# ---------------------------------------------------------------------------
CELL = 30          # 칸 한 변의 픽셀 크기
MARGIN = 12        # 보드 바깥 여백
HEADER = 64        # 상단 정보 바 높이
FPS = 60

# 난이도: (가로 칸 수, 세로 칸 수, 지뢰 개수)
DIFFICULTIES = {
    1: ("BEGINNER", 9, 9, 10),
    2: ("INTERMEDIATE", 16, 16, 40),
    3: ("EXPERT", 30, 16, 99),
}

# 색상
BG = (189, 189, 189)
DARK = (123, 123, 123)
LIGHT = (255, 255, 255)
COVER = (198, 198, 198)
OPEN = (222, 222, 222)
LINE = (128, 128, 128)
BLACK = (0, 0, 0)
RED = (219, 50, 54)
PANEL = (60, 60, 60)

NUM_COLORS = {
    1: (25, 118, 210),
    2: (56, 142, 60),
    3: (211, 47, 47),
    4: (57, 73, 171),
    5: (140, 60, 40),
    6: (0, 131, 143),
    7: (33, 33, 33),
    8: (117, 117, 117),
}

# 게임 상태
READY, PLAYING, WIN, LOSE = "READY", "PLAYING", "WIN", "LOSE"


class Board:
    """지뢰찾기 보드의 '데이터'만 담당하는 클래스 (화면 출력과 분리)."""

    def __init__(self, cols, rows, mines):
        self.cols = cols
        self.rows = rows
        self.mines = mines

        # 각 칸의 상태를 2차원 리스트로 관리
        self.mine = [[False] * cols for _ in range(rows)]     # 지뢰 여부
        self.number = [[0] * cols for _ in range(rows)]       # 주변 지뢰 개수
        self.revealed = [[False] * cols for _ in range(rows)] # 열림 여부
        self.flagged = [[False] * cols for _ in range(rows)]  # 깃발 여부

        self.placed = False       # 지뢰가 배치되었는지 (첫 클릭 이후 True)
        self.state = READY
        self.exploded = None      # 밟은 지뢰 좌표 (빨갛게 표시용)

    # -- 유틸 -----------------------------------------------------------
    def in_range(self, r, c):
        return 0 <= r < self.rows and 0 <= c < self.cols

    def neighbors(self, r, c):
        """(r, c) 주변 8칸의 좌표를 생성한다."""
        for dr in (-1, 0, 1):
            for dc in (-1, 0, 1):
                if dr == 0 and dc == 0:
                    continue
                nr, nc = r + dr, c + dc
                if self.in_range(nr, nc):
                    yield nr, nc

    @property
    def flag_count(self):
        return sum(row.count(True) for row in self.flagged)

    # -- 지뢰 배치 -------------------------------------------------------
    def place_mines(self, safe_r, safe_c):
        """첫 클릭 위치와 그 주변은 지뢰에서 제외하고 무작위 배치한다.

        (첫 클릭에서 바로 죽는 상황을 막는 'first-click safety' 규칙)
        """
        safe = {(safe_r, safe_c)} | set(self.neighbors(safe_r, safe_c))
        spots = [
            (r, c)
            for r in range(self.rows)
            for c in range(self.cols)
            if (r, c) not in safe
        ]
        for r, c in random.sample(spots, self.mines):
            self.mine[r][c] = True

        # 주변 지뢰 개수 미리 계산
        for r in range(self.rows):
            for c in range(self.cols):
                if not self.mine[r][c]:
                    self.number[r][c] = sum(
                        self.mine[nr][nc] for nr, nc in self.neighbors(r, c)
                    )
        self.placed = True

    # -- 플레이 동작 -----------------------------------------------------
    def reveal(self, r, c):
        """칸 열기. 빈 칸(0)이면 BFS로 연쇄적으로 펼친다."""
        if self.state in (WIN, LOSE):
            return
        if self.flagged[r][c] or self.revealed[r][c]:
            return

        if not self.placed:
            self.place_mines(r, c)
            self.state = PLAYING

        if self.mine[r][c]:
            self.revealed[r][c] = True
            self.exploded = (r, c)
            self.state = LOSE
            return

        # 너비 우선 탐색(BFS)으로 0인 칸을 따라 확장
        stack = [(r, c)]
        while stack:
            cr, cc = stack.pop()
            if self.revealed[cr][cc] or self.flagged[cr][cc]:
                continue
            self.revealed[cr][cc] = True
            if self.number[cr][cc] == 0:
                for nr, nc in self.neighbors(cr, cc):
                    if not self.revealed[nr][nc] and not self.mine[nr][nc]:
                        stack.append((nr, nc))

        self.check_win()

    def toggle_flag(self, r, c):
        if self.state in (WIN, LOSE) or self.revealed[r][c]:
            return
        self.flagged[r][c] = not self.flagged[r][c]

    def chord(self, r, c):
        """숫자 칸에서 주변 깃발 수가 숫자와 같으면 나머지를 한 번에 연다."""
        if not self.revealed[r][c] or self.number[r][c] == 0:
            return
        flags = sum(self.flagged[nr][nc] for nr, nc in self.neighbors(r, c))
        if flags != self.number[r][c]:
            return
        for nr, nc in self.neighbors(r, c):
            if not self.flagged[nr][nc] and not self.revealed[nr][nc]:
                self.reveal(nr, nc)

    def check_win(self):
        """지뢰가 아닌 칸을 모두 열었으면 승리."""
        opened = sum(row.count(True) for row in self.revealed)
        if opened == self.rows * self.cols - self.mines:
            self.state = WIN
            for r in range(self.rows):
                for c in range(self.cols):
                    if self.mine[r][c]:
                        self.flagged[r][c] = True


class Game:
    """pygame 화면/이벤트 루프를 담당하는 클래스."""

    def __init__(self, level=1):
        pygame.init()
        self.clock = pygame.time.Clock()
        self.font = pygame.font.Font(None, 24)      # 칸 숫자용
        self.big = pygame.font.Font(None, 34)       # 상단 패널용
        self.new_game(level)

    def new_game(self, level=None):
        if level is not None:
            self.level = level
        name, cols, rows, mines = DIFFICULTIES[self.level]
        self.level_name = name
        self.board = Board(cols, rows, mines)

        width = cols * CELL + MARGIN * 2
        height = rows * CELL + HEADER + MARGIN * 2
        self.screen = pygame.display.set_mode((width, height))
        pygame.display.set_caption(f"지뢰찾기 - {name}")

        # 상단 재시작(스마일) 버튼 영역
        self.face_rect = pygame.Rect(width // 2 - 18, MARGIN + 10, 36, 36)
        self.start_tick = None
        self.elapsed = 0

    # -- 좌표 변환 -------------------------------------------------------
    def cell_at(self, pos):
        """마우스 픽셀 좌표 -> (row, col). 보드 밖이면 None."""
        x, y = pos
        c = (x - MARGIN) // CELL
        r = (y - HEADER - MARGIN) // CELL
        if self.board.in_range(r, c) and x >= MARGIN and y >= HEADER + MARGIN:
            return int(r), int(c)
        return None

    def cell_rect(self, r, c):
        return pygame.Rect(
            MARGIN + c * CELL, HEADER + MARGIN + r * CELL, CELL, CELL
        )

    # -- 이벤트 ----------------------------------------------------------
    def handle_click(self, pos, button):
        if self.face_rect.collidepoint(pos):
            self.new_game()
            return

        cell = self.cell_at(pos)
        if cell is None:
            return
        r, c = cell

        pressed = pygame.mouse.get_pressed()
        if pressed[0] and pressed[2]:          # 좌+우 동시 -> 코딩
            self.board.chord(r, c)
        elif button == 1:                      # 좌클릭 -> 열기
            if self.board.revealed[r][c]:
                self.board.chord(r, c)         # 열린 숫자 칸 좌클릭도 코딩 처리
            else:
                self.board.reveal(r, c)
        elif button == 3:                      # 우클릭 -> 깃발
            self.board.toggle_flag(r, c)
        elif button == 2:                      # 휠 클릭 -> 코딩
            self.board.chord(r, c)

        if self.board.state == PLAYING and self.start_tick is None:
            self.start_tick = pygame.time.get_ticks()

    # -- 그리기 ----------------------------------------------------------
    def draw_bevel(self, rect, raised=True):
        """옛날 윈도우 지뢰찾기 느낌의 입체 테두리."""
        pygame.draw.rect(self.screen, COVER if raised else OPEN, rect)
        top, bottom = (LIGHT, DARK) if raised else (DARK, LIGHT)
        pygame.draw.line(self.screen, top, rect.topleft, rect.topright, 2)
        pygame.draw.line(self.screen, top, rect.topleft, rect.bottomleft, 2)
        pygame.draw.line(self.screen, bottom, rect.bottomleft, rect.bottomright, 2)
        pygame.draw.line(self.screen, bottom, rect.topright, rect.bottomright, 2)

    def draw_flag(self, rect):
        px, py = rect.centerx, rect.centery
        pygame.draw.line(self.screen, BLACK, (px - 1, py - 8), (px - 1, py + 7), 2)
        pygame.draw.polygon(
            self.screen, RED,
            [(px - 1, py - 8), (px + 9, py - 4), (px - 1, py)],
        )
        pygame.draw.line(self.screen, BLACK, (px - 7, py + 8), (px + 7, py + 8), 3)

    def draw_mine(self, rect, hit=False):
        if hit:
            pygame.draw.rect(self.screen, RED, rect)
            pygame.draw.rect(self.screen, LINE, rect, 1)
        pygame.draw.circle(self.screen, BLACK, rect.center, 7)
        cx, cy = rect.center
        for dx, dy in ((10, 0), (0, 10), (7, 7), (7, -7)):
            pygame.draw.line(
                self.screen, BLACK, (cx - dx, cy - dy), (cx + dx, cy + dy), 2
            )
        pygame.draw.circle(self.screen, LIGHT, (cx - 2, cy - 3), 2)

    def draw_panel(self):
        b = self.board
        w = self.screen.get_width()

        # 남은 지뢰 수
        remain = b.mines - b.flag_count
        box1 = pygame.Rect(MARGIN + 4, MARGIN + 10, 70, 36)
        pygame.draw.rect(self.screen, PANEL, box1)
        txt = self.big.render(f"{remain:03d}", True, RED)
        self.screen.blit(txt, txt.get_rect(center=box1.center))

        # 경과 시간
        if b.state == PLAYING and self.start_tick is not None:
            self.elapsed = (pygame.time.get_ticks() - self.start_tick) // 1000
        box2 = pygame.Rect(w - MARGIN - 74, MARGIN + 10, 70, 36)
        pygame.draw.rect(self.screen, PANEL, box2)
        txt = self.big.render(f"{min(self.elapsed, 999):03d}", True, RED)
        self.screen.blit(txt, txt.get_rect(center=box2.center))

        # 스마일 버튼
        self.draw_bevel(self.face_rect, raised=True)
        cx, cy = self.face_rect.center
        pygame.draw.circle(self.screen, (255, 214, 0), (cx, cy), 13)
        pygame.draw.circle(self.screen, BLACK, (cx, cy), 13, 1)
        if b.state == LOSE:  # X_X
            for sx in (-5, 5):
                pygame.draw.line(self.screen, BLACK, (cx + sx - 3, cy - 6), (cx + sx + 3, cy - 1), 2)
                pygame.draw.line(self.screen, BLACK, (cx + sx + 3, cy - 6), (cx + sx - 3, cy - 1), 2)
            pygame.draw.arc(self.screen, BLACK, (cx - 6, cy + 3, 12, 9), 0.4, 2.7, 2)
        else:
            pygame.draw.circle(self.screen, BLACK, (cx - 5, cy - 4), 2)
            pygame.draw.circle(self.screen, BLACK, (cx + 5, cy - 4), 2)
            pygame.draw.arc(self.screen, BLACK, (cx - 7, cy - 3, 14, 12), 3.5, 5.9, 2)

        # 난이도 표시
        label = self.font.render(f"{self.level_name}  (1/2/3)", True, (70, 70, 70))
        self.screen.blit(label, (w // 2 - label.get_width() // 2, MARGIN + 48))

    def draw_board(self):
        b = self.board
        for r in range(b.rows):
            for c in range(b.cols):
                rect = self.cell_rect(r, c)

                if not b.revealed[r][c]:
                    self.draw_bevel(rect, raised=True)
                    if b.flagged[r][c]:
                        self.draw_flag(rect)
                    # 패배 시 못 찾은 지뢰 공개
                    if b.state == LOSE and b.mine[r][c] and not b.flagged[r][c]:
                        self.draw_bevel(rect, raised=False)
                        self.draw_mine(rect)
                    # 패배 시 잘못 꽂은 깃발에 X 표시
                    if b.state == LOSE and b.flagged[r][c] and not b.mine[r][c]:
                        pygame.draw.line(self.screen, RED, rect.topleft, rect.bottomright, 2)
                        pygame.draw.line(self.screen, RED, rect.topright, rect.bottomleft, 2)
                else:
                    self.draw_bevel(rect, raised=False)
                    if b.mine[r][c]:
                        self.draw_mine(rect, hit=(b.exploded == (r, c)))
                    elif b.number[r][c] > 0:
                        n = b.number[r][c]
                        txt = self.font.render(str(n), True, NUM_COLORS[n])
                        self.screen.blit(txt, txt.get_rect(center=rect.center))

    def draw_overlay(self):
        if self.board.state not in (WIN, LOSE):
            return
        msg = "YOU WIN!" if self.board.state == WIN else "GAME OVER"
        color = (46, 125, 50) if self.board.state == WIN else RED
        txt = self.big.render(msg, True, LIGHT)
        pad = 12
        box = txt.get_rect(center=(self.screen.get_width() // 2,
                                   HEADER + MARGIN + self.board.rows * CELL // 2))
        bg = box.inflate(pad * 2, pad)
        pygame.draw.rect(self.screen, color, bg, border_radius=6)
        pygame.draw.rect(self.screen, LIGHT, bg, 2, border_radius=6)
        self.screen.blit(txt, box)

    # -- 메인 루프 -------------------------------------------------------
    def run(self):
        while True:
            for event in pygame.event.get():
                if event.type == pygame.QUIT:
                    pygame.quit()
                    sys.exit()
                elif event.type == pygame.KEYDOWN:
                    if event.key == pygame.K_ESCAPE:
                        pygame.quit()
                        sys.exit()
                    elif event.key in (pygame.K_1, pygame.K_2, pygame.K_3):
                        self.new_game(event.key - pygame.K_0)
                    elif event.key == pygame.K_r:
                        self.new_game()
                elif event.type == pygame.MOUSEBUTTONDOWN:
                    self.handle_click(event.pos, event.button)

            self.screen.fill(BG)
            self.draw_panel()
            self.draw_board()
            self.draw_overlay()
            pygame.display.flip()
            self.clock.tick(FPS)


def main():
    print(f"pygame version: {pygame.version.ver} (SDL {pygame.version.SDL})")
    Game(level=1).run()


if __name__ == "__main__":
    main()