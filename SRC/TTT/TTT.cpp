/*
====================================================================================================
Description TTT(Tic Tac Toe):
Purpose:
  Implement Tic Tac Toe through Game interface. Using standard rules.
Contains(Classes):
  TTT_Player
  TTT_Move
  TTT
TODO:
  Add *Radio?* Player functionality: Takes Pointer to code -> Runs -> returns move
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
Date:           21 September 2021
Script Version: 1.2
Description:
  Changed:
    - Player Pointers to TTT_Player.
    - GameMove Pointers to TTT_Move.
    TODO: Move:
      T* get(std::list<T*> _list, int _i)
      to new base library folder.
==========================================================
Date:           30 September 2021
Script Version: 1.3
Description:
  Added hash function to TTT class.
==========================================================
Date:           4 October 2021
Script Version: 1.4
Description:
  - Added JSON Read/Write Functionality using: nlohman JSON Library.
  - Changed File extension to CPP.
TODO: Change JSON Libary to compatable CUDA.
Possible interests:
- SIMDJSON (GitHub: https://github.com/simdjson/simdjson)
-
==========================================================
Date:           11 October 2021
Script Version: 1.4.01
Description:
  Changed File extension back to CU. attempting to compile with:
- SIMDJSON (GitHub: https://github.com/simdjson/simdjson)
TODO: Generalize JSON Saves to account for different Player Oders,
  IE: Player0 has different game representation than X.
==========================================================
*/
#ifndef TTT_CPP
#define TTT_CPP


//////////////////////////////////////////////////////////////////////////////
// Game Library for inheritance structure.
//////////////////////////////////////////////////////////////////////////////

#include "TTT.h"

#include <iostream>
#include <fstream>

#include <string.h>
//#include "../../ExternalLibraries/json-develop/single_include/nlohmann/json.hpp"

//https://stackoverflow.com/questions/63503620/cant-use-m128i-in-cuda-kernel
//#include "../../ExternalLibraries/simdjson/singleheader/simdjson.h"

//////////////////////////////////////////////////////////////////////////////
//https://github.com/kazuho/picojson/
//#include "../../ExternalLibraries/picojson/picojson.h"

//picojson
#include <iomanip>
#include <unordered_map>


//Read and save Game states.
#include <iostream>
#include <jsoncpp/json/json.h>
//#include "TTT.cu"


//TODO Move to Basic Libaries
//Example:
/*
int a_size = sizeof(p->Board) / sizeof(char);
std::string str = convertToString(p->Board, a_size);
*/
std::string convertToString(char* a, int size)
{
    int i;
    std::string s = "";
    for (i = 0; i < size; i++) {
        s = s + a[i];
        //std::cout << a[i] << "\n";
    }
    //std::cout << s << "\n";
    return s;
}

template <typename T>
T* get(std::list<T*> _list, int _i){
    typename std::list<T*>::iterator it = _list.begin();
    for(int i = 0; i<_i; i++){
        ++it;
    }
    return *it;
}





/*
TTT_Move
Purpose: A Helper class to hold the potential move data for a TTT Game.
  IE: X,Y coordinates. And possibly a Player Pointer.


@Methods:
  No Methods.  Intended to act as a Command pattern.
  TTT_Move to be integrated with ML Model to select move.
 */
struct TTT_Move : public GameMove
{

  public:
    int Row;
    int Col;
    //////////////////////////////////////////////////////////////////////////////
    // Initialization method.
    TTT_Move(int GivenRow,int GivenCol){
      Row = GivenRow;
      Col = GivenCol;
    }
    virtual ~TTT_Move(){}
};


/*
TTT_Player
@Purpose: Class to track TTT Players.
@Methods:
  MakeMove() function pointer to allow for Humans to play.
*/
struct TTT_Player : public Player
{
  public:
    //////////////////////////////////////////////////////////////////////////////
    // Game Data
    //////////////////////////////////////////////////////////////////////////////
    int PlayerNumber;
    char GameRepresentation;
    bool HumanPlayer;

