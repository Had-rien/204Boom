# 204Boom
2048 like game with bombs, written in C

The game is to be compiled using any C compiler, then played inside your favorite terminal!

This is a simple game were the goal is to make the biggest number you can, in order to reach the goal you have set yourself. Here is an exemple :
    If you press Q, to go left, the two one's will be attracted to the left, and fuse to make a 2!
    Before :                 After :
           <--						
    |---|---|---|---|       |---|---|---|---|
  1 | 1 |   | 1 |   |       | 2 |   |   |   |
    |---|---|---|---|       |---|---|---|---|
      1   2   3   4           1   2   3   4  
     ---------------         ---------------

You control the direction of attraction with Z,Q,S,D and have to confirm evry choice before they take effect.

If the grid is full and the move you choose is not possible, you will lose the game !

After you have reached 64, a new objet will arrive, BOMBS!
You can choose to make  a bomb explode insted of choosing a direction each turn, if a bomb is on the grid for more than 7 turn, it will also explode.
Three kinds of bombs exist :
	- + : It will detonate on top, bottom and the sides.
	- x : It will explode diagonally.
	- * : Is a combinationn of both.
To detonate a bomb press B then enter the coordinate oof the bomb.

When starting the game you will have to choose how you want to play.
I suggest goal over 1000 for a default a grid and a large grid for best experience.

ENJOY !
