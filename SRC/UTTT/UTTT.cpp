/*
====================================================================================================
Description UTTT(Ultimate Tic Tac Toe):
Purpose:
  Implement Ultimate Tic Tac Toe through Game interface. Using rules To be described...
Contains(Classes):
  UTTT_Player
  UTTT_Move
  UTTT
====================================================================================================
Date:           NA
Script Version: 1.0
Name:           Anthony M Schroeder
Email:          as3379@nau.edu
==========================================================

*/

#ifndef UTTT_CPP
#define UTTT_CPP


#include <iostream>
#include <string>
#include <list>

#include <jsoncpp/json/json.h>

#include "../Game.cpp"
#include "../TTT/TTT.cpp"
#include "UTTT.h"



/*
UTTT_Player
@Purpose: Class to track UTTT Players.
@Methods:
  MakeMove() function pointer to allow for Humans to play.
*/
struct UTTT_Player : public TTT_Player
{
  public:
    int PlayerNumber;
    bool HumanPlayer;
    char GameRepresentation;
    //UTTT_Player(){}


    //////////////////////////////////////////////////////////////////////////////
    // Initialization method.
    UTTT_Player(Json::Value root):
    TTT_Player(root){
      std::cout <<"creating UTTT_Player:\n";
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

  UTTT_Player(int GivenPlayer,char GivenGameRepresentation):
    TTT_Player(GivenPlayer,GivenGameRepresentation){
    PlayerNumber = GivenPlayer;
    HumanPlayer = false;
    GameRepresentation = GivenGameRepresentation;
    //printf("Player:%p:%c\n",this,GameRepresentation);
  }
  /**/
  /*UTTT_Player(nlohmann::json &j){
    int iChar = j["GameRepresentation"];
    GameRepresentation = iChar;


    PlayerNumber = j["PlayerNumber"];
    HumanPlayer = j["HumanPlayer"];
  }*/
  ~UTTT_Player(){}

  UTTT_Move* MakeMove(UTTT* GivenGame);
};
/*
void Add(nlohmann::json &j, UTTT_Player*p) {
  j = nlohmann::json::object({
    {"PlayerNumber", p->PlayerNumber},
    {"HumanPlayer", p->HumanPlayer},
    {"GameRepresentation", p->GameRepresentation}
  });
}*/

std::size_t Hash(UTTT_Player* k){
  //std::size_t HashSum;
  //std::string GameState = convertToString(k->Board,9);
  //HashSum = (std::hash<std::char>());

  return (int)(k->GameRepresentation);
}

/*
UTTT_Move
@Purpose: A Helper class to hold the potential move data for a UTTT Game. IE: X,Y coordinates. And possibly a Player Pointer.
@Methods:
  No Methods. Intended to only hold move data.
*/
struct UTTT_Move : public TTT_Move
{

  public:
    int GameRow;
    int GameCol;
    UTTT_Player* Player;

    int Row;
    int Col;

    UTTT_Move(
      int GivenGameRow,
      int GivenGameCol,
      int GivenRow,
      int GivenCol
    ):
    TTT_Move(GivenRow,GivenCol){
          GameRow = GivenGameRow;
          GameCol = GivenGameCol;
          Row = GivenRow;
          Col = GivenCol;
      }
      ~UTTT_Move(){}
};




void Free_UTTTMoveList(std::list<UTTT_Move*> GameMoves)
{
  //std::list<GameMove*> Moves = PossibleMoves();
  for (UTTT_Move* Move : GameMoves) { // c++11 range-based for loop
      //UTTT_Move* Move = static_cast<UTTT_Move*>(GMove);
      delete Move;
    }
}

UTTT_Move* UTTT_Player::MakeMove(UTTT* GivenGame)
{
  //GameMove TTTPlayer = static_cast<GameMove>(TTT_Move(0,0));
  int GameRow,GameCol,Row,Col;
  std::cout << "Please Enter GameRow: ";
  std::cin >> GameRow;
  std::cout << "Please Enter Col Axis: ";
  std::cin >> GameCol;
  std::cout << "Please Enter Row Axis: ";
  std::cin >> Row;
  std::cout << "Please Enter Col: ";
  std::cin >> Col;
   UTTT_Move* UTTTMove = new UTTT_Move(GameRow,GameCol,Row,Col);

   return UTTTMove;
}


class UTTT_SubGame : public TTT
{
  public:
    UTTT_Player Draw    = UTTT_Player(-1,'C');

    UTTT_SubGame(std::list<UTTT_Player*> GivenPlayers){
        this->DeclarePlayers(GivenPlayers);
        SimulationFinished = false;

        this->WinningPlayer  = NULL;
        MovesRemaining       = 9;
        this->SetUpBoard();
        GameHash = this->Hash();
      }
      //////////////////////////////////////////////////////////////////////////////
      // JSON Initialization method(Reading from file).

