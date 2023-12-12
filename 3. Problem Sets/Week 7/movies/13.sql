--write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred. 
SELECT p.name FROM people AS p
JOIN stars AS s ON p.id = s.person_id
WHERE s.movie_id IN (
    SELECT movie_id FROM stars
    WHERE person_id = (
        SELECT id FROM people
        WHERE name = "Kevin Bacon" AND birth = 1958
    )
) 
AND p.name != "Kevin Bacon";