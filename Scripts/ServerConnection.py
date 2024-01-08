import socket


class ServerConnection:
    def __init__(self):
        self.LOCAL_ENDPOINT_ADDRESS = "127.0.0.1"
        self.LOCAL_ENDPOINT_PORT = 8989
        self.LOCAL_ENDPOINT = (self.LOCAL_ENDPOINT_ADDRESS, self.LOCAL_ENDPOINT_PORT)
        self.REMOTE_ENDPOINT_ADDRESS = "127.0.0.1"
        self.REMOTE_ENDPOINT_PORT = 8990
        self.REMOTE_ENDPOINT = (self.REMOTE_ENDPOINT_ADDRESS, self.REMOTE_ENDPOINT_PORT)
        self.COMMAND_LENGTH = 256
        self.socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        self.socket.bind(self.LOCAL_ENDPOINT)

    def RawReceive(self):
        return self.socket.recv(self.COMMAND_LENGTH)

    def RawSend(self, command: str):
        return self.socket.sendto(command.encode("utf-8").ljust(self.COMMAND_LENGTH), self.REMOTE_ENDPOINT)