    //////////////////////////////////////////////////////////////////////////////
    // Initialization method.
    TTT_Player(Json::Value root){
      std::cout <<"creating TTT_Player:\n";
          Json::FastWriter fastWriter;
          std::string Temp;
      //////////////////////////////////////////////////////////////////////////////
      //Gather Player Number
        std::cout <<"gathering PlayerNumber:'"<<root["PlayerNumber"]<<"'\n";
      Temp = fastWriter.write(root["PlayerNumber"]);
      //Temp.erase(0, 1);                          //Remove leading  '"'
      //Temp.erase(Temp.size() - 2);      //Remove trailing '"\n'
      PlayerNumber = atoi(Temp.c_str());

      //////////////////////////////////////////////////////////////////////////////
      //Gather Player's GameRepresentation.
        std::cout <<"gathering GameRepresentation:\n";
      Temp = fastWriter.write(root["GameRepresentation"]);
      Temp.erase(0, 1);                          //Remove leading  '"'
      Temp.erase(Temp.size() - 2);      //Remove trailing '"\n'
      const char* MOD_JSON_BoardRep =Temp.c_str();   //Cast as Char* for copy

      //char* MOD_JSON_BoardRep  = JSON_BoardRep;#.c_str()
      //strcat produces Valgrind error.
      //https://codereview.stackexchange.com/questions/46619/conditional-jump-or-move-depends-on-uninitialised-value
      strncpy(&GameRepresentation, MOD_JSON_BoardRep, strlen(MOD_JSON_BoardRep) + 1);


      //////////////////////////////////////////////////////////////////////////////
      //Gather (is)HumanPlayer value.
/*
Temp = fastWriter.write(root["HumanPlayer"]);
Temp.erase(0, 1);                          //Remove leading  '"'
Temp.erase(Temp.size() - 2);      //Remove trailing '"\n'
PlayerNumber = atoi(Temp.c_str());*/
    }

    //////////////////////////////////////////////////////////////////////////////
    // Initialization method from JSON Files

   TTT_Player(int GivenPlayer,char GivenGameRepresentation){
    PlayerNumber = GivenPlayer;
    GameRepresentation = GivenGameRepresentation;
    HumanPlayer = false;
  }

  ~TTT_Player(){}
   TTT_Move* MakeMove(TTT* GivenGame);
   void Display();
   std::size_t Hash();
   Json::Value* JSON();
   Json::Value* Add(Json::Value*);

   void Save(std::string FilePath);
};

std::size_t TTT_Player::Hash(){
  //std::size_t HashSum;
  //std::string GameState = convertToString(k->Board,9);
  //HashSum = (std::hash<std::char>());

  return (int)(GameRepresentation);
}

//////////////////////////////////////////////////////////////////////////////
// Move Method for human players.
// TODO: integrate with command pattern, allowing for ML Models to select moves.
//TTT_Move doesnt need to be returned, TTTMove should be applied to Game here.
TTT_Move* TTT_Player::MakeMove(TTT* GivenGame)
{
   int X,Y;
   std::cout << "Please Enter X Axis: ";
   std::cin >> X;
   std::cout << "Please Enter Y Axis: ";
   std::cin >> Y;
   TTT_Move* TTTMove = new TTT_Move(X,Y);
   //GameMove* Move = static_cast<GameMove*>(TTTMove);

   //TODO: Include Move call here
   //GivenGame->Move(TTTMove);
   return TTTMove;
}

void TTT_Player::Display()
{
   //GameMove TTTPlayer = static_cast<GameMove>(TTT_Move(0,0));
   printf("PlayerNumber:%d\n",PlayerNumber);
   printf("GameRepresentation:%c\n",GameRepresentation);
   printf("HumanPlayer:%d\n",HumanPlayer);
}



void Free_TTTMoveList(std::list<TTT_Move*> GameMoves)
{
  //std::list<GameMove*> Moves = PossibleMoves();
  for (TTT_Move* GMove : GameMoves) { // c++11 range-based for loop
      //TTT_Move* Move = static_cast<TTT_Move*>(GMove);
      delete GMove;
    }
}



TTT_Player* CreateHuman_TTT_Player(int PlayerID, char PlayerCharacter){
  TTT_Player* Player = new TTT_Player(PlayerID,PlayerCharacter);
  // Change Player->Move pointer to request input.
  return Player;
}



Json::Value* TTT_Player::Add(Json::Value* JSONValue){
  (*JSONValue)["PlayerNumber"]       = PlayerNumber;
  (*JSONValue)["GameRepresentation"] = std::string(1,GameRepresentation);
  (*JSONValue)["HumanPlayer"]        = HumanPlayer;
  return JSONValue;
}

Json::Value* TTT_Player::JSON(){
  Json::Value* Player = new Json::Value();
  (*Player)["PlayerNumber"]       = PlayerNumber;
  (*Player)["GameRepresentation"] = std::string(1,GameRepresentation);
  (*Player)["HumanPlayer"]        = HumanPlayer;
  //std::cout << (*Player) << std::endl;
  return Player;
}



