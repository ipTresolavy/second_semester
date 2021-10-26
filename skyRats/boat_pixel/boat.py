import cv2
import numpy as np
MIN_RED = 100
MIN_VER = 100

cv_image = cv2.imread('Barco1.png')
cv_image2 = cv2.imread('Barco2.png')
cv_image3 = cv2.imread('Barco3.png')
hsv = cv2.cvtColor(cv_image,cv2.COLOR_BGR2HSV)
hsv2 = cv2.cvtColor(cv_image2,cv2.COLOR_BGR2HSV)
hsv3 = cv2.cvtColor(cv_image3,cv2.COLOR_BGR2HSV)
rows, cols, a = cv_image.shape
print(np.amax(rows, axis=0))
print(np.amax(hsv2, axis=1))
print(np.amax(hsv3, axis=1))

########## DETECTA LARANJA  ################################################
#Amarelo 28 105 222
#Verde low: 38 78 187  high: 42 86 201
#branco: 0 0 201 1 1 255

# blur = cv2.GaussianBlur(hsv, (3, 3), 0)
# cv2.imshow('raw hsv', hsv)
# cv2.imwrite("raw_hsv.png", hsv)
# cv2.waitKey(0)

lowerbBaseMovel = np.array([0, 0, 70])
upperbBaseMovel = np.array([359, 4, 255])
mask_BaseMovel = cv2.inRange(hsv, lowerbBaseMovel, upperbBaseMovel)    

rows_lar, cols_lar = mask_BaseMovel.shape
cv2.imshow('mask_BaseMovel', mask_BaseMovel)
cv2.waitKey(0)

lowerbbranco = np.array([0, 0, 200])
upperbbranco = np.array([4, 4, 255])
mask_branco = cv2.inRange(hsv, lowerbbranco, upperbbranco)    

rows_lar, cols_lar = mask_branco.shape
cv2.imshow('mask_branco', mask_branco)
cv2.waitKey(0)