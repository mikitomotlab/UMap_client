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

    return(frame)

def _init_(self):
    selr.image_pub = rospy.Publisher("image_topic", Image, queue_size=1)

    self.bridge = CvBridge()

def tocommunicate(self, image):
    self.image_pub.publish(self.bridge.cv2_to_imgmsg(image, "bgr8"))

def main():
    image = capture()
    tocommunicate(image)

main()
