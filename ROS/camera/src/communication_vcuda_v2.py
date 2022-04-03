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
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

class get_image:
    def __init__(self):
        self.bridge = CvBridge()
        self.image_sub = rospy.Subscriber("image_topic", Image, self.callback)

    def callback(self,data):
        try:
            self.cv_image = self.bridge.imgmsg_to_cv2(data, "bgr8")
            self.image = np.fromstring(data.data, np.uint8)
            self.image = cv2.imdecode(img_src, cv2.CV_LOAD_IMAGE_COLOR)
        except CvBridgeError as e:
            print('Cv_Bridge_Error')

    def image_cv2(self):
        print('cv2_image')
        return self.image

def to_server(image_src):
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
    #img_src = cv2.imread("/home/miki/development_v1/src/camera/src/image.png")
    cv2.imshow("try1", image_src)
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
image = get_image()
rospy.init_node('listenner')
to_server(image.image_cv2)
rospy.spin()    
