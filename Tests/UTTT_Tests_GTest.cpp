#ifndef UTTT_Tests_CU
#define UTTT_Tests_CU

#include "../SRC/TTT.cu"
#include "../SRC/UTTT.cu"

#include <gtest/gtest.h>


TEST(UTTT, Init)
    {
      UTTT_Player Player0 = UTTT_Player(0,'X');
      UTTT_Player Player1 = UTTT_Player(1,'O');
      Game *_Game = new UTTT({&Player0,&Player1});
      UTTT* UTTT_Game = static_cast<UTTT*>(_Game);

      //EXPECT_NE(NULL, TTT_Game->_Players);
      ASSERT_EQ(2, UTTT_Game->_Players.size());

      UTTT_Game->RollOut();
      UTTT_Game->TestForWinner();
       delete UTTT_Game;
    }

TEST(UTTT, PossibleGames)
    {
      UTTT_Player Player0 = UTTT_Player(0,'X');
      UTTT_Player Player1 = UTTT_Player(1,'O');
      Game *_Game = new UTTT({&Player0,&Player1});
      UTTT* UTTT_Game = static_cast<UTTT*>(_Game);
      std::list<Game*> _Games = UTTT_Game->PossibleGames();

      for (Game* i : _Games) { // c++11 range-based for loop
          UTTT* iUTTT_Game = static_cast<UTTT*>(i);
          ASSERT_EQ(2, iUTTT_Game->_Players.size());
        delete iUTTT_Game;
        }
      delete UTTT_Game;
    }




TEST(UTTT, UTTT_RollOut)
    {
      UTTT_Player Player0 = UTTT_Player(0,'X');
      UTTT_Player Player1 = UTTT_Player(1,'O');
      Game *_Game = new UTTT({&Player0,&Player1});
      _Game->RollOut();
      delete _Game;
    }



int main(int argc, char **argv)
    {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
    }

#endif //UTTT_Tests_CU
