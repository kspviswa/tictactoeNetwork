tictactoeNetwork
================

Classic tic-tac-toe game, implemented in pure c/c++ . Aims to be a multi-player game hosted on a distributed network

How this project is different
=============================

Basically tic-tac-toe is played in a multi-player mode, where same terminal / game environment is shared between
player 1 & player 2. The players take chance / turns to play the game. One of the player takes X and another takes O.

But this game has been played as a stand-alone game.

---

Game Design
==========

As part of mini project in distributed operating systems course, I had planned to take this game to next level.
Following are the enhancements, that are made to make this game more fun.

- [X] N number of players can take the game.

* However if the Nth player is such that N mod 2 == 1, then Nth player has to wait for his / her partner.
* As soon, a pair is formed, the game is started. The waiting partner will be notified upon arival of their pairs.

- [X] When a player gets into the game, he / she registers their Name, IP with the game co-oridinator.
- [X] Game co-ordinator sits on a centralised place / server who monitors & facilites all the matches.

- [X] Game co-ordinator can be started in either *Knock-out* or *Championship mode*.

Knock-out mode
-------------

In this mode, every match will be treated as a seperate *Knock-out challenge*. Hit or Miss that's it.
Every player can play _N_ number of matches to acquire his / her score.
There will be a global *Hall of Fame* page, which will dynamically update the top scorers for today.
* This can be made to on a web-page if possible.

Championship mode
-----------------

In this mode, there will be league matches, quarter finals, semi-finals & finals. Just like T20 world-cup / FIFA.
The number of matches to be played will be decided upon the arrival of participants before the stipulated time.
Players logging onto the server, after the stipulated time will be sent back.

If there are N players and N mod 2 == 1, then Nth player will get a chance upon the first league match.
Loser of First league match should compete with Nth player and the winner of first league match, should again compete with the
N player league match.
* If the winner again wins, he / she gets a bonus point.
** If the winner loses, nothing is affected.

* If the Nth player loses in first league match, he / she is out of game.
* If the Nth player wins both the matches, he / she is into the game.

---

Game environment
================

The game environment is very intutive. Each player has their own terminals.
Their terminal will be used by them only. i.e Parter use their terminals, but each player get to see what their *opponent* is doing.

Along with the game board, every user can get to know the following in their terminal.

* Match title [ Knock out / Championship match type - league, quarter / semi / finals etc]
* Time [ Duration of game ].
* Player details [ Name, IP, Total matches, Number of matches won].
* Seperate chat windows to communicate. [only ascii supported].

At the end of every match, match summary , scores, Link to next match if applicable will be visible.

Connectivity design
-------------------

Every client contacts the game server and game server can split load balance / split this job to another servers if needed.
The statistics & reports / results page will be made available on web / html page.

Planning to use Apache Thrift RPC service / write own stubs.

---

Areas of DOS application
========================

While this project is aimed to be submitted as mini-project for the Distributed operating systems courese,
it is very imperative to understand, what does this project demands, w.r.t DOS skilss.

- [X] Server handling parallelism.
- [X] Shared resource handling across multiple players.
- [X] Load balancing.
- [X] Transaction handling w.r.t Game co-ordinator.
- [X] Applications of good performance algorithms in allocating resources / players during heavy load.


---

Fun side of project : Playing with game polices
===============================================

You can play with game policies for fun. These policies can be made as dynamic and should be pushed on demand.

Policy examples
---------------

. If any player doesn't make a move within stipulated X time, then the chance will be given to next player
.. As a power / bonus point, this move can be auto suggested by the system.
... Here you can play with algorithms to suggest the best possible move, analysing the present game.

kspviswa.
---------
www.kspviswa.wordpress.com [Blog]
