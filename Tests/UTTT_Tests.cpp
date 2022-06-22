#ifndef UTTT_Tests_CU
#define UTTT_Tests_CU

#include "../SRC/TTT.cpp"
#include "../SRC/UTTT.cpp"



bool Validate_UTTT(UTTT*Given_UTTT){
  Given_UTTT->_Players.size();
  return 0;
}

bool UTTT_INIT(){
  //std::cout << "Hello World!";
  UTTT_Player Player0 = UTTT_Player(0,'X');
  UTTT_Player Player1 = UTTT_Player(1,'O');
  Game *_Game = new UTTT({&Player0,&Player1});

  delete _Game;
  return 0;
}

bool UTTT_PossibleGames_0(){
  //std::cout << "Hello World!";
  UTTT_Player Player0 = UTTT_Player(0,'X');
  UTTT_Player Player1 = UTTT_Player(1,'O');
  UTTT *_UTTTGame = new UTTT({&Player0,&Player1});
  //std::list<Game*> Games = _Game->PossibleGames();

  delete _UTTTGame;
  return 0;
}

bool UTTT_RollOut(){
  //std::cout << "Hello World!";
  UTTT_Player Player0 = UTTT_Player(0,'X');
  UTTT_Player Player1 = UTTT_Player(1,'O');
  Game *_Game = new UTTT({&Player0,&Player1});
  _Game->RollOut();
  delete _Game;
  return 0;
}

int main() {

  UTTT_Player* Player0 = new UTTT_Player(0,'X');
  UTTT_Player* Player1 = new UTTT_Player(1,'O');

  UTTT *_UTTTGame = new UTTT({Player0,Player1});

  std::cout << _UTTTGame->Generate_StringRepresentation();

  std::string LogPath = std::string("Test03.json");
  _UTTTGame->Save(LogPath);
  //Pause;



  printf("Reading file\n");
  UTTT* _Game2 = Read_UTTT_JSON(LogPath);


  printf("Freeing  _UTTTGame\n");
 delete _UTTTGame;

  printf("Freeing  Player0\n");
delete Player0;
  printf("Freeing  Player1\n");
delete Player1;
delete _Game2;
}

#endif //UTTT_Tests_CU
