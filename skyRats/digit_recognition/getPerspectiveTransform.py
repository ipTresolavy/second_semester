import numpy as np
import cv2

def getPoints(image):
	
	#Bloco 1

	x = 0
	y = 0
	
	while (x < np.shape(image)[1] and y < np.shape(image)[0]) and (image[5][x] < 250 and image[y][5] < 250):
		x += 1
		y += 1

	if x < np.shape(image)[1] and image[5][x] >= 250:
		x1 = x
		y1 = 5
	elif y < np.shape(image)[0] and image[y][5] >= 250 :
		x1 = 5
		y1 = y
	else:
		x1 = 0
		y1 = 0

	#Bloco 2

	x = 0
	y = np.shape(image)[0] - 1
	
	while (x < np.shape(image)[1] and y >= 0) and (image[np.shape(image)[0] - 5][x] < 250 and image[y][5] < 250):
		x += 1
		y -= 1

	if x < np.shape(image)[1] and image[np.shape(image)[0] - 5][x] >= 250:
		x2 = x
		y2 = np.shape(image)[0] - 5
	elif y >= 0 and image[y][5] >= 250 :
		x2 = 5
		y2 = y
	else:
		x2 = 0
		y2 = np.shape(image)[0] - 1



	#Bloco 3

	x = np.shape(image)[1] - 1
	y = 0
	
	while (x >= 0 and y < np.shape(image)[0]) and (image[5][x] < 250 and image[y][np.shape(image)[1] - 5] < 250):
		x -= 1
		y += 1

	if x >= 0 and image[5][x] >= 250:
		x3 = x
		y3 = 5
	elif y < np.shape(image)[0] and image[y][np.shape(image)[1] - 5] >= 250 :
		x3 = np.shape(image)[1] - 5
		y3 = y
	else:
		x3 = np.shape(image)[1] - 1
		y3 = 0


	# Bloco 4

	x = np.shape(image)[1] - 1
	y = np.shape(image)[0] - 1
	
	while (x >= 0 and y >= 0) and (image[np.shape(image)[0] - 5][x] < 250 and image[y][np.shape(image)[1] - 5] < 250):
		x -= 1
		y -= 1

	if x >= 0 and image[np.shape(image)[0] - 5][x] >= 250:
		x4 = x
		y4 = np.shape(image)[0] - 5
	elif y >= 0 and image[y][np.shape(image)[1] - 5] >= 250 :
		x4 = np.shape(image)[1] - 5
		y4 = y
	else:
		x4 = np.shape(image)[1] - 1
		y4 = np.shape(image)[0] - 1



	return [[x1, y1], [x2, y2], [x3, y3], [x4, y4]]


def orderPoints(pts):
    rect=np.zeros((4,2), dtype="float32")
    s= pts.sum(axis = 1)
    rect[0]=pts[np.argmin(s)]
    rect[2]=pts[np.argmax(s)]
    diff=np.diff(pts, axis = 1)
    rect[1]=pts[np.argmin(diff)]
    rect[3]=pts[np.argmax(diff)]
    return rect

	
def four_point_transform(image, pts):
	rect=orderPoints(pts)
	(tl, tr, bl, br)=rect
	widthA = np.sqrt(((br[0] - bl[0]) ** 2) + ((br[1] - bl[1]) ** 2))
	widthB = np.sqrt(((tr[0] - tl[0]) ** 2) + ((tr[1] - tl[1]) ** 2))
	maxWidth = max(int(widthA), int(widthB))
	heightA = np.sqrt(((tr[0] - br[0]) ** 2) + ((tr[1] - br[1]) ** 2))
	heightB = np.sqrt(((tl[0] - bl[0]) ** 2) + ((tl[1] - bl[1]) ** 2))
	maxHeight = max(int(heightA), int(heightB))
	absMax = max(maxHeight, maxWidth)
	dst = np.array([
		[0, 0],
		[absMax - 1, 0],
		[absMax - 1, absMax - 1],
		[0, absMax - 1]], dtype = "float32")
	M = cv2.getPerspectiveTransform(rect, dst)
	warped = cv2.warpPerspective(image, M, (absMax, absMax))
	return warped

# dst = np.array([
# 		[0, 0],
# 		[maxWidth - 1, 0],
# 		[maxWidth - 1, maxHeight - 1],
# 		[0, maxHeight - 1]], dtype = "float32")