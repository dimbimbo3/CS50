-- Keep a log of any SQL queries you execute as you solve the mystery.

.schema
/*
CREATE TABLE crime_scene_reports (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    street TEXT,
    description TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE interviews (
    id INTEGER,
    name TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    transcript TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE atm_transactions (
    id INTEGER,
    account_number INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    atm_location TEXT,
    transaction_type TEXT,
    amount INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE bank_accounts (
    account_number INTEGER,
    person_id INTEGER,
    creation_year INTEGER,
    FOREIGN KEY(person_id) REFERENCES people(id)
);
CREATE TABLE airports (
    id INTEGER,
    abbreviation TEXT,
    full_name TEXT,
    city TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE flights (
    id INTEGER,
    origin_airport_id INTEGER,
    destination_airport_id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    PRIMARY KEY(id),
    FOREIGN KEY(origin_airport_id) REFERENCES airports(id),
    FOREIGN KEY(destination_airport_id) REFERENCES airports(id)
);
CREATE TABLE passengers (
    flight_id INTEGER,
    passport_number INTEGER,
    seat TEXT,
    FOREIGN KEY(flight_id) REFERENCES flights(id)
);
CREATE TABLE phone_calls (
    id INTEGER,
    caller TEXT,
    receiver TEXT,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    duration INTEGER,
    PRIMARY KEY(id)
);
CREATE TABLE people (
    id INTEGER,
    name TEXT,
    phone_number TEXT,
    passport_number INTEGER,
    license_plate TEXT,
    PRIMARY KEY(id)
);
CREATE TABLE bakery_security_logs (
    id INTEGER,
    year INTEGER,
    month INTEGER,
    day INTEGER,
    hour INTEGER,
    minute INTEGER,
    activity TEXT,
    license_plate TEXT,
    PRIMARY KEY(id)
);
*/

--Query 1: Get the description of the crime at the specified location and date
SELECT description FROM crime_scene_reports
WHERE year = 2021 AND month = 7 AND day = 28 AND street LIKE "Humph%";
--Theft of the CS50 duck took place at 10:15am at the Humphrey Street bakery. 
--Interviews were conducted today with three witnesses who were present at the time – each of their interview transcripts mentions the bakery.

--Query 2: Check interviews from the day mentioning the bakery
SELECT name, transcript FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE "%bakery%";
/*
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
|  name   |                                                                                                                                                     transcript                                                                                                                                                      |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| Ruth    | Sometime within ten minutes of the theft, I saw the thief get into a car in the bakery parking lot and drive away. If you have security footage from the bakery parking lot, you might want to look for cars that left the parking lot in that time frame.                                                          |
| Eugene  | I don't know the thief's name, but it was someone I recognized. Earlier this morning, before I arrived at Emma's bakery, I was walking by the ATM on Leggett Street and saw the thief there withdrawing some money.                                                                                                 |
| Raymond | As the thief was leaving the bakery, they called someone who talked to them for less than a minute. In the call, I heard the thief say that they were planning to take the earliest flight out of Fiftyville tomorrow. The thief then asked the person on the other end of the phone to purchase the flight ticket. |
+---------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
*/

--Query 3: Followup on Ruth's interview by checking security footage within 10 minutes of the theft and get the names/ids of the car owners
SELECT bsl.id, bsl.hour, bsl.minute, bsl.activity, bsl.license_plate, p.id, p.name 
FROM bakery_security_logs AS bsl 
JOIN people AS p ON bsl.license_plate = p.license_plate
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND (minute >= 15 AND minute <= 25);
/*
+-----+------+--------+----------+---------------+--------+---------+
| id  | hour | minute | activity | license_plate |   id   |  name   |
+-----+------+--------+----------+---------------+--------+---------+
| 260 | 10   | 16     | exit     | 5P2BI95       | 221103 | Vanessa |
| 261 | 10   | 18     | exit     | 94KL13X       | 686048 | Bruce   | <-
| 262 | 10   | 18     | exit     | 6P58WS2       | 243696 | Barry   |
| 263 | 10   | 19     | exit     | 4328GD8       | 467400 | Luca    |
| 264 | 10   | 20     | exit     | G412CB7       | 398010 | Sofia   |
| 265 | 10   | 21     | exit     | L93JTIZ       | 396669 | Iman    |
| 266 | 10   | 23     | exit     | 322W7JE       | 514354 | Diana   | <-
| 267 | 10   | 23     | exit     | 0NTHK55       | 560886 | Kelsey  |
+-----+------+--------+----------+---------------+--------+---------+
*/

--Query 4: Followup on Eugene's interview by checking atm withdrawls for the day at Legget Street and the people associated with the accounts
SELECT atm.id, atm.account_number, atm.atm_location, atm.transaction_type, atm.amount, p.id, p.name
FROM atm_transactions AS atm 
JOIN bank_accounts AS ba ON atm.account_number = ba.account_number
JOIN people AS p ON ba.person_id = p.id
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location LIKE "Leggett%" AND transaction_type = "withdraw";
/*
+-----+----------------+----------------+------------------+--------+--------+---------+
| id  | account_number |  atm_location  | transaction_type | amount |   id   |  name   |
+-----+----------------+----------------+------------------+--------+--------+---------+
| 267 | 49610011       | Leggett Street | withdraw         | 50     | 686048 | Bruce   | <-
| 336 | 26013199       | Leggett Street | withdraw         | 35     | 514354 | Diana   | <-
| 269 | 16153065       | Leggett Street | withdraw         | 80     | 458378 | Brooke  |
| 264 | 28296815       | Leggett Street | withdraw         | 20     | 395717 | Kenny   |
| 288 | 25506511       | Leggett Street | withdraw         | 20     | 396669 | Iman    |
| 246 | 28500762       | Leggett Street | withdraw         | 48     | 467400 | Luca    |
| 266 | 76054385       | Leggett Street | withdraw         | 60     | 449774 | Taylor  |
| 313 | 81061156       | Leggett Street | withdraw         | 30     | 438727 | Benista |
+-----+----------------+----------------+------------------+--------+--------+---------+
*/

