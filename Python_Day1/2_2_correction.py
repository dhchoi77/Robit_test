# ===========================
# 도서 대출 관리 시스템
# ===========================

# 책 정보 {책이름 : 남은 수량}
books = {}

# 사용자 정보 {사용자 : [빌린 책]}
users = {}


# ---------------------------
# add
# ---------------------------
def add_book(command):

    if len(command) != 3:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    book = command[1]

    try:
        amount = int(command[2])
    except ValueError:
        print("ERROR: 수량은 정수여야 합니다.")
        return

    if amount < 0:
        print("ERROR: 수량은 0 이상이어야 합니다.")
        return

    if book in books:
        books[book] += amount
    else:
        books[book] = amount

    print(f"등록 완료 : {book} ({books[book]}권)")


# ---------------------------
# borrow
# ---------------------------
def borrow_book(command):

    if len(command) != 3:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    user = command[1]
    book = command[2]

    if book not in books:
        print("ERROR: 존재하지 않는 책입니다.")
        return

    if books[book] <= 0:
        print("ERROR: 남은 수량이 없습니다.")
        return

    if user not in users:
        users[user] = []

    if book in users[user]:
        print("ERROR: 이미 대출 중인 책입니다.")
        return

    books[book] -= 1
    users[user].append(book)

    print(f"{user}님이 '{book}'을(를) 대출했습니다.")


# ---------------------------
# return
# ---------------------------
def return_book(command):

    if len(command) != 3:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    user = command[1]
    book = command[2]

    if user not in users:
        print("ERROR: 존재하지 않는 사용자입니다.")
        return

    if book not in users[user]:
        print("ERROR: 대출 기록이 없습니다.")
        return

    users[user].remove(book)
    books[book] += 1

    print(f"{user}님이 '{book}'을(를) 반납했습니다.")


# ---------------------------
# status
# ---------------------------
def status_book(command):

    if len(command) != 2:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    book = command[1]

    if book not in books:
        print("ERROR: 존재하지 않는 책입니다.")
        return

    print(f"{book} : {books[book]}권")


# ---------------------------
# user
# ---------------------------
def user_info(command):

    if len(command) != 2:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    user = command[1]

    if user not in users or len(users[user]) == 0:
        print("EMPTY")
        return

    print(*users[user])


# ---------------------------
# list
# ---------------------------
def book_list(command):

    if len(command) != 1:
        print("ERROR: 입력 형식이 올바르지 않습니다.")
        return

    if len(books) == 0:
        print("EMPTY")
        return

    for book in books:
        print(f"{book} : {books[book]}권")


# ===========================
# 프로그램 시작 안내
# ===========================

print("=" * 40)
print("      도서 대출 관리 시스템")
print("=" * 40)
print("사용 가능한 명령어")
print("add 책이름 수량")
print("borrow 사용자 책이름")
print("return 사용자 책이름")
print("status 책이름")
print("user 사용자")
print("list")
print("Exit")
print("=" * 40)


# ===========================
# 메인
# ===========================

while True:

    command = input(">> ").split()

    if len(command) == 0:
        print("ERROR")
        continue

    cmd = command[0]

    if cmd == "add":
        add_book(command)

    elif cmd == "borrow":
        borrow_book(command)

    elif cmd == "return":
        return_book(command)

    elif cmd == "status":
        status_book(command)

    elif cmd == "user":
        user_info(command)

    elif cmd == "list":
        book_list(command)

    elif cmd == "Exit":
        print("프로그램을 종료합니다.")
        break

    else:
        print("ERROR: 존재하지 않는 명령어입니다.")