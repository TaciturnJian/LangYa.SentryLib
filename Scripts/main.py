import threading
import time
import ServerConnection

server = ServerConnection.ServerConnection()


def SetBlueOreStorage(isOpen: bool):
    if isOpen:
        server.RawSend("open blue")
    else:
        server.RawSend("close blue")


def SetRedOreStorage(isOpen: bool):
    if isOpen:
        server.RawSend("open red")
    else:
        server.RawSend("close red")


def ReceiveCommand():
    while True:
        command = server.RawReceive().decode("utf-8")
        words = command.split(" ")
        team_id = words[0]
        location_id = words[1]
        task_id = words[2]
        ore_reward_id = words[3]

        # TODO 在这里做客户端指令处理


threading.Thread(target=ReceiveCommand).start()

while True:
    # TODO 在这里做读取电平发送矿仓状态
    time.sleep(1)
