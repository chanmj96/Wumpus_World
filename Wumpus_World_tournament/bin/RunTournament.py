# Author: Vincent Ho
# Author: Abdullah Younis
# CS 199: Spring 2016

import os
import sys
from WumpusWorldGameState import *

aiFolder = sys.argv[1];
worldFolder = sys.argv[2];

AIs = [AI[:-4] for AI in os.listdir(aiFolder) if (AI[-4:] == ".pyc" and (("Ai" in AI) or ("AI" in AI)))]
AIs.extend([AI[:-3] for AI in os.listdir(aiFolder) if (AI[-3:] == ".py" and (("Ai" in AI) or ("AI" in AI)))])
worldFiles = [os.path.join(worldFolder, file) for file in os.listdir(worldFolder) if (file.endswith(".txt") and True)]

AIs_dict = {}
for AI in AIs:
    AIs_dict[AI] = 0

if not os.path.isfile("Tournament_Results.txt"):
    f = open("Tournament_Results.txt", "w")
    f.close()

with open('Tournament_Results.txt', 'a') as results:
    sys.path.append( os.path.abspath(aiFolder) )
    for AI in AIs:
        for i in worldFiles:
            with open(i) as f:
                dim = int(next(f))
                wumpus = next(f)
                gold = next(f)
                wx = int(wumpus.split('\t')[0])
                wy = int(wumpus.split('\t')[1])
                gx = int(gold.split('\t')[0])
                gy = int(gold.split('\t')[1])
                numPits = int(next(f))
                pits = []
                for line in f:
                    pits.append( (int(line.split('\t')[0]), int(line.split('\t')[1])) )
                    
                try:
                    AI_i = __import__(AI)
                    AIo = AI_i.Agent()
                    GameObj = WumpusGameState(dim, wx, wy, gx, gy, pits, AIo)
                    GameObj.start()
                    AIScore = GameObj.score
                    AIs_dict[AI] += AIScore
                    del AI_i
                except:
                    print(sys.exc_info())
                    sys.exit(1)
    
    for k, v in AIs_dict.items():
        results.write("{}: {}\n".format(k, str(v/len(worldFiles))))
    sys.path.remove( os.path.abspath(aiFolder) )