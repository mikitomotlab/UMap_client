#!/usr/bin/env python
# -*- coding: utf-8 -*-

import sys
import rospy
import cv2
import numpy as np
from std_msgs.msg import String
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError
import copy

def capture():
    cap = cv2.VideoCapture(0)
    if not cap.isOpened():
      print("cannot open camera")
    
    ret, frame = cap.read()
    cv2.imshow('try', frame)
    cv2.imwrite('/home/miki/development_v1/src/camera/src/images/image.png', frame)
    cv2.waitKey(3000)
    #cv2.destroyWindow(try)

    #return(frame)

    rospy.init_node('talker', anonymous=True)

    bridge = CvBridge()

    image_pub = rospy.Publisher('image_topic', Image, queue_size=10)

    msg = bridge.cv2_to_imgmsg(frame, encoding="bgr8")

    rate = rospy.Rate(10)
    while not rospy.is_shutdown():
        image_pub.publish(msg)
        rate.sleep()

    #for count in range(10):
        #image_pub.publish(msg)
       # rate.sleep
       # print (count)
  
   # image_pub.publish(msg)
   # rate.sleep()



    #image_pub.publish(bridge.cv2_to_imgmsg(frame, "bgr8"))
    
    print("sended image data")

def main():
    capture()

main()
