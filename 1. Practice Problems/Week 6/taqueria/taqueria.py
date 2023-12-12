def main():
    menu = {
    "Baja Taco": 4.25,
    "Burrito": 7.50,
    "Bowl": 8.50,
    "Nachos": 11.00,
    "Quesadilla": 8.50,
    "Super Burrito": 8.50,
    "Super Quesadilla": 9.50,
    "Taco": 3.00,
    "Tortilla Salad": 8.00
    }

    total = 0 # set initial order total

    # continues to prompt user for items
    # except if the user inputs Ctrl+D
    while True:
        try:
            # captures given input and converts it to title case
            item = input("Item:  ").title()
            
            # checks if the user's input is a key within the menu dictionary
            if item in menu:
                total += menu[item] # if so add its cost to the running tally
                print(f"Total: ${total:.2f}") # print out the current order total with 2 decimal place percision
        except EOFError:
            print("\n") # moves cursor to the next line
            break # ends the loop

main() # call main function

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/taqueria$ python taqueria.py 
Item:  test
Item:  burrito
Total: $7.50
Item:  bowl
Total: $16.00
Item:  taco
Total: $19.00
Item:  burger
Item:  super quesadilla
Total: $28.50
Item:  

"""