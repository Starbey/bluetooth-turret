import cv2
import numpy as np

cap = cv2.VideoCapture(0) # capture webcam

face_cascade = cv2.CascadeClassifier(cv2.data.haarcascades + "haarcascade_frontalface_default.xml")

while True:
    ret, frame = cap.read()

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
    faces = face_cascade.detectMultiScale(gray, scaleFactor = 1.05, minNeighbors = 7, minSize = (100, 100)) # returns list of rectangles around faces

    for (x, y, w, h) in faces:
        cv2.rectangle(frame, (x, y), (x + w, y + h), (255, 0, 0), 5)

    cv2.imshow("frame", frame)

    if cv2.waitKey(1) == ord('q'):
        break

cap.release() # release webcam
cv2.destroyAllWindows()
