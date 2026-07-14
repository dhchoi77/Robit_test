robot_status = [
  { "name": "mobilebot", "battery": 82, "position": (1.2, 0.5), "distance": 0.8},
  { "name": "drone", "battery": 18, "position": (0.3, 1.5), "distance": 0.4},
  { "name": "manipulator", "battery": 45, "position": (2.0, 1.0), "distance": 1.2},
]

def name_print(i):  #로봇의 name 출력
    print(f"[{robot_status[i]["name"]}]")

def battery_print(i):   #로봇의 battery 출력
    if (robot_status[i]["battery"]>=60):
        print("배터리: 배터리 충분")
    elif(robot_status[i]["battery"]>=20):
        print("배터리: 배터리 주의")
    else:
        print("배터리: 충전 필요")

def position_print(i):  #로봇의 position 출력
    print(f"위치: x={robot_status[i]["position"][0]}, y={robot_status[i]["position"][1]}")

def distance_print(i):  #로봇의 distance 출력
    if (robot_status[i]["distance"]>=0.5):
        print("상태: 전진가능")
    else:
        print("상태: 장애물 감지")

def robot_status_print ():  #robot_status 리스트 출력
    for i in range(len(robot_status)):  
        name_print(i)
        battery_print(i)
        position_print(i)
        distance_print(i)
        print()

robot_status_print()