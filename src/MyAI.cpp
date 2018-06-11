 // MyAI.cpp

 // scp -r /Users/mattchan/Documents/wum*/*cpp_shell/src/ chanmj2@odin.ics.uci.edu:/home/chanmj2/*cpp_shell/
#include "MyAI.hpp"

namespace wumpus_project
{
	MyAI::MyAI() : Agent()
	{
		// Class Attributes
		notShot = true;
		gold = false;
		lastAction = CLIMB;   
		facing = std::pair<int, int>(0,1);
		position = std::pair<int, int>(0,0);
		
		max_row = 100;
		max_col = 100;

		unexplored.insert(position);

		// ==================== IMPORTANT ==================== //
		// Fill these in with your name and student ID number. //
		idNumber = "82541550";		// 8 digit ID as a string
		author = "Chan, Matthew";		// Your name in that format

	}
		
	bool MyAI::checkPerceptIn(Percept p)
	{
		const bool is_in = ps.find(p) != ps.end();
		return is_in;
	}

	std::set<std::pair<int,int> > adjacentTiles(std::pair<int,int> tile)
	{
		std::set<std::pair<int,int> > ans;
		int r = tile.first;
		int c = tile.second;
		ans.insert(std::pair<int,int>(r+1,c));
		ans.insert(std::pair<int,int>(r-1,c));
		ans.insert(std::pair<int,int>(r,c+1));
		ans.insert(std::pair<int,int>(r,c-1));
		return ans;
	}

	bool contains(std::set<std::pair<int,int> > s, std::pair<int,int> p)
	{
		return s.find(p) != s.end();
	}

	bool MyAI::in_KB(std::pair<int,int> p)
	{
		return KB.find(p) != KB.end(); 
	}

	bool MyAI::in_Bounds(std::pair<int,int> p)
	{
		return p.first >= 0 && p.first < max_row && p.second >= 0 && p.second < max_col;
	}

	void MyAI::updateBounds()
	{
		if(checkPerceptIn(BUMP))
		{
			if(facing.first > 0)
			{
				max_row = facing.first + position.first;
			}
			else if(facing.second > 0)
			{
				max_col = facing.second + position.second;
			}
			for(auto it = unexplored.begin(); it != unexplored.end(); ++it)
			{
				if((*it).first >= max_row || (*it).second >= max_col)
					unexplored.erase(it);
			}
		}
	}

	double MyAI::distance(std::pair<int,int> dest)
	{
		return std::sqrt( std::pow((dest.first - position.first),2) + std::pow((dest.second - position.second),2) );
	}

	int find_turn(std::pair<int,int> dir1, std::pair<int,int> dir2)  //number of left_turns to get the second direction
	{
		// std::cout << dir1.first << "," << dir1.second << "\t" << dir2.first << "," <<dir2.second <<"\n";
		if(dir1.first == dir2.first && dir1.second == dir2.second)
			return 0;
		if((dir1.first + dir2.first == 0) && (dir1.second + dir2.second == 0))   //
			return 2;
		if((dir2.first-dir1.first)*(dir2.second+dir1.second) > 0)
			return 1;
		return 3;
	}

	void MyAI::find_and_shoot()
	{
		std::set<std::pair<int,int> > safe_tiles;
		bool pit = false;
		for(auto it = poss_wumpus.begin(); it != poss_wumpus.end(); ++it)
		{
			int adjacent = 0;
			int breezes = 0;
			for(auto iter = stenches.begin(); iter != stenches.end(); ++iter )
			{
				if(contains(adjacentTiles(*iter), (*it)))
				{
					adjacent++;
				}
				if(KB[*iter].find(BREEZE) != KB[*iter].end())
				{
					breezes++;
				}
			}
			if(adjacent < stenches.size())
				safe_tiles.insert(*it);
			if(breezes == stenches.size())
			{
				pit = true;
			}
		}
		for(auto safe = safe_tiles.begin(); safe != safe_tiles.end(); ++safe)
		{
			poss_wumpus.erase(*safe);
			if(!in_KB(*safe) && !contains(poss_pits, (*safe)))
				unexplored.insert(*safe);
		}
		//std::cout << poss_wumpus.size() << std::endl;

		if(poss_wumpus.size() == 1 && !pit)
		{
			int num_turns = find_turn(position, *(poss_wumpus.begin()));
			if(num_turns == 3)
			{
				next_moves.push_back(TURN_LEFT);
			}
			else
			{
				for(int i = 0; i < num_turns; i++)
					next_moves.push_back(TURN_RIGHT);
			}
			next_moves.push_back(SHOOT);
		}
	}

