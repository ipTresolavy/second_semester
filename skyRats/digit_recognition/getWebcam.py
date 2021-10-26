#from digit_recognition.digit_recog import digit_recog

# menos dilatação no warp_alpha
# consertar reconhecimento de dígitos (Digits[2] = '-') 
# consertar digit_recog no geral
# Lidar com quadrado da base de pouso 


import cv2
import numpy as np
from imutils import contours
import imutils
from getPerspectiveTransform import four_point_transform, getPoints
from digit_recog import digit_recog

DEBUG = False

class webcam:
    def __init__(self):
        self.cap = cv2.VideoCapture(0)

    def main_loop(self):
        while True:
            sucess, frame = self.cap.read()
            frame = np.array(frame)
            #print(frame.shape) #cursed BGR

            #Tratamento de imagem
            gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
            blurred = cv2.GaussianBlur(gray, (5, 5), 0)
            thresh = cv2.threshold(blurred, 150, 255,	cv2.THRESH_BINARY)[1]
            if DEBUG:
                cv2.imshow("threshold", thresh)
                cv2.waitKey(30) #pro pc do igor n morrer
            kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (1, 5))
            thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
            thresh = cv2.dilate(thresh,kernel,iterations = 1)
            if DEBUG:
                cv2.imshow("tratada", thresh)
                print(np.shape(thresh))
                cv2.waitKey(30) #pro pc do igor n morrer
            cnts = cv2.findContours(thresh.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
            cnts = imutils.grab_contours(cnts)
            warped = frame
            
            for c in cnts:
                (x1, y1, w1, h1) = cv2.boundingRect(c)
                i = 0
                if (h1)/(w1) >= 0.85 and (h1)/(w1) <= 1.15:
                    for c2 in cnts:
                        (x2, y2, w2, h2) = cv2.boundingRect(c2)
                        if( (x2 > x1) and (y2 > y1) and (x1 + w1 > x2 + w2) and (y1 + h1 > y2 + h2) ):
                            if i >= 10:
                                #cv2.rectangle(frame, (x1, y1), (x1 + w1, y1 + h1), (0,255,0))
                                toBeWarped  = four_point_transform(frame, np.array([[x1, y1], [x1, y1 + h1], [x1 + w1, y1], [x1 + w1, y1 + h1]], dtype="float32"))
                                cv2.imshow("frame", toBeWarped)
                                warped_alpha = four_point_transform(thresh, np.array([[x1, y1], [x1, y1 + h1], [x1 + w1, y1], [x1 + w1, y1 + h1]], dtype="float32"))
                                points = getPoints(warped_alpha)
                                warped = four_point_transform(toBeWarped, np.array(points, dtype="float32"))
                                #cv2.imshow("warped_alpha",warped_alpha)
                                #cv2.imshow("warped",warped)
                                #cv2.waitKey(15)
                                #cv2.waitKey(0)
                                if digit_recog(warped) == True:
                                    break
                            else:
                                i += 1
                                #cv2.rectangle(frame, (x2, y2), (x2 + w2, y2 + h2), (0,255,0))

                if i >= 10:
                    break

            #cv2.imshow("teste webcam", warped)
            #cv2.waitKey(30) #pro pc do igor n morrer


if __name__ == "__main__":
    var = webcam()
    var.main_loop()