      UTTT_SubGame(Json::Value ReadJSValue){

        Json::FastWriter fastWriter;
        printf("creating UTTT_SubGame:\n");

        MovesRemaining       = atoi(fastWriter.write(ReadJSValue["MovesRemaining"]).c_str());
        SimulationFinished = atoi(fastWriter.write(ReadJSValue["SimulationFinished"]).c_str());

        std::cout << ReadJSValue << std::endl;

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



        //////////////////////////////////////////////////////////////////////////////
        //Reconstruct Players
        //printf("creating TTT_Players :\n");
        //std::cout <<"Size:"<<ReadJSValue["Players"].size()<<"\n";
        for (auto const& id : ReadJSValue["Players"].getMemberNames()) {
          std::cout << id << std::endl;
            Players.push_back(new UTTT_Player(ReadJSValue["Players"][id]));
        }


        GameHash = this->Hash();
        this->WinningPlayer  = NULL;
        JsonRead = true;
        TestForWinner();
      }

      ~UTTT_SubGame(){


        if(JsonRead){
          for (TTT_Player* Player: Players) { // c++11 range-based for loop
            //free(Player);
            //delete Player;
          }
        }

      }
    bool Move(UTTT_Move* Move);
    void DeclarePlayers(std::list<UTTT_Player*> GivenPlayers);
    void AddPlayers(std::list<UTTT_Player*> Players);
    UTTT_Player* DeclareWinner(UTTT_Player* GivenWinner);
    std::list<UTTT_Move*> PossibleMoves();
    bool equal(TTT* OtherGame);
    Json::Value* JSON();
    Json::Value* Add(Json::Value* JSONValue);
    //bool ValidMove(GameMove* Move);
};


void UTTT_SubGame::DeclarePlayers(std::list<UTTT_Player*> GivenPlayers)
{
  //printf("Adding Players\n");
  for (UTTT_Player* player : GivenPlayers) { // c++11 range-based for loop
      Players.push_back(player);
      //_Players.push_back(i);
    }
}

std::list<UTTT_Move*> UTTT_SubGame::PossibleMoves()
{
  std::list<UTTT_Move*>Moves;

  //GameMove TTTPlayer = static_cast<GameMove>(TTT_Move(0,0));
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        if (Board[Row*3+Col] == ' ')
        {
          UTTT_Move* Move = new UTTT_Move(-1,-1,Row,Col);
          Moves.push_back(Move);
        }
    }
  }
  return Moves;
}
/*
Player* UTTT_SubGame::DeclareWinner(UTTT_Player* GivenWinner)
{
  if(WinningPlayer == NULL){
    //Player* Winner = static_cast<Player*>(GivenWinner);
    WinningPlayer=GivenWinner;
  }
  return static_cast<Player*>(WinningPlayer);
}
*/

bool UTTT_SubGame::Move(UTTT_Move* Move)
{

  //////////////////////////////////////////////////////////////////////////////
  // Validate Move is legal, before preforming move.
  if (this->ValidMove(Move))
  {
    //////////////////////////////////////////////////////////////////////////////
    // Modify Sub Game's Board, by adding the current Player's GameRepresentation
    Board[Move->Row*3+Move->Col] = Move->Player->GameRepresentation;

    Players.splice(Players.end(),        // destination position
                   Players,              // source list
                   Players.begin());     // source position
    //printf("TestForWinner() \n");
    TestForWinner();
    return true;
  }
  return false;
}


void UTTT_SubGame::AddPlayers(std::list<UTTT_Player*> Players)
{
  for (UTTT_Player* Player: Players) { // c++11 range-based for loop
    Players.push_back(Player);
  }
}

bool UTTT_SubGame::equal(TTT* OtherGame)
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


//TTT*TTT_Object
Json::Value* UTTT_SubGame::Add(Json::Value* JSONValue){
  int a_size = sizeof(Board) / sizeof(char);
  std::string _str = convertToString(Board, a_size);

  (*JSONValue)["Board"]          = _str;
  (*JSONValue)["Hash"]           = std::to_string(Hash());
  (*JSONValue)["MovesRemaining"] = MovesRemaining;
    //delete JSONValue;
  for (TTT_Player* i : Players) { // c++11 range-based for loop
      //std::cout << *(i->JSON()) << std::endl;
      //(*value_obj)["Players"][std::string(i->GameRepresentation)] = *(i->JSON());
      //TODO Change GameRepresentation to Player number/ID.
      //Create Original Player order Logic.
      //(*value_obj)["Players"][std::string(1,i->GameRepresentation)] = *(i->JSON());
      i->Add(&(*JSONValue)["Players"][std::string(1,i->GameRepresentation)]);

    }
  return JSONValue;
}

