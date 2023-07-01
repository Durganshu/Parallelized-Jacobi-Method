/**
 * Very simple example strategy:
 * Search all possible positions reachable via one move,
 * and return the move leading to best position
 *
 * (c) 2006, Josef Weidendorfer
 */

#include "search.h"
#include "board.h"
#include "eval.h"
#include <iostream>
#include <algorithm>

/**
 * To create your own search strategy:
 * - copy this file into another one,
 * - change the class name one the name given in constructor,
 * - adjust clone() to return an instance of your class
 * - adjust last line of this file to create a global instance
 *   of your class
 * - adjust the Makefile to include your class in SEARCH_OBJS
 * - implement searchBestMove()
 *
 * Advises for implementation of searchBestMove():
 * - call foundBestMove() when finding a best move since search start
 * - call finishedNode() when finishing evaluation of a tree node
 * - Use _maxDepth for strength level (maximal level searched in tree)
 */
class MinMaxStrategy : public SearchStrategy
{
public:
    // Defines the name of the strategy
    MinMaxStrategy() : SearchStrategy("MinMax") {}

    // Factory method: just return a new instance of this class
    SearchStrategy *clone() { return new MinMaxStrategy(); }

private:
    /**
     * Implementation of the strategy.
     */
    void searchBestMove();

    int MinMax(int currentdepth);
    int MinMaxParallel(int currentdepth, Board &board);
    int _currentDepth;
};

int MinMaxStrategy::MinMax(int currentdepth)//0
{
    
    if (currentdepth >= _maxDepth)
    {
        return evaluate();
    }
    //int eval;
    int max = -999999;
    int min = 999999;
    Move m;
    MoveList list;
    generateMoves(list);

    while(list.getNext(m)){
        int eval;
        playMove(m); 
        if(currentdepth + 1 < _maxDepth){
            eval = -MinMax(currentdepth+1);
        }
        else{
            eval = evaluate();
        }
        takeBack();

        if(eval > max){
            max = eval;
            foundBestMove(currentdepth, m ,eval);

        }
    }
    finishedNode(currentdepth, 0);
    return max;

    // if (_board->actColor() == 1)
    // {
    //     while (list.getNext(m)){
    //         playMove(m);
    //         if(currentdepth + 1 < _maxDepth){
    //             eval = -MinMax(currentdepth+1);
    //         }
    //         else{
    //             eval = evaluate();
    //         }
    //         takeBack();
    //         if(eval > max){
    //             max = eval;
    //             foundBestMove(currentdepth, m, max);
    //         }
    //     }
    //     finishedNode(currentdepth, 0);
    //     return max;
    // }
    // else{
    //     while (list.getNext(m)){
    //         playMove(m);
    //         if(currentdepth + 1 < _maxDepth){
    //             eval = -MinMax(currentdepth+1);
    //         }
    //         else{
    //             eval = evaluate();
    //         }
    //         // min = std::min(min, MinMax(currentdepth + 1));
    //         takeBack();
    //         if(eval > max){
    //             max = eval;
    //             foundBestMove(currentdepth, m ,max);
    //         }
    //     }
    //     finishedNode(currentdepth, 0);
    //     return max;
    // }
    // while (list.getNext(m))
    // {
    //     playMove(m);
    //     if (_board->actColor() == 1)
    //     {
    //         max = std::max(max, MinMax(currentdepth + 1));
    //         foundBestMove(currentdepth, m, max);
    //         finishedNode(currentdepth, 0);
    //         return max;
    //     }
    //     else
    //     {
    //         min = std::min(min, MinMax(currentdepth + 1));
    //         foundBestMove(currentdepth, m, min);
    //         finishedNode(currentdepth, 0);
    //         return min;
    //     }
    //     if ((currentdepth + 1) < _maxDepth)
    //     {
    //         eval = -MinMax(currentdepth + 1);
    //     }
    //     else
    //     {
    //         eval = evaluate();
    //         // std::cout<<"Evaluation: "<<eval<<'\n';
    //     }

    //     takeBack();

    //     if (eval > maxval)
    //     {
    //         maxval = eval;
    //         foundBestMove(currentdepth, m, maxval);
    //         finishedNode(currentdepth, 0);
    //     }
    //     // std::cout<<"MaxEval: "<<maxEval<<'\n';
    // }
    // return maxval;
}

// int MinMaxStrategy::MinMaxParallel(int currentdepth, Board& board)
// {
//     int maxEval = -999999;
//     Move m;

//     MoveList list;
//     // _currentMaxDepth = 1;
//     generateMoves(list);

//     int i =0;
//     if(currentdepth  >= _maxDepth)
//     {
//         return evaluate();
//     }

//     while (list.getNext(m))
//     {
//         #pragma omp task reduction (max: maxEval) firstprivate(m, currentdepth, board, maxEval)
//         {

//             board.playMove(m);
//             int eval;
//             {
//                 if (currentdepth+1 < _maxDepth)
//                 {
//                     eval = -MinMax(currentdepth + 1);
//                 }
//                 else
//                 {
//                     eval = evaluate();
//                 }
//             }
//             takeBack();

//             if (eval > maxEval)
//             {
//                 // std::cout << i++ << "\n";
//                 maxEval = eval;
//                 foundBestMove(currentdepth, m, eval);
//                 finishedNode(_maxDepth, 0);
//                 // if (currentdepth == 0)
//                 // {
//                 //     _currentBestMove = m;
//                 // }
//             }
//         }
//     }

//     return maxEval;

// }

void MinMaxStrategy::searchBestMove()
{
    // we try to maximize bestEvaluation
    int bestEval = minEvaluation();
    int eval;
    _currentDepth = 0;
    eval = MinMax(_currentDepth);
    std::cout <<"Evaluations= " << eval << "\n";
    
}

// register ourselve as a search strategy
MinMaxStrategy MinMaxStrategy;
