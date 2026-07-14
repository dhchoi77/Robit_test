def all_set ():
    print("append 값    리스트 맨 뒤에 값 추가")
    print("insert 인덱스 값 특정 위치에 값 추가")
    print("remove 값     특정 값 삭제")
    print("pop 인덱스   특정 위치 값 삭제")
    print("len         리스트 길이 출력")
    print("print        리스트 전체 출력")
    print("clear        리스트 초기화")


desire_list=[]  #메인 리스트
all_set()   #사용양식 출력
while (True):   #무한반복
    num=0   #" "인덱스 확인용 변수
    desire =input()
    for i in range(len(desire)): #" "인덱스 구하기
        if (desire[i]==" "):
            num=i
            request=desire[:i]  #" "인덱스 앞은 리스트메서드
            value=desire[i+1:]  #" "인덱스+1 뒤는 리스트에 추가할 값이나 매서드 인덱스등등
            break
    if (num==0):
        request=desire  #" "가 필요없는 len, print, clear를 판별하기 위한 조건문

    if (request=="append"):
        desire_list.append(value)

    elif (request=="insert"):
        for j in range(len(value)): #insert에 들어갈 인덱스값과 요소 구별하기
            if (value[j]==" "):
                insert_index=int(value[:j])
                value=value[j+1:]
                break
        if(0<=insert_index and insert_index<=len(desire_list)): #인덱스 값이 리스트 안에 있는지 확인하기
            desire_list.insert(insert_index,value)
        else:
            print("인덱스를 찾을 수 없습니다.")
        
    elif(request=="remove"):
        if (value in desire_list):  #value값이 리스트 안에 있는지 확인하기
            desire_list.remove(value)
        else:
            print("존재하지 않는 값입니다.")

    elif(request=="pop"):   
        value=int(value)
        if (desire_list):   #리스트 값이 차있는지 확인하기
            if(0<=value and value<len(desire_list)):    #인덱스값이 리스트 안에 있는지 확인하기
                desire_list.pop(value)
            else:
                print("인덱스를 찾을 수 없습니다.")
        else:
            print("리스트가 비어있습니다.")

    elif(request=="len"):
        print(len(desire_list))

    elif(request=="print"):
        print(desire_list)

    elif(request=="clear"):
        desire_list.clear()
    
