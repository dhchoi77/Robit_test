def add_log(robot_name, battery):
    logs=[] #logs를 함수에 들어올때마다 초기화시켜줘야 이전 값들이 누적되지 않는다.
    log = robot_name + " battery: " + str(battery)  #int형이던 battery를 str형으로 변환해줘야 문자열 연산이 된다.
    logs.append(log)
    return logs

print(add_log("frontbot", 80))
print(add_log("rearbot", 50))
print(add_log("armbot", 20))