//TTT*TTT_Object
Json::Value* UTTT_SubGame::JSON(){
  int a_size = sizeof(Board) / sizeof(char);
  std::string _str = convertToString(Board, a_size);
  Json::Value* JSONValue = new Json::Value();

  (*JSONValue)["Board"]          = _str;
  (*JSONValue)["Hash"]           = std::to_string(Hash());
  (*JSONValue)["MovesRemaining"] = MovesRemaining;
    //delete JSONValue;
  return JSONValue;
}


std::size_t Hash(UTTT_SubGame* k)
{
  std::size_t HashSum;
  std::string GameState = convertToString(k->Board,9);
  //std::cout << GameState <<"\n";
  HashSum = (std::hash<std::string>()(GameState));
  //std::cout << HashSum <<"\n";

  k->GameHash = HashSum;
  return HashSum;
}









/*
UTTT - (Ultimate Tic Tac Toe) buisness logic.
This class simulates UTTT (Ultimate Tic Tac Toe) and follows the Game class interface structure to allow for the easy integration of Tree Searches.

@param (std::list<Player*> GivenPlayers), as the players to play the game.

@relatesalso Game, TTT



Long -
Ultimate Tick Tack Toe is a simple advancement to Tick Tack Toe’s game, except
the board is expanded to contain nine miniature tick tack toe games. For a general
idea about the game, check out this YouTube video: https://www.youtube.com/watch?v=37PC0bGMiTI
Note the implemented rules in my program are slightly different and will be
added in an additional documentation/Tutorial.
*/
class UTTT : public Game
{
public:
  //////////////////////////////////////////////////////////////////////////////
  // Player(s) DATA
  //TODO: Take Draw player during Initialization.
  //////////////////////////////////////////////////////////////////////////////
  UTTT_Player Draw    = UTTT_Player(-1,'C');

  //Players are placed in the following list as a rotating queue.
  //Based on the structure, std::list<Player*> needs to be casted to std::list<UTTT_Player*>
  std::list<UTTT_Player*> Players;

  //Pointer to declare the winner.
  UTTT_Player*  WinningPlayer;

  //////////////////////////////////////////////////////////////////////////////
  // Game Data
  //////////////////////////////////////////////////////////////////////////////
  //NextMove_Row/NextMove_Col determines where the next player must play based on the previous player’s move.
  int NextMove_Row;
  int NextMove_Col;

  //MovesRemaining is a decrementing counter to determine if there are any remaining moves.
  int MovesRemaining;
  bool SimulationFinished;

  //Represenations of each game within the larger 3x3 game.
  UTTT_SubGame* Boards[3][3];
  std::size_t GameHash;


  //////////////////////////////////////////////////////////////////////////////
  // JSON File Data
  //////////////////////////////////////////////////////////////////////////////
  bool JsonRead;

  //////////////////////////////////////////////////////////////////////////////
  // JSON Initialization method(Reading from file).
  UTTT(Json::Value ReadJSValue){
  printf("creating UTTT:\n");

    //std::cout << ReadJSValue << std::endl;
    JsonRead = true;
    Json::FastWriter fastWriter;
    std::string Temp;


    Temp = fastWriter.write(ReadJSValue["NextMove_Row"]);

    NextMove_Row   = atoi(fastWriter.write(ReadJSValue["NextMove_Row"]).c_str());
    NextMove_Col   = atoi(fastWriter.write(ReadJSValue["NextMove_Col"]).c_str());
    MovesRemaining = atoi(fastWriter.write(ReadJSValue["MovesRemaining"]).c_str());
    printf("MovesRemaining:%d\n",MovesRemaining);
    SimulationFinished = atoi(fastWriter.write(ReadJSValue["SimulationFinished"]).c_str());;
    WinningPlayer  = NULL;

    //////////////////////////////////////////////////////////////////////////////
    //Reconstruct Boards
    printf("Reconstruct Boards :\n");
    //std::cout <<"Size:"<<ReadJSValue["Players"].size()<<"\n";
    for (auto const& id : ReadJSValue["Board"].getMemberNames()) {
      std::cout << id << std::endl;
        int Value = atoi( id.c_str());
        int Row = Value/3;
        int Col = Value%3;
        printf("%d(%d,%d):\n",Value,Row,Col);
        Boards[Row][Col] = (new UTTT_SubGame(ReadJSValue["Board"][id]));
    }
    //std::cout <<"Size:"<<ReadJSValue["Players"].size()<<"\n";

    //////////////////////////////////////////////////////////////////////////////
    //Reconstruct Players
    printf("Reconstruct UTTT_Players :\n");
    for (auto const& id : ReadJSValue["Players"].getMemberNames()) {
      std::cout << id << std::endl;
        Players.push_back(new UTTT_Player(ReadJSValue["Players"][id]));
    }
  }
  /*
  UTTT(nlohmann::json &j){

    NextMove_Row   = -1;
    NextMove_Col   = -1;
    MovesRemaining = 81;
    WinningPlayer  = NULL;

    //////////////////////////////////////////////////////////////////////////////
    // For Each Player within JSON file, place back within Players list.
    // NOTE: When Saving TTT Players to JSON file, the order is swapped(The
    //   First player is at the bottom of the list); the for loop automatically
    //   adds the players back into the order(The first player within the JSON
    //   file becomes the last player within the Player order).
    for (nlohmann::json ji: j["Players"]) { // c++11 range-based for loop
      Players.push_back(new UTTT_Player(ji));
    }


    //this->SetUpBoards(Players);

    for (int Row = 0; Row < 3; Row++)
    {
      for (int Col = 0; Col < 3; Col++)
      {
        std::cout <<"Game"+std::to_string(Row)+std::to_string(Col)<< '\n';

        Boards[Row][Col] = new UTTT_SubGame(j["Games"]["Game"+std::to_string(Row)+std::to_string(Col)]);

        if(j["Games"]["Game"+std::to_string(Row)+std::to_string(Col)].empty()){
          throw std::invalid_argument( "TTT JSON File doesnt contain Board(char[9])." );
          //throw "TTT JSON File doesnt contain Board(char[9]).";
          exit(1);
        }
      }
    }


    JsonRead = true;
  }
  */

