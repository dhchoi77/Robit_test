pattern = []    #미로
for i in range(10): #미로 입력
    text = input()
    pattern.append(list(map(int, text.split())))

ant=[1,1]   #초기 개미위치
while (True):
    pattern[ant[0]][ant[1]]=9   #개미이동경로표시하기
    if (ant[0]<10 and ant[1]<10):   #경계선 제한하기
        if (pattern[ant[0]][ant[1]+1]!=1):  #오른쪽으로 이동하기
            if (pattern[ant[0]][ant[1]+1]==2):  
                pattern[ant[0]][ant[1]+1]=9     #먹이 발견하면 이동경로 표시 후 반복문 빠져나오기
                break
            ant[1]+=1   #개미 위치 오른쪽으로 한칸 이동시키기
        elif(pattern[ant[0]+1][ant[1]]!=1):     #아래쪽으로 이동하기   
            if (pattern[ant[0]+1][ant[1]]==2):
                pattern[ant[0]+1][ant[1]]=9     #먹이 발견하면 이동경로 표시 후 반복문 빠져나오기
                break
            ant[0]+=1   #개미 위치 아래쪽으로 한칸 이동시키기
        else:
            break   #오른쪽, 아래쪽 모두 1이라면 반복문 빠져나오기
    else:
        break   #경계선에 닿으면 반복문 빠져나오기

print() # 미로 구분하기

for i in range(len(pattern)):   #미로 출력하기
    for j in range(len(pattern[i])):
        print(pattern[i][j],end=" ")
    print()

