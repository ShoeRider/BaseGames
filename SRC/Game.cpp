/*
Anthony M Schroeder


Purpose:
Create a Game interface in order to implement and integrate different games
easily.

Using a method called: Pure Virtual Functions.

*/
#ifndef GAME_CPP
#define GAME_CPP

#include <string>
#include <iostream>
#include <memory>

#include <list>
#include "Game.h"
#include <jsoncpp/json/json.h>

//Game Move interface. Used as a place holder for child classes holding
//   game move data.
struct GameMove
{
  public:
      GameMove(){}
      virtual ~GameMove()= default;
};


GameMove* get(std::list<GameMove*> _list, int _i){
    std::list<GameMove*>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}


//Player interface. Used as a place holder for child classes holding
//   game Player data.
struct Player
{
  public:
    int PlayerNumber;
    void (*ExternalSimulation)();
    bool HumanPlayer;
    Player(){}
  //  GameMove* MakeMove(Game* GivenGame)=0;
    ~Player(){};

};

/*
Game Interface:
Interface to standardize Games inorder to regularize Games for Tree searches.

Intended to be used for MCTS variants.

@Methods:
Search()
Algorithm():: A recursive implementation of the MCTS algorithm. Recursively creates a serach tree based on the MCTS, searching for the most optimal move.

 * @param
    Game*_Game,
    std::list<Player*> _GivenPlayers)

 *
 * @see MCTS_Node::Find_MAX_UCB1_Child()
 * @see Game interface(Found within Game.cu)

*/
class Game
{

  public:
      std::list<Player*> _Players;
      Player*  WinningPlayer = NULL;
      Player*  DrawPlayer    = NULL;
      Game(){}
      virtual ~Game(){}

        //The following Methods use the 'Pure Virtual Function' method,
        //  where "= 0" part makes this method pure virtual,
        //  and also makes this class abstract.

      //virtual bool ValidMove(int Row,int Col) = 0;


      //std::list<GameMove*> PossibleMoves() = 0;
      //std::list<Game*> PossibleGames()     = 0;
      virtual bool ValidMove(GameMove* Move)       = 0;
      //virtual bool Move(GameMove* Move)            = 0;
      virtual Game* CopyGame()                     = 0;
      //virtual void AvaliableMoves(int Depth) = 0;

      virtual std::string Generate_StringRepresentation() = 0;

      //virtual void DisplayInTerminal(int Depth) = 0;
      //virtual void ConfigurePlayers()   = 0;
      //virtual std::string GameHash()        = 0;
      virtual void PlayGame()           = 0;
      //virtual Player* TestForWinner()   = 0;



      //virtual std::string DeclareWinner(int Player) = 0;
      //virtual void StepSimulation() = 0;
      //virtual void CopySimulation() = 0;
      //virtual void SaveSimulation() = 0;
      //virtual void ReadSimulation() = 0;
      virtual Game* RollOut()            = 0;
      virtual void DisplayWinner()            = 0;

      //virtual Json::Value* add(Json::Value* JSONValue);
/*
virtual void DeclarePlayers(std::list<Player*> GivenPlayers) {
  for (Player* i : GivenPlayers) { // c++11 range-based for loop
      //printf("%p\n",i);
      _Players.push_back(i);
    }
};*/

};


//TODO: REMOVE, Create local library...
Game* get(std::list<Game*> _list, int _i){
    std::list<Game*>::iterator it = _list.begin();
    for(int i=0; i<_i; i++){
        ++it;
    }
    return *it;
}




template <typename Game_Tp,typename Player_Tp>
Player_Tp* PlayGameOut(Game_Tp Game){
  return 0;
}

/*
#include "../../ExternalLibraries/json-develop/single_include/nlohmann/json.hpp"

template <typename Game_Tp,typename Player_Tp>
void Add(nlohmann::json &j,std::list<Player_Tp*> &Players) {


  j = nlohmann::json::array();
  //nlohmann::json Player;

  //////////////////////////////////////////////////////////////////////////////
  // For Each Player within JSON file, place Players into list.
  // NOTE: When Saving TTT Players to JSON file, the order is swapped(The First
  //   player is at the bottom of the list). Reading the JSON file for loop
  //   automatically adds the players back into the correct order(The first
  //   player within the JSON file becomes the last player within the Player order).
  for (Player_Tp* i : Players) { // c++11 range-based for loop
    //Player = (const TTT &)* i;
    j.push_back(Json(*i));
  }
}
*/


#endif //GAME_CPP