  //////////////////////////////////////////////////////////////////////////////
  // Initialization method.
  UTTT(std::list<UTTT_Player*> GivenPlayers){
      Players = GivenPlayers;
      //this->DeclarePlayers(GivenPlayers);

      this->WinningPlayer  = NULL;
      NextMove_Row   = -1;
      NextMove_Col   = -1;
      MovesRemaining = 81;
      this->SetUpBoards(GivenPlayers);
      JsonRead = false;
      SimulationFinished = false;
      GameHash = this->Hash();

    }
    ~UTTT(){
      this->FreeBoards();
      if(JsonRead){
        for (UTTT_Player* i : Players) { // c++11 range-based for loop
            delete i;
          }
      }
      //delete Boards;
    }

    //////////////////////////////////////////////////////////////////////////////
    // Method Declarations.
    //////////////////////////////////////////////////////////////////////////////
    void SetUpBoards(std::list<UTTT_Player*> GivenPlayers);
    void FreeBoards();

    bool Move(UTTT_Move* Move);

    bool ValidMove(GameMove* Move);
    UTTT_Player* TestForWinner();
    void DisplayWinner();
    std::list<UTTT_Move*> PossibleMoves();
    std::list<UTTT*>     PossibleGames();
    std::string Generate_GameRowRepresentation(int Row);
    std::string Generate_StringRepresentation();

    //void DisplayInTerminal();
    UTTT* RollOut();
    UTTT* CopyGame();
    void PlayGame();
    void DeclarePlayers(std::list<UTTT_Player*> GivenPlayers);
    UTTT_Player* DeclareWinner(UTTT_Player* Winner);
    bool equal(UTTT* OtherGame);

    void Save(std::string LogPath);
    std::size_t Hash();
    Json::Value* JSON();
    void Add(Json::Value* JsonValue);
};


std::size_t UTTT::Hash()
{
  // Compute individual hash values for first,
  // second and third and combine them using XOR
  // and bit shifting:
  //std::hash<UTTT_SubGame>* Hash = new std::hash<UTTT_SubGame>;// = std::hash<TTT>(* _Game);
  std::size_t Itteration = 0;
  std::size_t HashSum = 0;
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        //Board[Row][Col] = ' ';
        Itteration = this->Boards[Row][Col]->Hash()*(Col+1)*(Row+1)>> 1;
        //std::cout << this->Boards[Row][Col]->Hash() << "\n";
        //Pause;
        HashSum        += Itteration;
        //printf("Itteration: %zu\n",Itteration);
    }
  }

/*
int Position = 0;
for (UTTT_Player* Player: Players) { // c++11 range-based for loop
  Position+=1;
  HashSum += Hash(Player)*Position;
}
*/
  //printf("\tSum: %zu\n",Sum);
  //printf("----------------------------\n");
  //delete Hash;
  //return ((
  //         ^ (hash<string>()(k.second) << 1)) >> 1)
  //         ^ (hash<int>()(k.third) << 1);
  return HashSum;
}



