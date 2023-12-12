def main():
    text = input("Text: ")

    index = calculateIndex(text)

    if index < 1:
        print("Before Grade 1")
    elif index > 16:
        print("Grade 16+")
    else:
        print(f"Grade {index}")


def calculateIndex(text):
    letters = words = sentences = 0 # initialize counters
    inSentence = False # initialize sentence flag

    length = len(text) # store the length of the string
    # loops through the 2nd to last character in the string (range automatically subtracts 1 too)
    for i in range(length-1):
        # DEBUGGING
        curChar = text[i]
        nextChar = text[i+1]

        # checks if this character is a letter
        if curChar.isalpha():
            letters += 1

            # checks if this is the start of a new sentence
            if not inSentence:
                inSentence = True # flag new sentence as ongoing
            # otherwise checks if the next character is an ending punctuation mark
            elif nextChar == '.' or nextChar == '!' or nextChar == '?':
                # account for this sentence, as well as the last word in it
                sentences += 1
                words += 1
                inSentence = False # flag the end of this sentence
        
        # ensures this is an ongoing sentence
        if inSentence:
            # checks if the current character is a space (denoting the end of the previous word)
            if curChar.isspace():
                # ensures multiple spaces in a row can only denote a single word
                if not nextChar.isspace():
                    words += 1
    
    print(f"{letters} letters")
    print(f"{words} words")
    print(f"{sentences} sentences")

    L = (letters / words) * 100
    S = (sentences / words) * 100
    index = 0.0588 * L - 0.296 * S - 15.8

    return round(index)


main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/3. Problem Sets/Week 6/sentimental-readability$ python readability.py 
Text: Alice was beginning to get very tired of sitting by her sister on the bank, and of having nothing to do: once or twice she had peeped into the book her sister was reading, but it had no pictures or conversations in it, "and what is the use of a book," thought Alice "without pictures or conversation?"
235 letters
57 words
1 sentences
Grade 8
"""