SELECT name
FROM
    (SELECT name, movie_id
    FROM people
    JOIN stars ON people.id = person_id
    WHERE movie_id IN
        (SELECT movies.id
        FROM movies
        JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id
        WHERE name IN
            (SELECT name
            FROM people
            WHERE name = 'Kevin Bacon'AND birth = 1958)))
WHERE name != 'Kevin Bacon';