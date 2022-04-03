#! /usr/bin/env python
# -*- coding: utf-8 -*-

import rospy
import SocketServer
import socket
import datetime
import glob
import cv2
import sys
from std_msgs.msg import String

HOST = '192.168.11.20'
#HOST = '10.0.2.15'
PORT = 8080



print ("Waiting for connections...")

class TCPHandler(SocketServer.BaseRequestHandler):

    

    #print ("Waiting for connections...")

    #リクエストを受け取るたびに呼ばれる関数
    def handle(self):

        #print ("Waiting for connections...")


        print ("connect from:", self.client_address)

        #どのクライアントからの画像なのかを文字列で受け取る
        #rcvmsg = self.request.recv(1024).strip()
        rcvmsg = self.request.recv(1024).encode('ascii')
        print (rcvmsg)
        time_now = datetime.datetime.now()
        print(time_now)


SocketServer.TCPServer.allow_reuse_address = True
server = SocketServer.TCPServer((HOST, PORT), TCPHandler)
#server = socketserver.ThreadingTCPServer((HOST, PORT), TCPHandler)
#^Cを押したときにソケットを閉じる
try:
    server.serve_forever()
except KeyboardInterrupt:
    pass
server.shutdown()
sys.exit()
