Labyrinthian
============

-This game was the final project for a game development class at Carleton University.

-A player is placed in a multi level world where they must navigate their way to the bottom
level of a procedurally generated maze without getting hit by obstacles.

-The maze is created with a modified prims algorithm, so that it is complete. The obstacle in the maze
is a rolling ball, which follows the largest path in the maze, which is calculated through dijkstras algorithm 
when the maze is created.

-The graphics, physics engine, world generation were done by Nick Tierney
Camera
FloorPanel
Level
Maze
MeshBox
MeshModel
MeshObject
MeshSphere
PhysicsEngine
Player
RollingBall
Space
SpaceNode
Wall

-The audio engine was done by Ziyao Zhou:
AudioPlayer
Selector

-The menu and gui was done by Tyler Haskell:
LevelCompleteScreen
Hud
BestTime
GameMenu

-Part of the game loop was based on code given by Dr. Doron Nussbaum:
mgGame
io_demo
gameIO
