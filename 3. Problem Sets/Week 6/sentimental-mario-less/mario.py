def main():
    # loops to ensure an integer is entered (from 1-8)
    while True:
        try:
            height = int(input("Height: "))
            if height >= 1 and height <= 8:
                break # ends loop on valid input
            else:
                print("Height must be between 1 and 8.")
        except ValueError:
            print("Invalid input...")
    
    # loops through half-pyramid
    for rows in range(height):
        for cols in range(height):
            if cols < (height - (1+rows)):
                print(" ", end="")
            else:
                print("#", end="")
        print("") # allow new line for next row

main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-mario-less$ python mario.py
Height: test
Invalid input...
Height: 0
Height must be between 1 and 8.
Height: 8
       #
      ##
     ###
    ####
   #####
  ######
 #######
########
"""