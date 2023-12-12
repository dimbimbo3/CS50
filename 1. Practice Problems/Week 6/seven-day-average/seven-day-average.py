import csv
import requests


def main():
    # Read NYTimes Covid Database
    download = requests.get(
        "https://raw.githubusercontent.com/nytimes/covid-19-data/master/us-states.csv"
    )
    decoded_content = download.content.decode("utf-8")
    file = decoded_content.splitlines()
    reader = csv.DictReader(file)

    # Construct 14 day lists of new cases for each states
    new_cases = calculate(reader)

    # Create a list to store selected states
    states = []
    print("Choose one or more states to view average COVID cases.")
    print("Press enter when done.\n")

    # Continue to prompt the user
    while True:
        state = input("State: ")
        if state in new_cases:
            states.append(state)
        if len(state) == 0:
            break

    print(f"\nSeven-Day Averages")

    # Print out 7-day averages for this week vs last week
    comparative_averages(new_cases, states)


# Create a dictionary to store 14 most recent days of new cases by state
def calculate(reader):
    # initialize empty dictionaries (associative arrays)
    new_cases = dict()
    previous_cases = dict()

    # loop through each line in the csv
    for line in reader:
        state = line["state"] # stores the name of the state on this line
        totalCases = int(line["cases"]) # converts the given state's current case total into an integer and stores it

        # checks if this is the first occurance of this state (key)
        if state not in new_cases:
            new_cases[state] = [] # initializes an empty list for this state's new cases
        else:
            newCases = totalCases - previous_cases[state] # calculates the state's new cases for the day using the current total and the previous day's
            new_cases[state].append(newCases) # appends the state's new cases for the day to the end of its list in the dictionary

            # ensures that only the past 14 days of new cases are being stored
            if len(new_cases[state]) > 14:
                new_cases[state].pop(0) # removes the first element in the list
        
        previous_cases[state] = totalCases # sets/updates previous case total for the state's next occurance
    
    return new_cases # returns newly populated dicationary



# Calculate and print out seven day average for given state
def comparative_averages(new_cases, states):
    # loops through each state in the user's provided list
    for state in states:
        week2Avg = sum(new_cases[state][:7]) / 7 # last week's average
        week1Avg = sum(new_cases[state][7:]) / 7 # this week's average
        difference = (week1Avg - week2Avg) # calculates the difference between the averages

        # tries to calculate the percentage of change
        try:
            percentage = (difference / week2Avg) * 100

            # checks whether the difference was a positive or a negative
            if difference > 0:
                change = "an increase"
            else:
                change = "a decrease"
                percentage *= -1 # convert the negative percentage to a positive
            
            print(f"{state} had a 7-day average of {int(week1Avg)} and {change} of {int(percentage)}%")
        except ZeroDivisionError:
            print("[ERROR: Division by zero]")
            print(f"{state} had a 7-day average of {int(week1Avg)} and an undefined change in percentage.")



main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/Week 6/seven-day-average$ python seven-day-average.py 
Choose one or more states to view average COVID cases.
Press enter when done.

State: test
State: New York
State: Wyoming
State: Arkansas
State: Massachusetts
State: Texas
State: American Samoa
State: 

Seven-Day Averages
New York had a 7-day average of 1040 and a decrease of 13%
Wyoming had a 7-day average of 22 and a decrease of 37%
Arkansas had a 7-day average of 51 and a decrease of 65%
Massachusetts had a 7-day average of 362 and a decrease of 22%
Texas had a 7-day average of 1357 and a decrease of 89%
[ERROR: Division by zero]
American Samoa had a 7-day average of 0 and an undefined change in percentage.
"""