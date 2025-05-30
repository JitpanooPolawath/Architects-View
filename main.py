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
                print("Parsing image...")
                result = reader.readtext('test1.jpg',detail=0)
                finalRe = []
                with open("detection.txt","w") as f:
                    f.write(str(len(result))+"\n")
                    for r in result:
                        print(r)
                        againN = input("If result is correct press [Enter] else type [Again]: ")
                        if againN.lower() == "again":
                            nSpec = input("input specification: ")
                            r = nSpec
                        finalRe.append(r)
                        f.write(r+"\n")
                    
                for r in finalRe:
                    print(r)
                n = input("If result is correct type [Yes] else [No]: ")
                if n.lower() == "yes":
                    break
                else:
                    print("\n\n\n\nRetry image")
                    continue
        else:
            break
    
    f.close()
    cam.release()

    cv.destroyAllWindows()    
    
    # Execute program

    returnCode = os.system('"Architect\'s View.exe"')
    
    if returnCode != 0:
        print("Parsing error, please retry again with better lighting for positing")
        