	void MyAI::updateHazards()
	{
		if(checkPerceptIn(BREEZE))
		{
			auto m = adjacentTiles(position);
			for(auto it = m.begin(); it != m.end(); ++it)
			{
				if(in_Bounds(*it) && !in_KB(*it))
				{
					auto n = adjacentTiles((*it));
					bool safe = false;
					for(auto iter = n.begin(); iter != n.end(); ++iter)
					{
						if(in_KB(*iter) && KB[(*iter)].find(BREEZE) == KB[(*iter)].end())
							safe = true;
					}
					if(!safe)
					{
						poss_pits.insert(*it);
						if(contains(unexplored, (*it)))
							unexplored.erase((*it));
					}
				}
			}
		}
		else
		{
			auto m = adjacentTiles(position);
			for(auto it = m.begin(); it != m.end(); ++it)
			{
				auto tile = (*it);
				auto x = poss_pits.find(tile);
				if(x != poss_pits.end())
				{
					poss_pits.erase(x);
					if(!contains(poss_wumpus, tile) && in_Bounds(tile) && !in_KB(tile))
						unexplored.insert(tile);
				}
			}
		}

		if(checkPerceptIn(STENCH))
		{
			stenches.insert(position);
			auto m = adjacentTiles(position);
			for(auto it = m.begin(); it != m.end(); ++it)
			{
				if(in_Bounds(*it) && !in_KB(*it))	// code for ensuring adjacent tile to STENCH is safe
				{
					auto n = adjacentTiles((*it));
					bool safe = false;
					for(auto iter = n.begin(); iter != n.end(); ++iter)
					{
						if(in_KB(*iter) && KB[(*iter)].find(STENCH) == KB[(*iter)].end())
							safe = true;
					}
					if(!safe)
					{
						poss_wumpus.insert(*it);
						if(contains(unexplored, (*it)))
							unexplored.erase((*it));
					}
				}
			}
			if(stenches.size() >= 2 && poss_wumpus.size() >= 1)
				find_and_shoot();
		}
		else
		{
			auto n = adjacentTiles(position);
			for(auto it = n.begin(); it != n.end(); ++it)
			{
				auto tile = (*it);
				auto y = poss_wumpus.find(tile);
				if(y != poss_wumpus.end())
				{
					poss_wumpus.erase(y);
					if(!contains(poss_pits, tile) && in_Bounds(tile) && !in_KB(tile))
						unexplored.insert(tile);
				}
			}
		}
	}

	struct Node
	{
		std::vector< std::pair<int,int> > path;
		std::vector<Agent::Action> commands;
		std::pair<int,int> dir;
		int cost;
		bool operator>(const Node& rhs) const
		{
			return cost > rhs.cost;
		}
	};
	typedef struct Node Node;
	

	struct Comparator
	{
		bool operator()(Node& n1, Node& n2)
		{
		return (n1>n2);
		}
	};

