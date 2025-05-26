import easyocr

if __name__ == "__main__":
    reader = easyocr.Reader(['en'], gpu=True)
    result = reader.readtext('test1.png', detail=0)

    with open("detection.txt","w") as f:
        f.write(str(len(result))+"\n")
        for r in result:
            f.write(r+"\n")