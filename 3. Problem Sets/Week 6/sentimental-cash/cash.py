def main():
    # loop ensures valid user input
    while True:
        try:
            change = float(input("Change owed: $"))
            if change < 0:
                print("Change cannot be negative.")
            else:
                break # break out of loop if input is a non-negative number
        except ValueError:
            print("Invalid Input...")

    change = round(change, 2) * 100 # rounds user input to 2 decimal places & then converts from dollars to cents
    
    quarters = getQuarters(change)
    change -= 25 * quarters

    dimes = getDimes(change)
    change -= 10 * dimes

    nickels = getNickels(change)
    change -= 5 * nickels

    pennies = change # any remaining change will be pennies

    coins = quarters + dimes + nickels + pennies
    print(f"{coins}")


# determines how many quarters to give
def getQuarters(cents):
    return cents // 25 # floor division (int division: rounds down to whole number)


# determines how many dimes to give
def getDimes(cents):
    return cents // 10 # floor division (int division: rounds down to whole number)


# determines how many nickels to give
def getNickels(cents):
    return cents // 5 # floor division (int division: rounds down to whole number)


main() # call to main function

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-cash$ python cash.py
Change owed: $-1
Change cannot be negative.
Change owed: $test
Invalid Input...
Change owed: $0.41
4
"""