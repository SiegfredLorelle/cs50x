# Simulate a sports tournament

import csv
import sys
import random

# Number of simluations to run
N = 1000


def main():

    # Ensure correct usage
    if len(sys.argv) != 2:
        sys.exit("Usage: python tournament.py FILENAME")

    teams = []
    # TODO: Read teams into memory from file
    # Open the file and seperate per line
    with open(sys.argv[1], "r") as file:
        reader = csv.DictReader(file)

        # Loop through each line the file
        for row in reader:
            # Create a dictionary for each team and put their team name and rating
            team_dict = {}
            team_dict["team"] = row["team"]
            team_dict["rating"] = int(row["rating"])
            # Append each teams' dictionary to teams list
            teams.append(team_dict)

    counts = {}
    # TODO: Simulate N tournaments and keep track of win counts
    # Repeat 1000x
    for i in range(N):
        # Simulate the tournament and return the overall winner of the tournament
        winner = simulate_tournament(teams)
        # Count how many times a team have won a tournament
        if winner in counts:
            counts[winner] += 1
        else:
            counts[winner] = 1

    # Print each team's chances of winning, according to simulation
    for team in sorted(counts, key=lambda team: counts[team], reverse=True):
        print(f"{team}: {counts[team] * 100 / N:.1f}% chance of winning")


def simulate_game(team1, team2):
    """Simulate a game. Return True if team1 wins, False otherwise."""
    rating1 = team1["rating"]
    rating2 = team2["rating"]
    probability = 1 / (1 + 10 ** ((rating2 - rating1) / 600))
    return random.random() < probability


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


def simulate_tournament(teams):
    """Simulate a tournament. Return name of winning team."""
    # TODO
    # Battle the teams in the tournament until 1 team remains
    while len(teams) != 1:
        teams = simulate_round(teams)

    # Return the remaining team(winner)
    if len(teams) == 1:
        return teams[0]["team"]


if __name__ == "__main__":
    main()
