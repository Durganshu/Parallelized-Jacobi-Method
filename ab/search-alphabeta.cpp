/**
 * A real world, sequential strategy:
 * Alpha/Beta with Iterative Deepening (ABID)
 *
 * (c) 2005, Josef Weidendorfer
 */

#include <stdio.h>
#include <iostream> 
#include "search.h"
#include "board.h"
#include "eval.h"

class AlphaBetaStrategy : public SearchStrategy {
public:
  AlphaBetaStrategy() : SearchStrategy("AlphaBeta") {}
  SearchStrategy *clone() { return new AlphaBetaStrategy(); }


private:
  void searchBestMove();
  /* recursive alpha/beta search */
  int alphabeta(int depth, int alpha, int beta);
  int alphabeta_parallel(int currentdepth, int alpha, int beta, Board& _board, Evaluator& evaluator);

  int _currentMaxDepth;
  Move _currentBestMove;
  omp_lock_t lockArray[10];
  bool reachedBottom=false;
};

/**
 * Entry point for search
 *
 * Does iterative deepening and alpha/beta width handling, and
 * calls alpha/beta search
 */
void AlphaBetaStrategy::searchBestMove() {
  
  reachedBottom=false;
  for (int i=0; i<10; i++)
        omp_init_lock(&(lockArray[i]));
    
  int eval;
  
  _currentMaxDepth = 0;

  #pragma omp parallel
  {
     #pragma omp single
      eval = alphabeta_parallel(_currentMaxDepth, -16000, 16000, *_board, *_ev);
  }
  
  // eval = alphabeta(_currentMaxDepth, -16000, 16000);
  _bestMove = _currentBestMove; //update _bestmove

  for (int i=0; i<10; i++)
      omp_destroy_lock(&(lockArray[i]));
}

/*
 * Alpha/Beta search
 *
 * - first, start with principal variation
 * - depending on depth, we only do depth search for some move types
 */
int AlphaBetaStrategy::alphabeta(int currentdepth, int alpha, int beta) {

  if (currentdepth >= _maxDepth)
    {
        return evaluate();
    }
    //int eval;
    int max = -999999;
    Move m;
    MoveList list;
    generateMoves(list);

    while(list.getNext(m)){
        int eval;
        playMove(m); 
        if(currentdepth + 1 < _maxDepth){
            eval = -alphabeta(currentdepth+1, -beta, -alpha);
        }
        else{
            eval = evaluate();
        }
        takeBack();

        if(eval > max){
            max = eval;
            foundBestMove(currentdepth, m ,eval);

            if (currentdepth == 0) _currentBestMove = m;

        }

        //alpha beta pruning
        if (eval > alpha)
        {
            alpha = eval;
        }

        if (beta <= alpha)
        {
            break;
        }
    }
    finishedNode(currentdepth, 0);
    return max;
}

int AlphaBetaStrategy::alphabeta_parallel(int currentdepth, int alpha, int beta, Board& board, Evaluator& evaluator) {

  int someVal = -999999;
  int* max = &someVal;
  
  Move m;
  MoveList list;
  board.generateMoves(list);

  while(list.getNext(m)){

    bool createThread = false;

    if(reachedBottom && (currentdepth < _maxDepth - 2)){ //PV-Splitting: Only create threads on PV nodes
            createThread = true;
    }

    if(!createThread) 
    { //handle sequentially to safe on copy operations

      board.playMove(m);
      int eval;
      if (currentdepth + 1 < _maxDepth) //SearchStrategy::_maxDepth
      {
          eval = -alphabeta_parallel(currentdepth + 1, -beta, -alpha, board, evaluator); //call for the enemy here, so change sign to get the best move for us
      }
      else
      {
          reachedBottom = true;
          eval = evaluator.calcEvaluation(&board);
      }
      
      board.takeBack();

      if (eval > *max)
      {
          *max = eval;
        
          foundBestMove(currentdepth, m, eval);
          
          if (currentdepth == 0)
          { //if we are at start of tree set move as nex best
              _currentBestMove = m;
          }
      }      
      if (eval > alpha) alpha = eval;
      
      if (beta <= alpha) break;

    }

    else
    {
      bool get_out = false;
      #pragma omp task firstprivate(m, currentdepth, max, board, evaluator)
      {
        int eval;
        board.playMove(m); 

        if(currentdepth + 1 < _maxDepth){
          eval = -alphabeta_parallel(currentdepth+1, -beta, -alpha, board, evaluator);
        }

        else{
          eval = evaluator.calcEvaluation(&board);
        }

        board.takeBack();

        // #pragma omp critical
        omp_set_lock(&(lockArray[currentdepth]));
        
        if(eval > *max){
          // std::cout << currentdepth << " " << omp_get_thread_num() << "\n";
          *max = eval;
          foundBestMove(currentdepth, m ,eval);
          if (currentdepth == 0) _currentBestMove = m;
        }
        //alpha beta pruning
        if (eval > alpha)
        {
          alpha = eval;
        }
        if (beta <= alpha)
        {
          get_out = true;
        }
        
        omp_unset_lock(&(lockArray[currentdepth]));

      }
      if(get_out) break;
    }
    
      
  }
  #pragma omp taskwait 
    // finishedNode(currentdepth, 0);
    return *max;
}

// register ourselve
AlphaBetaStrategy alphabetaStrategy;
