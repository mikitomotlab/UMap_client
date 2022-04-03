#!/usr/bin/env python
import rospy
import cv2
import sys
from sensor_msgs.msg import Image
from cv_bridge import CvBridge, CvBridgeError

def callback(data):
    rospy.loginfo(rospy.get_caller_id()+"i heard %s", data.data)
    print ("get image")


def listener():

    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("image_topic", Image, callback)
    rospy.spin()



if __name__=='__main__':
    listener()
