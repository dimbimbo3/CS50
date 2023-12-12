def main():
    userInput = input("Greeting: ")
    #checks if the string contains the substring
    if "hello" in userInput.lower():
        print("$0")
    #else checks if the string starts with h
    elif userInput.lower()[0] == 'h':
        print("$20")
    else:
        print("$100")

main()