/*
SetUpBoard
  Generic method to initilize each sub-Game class within the 3x3 game.

param (std::list<Player*> GivenPlayers), as the players to play the game.
*/
void UTTT::SetUpBoards(std::list<UTTT_Player*> GivenPlayers)
{

  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        Boards[Row][Col] =  new UTTT_SubGame(GivenPlayers);
    }
  }
}

/*
FreeBoards frees all sub-games to prevent memory leaks.
*/
void UTTT::FreeBoards()
{
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        delete Boards[Row][Col];
    }
  }
}

/*
DeclarePlayers
 */
void UTTT::DeclarePlayers(std::list<UTTT_Player*> GivenPlayers)
{
  for (UTTT_Player* i : GivenPlayers) { // c++11 range-based for loop
      //UTTT_Player* UTTTPlayer = static_cast<UTTT_Player*>(i);
      Players.push_back(i);
    }
}


/*
DeclarePlayers
 */
bool UTTT::equal(UTTT* OtherGame)
{
  if(GameHash != OtherGame->GameHash){
    return false;
  }
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        if (!Boards[Row][Col]->equal(OtherGame->Boards[Row][Col])){
          return false;
        }
    }
  }
  return true;
}



/*
CopyGame creates a complete copy of the game representation(Except for Players).
*/
UTTT* UTTT::CopyGame(){

  UTTT* New_UTTT = new UTTT(*this);
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
      UTTT_SubGame* SubGame = this->Boards[Row][Col];
      New_UTTT->Boards[Row][Col] =  new UTTT_SubGame(*SubGame);
    }
  }
  /*
  TODO: Check Players List Pointers are created.
  */

  //for (Player* i : New_UTTT->Players) { // c++11 range-based for loop
      //printf("UTTT Player List:%p\n",i);
    //}

  return (New_UTTT);
}


//////////////////////////////////////////////////////////////////////////////
// Game Functionality
//////////////////////////////////////////////////////////////////////////////
/*
*/
bool UTTT::ValidMove(GameMove* Move)
{

  UTTT_Move* UTTTMove = dynamic_cast<UTTT_Move*>(Move);
  if(
    NextMove_Row == -1 ||
    NextMove_Col == -1
  ){
    return Boards[UTTTMove->GameRow][UTTTMove->GameCol]->ValidMove(Move);
  }

  if(
    UTTTMove->GameRow == NextMove_Row &&
    UTTTMove->GameCol == NextMove_Col
  ){
    return Boards[UTTTMove->GameRow][UTTTMove->GameCol]->ValidMove(Move);
  }

  return false;
}



bool UTTT::Move(UTTT_Move* Move)
{
  UTTT_Move* UTTTMove = dynamic_cast<UTTT_Move*>(Move);
  //printf("UTTTMove:%p\n",&UTTTMove);
  //printf("UTTTMove->GameRow:%d\n",UTTTMove->GameRow);
  //printf("UTTTMove->GameCol:%d\n",UTTTMove->GameCol);

  UTTTMove->Player = *Players.begin();
  //printf("UTTTMove->Row:%d\n",UTTTMove->Row);
  //printf("UTTTMove->Col:%d\n",UTTTMove->Col);


  if (this->ValidMove(Move))
  {
    MovesRemaining--;

    // move first element to the end
    Boards[UTTTMove->GameRow][UTTTMove->GameCol]->Move(Move);

    NextMove_Row = UTTTMove->Row;
    NextMove_Col = UTTTMove->Col;
    TestForWinner();

    Players.splice(Players.end(),        // destination position
                   Players,              // source list
                   Players.begin());     // source position
    //printf("valid Move");
    return true;
  }
  //printf("Invalid Move");
  return false;
}
/*
if (this->ValidMove(Move))
{
  UTTT_Move* UTTTMove = dynamic_cast<UTTT_Move*>(Move);
  UTTTMove->Player = PlayerCharacter;
  return Boards[UTTTMove->Row][UTTTMove->Col].Move(Move);
}
return false;
*/


std::string UTTT::Generate_GameRowRepresentation(int Row)
{

  std::string GameRep = "";
  /*
  Here is a graph of the individual smaller games.
  X|X|X|
  --------
   | | |
  --------
   | | |
  */
  for (int SubRow = 0; SubRow < 3; SubRow++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
        for (int SubCol = 0; SubCol < 3; SubCol++)
        {
            char GameCharacter = Boards[Row][Col]->Board[SubRow*3+SubCol];
            GameRep.append(&GameCharacter);
            GameRep.append("|");
        }
        GameRep.append("   ");
    }
    GameRep.append("\n---------------------------\n");
  }

  return GameRep;

}

