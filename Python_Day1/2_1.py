def compress(text):
    if not text:                       # 빈 문자열 처리
        return text

    parts = []
    count = 1
    for i in range(1, len(text)):
        if text[i] == text[i - 1]:     # 이전 문자와 같으면 count 증가
            count += 1
        else:                          # 다른 문자가 나오면 지금까지의 묶음을 리스트에 저장
            parts.append(text[i - 1] + str(count))
            count = 1
    parts.append(text[-1] + str(count))  # 마지막 묶음 처리

    code = "".join(parts)   #리스트 합치기

    return text if len(code) > len(text) else code  # 압축 결과가 원본보다 길어지면 원본을 반환


def is_valid_code(code):
    if not isinstance(code, str) or len(code) == 0:     #code가 int 값이거나 ''아무것도 없는 빈것이라면 ERROR 출력
        return False

    i = 0
    n = len(code)
    while i < n:
        if not code[i].isalpha():      # 문자 자리에 알파벳이 아니면 오류 ("3a")
            return False
        i += 1

        start = i
        while i < n and code[i].isdigit():   # 숫자 부분 읽기
            i += 1

        if i == start:                 # 숫자가 하나도 없음 ("ab", "a3b")
            return False
        if code[start] == '0':         # "a0", "a01" → 1 이상이 아님
            return False

    return True


def decompress(code):
    if not is_valid_code(code):        # 검증을 먼저 수행
        return "ERROR"

    result = []
    i = 0
    n = len(code)
    while i < n:
        ch = code[i]                   # 문자넣기
        i += 1

        start = i
        while i < n and code[i].isdigit():
            i += 1
        cnt = int(code[start:i])       # 반복 횟수 (두 자리 이상 가능)

        result.append(ch * cnt)

    return "".join(result)



while(True):
    request=input()
    list_request=[]
    list_request=request.split("(") #"("기준으로 자르기
    value=list_request[2]   #code값이나 text값이므로 따로 관리
    value=value[1:]     #"부분 잘라내기
    value=value[:-3]    #"))부분 잘라내기
    if (list_request[1]=="compress"):
        print(compress(value))
    elif(list_request[1]=="decompress"):
        print(decompress(value))
    