/*
====================================================================================================
Description TTT(Tic Tac Toe):
- Purpose:
Implement Tic Tac Toe through Game interface. Using standard rules.

====================================================================================================
Date:           NA
Script Version: 1.0
Name:           Anthony M Schroeder
Email:          as3379@nau.edu
==========================================================
Date:           15 September 2021
Script Version: 1.1
Description: Remove Game* from TTT to create usable version for MCTS Templates.
==========================================================
*/

#ifndef TTT_H
#define TTT_H


//////////////////////////////////////////////////////////////////////////////
// Standard Libraries.
//////////////////////////////////////////////////////////////////////////////
#include <iostream>
#include <string>
#include <list>
#include <cstdlib>

//////////////////////////////////////////////////////////////////////////////
// Game Library for inheritance structure.
//////////////////////////////////////////////////////////////////////////////
#include "../Game.h"




//struct GameMove;
//struct Player;



class TTT_Move;
class TTT_Player;
class TTT;

#include "JSON_TTT.cpp"
#include "TTT.cpp"
//#include "../SRC/TTT.cpp"

#define Pause int ASDF; std::cin >> ASDF;

#endif //TTT_H
