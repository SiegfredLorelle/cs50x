-- Keep a log of any SQL queries you execute as you solve the mystery.

-- initial info: July 28 at Humphrey street a duck was stolen

-- LOG1: I wanted to see how the database looks like, I copy pasted it in a notepad to view it easier as I go on the mystery
.schema

-- LOG2: Checking if the crime scene reports has valuable info based on the given date and street
SELECT day, description
FROM crime_scene_reports
WHERE year = 2021 AND day = 28 AND street LIKE '%Humphrey%';
-- new info: crime occured at 10:15am, 3 witnesses mention the bakery,
    -- another report at the same day 16:36 about littering (might be connected on the case)

-- LOG3: Check the interview transcript of the 3 witnesses based on the date and the mention of bakery
SELECT day, transcript
FROM interviews
WHERE year = 2021 AND month = 7 AND day = 28 AND transcript LIKE '%bakery%';
-- new info(w doubt, witnesses might lie): 10 mins after theft, thief went to the bakery parking and left in a car
    -- recognized by the witness, saw thief at atm on Leggett St, thief withdrawing money
    --  After the thief left bakery, called someone for less than 1min. asked the person to buy ticket for earliest flight tom

-- LOG4: Check the bakery logs for the time mentioned by the 1st witness
SELECT license_plate
FROM bakery_security_logs
WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 26;
-- new info: got 8 license plates that matches the descriptiosn of the 1st witness

-- LOG5: Check the atm_transcactions as told by 2nd witness
SELECT account_number, amount
FROM atm_transactions
WHERE year = 2021 AND month = 7 AND day = 28 AND atm_location LIKE '%Leggett%' AND transaction_type = 'withdraw';
-- new info: got 8 account numbers with matching amount of withdrawal

-- LOG6: Check phones calls based on what 3rd witness said
SELECT caller, receiver
FROM phone_calls
WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60;
-- new info: got 10 calls with caller and receiver that lasted 60 seconds or less,
    -- might get the person on people table using the plate, and phone number

-- LOG7: Check the people with matching plate, and phone number
SELECT name, phone_number, passport_number, license_plate
FROM people
WHERE phone_number IN
    (SELECT caller
    FROM phone_calls
    WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60)
AND license_plate IN
    (SELECT license_plate
    FROM bakery_security_logs
    WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 26);
-- new info: got 4 persons' name and passport that matches the details told by witness 2 and 3 (Sofia, Diana, Kelsey, Bruce)

-- LOG:8 Check the 4 suspects' details on passenger table based on their passport
SELECT name, flight_id, people.passport_number, seat
FROM passengers
JOIN people ON passengers.passport_number = people.passport_number
WHERE passengers.passport_number IN
    (SELECT passport_number
    FROM people
    WHERE phone_number IN
        (SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60)
    AND license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 26));
    -- new info: Diana had 3 flights while the other suspects had 1. Got all flight ids that migh lead to their flight details

-- LOG9: Check the suspects' flight details on the June 29 earliest flight
SELECT name, flights.id, hour, minute, origin_airport_id,  destination_airport_id
FROM flights
JOIN passengers ON flights.id = passengers.flight_id
JOIN  people ON passengers.passport_number = people.passport_number
WHERE passengers.passport_number IN
    (SELECT passport_number
    FROM people
    WHERE phone_number IN
        (SELECT caller
        FROM phone_calls
        WHERE year = 2021 AND month = 7 AND day = 28 AND duration <= 60)
    AND license_plate IN
        (SELECT license_plate
        FROM bakery_security_logs
        WHERE year = 2021 AND month = 7 AND day = 28 AND hour = 10 AND activity = 'exit' AND minute >= 15 AND minute <= 26))
AND month = 7 AND day = 29
ORDER BY hour ASC;
-- new info: Sofia, Bruce, and Kelsey are the only suspects. All of them are in the same earliest flight (flight_id 36)
            --  They left from origin_airport_id 8 (probaby fiftyville) and went to destination_airport_id 4;

-- LOG10: check the name of the origin and destination of their flight
SELECT *
FROM airports
WHERE id = 4;
-- new info: The theif indeed came from fiftyville and escaped to New York City

-- LOG11: check the bank account of the suspects
SELECT *
FROM bank_accounts
JOIN people ON person_id = people.id
WHERE name IN ('Sofia', 'Bruce', 'Kelsey');
-- new info: Bruce is probably the thief, he is the only suspect with a bank account recorded on the table

-- LOG12: find the accomplice by checking to the receiver of the call
SELECT receiver
FROM phone_calls
JOIN people ON phone_calls.caller = people.phone_number
WHERE name = 'Bruce'AND year = 2021 AND month = 7 AND day = 28 AND duration <= 60;
-- new info: the number that Bruce was talking to is (375) 555-8161

-- LOG13: identify the owner of the number that Bruce called
SELECT name, phone_number
FROM people
WHERE phone_number = '(375) 555-8161';
-- new info: the accomplice's name is probably Robin