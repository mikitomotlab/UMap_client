# -*- coding:utf-8 -*-
#!/usr/bin/python

import socket
import cv2
#from PIL import Image
import numpy as np
import SocketServer
import sys 
#import io
import logging
from picamera import PiCamera
from time import sleep
import glob
import os
import time
import math
import IMU
import datetime


host = "192.168.11.100" #お使いのサーバーのホスト名を入れます
#host = "192.168.2.7" #お使いのサーバーのホスト名を入れます
port = 12345 #適当なPORTを指定してあげます

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #オブジェクトの作成をします

client.connect((host, port)) #これでサーバーに接続します

#client.send(b'test')

#msg = client.recv(1024)
#print(msg)

print ("success to connect")

date = datetime.datetime.today()
date =date.strftime('%y%m%d%H%M%S')

myname = 'raspi7'
date = str(date)
date += myname

client.send (date)

msg = client.recv(1024)

print (msg)


#写真を撮影
camera = PiCamera()
camera.rotation = 180
camera.start_preview(alpha=200)
sleep(1)
#camera.resolution = (2880,1620)
camera.resolution = (1920,1080)

camera.capture('/home/pi/Desktop/sen/image.png')

#os.system('sh raspistill.sh')

camera.stop_preview()

RAD_TO_DEG = 57.29578
M_PI = 3.14159265358979323846
G_GAIN = 0.070  # [deg/s/LSB]  If you change the dps for gyro, you need to update this value accordingly
AA =  0.40      # Complementary filter constant

gyroXangle = 0.0
gyroYangle = 0.0
gyroZangle = 0.0
CFangleX = 0.0
CFangleY = 0.0



IMU.detectIMU()     #Detect if BerryIMUv1 or BerryIMUv2 is connected.
IMU.initIMU()       #Initialise the accelerometer, gyroscope and compass


a = datetime.datetime.now()

	
	
#Read the accelerometer,gyroscope and magnetometer values
ACCx = IMU.readACCx()
ACCy = IMU.readACCy()
ACCz = IMU.readACCz()
GYRx = IMU.readGYRx()
GYRy = IMU.readGYRy()
GYRz = IMU.readGYRz()
MAGx = IMU.readMAGx()
MAGy = IMU.readMAGy()
MAGz = IMU.readMAGz()

#change the accerometer's value(x,y,z)to(y,z,x)

ACC = ACCx
ACCx = ACCy
ACCy = ACCz
ACCz = ACC


#Convert Accelerometer values to degrees
AccXangle =  (math.atan2(ACCy,ACCz)+M_PI)*RAD_TO_DEG
AccYangle =  (math.atan2(ACCz,ACCx)+M_PI)*RAD_TO_DEG
AccZangle =  (math.atan2(ACCx,ACCy)+M_PI)*RAD_TO_DEG

#convert the values to -180 and +180
AccXangle -= 180.0
if AccYangle > 90:
   AccYangle -= 270.0
else:
        AccYangle += 90.0
        




#slow program down a bit, makes the output more readable
time.sleep(0.03)

rollangle = AccYangle


rollangle = str(rollangle)

client.send (rollangle)

comment = client.recv(1024)

print (comment)

tiltangle = AccXangle


tiltangle = str(tiltangle)

client.send (tiltangle)

nextcomment = client.recv(1024)

print (nextcomment)

# 画像読み込み

img_src = cv2.imread("/home/pi/Desktop/sen/image.png")

#画像の保存
#f = glob.glob('/home/pi/Desktop/file/img-*.png')

#j = len(f)
#j = j+1
#j = str(j)

#cv2.imwrite("/home/pi/Desktop/file/img-"+j.zfill(3)+".png",img_src)

cv2.imwrite("/home/pi/Desktop/file/img-"+date+".png",img_src)


#サイズの変更

img_src = cv2.resize(img_src,(480,270))

#グレースケールに変換
#im = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
#print("OK")

# jpegの圧縮率を設定 0～100値が高いほど高品質。何も指定しなければ95
encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 100]

# 文字列に変換
jpegsByte = cv2.imencode('.jpeg', img_src, encode_param)[1].tostring()



client.send(jpegsByte)



