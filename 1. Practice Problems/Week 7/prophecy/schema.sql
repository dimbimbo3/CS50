CREATE TABLE students(
    stuID INTEGER UNIQUE NOT NULL,
    student_name TEXT,
    PRIMARY KEY(stuID)
);

CREATE TABLE houses(
    house TEXT UNIQUE NOT NULL,
    head TEXT,
    PRIMARY KEY(house)
);

CREATE TABLE assignments(
    house TEXT NOT NULL,
    stuID INTEGER NOT NULL,
    FOREIGN KEY(house) REFERENCES houses(house),
    FOREIGN KEY(stuID) REFERENCES students(stuID),
    PRIMARY KEY(house, stuID)
);