std::string UTTT::Generate_StringRepresentation()
{
  TestForWinner();
  std::string GameRep = "UTTT Winner: ";
  //printf("UTTT Winner:%p\n",WinningPlayer);
  if (WinningPlayer != NULL){
    //Convert from Generic Player to TTT_Player Structure
    UTTT_Player* UTTTPlayer = static_cast<UTTT_Player*>(WinningPlayer);

    GameRep += (UTTTPlayer->GameRepresentation); //Use '+=' when appending a char
  }
  else{
    GameRep.append("C");
  }
  GameRep.append("\n");


  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
      //printf("%p,%p\n",Boards[Row][Col].WinningPlayer,Boards[Row][Col].TestForWinner());
      //std::cout << Boards[Row][Col].Generate_StringRepresentation();
      //std::cout << "\n";
      //
      if (Boards[Row][Col]->WinningPlayer != NULL){
        TTT_Player* TTTPlayer = static_cast<TTT_Player*>(Boards[Row][Col]->WinningPlayer);
        char position = TTTPlayer->GameRepresentation;
        GameRep.append(&position);

      }
      else{
        GameRep.append("C");
      }
        GameRep.append("|");
    }
    GameRep.append("\n--------\n");
  }
  GameRep.append("\n\n");
  for (int Row = 0; Row < 3; Row++)
  {
    GameRep.append(Generate_GameRowRepresentation(Row));
    GameRep.append("---------------------------\n");
  }
  return GameRep;
}


UTTT_Player* UTTT::DeclareWinner(UTTT_Player* GivenWinner)
{
  //printf("DeclareWinner:%p\n",GivenWinner);

  if(WinningPlayer == NULL){
    //Player* Winner = static_cast<Player*>(GivenWinner);

    SimulationFinished = true;
    WinningPlayer = GivenWinner;
  }
  return (WinningPlayer);
}

void UTTT::DisplayWinner(){
  if(WinningPlayer!=NULL){
    printf("Player %d Has won!",WinningPlayer->PlayerNumber);
  }
};


UTTT_Player* UTTT::TestForWinner()
{
  //printf("TestForWinner(UTTT Game)\n");
  //std::cout <<"MovesRemaining: "<< this->MovesRemaining<<"\n";
  //std::cout <<"WinningPlayer: "<< WinningPlayer<<"\n";

  if(
    WinningPlayer != NULL
  ){
    return WinningPlayer;
  }
    //printf("TestForWinner(UTTT Game) Game by Game\n");
  //printf("UTTT Winner%p\n",WinningPlayer);
  for (int Row_Col = 0; Row_Col < 3; Row_Col++)
  {


/*
if(Boards[Row_Col][0]->WinningPlayer != NULL)
{
  std::cout << "[Row_Col][0]" << Boards[Row_Col][0]->WinningPlayer <<"'"<< static_cast<TTT_Player*>(Boards[Row_Col][0]->WinningPlayer)->GameRepresentation << "'\n";
}
if(Boards[Row_Col][1]->WinningPlayer != NULL)
{
  std::cout << "[Row_Col][1]" << Boards[Row_Col][1]->WinningPlayer <<"'"<< static_cast<TTT_Player*>(Boards[Row_Col][1]->WinningPlayer)->GameRepresentation << "'\n";
}
if(Boards[Row_Col][2]->WinningPlayer != NULL)
{
  std::cout << "[Row_Col][2]" << Boards[Row_Col][2]->WinningPlayer <<"'"<< static_cast<TTT_Player*>(Boards[Row_Col][2]->WinningPlayer)->GameRepresentation << "'\n\n";
}*/


  //printf("TestForWinner(UTTT Game) Row-COL:%d\n",Row_Col);

    if(
      Boards[Row_Col][0]->TestForWinner() == Boards[Row_Col][1]->TestForWinner() &&
      Boards[Row_Col][0]->TestForWinner() == Boards[Row_Col][2]->TestForWinner() &&
      Boards[Row_Col][0]->TestForWinner() != NULL
    )
    {
        //printf("TestForWinner(UTTT Game) Row\n");
      /*
      Winning Row Method Found. Example:
      X|X|X|
      --------
       | | |
      --------
       | | |
      */
      //printf("Found solution\n");
      return DeclareWinner(static_cast<UTTT_Player*>(Boards[Row_Col][Row_Col]->TestForWinner()));

    }
    else if(
      Boards[0][Row_Col]->TestForWinner() == Boards[1][Row_Col]->TestForWinner() &&
      Boards[0][Row_Col]->TestForWinner() == Boards[2][Row_Col]->TestForWinner() &&
      Boards[0][Row_Col]->TestForWinner() != NULL
    )
    {
  //printf("TestForWinner(UTTT Game) COL\n");
      /*
      Winning Column Method Found. Example:
      X| | |
      --------
      X| | |
      --------
      X| | |
      */
      //printf("Found solution\n");
      return DeclareWinner(static_cast<UTTT_Player*>(Boards[0][Row_Col]->TestForWinner()));
      //this->DeclareWinner(Boards[0][Row_Col].WinningPlayer);

    }
  }


  //printf("TestForWinner(UTTT Game) Diag\n");
  if(
    Boards[0][0]->TestForWinner() == Boards[1][1]->TestForWinner() &&
    Boards[0][0]->TestForWinner() == Boards[2][2]->TestForWinner() &&
    Boards[0][0]->TestForWinner() != NULL
  )
  {
  //printf("TestForWinner(UTTT Game) Diag1\n");
/*
Winning Diagonal Method Found. Example:
  X| | |
  --------
   |X| |
  --------
   | |X|
  */
  //printf("Found solution\n");
  return DeclareWinner(static_cast<UTTT_Player*>(Boards[0][0]->TestForWinner()));

  }
  else if(
    Boards[0][2]->TestForWinner() == Boards[1][1]->TestForWinner() &&
    Boards[0][2]->TestForWinner() == Boards[2][0]->TestForWinner() &&
    Boards[0][2]->TestForWinner() != NULL
  )
  {
  //printf("TestForWinner(UTTT Game) Diag2\n");
/*
Winning Diagonal Method Found. Example:
   | |X|
  --------
   |X| |
  --------
  X| | |
  */
  //printf("Found solution\n");
    return DeclareWinner(static_cast<UTTT_Player*>(Boards[0][2]->TestForWinner()));
  }
  if(this->MovesRemaining == 0){
    WinningPlayer = &Draw;
    //printf("No Remaining Moves\n");
    return WinningPlayer;
  }
  //printf("returning WinningPlayer\n");
  //printf("Reached End Returning NULL:%p\n",WinningPlayer);
  return WinningPlayer;
}

