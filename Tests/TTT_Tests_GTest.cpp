#ifndef TTT_Tests_CU
#define TTT_Tests_CU

#include "../SRC/TTT.cu"

#include <list>
#include <stdlib.h>
#include <gtest/gtest.h>
/*
Game *f = new TTT();

  GM = new TTT_Move(2,2);
  f->Move(GM);
  std::cout << f->GenerateStringRepresentation();
  delete GM;

f->DisplayWinner();
*/

bool Validate_TTTGame(TTT*Given_TTT){
  Given_TTT->_Players.size();
  return 0;
}


/*
std::list<GameMove*>GameMoves = f->PossibleMoves();
Free_TTTList(GameMoves);
*/
TEST(TTT, Init)
    {
      TTT_Player Player0 = TTT_Player(0,'X');
      TTT_Player Player1 = TTT_Player(1,'O');
      Game *_Game = new TTT({&Player0,&Player1});
      TTT* TTT_Game = static_cast<TTT*>(_Game);

      //EXPECT_NE(NULL, TTT_Game->_Players);
      ASSERT_EQ(2,    TTT_Game->_Players.size());

      _Game->RollOut();
      _Game->TestForWinner();
       delete _Game;
    }

TEST(TTT, PossibleGames)
    {
      TTT_Player Player0 = TTT_Player(0,'X');
      TTT_Player Player1 = TTT_Player(1,'O');
      Game *_Game = new TTT({&Player0,&Player1});
      std::list<Game*> _Games = _Game->PossibleGames();

      for (Game* i : _Games) { // c++11 range-based for loop
          TTT* iTTT_Game = static_cast<TTT*>(i);
          ASSERT_EQ(2, iTTT_Game->_Players.size());

        }
      delete _Game;
    }


int main(int argc, char **argv)
    {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }


 /*
 GameMove* GM = new TTT_Move(2,2);
 f->Move(GM);
 std::cout << f->GenerateStringRepresentation();
 delete GM;
 */


#endif //TTT_Tests_CU

/*


 */
