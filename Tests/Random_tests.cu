#ifndef Random_Tests
#define TTT_Tests_CU

#include "../SRC/TTT.cu"

#include <list>
/*
Game *f = new TTT();

  GM = new TTT_Move(2,2);
  f->Move(GM);
  std::cout << f->GenerateStringRepresentation();
  delete GM;



f->DisplayWinner();
*/


/*
std::list<GameMove*>GameMoves = f->PossibleMoves();
Free_TTTList(GameMoves);
*/

int main() {
  std::list<Int>;
  std::list<GameMove*>;

  for (Game* Instance : PossibleInstances){
      if(Instance != NULL)
      {
        NewNode = new MCTS_Node(Instance);
        NewNode->Parent = this;
        Children.push_back(NewNode);
        ChildrenAdded++;
      }
  }
 return 0;
}

#endif //TTT_Tests_CU

/*


 */
