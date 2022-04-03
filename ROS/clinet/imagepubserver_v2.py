# -*- coding:utf-8 -*-
import socket
import cv2
import numpy as np
#import Image
import socketserver
import sys
import datetime
#import commands
import os
#from pylsd.lsd import lsd

#######################引数でエッジ検出のtypeを分けれるようにする
#######################GoPro用で歪みをとっている
#print ("Waiting for connections...")

host = "192.168.11.105" #お使いのサーバーのホスト名を入れます
port = 1245 #クライアントと同じPORTをしてあげます


#serversock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#serversock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
#serversock.bind((host,port)) #IPとPORTを指定してバインドします
#serversock.listen(10) #接続の待ち受けをします（キューの最大数を指定）
#lsd画像に変換

global wide

global height

wide = 480
height = 270

def main():
    while True:
        print ('Waiting for connections...')
        host = "192.168.11.105" #お使いのサーバーのホスト名を入れます
        port = 1245 #クライアントで設定したPORTと同じもの指定してあげます

        serversock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        serversock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        serversock.bind((host,port)) #IPとPORTを指定してバインドします
        serversock.listen(10) #接続の待ち受けをします（キューの最大数を指定）

        clientsock, client_address = serversock.accept() #接続されればデータを格納

        size = clientsock.recv(1024)
        print(size)
        clientsock.send(b'get size')
        size = size.decode('utf-8')
        size = int(size)
        size = size - 4

        #画像の受け取り
        buf=b''
        recvlen=100
        while recvlen>0:
            receivedstr=clientsock.recv(62208)
            #receivedstr=str(receivedstr,'utf-8')
            recvlen=len(receivedstr)
            #if receivedstr in "finish": break
            #print (len(receivedstr))
            buf +=receivedstr
            print(sys.getsizeof(buf))
            if (sys.getsizeof(buf) == size):
                break
        #receivedstr=str(receivedstr,'utf-8')
        receivedstr=str(receivedstr)
        print ("check2")
        clientsock.send(b'get image')
        #receivedstr=receivedstr.tostring()
        #narray=np.fromstring(buf,dtype='uint8')
        narray=np.frombuffer(buf,dtype='uint8')
        print ("check2")
        narray = cv2.imdecode(narray,1)
        print ("check3")
        #時間の取得
        date = datetime.datetime.today()
        print ("check4")

        #センシング画像を表示
        cv2.imshow('img',narray)
        cv2.waitKey(5000)
        cv2.destroyAllWindows()

        PATH ='C:\\Users\\k0913\\image\\image.jpg'
        cv2.imwrite(PATH,narray)
        #clientsock.send(b'get image')

        clientsock.close()

main()
SocketServer.TCPServer.allow_reuse_address = True
server = SocketServer.TCPServer((host, port), main)
#server = socketserver.ThreadingTCPServer((HOST, PORT), TCPHandler)
#^Cを押したときにソケットを閉じる
try:
    server.serve_forever()
except KeyboardInterrupt:
    pass
server.shutdown()
sys.exit()
