/*
====================================================================================================
Description UTTT(Ultimate Tic Tac Toe):
- Purpose:

====================================================================================================
Date:           NA
Script Version: 1.0
Name:           Anthony M Schroeder
Email:          as3379@nau.edu
==========================================================

*/

#ifndef UTTT_H
#define UTTT_H


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
#include "../Game.cpp"
#include "../TTT/TTT.cpp"
#include "UTTT.cpp"

struct UTTT_Player;
struct UTTT_Move;


class UTTT_Player;
class UTTT_Move;
class UTTT;
std::size_t Hash(UTTT* k);

#define Pause int ASDF; std::cin >> ASDF;

#endif //UTTT_H