void TTT_Player::Save(std::string FilePath){
  Json::Value* value_obj = JSON();

  std::ofstream file_id;
  file_id.open(FilePath);

  //populate 'value_obj' with the objects, arrays etc.

  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(*value_obj);

  file_id.close();
  delete value_obj;
}

TTT_Player* Read_TTT_Player_JSON(std::string FilePath){
    //std::cout << "Reading File" << std::endl;
    std::ifstream file(FilePath);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        std::cout << "Error parsing the string" << std::endl;
    }

    std::cout << root << std::endl;

    return new TTT_Player(root);
}

/*
void Add(nlohmann::json &j,std::list<TTT_Player*> &Players) {


  j = nlohmann::json::array();
  //nlohmann::json Player;

  //////////////////////////////////////////////////////////////////////////////
  // For Each Player within JSON file, place Players into list.
  // NOTE: When Saving TTT Players to JSON file, the order is swapped(The First
  //   player is at the bottom of the list). Reading the JSON file for loop
  //   automatically adds the players back into the correct order(The first
  //   player within the JSON file becomes the last player within the Player order).
  for (TTT_Player* i : Players) { // c++11 range-based for loop
    //Player = (const TTT &)* i;
    j.push_back(Json(*i));
  }
}
*/








/*
TTT(Tic Tac Toe):
Implement Tic Tac Toe through Game interface. Using standard rules.

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
class TTT : public Game
{
protected:


public:
  //////////////////////////////////////////////////////////////////////////////
  // Player(s) DATA
  //TODO: Take Draw player during Initialization.
  //////////////////////////////////////////////////////////////////////////////
  TTT_Player Draw    = TTT_Player(-1,'C');

  std::list<TTT_Player*> Players;
  TTT_Player*  WinningPlayer = NULL;

  //////////////////////////////////////////////////////////////////////////////
  // Game Data
  //////////////////////////////////////////////////////////////////////////////
  //MovesRemaining is a decrementing counter to determine if there are any remaining moves.
  int MovesRemaining;
  int MovesMade;  //TODO, Implement MovesMade
  bool SimulationFinished;
  //Represenation of the game.
  //char Board[3][3];
  //TTTMove->Row*3+TTTMove->Col
  char Board[9];
  std::size_t GameHash;

  //////////////////////////////////////////////////////////////////////////////
  // JSON File Data
  //////////////////////////////////////////////////////////////////////////////
  bool JsonRead;

  //////////////////////////////////////////////////////////////////////////////
  // Initialization method.
  TTT(){
    //printf("Calling Default Constructor... \n");
    //throw "Calling Default Constructor... \n";
    JsonRead = false;

  }

  //////////////////////////////////////////////////////////////////////////////
  // JSON Initialization method(Reading from file).
  TTT(Json::Value ReadJSValue){
  printf("Redeclare TTT:\n");
    Json::FastWriter fastWriter;
    std::cout << ReadJSValue << std::endl;


    MovesRemaining     = atoi(fastWriter.write(ReadJSValue["MovesRemaining"]).c_str());
    SimulationFinished = atoi(fastWriter.write(ReadJSValue["SimulationFinished"]).c_str());
    //Preform string manipulation to recreate the TTT Board.

    std::string JSON_BoardRep = fastWriter.write(ReadJSValue["Board"]);
    //ReadJSValue["Board"] has the format: "123456789"\n
    JSON_BoardRep.erase(0, 1);                          //Remove leading  '"'
    JSON_BoardRep.erase(JSON_BoardRep.size() - 2);      //Remove trailing '"\n'
    const char* MOD_JSON_BoardRep =JSON_BoardRep.c_str();   //Cast as Char* for copy

    //char* MOD_JSON_BoardRep  = JSON_BoardRep;#.c_str()
    //strcat produces Valgrind error.
    //https://codereview.stackexchange.com/questions/46619/conditional-jump-or-move-depends-on-uninitialised-value
    strncpy(Board, MOD_JSON_BoardRep, strlen(MOD_JSON_BoardRep) + 1);
    //std::cout << Board << "-asdf\n";
    std::cout << strlen(MOD_JSON_BoardRep) << "\n";

    //std::cout <<"Size:"<<ReadJSValue["Players"].size()<<"\n";

    //////////////////////////////////////////////////////////////////////////////
    //Redeclare Players
    //printf("Redeclare TTT_Players :\n");
    for (auto const& id : ReadJSValue["Players"].getMemberNames()) {
      std::cout << id << std::endl;
        Players.push_back(new TTT_Player(ReadJSValue["Players"][id]));
    }

    JsonRead = true;

  }

  TTT(std::list<TTT_Player*> GivenPlayers){
      //this->DeclarePlayers(GivenPlayers);
      Players = GivenPlayers;
      this->WinningPlayer  = NULL;
      MovesRemaining       = 9;
      JsonRead = false;
      SimulationFinished = false;
      this->SetUpBoard();
      GameHash = this->Hash();
      //std::cout<< "GameHash:" << GameHash <<"\n";
    }
    virtual ~TTT(){


      if(JsonRead){
        for (TTT_Player* Player: Players) { // c++11 range-based for loop
          //free(Player);
          delete Player;
        }
      }

    }

    //////////////////////////////////////////////////////////////////////////////
    // Method Declarations.
    //////////////////////////////////////////////////////////////////////////////
    TTT_Player* GetWinner();
    void DisplayWinner();
    void DeclarePlayers(std::list<TTT_Player*> GivenPlayers);
    void SetUpBoard();
    TTT* CopyGame();
    void RotatePlayers();
    bool Move(GameMove* Move);

    bool ValidMove(GameMove* Move);
    TTT_Player* TestForWinner();

    std::list<TTT_Move*> PossibleMoves();
    std::list<TTT*>     PossibleGames();
    std::string Generate_StringRepresentation();

    TTT_Player* DeclareWinner(TTT_Player* Winner);
    char GetWinnersCharacter();
    //void DisplayInTerminal();
    TTT* RollOut();
    void PlayGame();
    //hash<TTT> GenerateHash(std::list<TTT_Player*> GivenPlayers);
    bool equal(TTT* OtherGame);

    void Save(std::string LogPath);
    void Read(std::string LogPath);
    std::size_t Hash();

    Json::Value* JSON();
};

//#include<bits/stdc++>
//template< class Key >
//struct hash<class template>;



std::size_t TTT::Hash(){
  std::size_t HashSum;
  std::string GameState = convertToString(this->Board,9);
  //std::cout << GameState <<"\n";
  HashSum = (std::hash<std::string>()(GameState));
  //std::cout << HashSum <<"\n";

/*
int Position = 0;
for (TTT_Player* Player: this->Players) { // c++11 range-based for loop
  Position+=1;
  HashSum += Player->Hash()*Position;
}*/
  GameHash = HashSum;
  return HashSum;
}




