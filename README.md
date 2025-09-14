# Squakus

## Origins of Squakus
Squakus began in a class structured around mathematical games. During our time in that course, we became interested in the possibilities
surrounding polyominoes. We first played a game called Blokus that leverages polyominoes up to n = 5. 
Here are the Blokus pieces for one player:
<p align="center">
  <img src="images/polyominoes.png" alt="Blokus Pieces" title="Blokus Pieces" />
</p>

Playing this game, we found immense curiosity in excentricies of each polyomino piece. We felt like we can leverage the unique shape of each piece to create a fast paced pattern matching game.

## Creating the gameplay loop
We initially raced each other to see who can make rectangles of varying dimensions first. This was fun, however the shapes where not particularly interesting and thye did not leverage each polyomino to its fullest. The next issue we found was that someone who is more inclined to pattern matching spatial reasoning would win almost every round. We needed to find a way for the game to scale as it progresses, giving other players a competitive advantage over the leader.