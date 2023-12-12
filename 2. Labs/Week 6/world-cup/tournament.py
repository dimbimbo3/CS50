# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000000


def main():
    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = [] # empty list to store team dictionaries
    #tries to open the specified file
    try:
        # with-statement closes the given file automatically after its block of code runs
        with open(sys.argv[1], "r") as file:
            reader = csv.DictReader(file)

            # Read each team and their rating (converted to an integer) into memory from csv file
            for line in reader:
                teams.append({"team" : line["team"], "rating" : int(line["rating"])})
    except IOError:
        print(f"ERROR: {sys.argv[1]} could not be opened. Terminating...")
        sys.exit(1)

    counts = {} # empty dictionary for each team's simulated wins
    # Simulate N tournaments and keep track of win counts
    for i in range(N):
        winner = simulate_tournament(teams) # stores winning team of tournament

        # checks if the team has won in prior simulations
        if winner in counts:
            counts[winner] += 1 # increment this team's win counter by 1
        else:
            counts[winner] = 1 # else this team has not won before, so initialize their win count to 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


# used by simulate_round function to determine the winning team of a game
def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


# used by simulate_tournament to determine all the winning teams of a round in the tournament
def simulate_round(teams):
    """Simulate a round. Return a list of winning teams."""
    winners = []

    # Simulate games for all pairs of teams
    for i in range(0, len(teams), 2):
        if simulate_game(teams[i], teams[i + 1]):
            winners.append(teams[i])
        else:
            winners.append(teams[i + 1])

    return winners


# simulates a tournament to determine which team wins in the end
def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    
    # simulates rounds until there is only one winner
    while len(teams) > 1:
        teams = simulate_round(teams)

    return teams[0]["team"] # returns the winning team name


if __name__ == "__main__":
    main()

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ python tournament.py test
ERROR: test could not be opened. Terminating...
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Belgium: 20.0% chance of winning
Switzerland: 20.0% chance of winning
Portugal: 20.0% chance of winning
Brazil: 10.0% chance of winning
Colombia: 10.0% chance of winning
Spain: 10.0% chance of winning
Croatia: 10.0% chance of winning

real    0m0.032s
user    0m0.008s
sys     0m0.013s
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Brazil: 22.0% chance of winning
Belgium: 20.0% chance of winning
Portugal: 18.0% chance of winning
Switzerland: 13.0% chance of winning
Spain: 6.0% chance of winning
Argentina: 5.0% chance of winning
England: 4.0% chance of winning
Denmark: 4.0% chance of winning
France: 4.0% chance of winning
Colombia: 3.0% chance of winning
Croatia: 1.0% chance of winning

real    0m0.033s
user    0m0.015s
sys     0m0.007s
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Brazil: 20.6% chance of winning
Belgium: 19.9% chance of winning
Portugal: 15.3% chance of winning
Spain: 12.9% chance of winning
Switzerland: 11.4% chance of winning
Argentina: 7.1% chance of winning
France: 4.0% chance of winning
Denmark: 2.2% chance of winning
England: 2.2% chance of winning
Colombia: 1.6% chance of winning
Croatia: 1.2% chance of winning
Sweden: 0.6% chance of winning
Mexico: 0.5% chance of winning
Uruguay: 0.5% chance of winning

real    0m0.038s
user    0m0.026s
sys     0m0.000s
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Belgium: 21.0% chance of winning
Brazil: 21.0% chance of winning
Portugal: 14.5% chance of winning
Spain: 12.1% chance of winning
Switzerland: 10.1% chance of winning
Argentina: 7.8% chance of winning
France: 3.3% chance of winning
Denmark: 3.3% chance of winning
England: 2.5% chance of winning
Colombia: 1.5% chance of winning
Croatia: 1.3% chance of winning
Mexico: 0.6% chance of winning
Uruguay: 0.5% chance of winning
Sweden: 0.5% chance of winning

real    0m0.088s
user    0m0.076s
sys     0m0.000s
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Belgium: 21.2% chance of winning
Brazil: 20.7% chance of winning
Portugal: 14.2% chance of winning
Spain: 11.9% chance of winning
Switzerland: 10.5% chance of winning
Argentina: 7.7% chance of winning
France: 3.3% chance of winning
Denmark: 3.2% chance of winning
England: 2.8% chance of winning
Colombia: 1.7% chance of winning
Croatia: 1.3% chance of winning
Mexico: 0.6% chance of winning
Sweden: 0.5% chance of winning
Uruguay: 0.4% chance of winning
Japan: 0.0% chance of winning
Russia: 0.0% chance of winning

real    0m0.593s
user    0m0.571s
sys     0m0.011s
"""

"""
dan@Dan-PC:/mnt/c/Users/Dan/xCS50/2. Labs/Week 6/world-cup$ time python tournament.py 2018m.csv
Belgium: 21.1% chance of winning
Brazil: 20.7% chance of winning
Portugal: 14.2% chance of winning
Spain: 12.0% chance of winning
Switzerland: 10.4% chance of winning
Argentina: 7.8% chance of winning
France: 3.4% chance of winning
Denmark: 3.2% chance of winning
England: 2.8% chance of winning
Colombia: 1.6% chance of winning
Croatia: 1.3% chance of winning
Mexico: 0.5% chance of winning
Sweden: 0.5% chance of winning
Uruguay: 0.5% chance of winning
Russia: 0.0% chance of winning
Japan: 0.0% chance of winning

real    0m5.408s
user    0m5.386s
sys     0m0.010s
"""