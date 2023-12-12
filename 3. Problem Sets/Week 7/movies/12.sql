--write a SQL query to list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred. 
SELECT title FROM movies AS m
JOIN stars AS s ON m.id = s.movie_id
WHERE s.person_id = (
    SELECT id FROM people
    WHERE name = "Bradley Cooper"
)
AND m.id IN (
    SELECT movie_id FROM stars
    WHERE person_id = (
        SELECT id FROM people
        WHERE name = "Jennifer Lawrence"
    )
);