	std::vector< Agent::Action > MyAI::uniform_cost_search(std::pair<int,int> goal)
	{
		std::priority_queue< Node, std::vector<Node>, Comparator> PQ;
		Node* start = new Node;
		start->path.push_back(position);  start->dir = facing;  start->cost = 0;
		PQ.push(*start);
		while(!PQ.empty())
		{
		  	Node n = PQ.top();
		  	std::pair<int,int> search = n.path.back();
		  	PQ.pop();
			if(search.first == goal.first && search.second == goal.second)
			{
				// for(int i = 0; i < n.path.size(); i++)
				// 	std::cout << n.path[i].first <<","<< n.path[i].second << std::endl;
				// for(int i = 0; i < n.commands.size(); i++)
				// 	std::cout << n.commands[i] << std::endl;
				return n.commands;
			}
			else
			{
				// for(auto it = poss_pits.begin(); it != poss_pits.end(); ++it)
					// std::cout << (*it).first << "," << (*it).second << std::endl;
				auto m = adjacentTiles(search);
				for(auto it = m.begin(); it != m.end(); ++it)
				{
					if(in_KB(*it) && !contains(poss_pits, (*it)) && !contains(poss_wumpus, (*it)))
					{
						Node* node = new Node;
						node->commands = n.commands;
						node->path = n.path;
						node->path.push_back(*it);  
						node->dir = std::make_pair((*it).first-search.first, (*it).second-search.second);   


						int num_turns = find_turn(n.dir, node->dir);
						if(num_turns == 3)
						{
							node->commands.push_back(TURN_LEFT);
							num_turns = 1;
						}
						else
						{
							for(int i = 0; i < num_turns; i++)
								node->commands.push_back(TURN_RIGHT);
						}
						node->commands.push_back(FORWARD);
						node->cost = n.cost+1+num_turns;
						PQ.push(*node);
					}
				}
			}
		}
	}

	void MyAI::perceiveResult ( std::set<Agent::Percept> percepts )
	{
		ps = percepts;
	}

	Agent::Action MyAI::getAction()   // take shortest path back
	{	
		// std::cout << "\t" << facing.first << "," << facing.second << "\n";
		// for(auto it = unexplored.begin(); it != unexplored.end(); ++it)
			// std::cout << (*it).first << "," << (*it).second << std::endl;
		if (checkPerceptIn(GLITTER))
		{
			gold = true;
			next_moves = uniform_cost_search(std::make_pair(0,0));
			next_moves.push_back(CLIMB);
			return GRAB;
		}
		if(!next_moves.empty())
		{
			Agent::Action a = next_moves[0];			
			next_moves.erase(next_moves.begin());
			return a;
		}

		if(lastAction == SHOOT)
		{
			if(checkPerceptIn(SCREAM))
				unexplored.insert(std::make_pair(position.first+facing.first, position.second+facing.second));
		}

		Agent::Action act;
		KB[position] = ps;
		updateBounds();

		// std::cout << "***" << std::endl;
		if(contains(unexplored,position))
		{
			if(!checkPerceptIn(BREEZE) && !checkPerceptIn(STENCH))
			{
				// std::cout << "&&&" << std::endl;
				auto m = adjacentTiles(position);
				for(auto it = m.begin(); it != m.end(); ++it)
				{
					auto tile = (*it);
					if(in_Bounds(tile) && !in_KB(tile))
					{
						// std::cout << "XXXX" << std::endl;
						unexplored.insert(tile);
					}
				}
			}
			updateHazards();
			unexplored.erase(position);
			if(!next_moves.empty())
			{
				act = next_moves[0];
				next_moves.erase(next_moves.begin());
				return act;
			}
		}

		if(lastAction == CLIMB && checkPerceptIn(STENCH))
		{
			return SHOOT;
		}

		if(unexplored.empty())
		{
			// std::cout << "++++++" << std::endl;
			next_moves = uniform_cost_search(std::make_pair(0,0));
			next_moves.push_back(CLIMB);
			act = next_moves[0];
			next_moves.erase(next_moves.begin());
			return act;
		}

		// std::cout << "@@@@" << std::endl;

		// for(auto it = unexplored.begin(); it != unexplored.end(); ++it)
			// std::cout << "\t"<<(*it).first << "," << (*it).second << std::endl;

		std::pair<int,int> closest = position;			
		double dist = 100.0;
		for(auto it = unexplored.begin(); it != unexplored.end(); ++it)			//go to next closest unexplored tile
		{
			auto tile = (*it);
			std::pair<int,int> front(position.first + facing.first, position.second + facing.second);
			if(front.first == tile.first && front.second == tile.second)
			{
				act = FORWARD;
				return act;
			}
			else if( distance(tile) < dist )
			{
				dist = distance(tile);
				closest = tile;
			}
		}

		// std::cout << "$$$$" << std::endl;

		KB[closest] = std::set<Agent::Percept>();
		next_moves = uniform_cost_search(closest);
		act = next_moves[0];
		next_moves.erase(next_moves.begin());
		return act;
	}
}
	