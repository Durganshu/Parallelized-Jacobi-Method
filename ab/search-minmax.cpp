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
    int _currentDepth;
};

int MinMaxStrategy::MinMax(int currentdepth)
{
    int maxEval = -15000, min = 15000;
    Move m;

    MoveList list;
    // _currentMaxDepth = 1;
    generateMoves(list);
    // int i =0;
    while (list.getNext(m))
    {
        // std::cout << i++ << "\n";
        playMove(m);
        int eval;
        if (currentdepth+1 < _maxDepth)
        {
            eval = -MinMax(currentdepth + 1);
        }
        else
        {
            eval = evaluate();
        }
        takeBack();

        if (eval > maxEval)
        {
            maxEval = eval;
            foundBestMove(currentdepth, m, eval);
            // if (currentdepth == 0)
            // {
            //     _currentBestMove = m;
            // }
        }

    }


}

void MinMaxStrategy::searchBestMove()
{
    // we try to maximize bestEvaluation
    int bestEval = minEvaluation();
    int eval;
    _currentDepth = 0;
    MinMax(_currentDepth);
    finishedNode(_maxDepth, 0);
}

// register ourselve as a search strategy
MinMaxStrategy MinMaxStrategy;