void TTT::SetUpBoard()
{
  for (int EachPositions = 0; EachPositions < 9; EachPositions++)
  {
    Board[EachPositions] = ' ';
  }
}



void TTT::DeclarePlayers(std::list<TTT_Player*> GivenPlayers)
{
  //printf("Adding Players\n");
  for (TTT_Player* i : GivenPlayers) { // c++11 range-based for loop
      TTT_Player* TTTPlayer = static_cast<TTT_Player*>(i);
      Players.push_back(TTTPlayer);
      //_Players.push_back(i);
    }
}


void TTT::RotatePlayers(){
  Players.splice(Players.end(),        // destination position
                 Players,              // source list
                 Players.begin());     // source position

};


//////////////////////////////////////////////////////////////////////////////
// Game Functionality
//////////////////////////////////////////////////////////////////////////////

bool TTT::ValidMove(GameMove* Move)
{
  //printf("TTT MovesRemaining:%d\n",MovesRemaining);
  if(MovesRemaining == 0 ){
    DeclareWinner(&Draw);
    return false;
  }

  TTT_Move* TTTMove = static_cast<TTT_Move*>(Move);

  //printf("TTTMove->Row:%d\n",TTTMove->Row);
  //printf("TTTMove->Col:%d\n",TTTMove->Col);
  //printf("Board[TTTMove->Row][TTTMove->Col]:%c\n",Board[TTTMove->Row][TTTMove->Col]);
  if (Board[TTTMove->Row*3+TTTMove->Col] == ' ')
  {
    //Valid Move
    //printf("TTT Valid Move\n");
    return true;
  }
  else
  {
    //Invalid Move
    //printf("TTT InValid Move\n");
    return false;
  }
}


bool TTT::Move(GameMove* Move)
{
  TTT_Move* TTTMove = static_cast<TTT_Move*>(Move);

  if (this->ValidMove(Move))
  {
    MovesRemaining--;

    // move first element to the end
    Board[TTTMove->Row*3+TTTMove->Col] = Players.front()->GameRepresentation;
    TestForWinner();
    RotatePlayers();
    return true;
  }
  return false;
}

