 // MyAI.hpp

#ifndef MYAI_LOCK
#define MYAI_LOCK

#include "Agent.hpp"
#include <stack>
#include <map>
#include <queue>
#include <cmath>
#include <math.h>

namespace wumpus_project
{
	class MyAI : public Agent
	{
	public:
		MyAI ( void );
		
		  // Returns true if a given percept is @ current position; otherwise false
		bool    checkPerceptIn	( Percept p );						
		virtual Action getAction ( void );
		virtual void perceiveResult( std::set<Percept> percepts );
		void 	updateHazards();
		void 	updateBounds();
		bool	in_KB(std::pair<int,int> p);
		bool 	in_Bounds(std::pair<int,int> p);
		double 	distance(std::pair<int,int> dest);
		std::vector< Agent::Action > uniform_cost_search(std::pair<int,int> goal);
		void 	find_and_shoot();

	private:
		std::set<Agent::Percept> 	ps;	
		std::map<std::pair<int,int>,std::set<Agent::Percept> > KB;
		std::set<std::pair<int,int> > poss_pits;
		std::set<std::pair<int,int> > poss_wumpus;
		std::set<std::pair<int,int> > stenches;

		std::set<std::pair<int,int> > unexplored;
		std::vector<Agent::Action> next_moves;
		int max_row;
		int max_col;
		
	};
}

#endif /* MYAI_LOCK */