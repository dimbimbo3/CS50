class Jar:
    def __init__(self, capacity=12):
        if capacity < 0:
            raise ValueError("The jar's capacity must be a non-negative number.")
        else:
            # initialize class values
            self._capacity = capacity
            self._size = 0

    def __str__(self):
        return "🍪" * self.size

    def deposit(self, n):
        if (self.size + n) > self.capacity:
            raise ValueError("The given deposit exceeds the jar's capacity.")
        else:
            self._size += n

    def withdraw(self, n):
        if n > self.size:
            raise ValueError("The given withdrawl exceeds the current number of cookies.")
        else:
            self._size -= n

    @property
    def capacity(self):
        return self._capacity
    
    @property
    def size(self):
        return self._size

# main function
def main():
    # instantiate a new jar object
    jar = Jar()
    # prints out the jar's capacity
    print("Capacity = " + str(jar.capacity))
    # prints the current contents of the jar
    print(jar)
    # adds two cookies to the jar
    jar.deposit(2)
    # prints the new contents of the jar (+2)
    print(jar)
    # removes one cookie from the jar
    jar.withdraw(1)
    # prints the new contents of the jar (-1)
    print(jar)

main() # call main

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/1. Practice Problems/week 6/jar$ python3 jar.py
Capacity = 12

🍪🍪
🍪
"""