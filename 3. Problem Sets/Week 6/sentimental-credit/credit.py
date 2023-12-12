def main():
    # loops to ensure only numbers are entered
    while True:
        cardNum = input("Number: ")
        if cardNum.isdigit():
            break
        else:
            print("Invalid input...")
    
    length = len(cardNum) # stores the length of the entered card number

    # checks the validity of the given card number
    if(validity(cardNum)):
        if length == 13:
            print("VISA")
        elif length == 15:
            print("AMEX")
        elif length == 16:
            if cardNum[0] == "4":
                print("VISA")
            else:
                print("MASTERCARD")
    else:
        print("INVALID")


# determines if the given card number is valid
def validity(cardNum):
    length = len(cardNum) # stores the length of the entered card number

    # if the card number isn't 13, 15, or 16 digits long then it's not valid
    if length != 13 and length != 15 and length != 16:
        return False

    total = 0 # initialize running total
    
    # loops through every other digit from the 2nd to last position up to the starting index
    for i in range(length-2, -1, -2):
        digit = int(cardNum[i]) # converts the digit of the card to an integer
        multiplied = digit * 2
        total += sumOfDigits(multiplied) # calculates the sum of the digits in the given multiplied number

    # loops through every other digit from the last position up to the starting index
    for i in range(length-1, -1, -2):
        digit = int(cardNum[i]) # converts the character to an integer
        total += digit

    # checks if the last digit of the overall total is a 0
    if total % 10 == 0:
        return True
    else:
        return False


# calculates the sum of a number's digits
def sumOfDigits(number):
    sum = 0 # initialize sum

    # loops through each digit in the number
    while number > 0:
        digit = number % 10
        sum += digit
        number = number // 10 # floor division (rounds down)
    
    return sum


main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-credit$ python credit.py 
Number: 4003-6000-0000-0014 
Invalid input...
Number: foo
Invalid input...
Number: 4003600000000014
VISA
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-credit$ python credit.py 
Number: 6176292929
INVALID
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-credit$ python credit.py 
Number: 378282246310005
AMEX
"""