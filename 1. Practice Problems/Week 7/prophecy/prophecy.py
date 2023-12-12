import csv
from cs50 import SQL

def main():
    db = SQL("sqlite:///roster_new.db")
    data = getData()
    #insertStudents(data, db)
    #insertHouses(data, db)
    #insertAssignments(data, db)
    updateSchema(data, db)


# retrieves all data from csv file and stores in a list of dictionaries
def getData():
    with open("students.csv", "r") as inFile:
        reader = csv.DictReader(inFile)
        
        data = [] # initialize empty list for csv data
        for line in reader:
            data.append(line)
    
    return data

"""
# inserts the students in accordance with the updated schema
def insertStudents(data, db):
    for student in data:
        db.execute("INSERT INTO students(stuID, student_name) VALUES(?, ?)", student["id"], student["student_name"])


# inserts the houses in accordance with the updated schema
def insertHouses(data, db):
    existingHouses = [] # initialize empty list to keep track of existing houses
    for student in data:
        # ensures each house only gets inserted once
        if student["house"] not in existingHouses:
            existingHouses.append(student["house"])
            db.execute("INSERT INTO houses(house, head) VALUES(?, ?)", student["house"], student["head"])


# inserts assignments in accordance with the updated schema
def insertAssignments(data, db):
    for student in data:
        db.execute("INSERT INTO assignments(house, stuID) VALUES(?, ?)", student["house"], student["id"])
"""

# inserts all tables into new database schema
def updateSchema(data, db):
    existingHouses = [] # initialize empty list to keep track of existing houses
    # loops through each student's row of data from csv
    for student in data:
        # students table
        db.execute("INSERT INTO students(stuID, student_name) VALUES(?, ?)", student["id"], student["student_name"])
        # houses table
        if student["house"] not in existingHouses:
            existingHouses.append(student["house"]) # ensures each house only gets inserted once
            db.execute("INSERT INTO houses(house, head) VALUES(?, ?)", student["house"], student["head"])
        # assignments table
        db.execute("INSERT INTO assignments(house, stuID) VALUES(?, ?)", student["house"], student["id"])

main()