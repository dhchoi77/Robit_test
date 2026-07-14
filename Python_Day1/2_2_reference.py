# ===========================
# 도서 대출 관리 시스템
# ===========================

# 책 정보
# {책이름 : 남은 수량}
books = {}

# 사용자 대출 정보
# {사용자 : [책1, 책2, ...]}
users = {}


# ---------------------------
# add
# ---------------------------
def add_book(book, amount):
    if book in books:
        books[book] += amount
    else:
        books[book] = amount


# ---------------------------
# borrow
# ---------------------------
def borrow_book(user, book):

    # 책 존재 여부
    if book not in books:
        print("ERROR: 존재하지 않는 책")
        return

    # 수량 확인
    if books[book] <= 0:
        print("ERROR: 수량 부족")
        return

    # 사용자 등록
    if user not in users:
        users[user] = []

    # 같은 책 이미 대출했는지
    if book in users[user]:
        print("ERROR: 이미 대출 중")
        return

    # 대출
    books[book] -= 1
    users[user].append(book)


# ---------------------------
# return
# ---------------------------
def return_book(user, book):

    if user not in users:
        print("ERROR: 사용자 없음")
        return

    if book not in users[user]:
        print("ERROR: 대출 기록 없음")
        return

    users[user].remove(book)
    books[book] += 1


# ---------------------------
# status
# ---------------------------
def status_book(book):

    if book not in books:
        print("ERROR")
    else:
        print(books[book])


# ---------------------------
# user
# ---------------------------
def user_info(user):

    if user not in users or len(users[user]) == 0:
        print("EMPTY")
    else:
        for book in users[user]:
            print(book)


# ---------------------------
# list
# ---------------------------
def book_list():

    if len(books) == 0:
        print("EMPTY")
        return

    for book in books:
        print(book, books[book])


# ===========================
# 메인
# ===========================

while True:

    command = input().split()

    if len(command) == 0:
        print("ERROR")
        continue

    cmd = command[0]

    # -----------------------
    # add
    # -----------------------
    if cmd == "add":

        if len(command) != 3:
            print("ERROR")
            continue

        book = command[1]

        try:
            amount = int(command[2])
        except:
            print("ERROR")
            continue

        if amount < 0:
            print("ERROR")
            continue

        add_book(book, amount)

    # -----------------------
    # borrow
    # -----------------------
    elif cmd == "borrow":

        if len(command) != 3:
            print("ERROR")
            continue

        borrow_book(command[1], command[2])

    # -----------------------
    # return
    # -----------------------
    elif cmd == "return":

        if len(command) != 3:
            print("ERROR")
            continue

        return_book(command[1], command[2])

    # -----------------------
    # status
    # -----------------------
    elif cmd == "status":

        if len(command) != 2:
            print("ERROR")
            continue

        status_book(command[1])

    # -----------------------
    # user
    # -----------------------
    elif cmd == "user":

        if len(command) != 2:
            print("ERROR")
            continue

        user_info(command[1])

    # -----------------------
    # list
    # -----------------------
    elif cmd == "list":

        if len(command) != 1:
            print("ERROR")
            continue

        book_list()

    # -----------------------
    # Exit
    # -----------------------
    elif cmd == "Exit":

        break

    # -----------------------
    # 잘못된 명령
    # -----------------------
    else:
        print("ERROR")