void TTT::DisplayWinner(){
  printf("WinningPlayer:%p\n",WinningPlayer);
  if(this->WinningPlayer != NULL){
    TTT_Player* TTTPlayer = static_cast<TTT_Player*>(WinningPlayer);
    printf("Player %c Has won!",TTTPlayer->GameRepresentation);
  }

};


std::string TTT::Generate_StringRepresentation()
{

  std::string Game = "Winner: ";

  if (WinningPlayer != NULL){
    //Convert from Generic Player to TTT_Player Structure
    TTT_Player* TTTPlayer = static_cast<TTT_Player*>(WinningPlayer);

    Game += (TTTPlayer->GameRepresentation); //Use '+=' when appending a char
  }
  else{
    Game.append("C");
  }

  Game.append("\n");
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        char position = Board[Row*3+Col];
        Game.append(&position);
        Game.append("|");
    }
    Game.append("\n--------\n");
  }
  return Game;
}

TTT_Player* TTT::DeclareWinner(TTT_Player* GivenWinner)
{
  if(WinningPlayer == NULL){
    //Player* Winner = static_cast<Player*>(GivenWinner);
    WinningPlayer=GivenWinner;
    SimulationFinished = true;
    //std::cout << this->Generate_StringRepresentation();
    //printf("WinningPlayer:%p\n",WinningPlayer);

  }
  return GetWinner();
}


char TTT::GetWinnersCharacter()
{
  TTT_Player* Winner = TestForWinner();
  if(Winner != NULL)
  {
  return static_cast<TTT_Player*>(TestForWinner())->GameRepresentation;
  }
  else{
    return ' ';
  }
}


TTT_Player* TTT::GetWinner(){
  return (WinningPlayer);
};

// Returns True/False If Winner is found
TTT_Player* TTT::TestForWinner()
{
  //std::cout <<"Moves remaining(TTT Game): "<< this->MovesRemaining<<"\n";
  if(
    WinningPlayer != NULL
  ){
    return WinningPlayer;
  }

  for (int Row_Col = 0; Row_Col < 3; Row_Col++)
  {
    if(
      Board[Row_Col*3] == Board[Row_Col*3+1] &&
      Board[Row_Col*3] == Board[Row_Col*3+2] &&
      Board[Row_Col*3] != ' '
    )
    {
      /*
      Winning Row Method Found. Example:
      X|X|X|
      --------
       | | |
      --------
       | | |
      */

      return this->DeclareWinner(Players.front());

    }
    else if(
      Board[Row_Col] == Board[3+Row_Col] &&
      Board[Row_Col] == Board[6+Row_Col] &&
      Board[Row_Col] != ' '
    )
    {
      /*
      Winning Column Method Found. Example:
      X| | |
      --------
      X| | |
      --------
      X| | |
      */
      return this->DeclareWinner(Players.front());

    }
  }


  if(
    Board[0] == Board[4] &&
    Board[0] == Board[8] &&
    Board[0] != ' '
  )
  {
/*
Winning Diagonal Method Found. Example:
  X| | |
  --------
   |X| |
  --------
   | |X|
  */
      return this->DeclareWinner(Players.front());

  }
  else if(
    Board[2] == Board[4] &&
    Board[2] == Board[6] &&
    Board[2] != ' '
  )
  {
/*
Winning Diagonal Method Found. Example:
   | |X|
  --------
   |X| |
  --------
  X| | |
  */
      return this->DeclareWinner(Players.front());
  }

  if(this->MovesRemaining == 0){
    //WinningPlayer = &Draw;
    //return WinningPlayer;
    return this->DeclareWinner(&Draw);
  }
  return WinningPlayer;
}


std::list<TTT_Move*> TTT::PossibleMoves()
{
  std::list<TTT_Move*>Moves;

  //GameMove TTTPlayer = static_cast<GameMove>(TTT_Move(0,0));
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        if (Board[Row*3+Col] == ' ')
        {
          TTT_Move* TTTMove = new TTT_Move(Row,Col);
          //GameMove* Move = static_cast<GameMove*>(TTTMove);
          Moves.push_back(TTTMove);
        }
    }
  }
  return Moves;
}

