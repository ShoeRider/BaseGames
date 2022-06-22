#ifndef TTT_Tests_CU
#define TTT_Tests_CU

#include "../SRC/TTT.cpp"

#include <list>
#include <stdlib.h>



/*

*/

int main() {
  std::srand(15);
  //std::cout << "Hello World!";
  TTT_Player* Player0 = new TTT_Player(0,'X');
  TTT_Player* Player1 = new TTT_Player(1,'O');

  TTT *_Game = new TTT({Player0,Player1});
  //f->PlayGame();
  //_Game->RollOut();

  printf("Final Hash: %zu\n",Hash(_Game));

  //TTT_Move* Move = Player0->MakeMove(_Game);

  TTT_Move* TTTMove = new TTT_Move(2,2);
  _Game->Move(TTTMove);
  printf("Freeing  TTTMove\n");
  delete TTTMove;
  printf("Final Hash: %zu\n",Hash(_Game));
  std::cout << _Game->Generate_StringRepresentation();
  //Hash(_Game);



  std::string LogPath = std::string("Test02.json");
  //_Game->Save(LogPath);
  Pause;

  printf("Reading file\n");
  TTT* _Game2 = Read_TTT_JSON(LogPath);
  std::cout << _Game2->Generate_StringRepresentation();
  //_Game2->Read(LogPath);



 /*
 GameMove* GM = new TTT_Move(2,2);
 f->Move(GM);
 std::cout << f->GenerateStringRepresentation();
 delete GM;
 */

//RollOut()

 //_Game->TestForWinner();
 //_Game->DisplayWinner();


   printf("Freeing  _Game\n");
  delete _Game;
    printf("Freeing  _Game2\n");
  delete _Game2;
    printf("Freeing  Player0\n");
  delete Player0;
    printf("Freeing  Player1\n");
  delete Player1;
 return 0;
}

#endif //TTT_Tests_CU

/*


 */
