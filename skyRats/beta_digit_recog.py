import cv2
from imutils.perspective import four_point_transform
from imutils import contours
import imutils
import os

from numpy import double, float64

def digit_recog(image):
    duration = 3  # seconds
    freq = 440  # Hz

    DIGITS_LOOKUP = {
        (1, 1, 1, 0, 1, 1, 1): 0,
        (0, 0, 1, 0, 0, 1, 0): 1,
        (1, 0, 1, 1, 1, 1, 0): 2,
        (1, 0, 1, 1, 0, 1, 1): 3,
        (0, 1, 1, 1, 0, 1, 0): 4,
        (1, 1, 0, 1, 0, 1, 1): 5,
        (1, 1, 0, 1, 1, 1, 1): 6,
        (1, 0, 1, 0, 0, 1, 0): 7,
        (1, 1, 1, 1, 1, 1, 1): 8,
        (1, 1, 1, 1, 0, 1, 1): 9
    }

    #path = "display_metano0.png"
    #Baixando a imagem
    #image = cv2.imread("" + path)

    #Tratamento de imagem
    gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
    blurred = cv2.GaussianBlur(gray, (5, 5), 0)
    thresh = cv2.threshold(blurred, 0, 255,	cv2.THRESH_BINARY | cv2.THRESH_OTSU)[1]
    kernel = cv2.getStructuringElement(cv2.MORPH_ELLIPSE, (1, 5))
    #sat= cv2.cvtColor(np.resize(blurred, (406, 406, 3)),cv2.COLOR_BGR2HSV)[1]
    #thresh = cv2.threshold(sat,50,255, cv2.THRESH_BINARY)[1]
    #morph = cv2.morphologyEx(thresh, cv2.MORPH_CLOSE, kernel, iterations = 1)
    #mask = cv2.morphologyEx(morph, cv2.MORPH_OPEN, kernel, iterations =1)
    #otsu = cv2.threshold(gray, 0 , 255, cv2.THRESH_BINARY+cv2.THRESH_OTSU)
    #otsu_result = np.asarray(list(otsu)).copy()
    #otsu_result[mask==0]==0
    #image_result=image.copy()
    #image_result[mask==0]==0
    thresh = cv2.morphologyEx(thresh, cv2.MORPH_OPEN, kernel)
    
    thresh = cv2.dilate(thresh,kernel,iterations = 2)
    cnts = cv2.findContours(thresh.copy(), cv2.RETR_LIST, cv2.CHAIN_APPROX_SIMPLE)
    cnts = imutils.grab_contours(cnts)
    digitCnts = []

    for c in cnts:
        (x, y, w, h) = cv2.boundingRect(c)
        if((w<=20 and h<=150) or (w<=70 and h<=35) or ((w<=95 and w>=75) and (h<=170 and h>=155))):
            cv2.rectangle(image, (x, y), (x + w, y + h), (0,255,0))
            digitCnts.append(c)

    linha1 = []
    linha2 = []

    for c in digitCnts:
        (x, y, w, h) = cv2.boundingRect(c)
        if y<200:
            linha1.append(c)
        else:
            linha2.append(c)
    if len(linha1) > 0 and len(linha2) > 0:
        contours.sort_contours(linha1)
        contours.sort_contours(linha2)

    Digits = []
    first_number_digit_count = 0
    second_number_digit_count = 0

    for c in linha1:
        if first_number_digit_count < 3:
            (x, y, w, h) = cv2.boundingRect(c)
            roi = thresh[y:y+h, x:x+w]
            (roiH, roiW) = roi.shape
            if (roiW<=20 and roiH<=150): #digit one
                Digits.append("1")
                first_number_digit_count += 1
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 1)
                cv2.putText(image,"1", (x - 10, y - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
                continue
            else:                        #any other digit
                (dW, dH) = (int(roiW * 0.25), int(roiH * 0.15))
                dHC = int(roiH * 0.05)
                segments = [
                    ((0, 0), (w, dH)),	# top
                    ((0, 0), (dW, h // 2)),	# top-left
                    ((w - dW, 0), (w, h // 2)),	# top-right
                    ((0, (h // 2) - dHC) , (w, (h // 2) + dHC)), # center
                    ((0, h // 2), (dW, h)),	# bottom-left
                    ((w - dW, h // 2), (w, h)),	# bottom-right
                    ((0, h - dH), (w, h))	# bottom
                ]
                on = [0] * len(segments)
                for (i, ((xA, yA), (xB, yB))) in enumerate(segments):
                    segROI = roi[yA:yB, xA:xB]
                    total = cv2.countNonZero(segROI)
                    area = (xB - xA) * (yB - yA)
                    try:
                        if float(area) > 0 and total / float(area) > 0.5:
                            on[i]= 1
                    except KeyError:
                        print("nao consegue dividir")
                        return False

                try: 
                    digit = DIGITS_LOOKUP[tuple(on)]
                except KeyError:
                    print("Erro no primeiro numero")
                    return False
                Digits.append(str(digit))
                first_number_digit_count += 1
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 1)
                cv2.putText(image, str(digit), (x - 10, y - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)

    for c in linha2:
        if second_number_digit_count < 4:
            (x, y, w, h) = cv2.boundingRect(c)
            roi = thresh[y:y+h, x:x+w]
            (roiH, roiW) = roi.shape
            if (  (roiW >= 60 and roiW<=70) and (roiH >= 30 and roiH<=35)  ): #-
                Digits.append("-")
                second_number_digit_count += 1
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 1)
                cv2.putText(image,"minus sign", (x - 10, y - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.45, (0, 255, 0), 2)
                continue
            elif (roiW<=20 and roiH<=150): #digit one
                Digits.append("1")
                second_number_digit_count += 1
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 1)
                cv2.putText(image,"1", (x - 10, y - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
                continue
            else:                           #any other digit
                (dW, dH) = (int(roiW * 0.25), int(roiH * 0.15))
                dHC = int(roiH * 0.05)
                segments = [
                    ((0, 0), (w, dH)),	# top
                    ((0, 0), (dW, h // 2)),	# top-left
                    ((w - dW, 0), (w, h // 2)),	# top-right
                    ((0, (h // 2) - dHC) , (w, (h // 2) + dHC)), # center
                    ((0, h // 2), (dW, h)),	# bottom-left
                    ((w - dW, h // 2), (w, h)),	# bottom-right
                    ((0, h - dH), (w, h))	# bottom
                ]
                on = [0] * len(segments)
                for (i, ((xA, yA), (xB, yB))) in enumerate(segments):
                    segROI = roi[yA:yB, xA:xB]
                    total = cv2.countNonZero(segROI)
                    area = (xB - xA) * (yB - yA)
                    if area > 0 and total / float(area) > 0.5:
                        on[i]= 1

                try: 
                    digit = DIGITS_LOOKUP[tuple(on)]
                except KeyError:
                    print("Erro no segundo numero")
                    return False
                Digits.append(str(digit))
                second_number_digit_count += 1
                cv2.rectangle(image, (x, y), (x + w, y + h), (0, 255, 0), 1)
                cv2.putText(image, str(digit), (x - 10, y - 10),
                cv2.FONT_HERSHEY_SIMPLEX, 0.65, (0, 255, 0), 2)
        
    primeiro_digito = 0
    i = 0
    if first_number_digit_count > 0:
        while i < first_number_digit_count:
            primeiro_digito += int(Digits[i]) * (10**i)
            i += 1

    segundo_digito = ''
    if i < first_number_digit_count + second_number_digit_count and Digits[i] == "-":
        while(i < first_number_digit_count + second_number_digit_count):
            segundo_digito += Digits[i]
            i += 1
        segundo_digito = int(''.join(segundo_digito))
        segundo_digito *= -1
    else:
        while(i < first_number_digit_count + second_number_digit_count):
            segundo_digito += Digits[i]
            i += 1
        segundo_digito = int(''.join(segundo_digito))

    print(primeiro_digito, segundo_digito)

    if primeiro_digito > 55 or primeiro_digito < 45:
        print('PERCENTUAL DE GAS FORA DE CONFORMIDADE')
        #os.system('play -nq -t alsa synth {} sine {}'.format(duration, freq))
    else:
        print('\033[1;37;42m PERCENTUAL DE GAS DENTRO DOS CONFORMES \033[0;0m')

    print("\n(Apos 30 segundos) Procedendo para leitura do ajuste de zero...\n")

    if segundo_digito > 5 or segundo_digito <= -5:
        print('AJUSTE DE ZERO FORA DE CONFORMIDADE')
        #os.system('play -nq -t alsa synth {} sine {}'.format(duration, freq))
    else:
        print('AJUSTE DE ZERO DENTRO DOS CONFORMES')

    return True
    #Teste de imagem
    #cv2.imshow("teste retangulos", image)
    #cv2.waitKey(30)