--Query 5: Followup on Raymond's interview by checking phonecalls made on the day for less than a minute, as well who they were from and to
SELECT pc.id, pc.caller, p1.id, p1.name, pc.receiver, p2.id, p2.name, pc.duration
FROM phone_calls as pc
JOIN people AS p1 ON pc.caller = p1.phone_number
JOIN people AS p2 ON pc.receiver = p2.phone_number
WHERE year = 2021 AND month = 7 AND day = 28 AND duration < 60;
/*
+-----+----------------+--------+---------+----------------+--------+------------+----------+
| id  |     caller     |   id   |  name   |    receiver    |   id   |    name    | duration |
+-----+----------------+--------+---------+----------------+--------+------------+----------+
| 221 | (130) 555-0289 | 398010 | Sofia   | (996) 555-8899 | 567218 | Jack       | 51       |
| 224 | (499) 555-9472 | 560886 | Kelsey  | (892) 555-8872 | 251693 | Larry      | 36       |
| 233 | (367) 555-5533 | 686048 | Bruce   | (375) 555-8161 | 864400 | Robin      | 45       | <-
| 251 | (499) 555-9472 | 560886 | Kelsey  | (717) 555-1342 | 626361 | Melissa    | 50       |
| 254 | (286) 555-6063 | 449774 | Taylor  | (676) 555-6554 | 250277 | James      | 43       |
| 255 | (770) 555-1861 | 514354 | Diana   | (725) 555-3243 | 847116 | Philip     | 49       | <-
| 261 | (031) 555-6622 | 907148 | Carina  | (910) 555-3251 | 712712 | Jacqueline | 38       |
| 279 | (826) 555-1652 | 395717 | Kenny   | (066) 555-9701 | 953679 | Doris      | 55       |
| 281 | (338) 555-6650 | 438727 | Benista | (704) 555-2131 | 484375 | Anna       | 54       |
+-----+----------------+--------+---------+----------------+--------+------------+----------+
*/

--Query 6: Determine the earliest flight the next day from the 'Fiftyville' airport (which the culprit will be on), as well as the airport and city the thief escaped to
SELECT f.id, f.origin_airport_id, ap1.city, ap1.full_name, destination_airport_id, ap2.city, ap2.full_name, f.year, f.month, f.day, f.hour, f.minute
FROM flights as f
JOIN airports as ap1 on f.origin_airport_id = ap1.id
JOIN airports as ap2 on f.destination_airport_id = ap2.id
WHERE ap1.city = "Fiftyville" AND f.year = 2021 AND f.month = 7 AND f.day = 29
ORDER BY f.hour ASC;
/*
+----+-------------------+------------+-----------------------------+------------------------+---------------+-------------------------------------+------+-------+-----+------+--------+
| id | origin_airport_id |    city    |          full_name          | destination_airport_id |     city      |              full_name              | year | month | day | hour | minute |
+----+-------------------+------------+-----------------------------+------------------------+---------------+-------------------------------------+------+-------+-----+------+--------+
| 36 | 8                 | Fiftyville | Fiftyville Regional Airport | 4                      | New York City | LaGuardia Airport                   | 2021 | 7     | 29  | 8    | 20     |
| 43 | 8                 | Fiftyville | Fiftyville Regional Airport | 1                      | Chicago       | O'Hare International Airport        | 2021 | 7     | 29  | 9    | 30     |
| 23 | 8                 | Fiftyville | Fiftyville Regional Airport | 11                     | San Francisco | San Francisco International Airport | 2021 | 7     | 29  | 12   | 15     |
| 53 | 8                 | Fiftyville | Fiftyville Regional Airport | 9                      | Tokyo         | Tokyo International Airport         | 2021 | 7     | 29  | 15   | 20     |
| 18 | 8                 | Fiftyville | Fiftyville Regional Airport | 6                      | Boston        | Logan International Airport         | 2021 | 7     | 29  | 16   | 0      |
+----+-------------------+------------+-----------------------------+------------------------+---------------+-------------------------------------+------+-------+-----+------+--------+
*/

--Query 7: Check passenger list for flight 18 to determine whether Bruce or Diana is the culprit
SELECT pass.flight_id, pass.passport_number, p.name
FROM passengers AS pass
JOIN people AS p ON pass.passport_number = p.passport_number
JOIN flights AS f ON pass.flight_id = f.id
WHERE pass.flight_id = 36;
/*
+-----------+-----------------+--------+
| flight_id | passport_number |  name  |
+-----------+-----------------+--------+
| 36        | 7214083635      | Doris  |
| 36        | 1695452385      | Sofia  |
| 36        | 5773159633      | Bruce  | <-
| 36        | 1540955065      | Edward |
| 36        | 8294398571      | Kelsey |
| 36        | 1988161715      | Taylor |
| 36        | 9878712108      | Kenny  |
| 36        | 8496433585      | Luca   |
+-----------+-----------------+--------+
*/