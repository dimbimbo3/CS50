from pyfiglet import Figlet
from random import randint
import sys

# global variables
figlet = Figlet()
fonts = figlet.getFonts() # list of available fonts

# main function
def main():
    argCheck()
    if(len(sys.argv) == 1):
        figlet.setFont(font=fonts[randint(0, len(fonts) - 1)]) # randomly assign a font from the list
    else:
        figlet.setFont(font=sys.argv[2])
    userInput = input("Input: ")
    print("Output:\n" + figlet.renderText(userInput))


# ensures proper usage of arguments
def argCheck():
    # only allows 0 or 2 additional arguments
    if len(sys.argv) == 3:
        # checks that a valid flag and font were entered
        if (sys.argv[1] != "-f" and sys.argv[1] != "--font") or sys.argv[2] not in fonts:
            print("usage: python figlet.py -f,--font fontName")
            sys.exit(1)
        else:
            return
    elif len(sys.argv) == 1:
        return
    else:
        print("usage: python figlet.py -f,--font fontName")
        sys.exit(1)
        

main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py test
usage: python figlet.py -f,--font fontName
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py -a slant
usage: python figlet.py -f,--font fontName
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py -f invalid_font
usage: python figlet.py -f,--font fontName
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py
Input: hello, world
Output:
 __  __     ______     __         __         ______    
/\ \_\ \   /\  ___\   /\ \       /\ \       /\  __ \   
\ \  __ \  \ \  __\   \ \ \____  \ \ \____  \ \ \/\ \  
 \ \_\ \_\  \ \_____\  \ \_____\  \ \_____\  \ \_____\ 
  \/_/\/_/   \/_____/   \/_____/   \/_____/   \/_____/ 
                                                       
 __     __     ______     ______     __         _____    
/\ \  _ \ \   /\  __ \   /\  == \   /\ \       /\  __-.  
\ \ \/ ".\ \  \ \ \/\ \  \ \  __<   \ \ \____  \ \ \/\ \ 
 \ \__/".~\_\  \ \_____\  \ \_\ \_\  \ \_____\  \ \____- 
  \/_/   \/_/   \/_____/   \/_/ /_/   \/_____/   \/____/ 
                                                         
                                     
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py -f slant
Input: CS50
Output:
   ___________ __________ 
  / ____/ ___// ____/ __ \
 / /    \__ \/___ \/ / / /
/ /___ ___/ /___/ / /_/ / 
\____//____/_____/\____/  
                          
                 
"""


"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/figlet$ python figlet.py --font alphabet
Input: Moo
Output:
M   M         
MM MM         
M M M ooo ooo 
M   M o o o o 
M   M ooo ooo 
              
              

"""