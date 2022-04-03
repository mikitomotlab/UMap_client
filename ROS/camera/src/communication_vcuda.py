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

def to_server():
    rospy.init_node('communicate', anonymous=True)
    host = "192.168.11.180"
    port = 12345

    client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #オブジェクトの作成をします
    client.connect((host, port)) #これでサーバーに接続します

    date = datetime.datetime.today()
    date =date.strftime('%y%m%d%H%M%S')

    myname = 'raspi7'
    date = str(date)
    date += myname

    client.send (date)

    msg = client.recv(1024)

    print (msg)
  
    #img_src = cv2.imread("~/development_v1/src/camera/src/image.png")
    #img_src = cv2.imread("image.png")
    img_src = cv2.imread("/home/miki/development_v1/src/camera/src/image.png")
    cv2.imshow("try1", img_src)
    cv2.waitKey(5000)
  
    img_src = cv2.resize(img_src, (480, 270)) 

    encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 90]

    imbyte = cv2.imencode('.jpeg', img_src, encode_param)[1].tostring()

    size = sys.getsizeof(imbyte)
    size = str(size)
    client.send(size)
    #print (size)
    a = client.recv(1024)
    print (a)

    client.send(imbyte)
    #b = client.recv(1024)
    #print (b)
    client.shutdown(1)
    client.close()
   


#if _name_ == '_main_':
to_server()
    
