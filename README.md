# Wumpus_World
Solution code for a Machine Learning Game teaching concepts such as expanding a knowledge base and performing inferences/actions based on information stored in the knowledge base.

Game Description:
An Actor moves around a grid and encounters two percepts, Breeze and Stench, which indicate that a hazard, either a Pit or Wumpus object, are in an adjacent tile. The goal of the game is to gain the most points, which entails finding the random tile with gold, picking up the gold, and returning to the fixed starting position. In each move of the game, Actor can take one of the following actions: Move into an adjacent (non-diagonal) tile, Turn Left/Right, Shoot an arrow, Pick up the gold, or Climb the ladder placed at the starting position. The game will end early with deducted points if the Actor steps into a tile with a Pit or Wumpus. Only one Wumpus is present in each board and can be killed by shooting an arrow at it, effectively removing that hazard from the board.

My final solution for this project encorporated Depth-First Search with a stack that pushed adjacent tiles verified to not have a hazard in them based on knowledge-base inferences, along with a Uniform-Cost Search algorithm to find the quickest path to a given tile from the current position, and had an average of 50 points higher than the final goal of 200 points.
