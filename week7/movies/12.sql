SELECT title
FROM movies
JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id
WHERE movies.id IN  -- All of Depps' movie
    (SELECT movies.id
    FROM movies
    JOIN stars ON movies.id = stars.movie_id JOIN people ON stars.person_id = people.id
    WHERE name = 'Johnny Depp')
AND name = "Helena Bonham Carter";