std::list<UTTT_Move*> UTTT::PossibleMoves()
{
  //printf("NextMove(%i,%i)\n",NextMove_Row,NextMove_Col);
  std::list<UTTT_Move*>Moves;
  if(
    NextMove_Row == -1 ||
    NextMove_Col == -1
  ){
    for (int Row = 0; Row < 2; Row++)
    {
      for (int Col = 0; Col < 2; Col++)
      {
        std::list<UTTT_Move*> GMoves = Boards[Row][Col]->PossibleMoves();
        //printf("GMoves(%lu)\n",(GMoves.size()));
        for (GameMove* GMove : GMoves) { // c++11 range-based for loop

             UTTT_Move* UTTT_GMove = static_cast<UTTT_Move*>(GMove);
             UTTT_GMove->GameRow = Row;
             UTTT_GMove->GameCol = Col;

             //GMove = static_cast<GameMove*>(UTTT_GMove);
             Moves.push_back(UTTT_GMove);
          }
      }
    }
    //printf("Moves(%lu)\n",(Moves.size()));
    return Moves;
  }
  else{

    for (UTTT_Move* GMove : Boards[NextMove_Row][NextMove_Col]->PossibleMoves()) { // c++11 range-based for loop
         UTTT_Move* UTTT_GMove = static_cast<UTTT_Move*>(GMove);
         UTTT_GMove->GameRow = NextMove_Row;
         UTTT_GMove->GameCol = NextMove_Col;

         //GMove = static_cast<GameMove*>(UTTT_GMove);
         Moves.push_back(UTTT_GMove);
      }
    //printf("UTTT_:NextMove_Row:%d\n",NextMove_Row);
    //printf("UTTT_:NextMove_Col:%d\n",NextMove_Col);
    return Moves;
  }

}
std::list<UTTT*> UTTT::PossibleGames()
{
  std::list<UTTT_Move*> Moves = PossibleMoves();
  std::list<UTTT*>Games;
  UTTT* Branch;
  for (UTTT_Move* GMove : Moves) { // c++11 range-based for loop
       //Branch = new UTTT(*this);
       Branch = static_cast<UTTT*>(CopyGame());
       Branch->Move(GMove);
       Games.push_back(Branch);
       //Free each Move Structure
       delete GMove;
    }


  return Games;
}





void UTTT::PlayGame()
{
  UTTT_Move* Move;
  UTTT_Player* Currentplayer;

  UTTT_Player* TTTPlayer = static_cast<UTTT_Player*>(TestForWinner());
  while(TTTPlayer == NULL){

    Currentplayer = Players.front();

    Move          = (*Currentplayer).MakeMove(this);
    this->Move(Move);
    delete Move;

    std::cout << this->Generate_StringRepresentation();
    TTTPlayer = static_cast<UTTT_Player*>(TestForWinner());
  }
}