std::list<TTT*> TTT::PossibleGames()
{
  std::list<TTT_Move*> Moves = PossibleMoves();
  std::list<TTT*>Games;
  TTT* Branch;
  for (TTT_Move* GMove : Moves) { // c++11 range-based for loop
       Branch = new TTT(*this);
       Branch->Move(GMove);
       Games.push_back(Branch);
       //Free each Move Structure
/*
printf("%p\n",&(Branch));
printf("Create Instance->Players:%p\n",(Branch->_Players));
printf("PossibleGames's Players:%p\n",&(Branch->_Players));
for (Player* _Pl : Branch->_Players){
      printf("\t:%p\n",(_Pl));
}
*/
       delete GMove;
    }
  //printf("Freeing Moves list \n");
  //delete &Moves;
  return Games;
}


TTT* TTT::CopyGame(){
  return (new TTT(*this));
}


bool TTT::equal(TTT* OtherGame)
{
  if(GameHash != OtherGame->GameHash){
    return false;
  }
  for (int Row = 0; Row < 9; Row++)
  {
    if (Board[Row] != OtherGame->Board[Row]){
      return false;
    }
  }
  return true;
}



TTT* TTT::RollOut(){
  GameMove* Move;
  int Range;

  //TTT_Player* TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  while(WinningPlayer == NULL){

    //TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
    std::list<TTT_Move*>GameMoves = PossibleMoves();
    Range = GameMoves.size();
    //printf("Range:%d\n",Range);
    Move          = get(GameMoves,(rand() % (Range)));
    this->Move(Move);
    //printf("Freeing memory\n");
    Free_TTTMoveList(GameMoves);
    //delete &GameMoves;
    //delete Move;

    //std::cout << this->Generate_StringRepresentation();
    //TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  }
  //printf("WinningPlayer:%p\n",WinningPlayer);
  return this;
}

void TTT::PlayGame()
{
  GameMove* Move;
  TTT_Player* Currentplayer;

  TTT_Player* TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  while(TTTPlayer == NULL){
    Currentplayer = Players.front();

    Move          = (*Currentplayer).MakeMove(this);
    this->Move(Move);
    delete Move;

    //std::cout << this->Generate_StringRepresentation();
    TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  }
}








//////////////////////////////////////////////////////////////////////////////
// Save and Read from Files using JSON.
//////////////////////////////////////////////////////////////////////////////

/*
#include<jsoncpp/json/value.h>
#include<jsoncpp/json/json.h>
//#include <json.h>
#include <iostream>
#include <fstream>
#include <string>
*/


//
/*
void TTT::Add(TTT*p) {
    int a_size = sizeof(p->Board) / sizeof(char);
    std::string str = convertToString(p->Board, a_size);
    Json::Value JSONValue = new Json::Value();

    JSONValue["Board"]          = str;
    Json::Int64 Hash = p->Hash();
    JSONValue["Hash"]           = Hash;
    JSONValue["MovesRemaining"] = p->MovesRemaining;


}
*/








//TTT*TTT_Object
Json::Value* TTT::JSON(){
  int a_size = sizeof(Board) / sizeof(char);
  std::string _str = convertToString(Board, a_size);
  Json::Value* JSONValue = new Json::Value();

  (*JSONValue)["Board"]          = _str;
  (*JSONValue)["Hash"]           = std::to_string(Hash());
  (*JSONValue)["MovesRemaining"] = MovesRemaining;

  if(WinningPlayer==NULL){
    (*JSONValue)["WinningPlayer"] = MovesRemaining;
  }
  else{

  }

    //delete JSONValue;
  return JSONValue;
}

void TTT::Save(std::string FilePath){
  Json::Value* value_obj = JSON();
  for (TTT_Player* i : Players) { // c++11 range-based for loop
      //std::cout << *(i->JSON()) << std::endl;
      //(*value_obj)["Players"][std::string(i->GameRepresentation)] = *(i->JSON());
      //TODO Change GameRepresentation to Player number/ID.
      //Create Original Player order Logic.
      //(*value_obj)["Players"][std::string(1,i->GameRepresentation)] = *(i->JSON());
      i->Add(&(*value_obj)["Players"][std::string(1,i->GameRepresentation)]);

    }

    //std::cout << (*value_obj) << std::endl;

  std::ofstream file_id;
  file_id.open(FilePath);

  //populate 'value_obj' with the objects, arrays etc.

  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(*value_obj);

  file_id.close();

  delete value_obj;
}




TTT* Read_TTT_JSON(std::string FilePath){
    //std::cout << "Reading File" << std::endl;
    std::ifstream file(FilePath);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        std::cout << "Error parsing the string" << std::endl;
    }
    TTT* Game = new TTT(root);
    //std::cout << root << std::endl;

    return Game;
}







#endif //TTT_CPP
