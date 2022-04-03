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


host = "192.168.11.25" #お使いのサーバーのホスト名を入れます
port = 12345 #適当なPORTを指定してあげます

client = socket.socket(socket.AF_INET, socket.SOCK_STREAM) #オブジェクトの作成をします

client.connect((host, port)) #これでサーバーに接続します

#client.send(b'test')

#msg = client.recv(1024)
#print(msg)

print ("success to conect")

#写真を撮る
camera = PiCamera()
camera.rotation = 180
camera.start_preview(alpha=200)
sleep(5)
camera.capture('/home/pi/Desktop/image.png')
#camera.capture('/home/pi/Desktop/image.jpg')
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


##Calculate loop Period(LP). How long between Gyro Reads
b = datetime.datetime.now() - a
a = datetime.datetime.now()
LP = b.microseconds/(1000000*1.0)
print "Loop Time | %5.2f|" % ( LP ),


#Convert Gyro raw to degrees per second
rate_gyr_x =  GYRx * G_GAIN
rate_gyr_y =  GYRy * G_GAIN
rate_gyr_z =  GYRz * G_GAIN


#Calculate the angles from the gyro. 
gyroXangle+=rate_gyr_x*LP
gyroYangle+=rate_gyr_y*LP
gyroZangle+=rate_gyr_z*LP


#Convert Accelerometer values to degrees
AccXangle =  (math.atan2(ACCy,ACCz)+M_PI)*RAD_TO_DEG
AccYangle =  (math.atan2(ACCz,ACCx)+M_PI)*RAD_TO_DEG


#convert the values to -180 and +180
AccXangle -= 180.0
if AccYangle > 90:
   AccYangle -= 270.0
else:
        AccYangle += 90.0



#Complementary filter used to combine the accelerometer and gyro values.
CFangleX=AA*(CFangleX+rate_gyr_x*LP) +(1 - AA) * AccXangle
CFangleY=AA*(CFangleY+rate_gyr_y*LP) +(1 - AA) * AccYangle



#Calculate heading
heading = 180 * math.atan2(MAGy,MAGx)/M_PI

#Only have our heading between 0 and 360
if heading < 0:
   heading += 360


#Normalize accelerometer raw values.
accXnorm = ACCx/math.sqrt(ACCx * ACCx + ACCy * ACCy + ACCz * ACCz)
accYnorm = ACCy/math.sqrt(ACCx * ACCx + ACCy * ACCy + ACCz * ACCz)


#Calculate pitch and roll
pitch = math.asin(accXnorm)
roll = -math.asin(accYnorm/math.cos(pitch))


#Calculate the new tilt compensated values
magXcomp = MAGx*math.cos(pitch)+MAGz*math.sin(pitch)
magYcomp = MAGx*math.sin(roll)*math.sin(pitch)+MAGy*math.cos(roll)-MAGz*math.sin(roll)*math.cos(pitch)

#Calculate tilt compensated heading
tiltCompensatedHeading = 180 * math.atan2(magYcomp,magXcomp)/M_PI

if tiltCompensatedHeading < 0:
    tiltCompensatedHeading += 360



if 1:			#Change to '0' to stop showing the angles from the accelerometer
        print ("\033[1;34;40mACCX Angle %5.2f ACCY Angle %5.2f  \033[0m  " % (AccXangle, AccYangle)),

if 1:			#Change to '0' to stop  showing the angles from the gyro
        print ("\033[1;31;40m\tGRYX Angle %5.2f  GYRY Angle %5.2f  GYRZ Angle %5.2f" % (gyroXangle,gyroYangle,gyroZangle)),

if 1:			#Change to '0' to stop  showing the angles from the complementary filter
        print ("\033[1;35;40m   \tCFangleX Angle %5.2f \033[1;36;40m  CFangleY Angle %5.2f \33[1;32;40m" % (CFangleX,CFangleY)),
        
if 1:			#Change to '0' to stop  showing the heading
        print ("HEADING  %5.2f \33[1;37;40m tiltCompensatedHeading %5.2f" % (heading,tiltCompensatedHeading))
        




#slow program down a bit, makes the output more readable
time.sleep(0.03)


# 画像読み込み
img_src = cv2.imread("/home/pi/Desktop/image.png")

# 画像サイズの取得(横, 縦)
#print(img_src.shape[0])
size = tuple([img_src.shape[1], img_src.shape[0]])

# 画像の中心位置(x, y)
center = tuple([int(size[1]/2), int(size[0]/2)])

# 回転させたい角度（正の値は反時計回り）
angle = CFangleX

# 拡大比率
scale = 0.3

# 回転変換行列の算出
rotation_matrix = cv2.getRotationMatrix2D(center, angle, scale)

# アフィン変換
img_rot = cv2.warpAffine(img_src, rotation_matrix, size, flags=cv2.INTER_CUBIC)

# 表示
cv2.imshow("Show ROTATION Image", img_rot)
cv2.waitKey(5000)
cv2.destroyAllWindows()

#保存
files = glob.glob('/home/pi/Desktop/img-*.jpg')
i = 0
i = len(files)
#print(i)
i = str(i + 1)

name = i.zfill(3)+".jpg",img_rot

cv2.imwrite("/home/pi/Desktop/img-"+i.zfill(3)+".jpg",img_rot)
print(i.zfill(3))




#画像を取り入れる
im = cv2.imread('/home/pi/Desktop/img-%s.jpg'%i.zfill(3))
#im = cv2.imread('/home/pi/Desktop/image.png')
#サイズの変更
im = cv2.resize(im,(192,108))

#グレースケールに変換
#im = cv2.cvtColor(im, cv2.COLOR_BGR2GRAY)
#print("OK")
# jpegの圧縮率を設定 0～100値が高いほど高品質。何も指定しなければ95
encode_param = [int(cv2.IMWRITE_JPEG_QUALITY), 100]

# 文字列に変換
jpegsByte = cv2.imencode('.jpeg', im, encode_param)[1].tostring()



client.send(jpegsByte)

print("OK")