//////////////////////////////////////////////////////////////////////////////
// MCTS/TreeSearch Functionality
//////////////////////////////////////////////////////////////////////////////
UTTT* UTTT::RollOut()
{
  //std::cout << Generate_StringRepresentation();
  //printf("Prefoming Rollout\n");
  UTTT_Move* Move;

  int Range;

  TTT_Player* TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  while(TTTPlayer == NULL){

    std::list<UTTT_Move*>GameMoves = PossibleMoves();
    Range = GameMoves.size();
    if(Range == 0){
      return this;
    }
    //printf("Range:%d\n",Range);
    Move          = get(GameMoves,(rand() % (Range)));
    //printf("Move:%p\n",Move);
    this->Move(Move);
    //printf("Freeing memory\n");
    Free_UTTTMoveList(GameMoves);
    //delete &GameMoves;
    //delete Move;

    //std::cout << this->Generate_StringRepresentation();
    TTTPlayer = static_cast<TTT_Player*>(TestForWinner());
  }
  return this;
}


/*
void Add(nlohmann::json &j, UTTT*p) {

  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
      //std::cout <<"Game"+std::to_string(Row)+std::to_string(Col)<< '\n';
      Add(j["Game"+std::to_string(Row)+std::to_string(Col)],p->Boards[Row][Col]);
    }
  }
  //printf("Adding UTTT Players\n");

  //Add(j["Players"],p->Players);


}*/

/*
template <typename Game_Tp, typename Player_Tp>
nlohmann::json Json(UTTT* p) {
  nlohmann::json data;
  Add(data,p);
  return data;
}*/


void UTTT::Add(Json::Value* JSONValue){
  for (int Row = 0; Row < 3; Row++)
  {
    for (int Col = 0; Col < 3; Col++)
    {
      //std::cout << Row*3+Col << "\n";
      //std::cout << (*(Boards[Row][Col])->JSON()) << "\n";
      //(*JSONValue)["Board"][std::to_string(Row*3+Col)] =(*(Boards[Row][Col])->JSON());
      //i->Add(&(*value_obj)["Players"][std::string(1,i->GameRepresentation)]);
      (*(Boards[Row][Col])->Add(&(*JSONValue)["Board"][std::to_string(Row*3+Col)]));
    }
  }

  (*JSONValue)["Hash"]           = std::to_string(Hash());
  (*JSONValue)["NextMove_Row"] = NextMove_Row;
  (*JSONValue)["NextMove_Col"] = NextMove_Col;
  (*JSONValue)["MovesRemaining"] = MovesRemaining;
  (*JSONValue)["WinningPlayer"] = WinningPlayer;
  (*JSONValue)["SimulationFinished"] = SimulationFinished;


    //std::cout <<(*JSONValue) << "\n";
  //return JSONValue;
}

Json::Value* UTTT::JSON(){


  //int a_size = sizeof(Board) / sizeof(char);
  //std::string _str = convertToString(Board, a_size);
  Json::Value* JSONValue = new Json::Value();
  Add(JSONValue);
    //std::cout <<(*JSONValue) << "\n";
  return JSONValue;
}





void UTTT::Save(std::string FilePath){
  Json::Value* JSONValue = JSON();
  Json::Value tmp;
  for (TTT_Player* Player : Players) { // c++11 range-based for loop
      //std::cout << *(i->JSON()) << std::endl;
      //(*value_obj)["Players"][std::string(i->GameRepresentation)] = *(i->JSON());
      //TODO Change GameRepresentation to Player number/ID.
      //Create Original Player order Logic.
      //(*JSONValue)["Players"][std::string(1,i->GameRepresentation)] = *(i->JSON());
      Player->Add(&(*JSONValue)["Players"][std::string(1,Player->GameRepresentation)]);


    }
    std::cout <<(*JSONValue) << "\n";
    //std::cout << (*value_obj) << std::endl;

  std::ofstream file_id;
  file_id.open(FilePath);

  //populate 'value_obj' with the objects, arrays etc.

  Json::StyledWriter styledWriter;
  file_id << styledWriter.write(*JSONValue);

  file_id.close();
  delete JSONValue;
}

UTTT* Read_UTTT_JSON(std::string FilePath){
    //std::cout << "Reading File" << std::endl;
    std::ifstream file(FilePath);
    Json::Value root;
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( file, root );
    if ( !parsingSuccessful )
    {
        std::cout << "Error parsing the string" << std::endl;
    }

    UTTT* Game = new UTTT(root);
    //std::cout << root << std::endl;

    return Game;
}




#endif //UTTT_CPP
