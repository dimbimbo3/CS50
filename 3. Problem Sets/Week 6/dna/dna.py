import csv
import sys


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    data = [] # initialize empty list for people in the database
    # Read database file into a list of dictionaries
    try:
        with open(sys.argv[1], "r") as file:
            reader = csv.DictReader(file)

            # reads and stores every line from the csv file into the list
            for line in reader:
                data.append(line)
    # catch all exceptions
    except Exception:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Read DNA sequence file into a variable
    try:
        with open(sys.argv[2], "r") as file:
            sequence = file.read() # reads in the entire file
    # catch all exceptions
    except Exception:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    # Find longest match of each STR in DNA sequence
    matches = {} # initialize empty dictionary for longest STR subsequences in the given DNA sequence
    # loops through each STR from given database
    for key in data[0]:
        # ensures only STRs are accounted for
        if key != "name":
            matches[key] = longest_match(sequence, key) # counts the total of matches for the given STR and stores it

    # Check database for matching profiles
    # loop through each person from the database
    for person in data:
        noMatch = False # initialize flag for determining if someone is not a match

        # loops through each longest STR in the sequence
        for dnaSTR in matches:
            longestSTR = matches[dnaSTR]
            STRcount = int(person[dnaSTR])
            # compares each longest STR match with the given person's count
            # to determine if this person should be ruled out as a match
            if STRcount != longestSTR:
                noMatch = True
                break
            
            # if one of this person's STR counts didn't match the longest amount for that STR
            # then they aren't a match, so move on to the next person
            if noMatch == True:
                break
        
        # if this all of this person's STR counts never ruled them out 
        # then the sequence must be theirs
        if noMatch == False:
            print(person["name"])
            return # ends script

    print("No match")
    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1
            
            # If there is no match in the substring
            else:
                break
        
        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/small.csv sequences/1.txt
Bob
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/small.csv sequences/2.txt
No match
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/small.csv sequences/3.txt
No match
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/small.csv sequences/4.txt
Alice
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/5.txt
Lavender
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/6.txt
Luna
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/7.txt
Ron
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/8.txt
Ginny
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/9.txt
Draco
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/10.txt
Albus
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/11.txt
Hermione
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/12.txt
Lily
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/13.txt
No match
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/14.txt
Severus
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/15.txt
Sirius
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/16.txt
No match
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/17.txt
Harry
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/18.txt
No match
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/19.txt
Fred
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/dna$ python dna.py databases/large.csv sequences/20.txt
No match
"""