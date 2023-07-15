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
#include <cstring>

class AlphaBetaStrategy : public SearchStrategy {
public:
  AlphaBetaStrategy() : SearchStrategy("AlphaBeta") {}
  SearchStrategy *clone() { return new AlphaBetaStrategy(); }


private:
  void searchBestMove();
  /* recursive alpha/beta search */
  int alphabeta(int depth, int alpha, int beta);
  int alphabeta_parallel(int currentdepth, int alpha, int beta, Board& _board, Evaluator& evaluator);
  int alphabeta_pv_split(int currentdepth, int alpha, int beta , int depthOfPv, int curMaxdepth, Board& board, Evaluator& evaluator);

  Variation _pv; 
  bool _inPv;
  bool _foundBestFromPrev;
  
  int _currentMaxDepth; 
  Move _currentBestMove;
  
  int upperAlphaArray[10];
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
    
  int eval;
  
  _currentMaxDepth = 0;

  _inPv = (_pv[0].type != Move::none);

  int test;

  #pragma omp parallel
  {
     #pragma omp single
      // eval = alphabeta_parallel(_currentMaxDepth, -16000, 16000, *_board, *_ev);
      // eval = alphabeta_pv_split(_currentMaxDepth, -16000, 16000, 0, *_board, *_ev);
      eval = alphabeta_pv_split(0, -16000, 16000, 0, SearchStrategy::_maxDepth, *_board, *_ev);
  }
  
  // eval = alphabeta(_currentMaxDepth, -16000, 16000);
  _bestMove = _currentBestMove; //update _bestmove

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
          { 
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

         #pragma omp critical
        {
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
        }

      }
      if(get_out) break;
    }
    
      
  }
  #pragma omp taskwait 
    // finishedNode(currentdepth, 0);
    return *max;
}

int AlphaBetaStrategy::alphabeta_pv_split(int currentdepth, int alpha, int beta , int depthOfPv, int curMaxdepth, Board& board, Evaluator& evaluator){
    
    int someVal = -999999;
    int* maxEval = &someVal;

    Move m;
    Move nodeBestMove;
    MoveList list;

    board.generateMoves(list); //generate currently possible moves

    bool arePv = !reachedBottom;
    if(arePv)
    {
        upperAlphaArray[currentdepth] = alpha;
        depthOfPv=currentdepth;
    }
        

    if(_inPv){ //check move in pv first to get best moves
        
        m = _pv[currentdepth];

        if(m.type != Move::none && !list.isElement(m,0,true)){ // if pv move is not possible set to null in order to stop pv search and get next from list in loop
            m.type = Move::none;
        }

        if(m.type == Move::none){ //handle cases in which we are too deep already or didn't find pv move in possible list: stop pv
            #pragma omp critical
            {
                _inPv = false;
            }
                
        }
    }


    while(true) { //iterate through each possible move

        if(m.type == Move::none){ //get next move if not yet taken from pv
            if(!list.getNext(m))
                break;
        }

        bool createThread = false;

        if(arePv && reachedBottom && (currentdepth < curMaxdepth - 2)){ //PV-Splitting: Only create threads on PV nodes
            createThread = true;
        }

        if(!createThread) { //handle sequentially to safe on copy operations

            board.playMove(m);
            int eval;
            if (currentdepth + 1 < curMaxdepth) //SearchStrategy::_maxDepth
            {   
              eval = -alphabeta_pv_split(currentdepth + 1, -beta, -alpha , depthOfPv, curMaxdepth, board, evaluator); 
            }
            else
            {
                reachedBottom = true;
                eval = evaluator.calcEvaluation(&board);
            }
            board.takeBack();

            if (eval > *maxEval)
            {
                *maxEval = eval;

                _pv.update(currentdepth, m);
                foundBestMove(currentdepth, m, eval);

                if (currentdepth == 0)
                { //if we are at start of tree set move as nex best
                    _currentBestMove = m;
                }
            }
            if (!arePv)
            {   
                    int upperAlpha = upperAlphaArray[depthOfPv];

                    if ((currentdepth - depthOfPv) % 2 == 0) // TODO ohboi this could be trouble. In case of bugs look here
                    {
                        if (upperAlpha > alpha)
                        {
                            alpha = upperAlpha;
                        }
                    }
                    else
                    {
                        if (-upperAlpha < beta)
                        {
                            beta = -upperAlpha;
                        }
                    }
                
            }

            if (eval > alpha) alpha = eval;

            if (beta <= alpha) break;

        }
        else { //parallel: create tasks on lower depths

            bool breakLoop = false;
            #pragma omp task firstprivate(m, currentdepth, board, maxEval, evaluator, depthOfPv)
            {   

                Move newTaskMList[10];//children write here

                board.playMove(m);
                int eval;
                if (currentdepth + 1 < curMaxdepth)
                {   
                  eval = -alphabeta_pv_split(currentdepth + 1, -beta, -alpha , depthOfPv, curMaxdepth, board, evaluator); 
                }
                else
                {
                    eval = evaluator.calcEvaluation(&board);
                }

                board.takeBack();

                #pragma omp critical
                {
                  if (eval > *maxEval)
                  {
                      *maxEval = eval;

                      //safe m -> do own variation pv?
                      newTaskMList[currentdepth] = m;
                      foundBestMove(currentdepth, m, eval);

                      //copy everything from TaskMList to mlist
                      for(int d = currentdepth; d < curMaxdepth; d++){
                        _pv.update(currentdepth, newTaskMList[d]);
                      }

                      if (currentdepth == 0)
                      { //if we are at start of tree set move as nex best
                          _currentBestMove = m;
                      }
                  }

                  if (eval > alpha)
                  {
                      alpha = eval;
                      if (arePv)
                      {
                          upperAlphaArray[depthOfPv] = eval;
                      }
                  }

                  if (beta <= alpha)
                  {
                      breakLoop = true;
                  }
                }
        

                
                if (!arePv)
                {   

                    int upperAlpha = upperAlphaArray[depthOfPv];

                    if ((currentdepth - depthOfPv) % 2 == 0) 
                    {
                        if (upperAlpha > alpha)
                        {
                            alpha = upperAlpha;
                        }
                    }
                    else
                    {
                        if (-upperAlpha < beta)
                        {
                            beta = -upperAlpha;
                        }
                    }
                }

            }

            if(breakLoop)
                break;

        }

        m.type = Move::none;
    }
    
    #pragma omp taskwait 
    return *maxEval;
}


// register ourselve
AlphaBetaStrategy alphabetaStrategy;
