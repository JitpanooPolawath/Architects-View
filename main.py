import sys
import easyocr
import cv2 as cv
import os

if __name__ == "__main__":
    
    print("Loading OCR.....")
    reader = easyocr.Reader(['en'])
    # Activate camera
    cam = cv.VideoCapture(0)
    cv.namedWindow("ArchView")
    print("Press [Space] to take the image or [Escape] to exit the program")
    while True:
        result, image = cam.read()
        if result:
            cv.imshow("ArchView", image)
            
            k = cv.waitKey(1)

            if k%256 == 27:
                break 
            if k%256 == 32:
                
                cv.imwrite("test1.jpg", image)
                break
        else:
            break
    
    cam.release()

    cv.destroyAllWindows()

    # Initialize OCR
    print("Parsing image...")
    result = reader.readtext('test1.jpg',detail=0)
    
    with open("detection.txt","w") as f:
        f.write(str(len(result))+"\n")
        for r in result:
            f.write(r+"\n")
    
    # Execute program

    returnCode = os.system('"Architect\'s View.exe"')
    
    if returnCode != 0:
        print("Parsing error, please retry again with better lighting for positing")
        
