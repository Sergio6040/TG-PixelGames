#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <math.h>

class FPathFinding : public olc::PixelGameEngine
{
public:
	FPathFinding()
	{
		sAppName = "Pathfinding";
	}

private:
	struct FNode
	{
		bool bObstacle = false;
		bool bVisited = false;
		float GlobalGoal;
		float LocalGoal;
		int X;
		int Y;
		std::vector<FNode*> VectorNeighbours;
		FNode* Parent;
	};

	FNode* Nodes = nullptr;
	int MapWidth = 16;
	int MapHeight = 16;

	FNode* NodeStart = nullptr;
	FNode* NodeEnd = nullptr;

protected:
	virtual bool OnUserCreate()
	{
		Nodes = new FNode[MapWidth * MapHeight];
		for (int i = 0; i < MapWidth; i++)
		{
			for (int j = 0; j < MapHeight; j++)
			{
				Nodes[j * MapWidth + i].X = i;
				Nodes[j * MapWidth + i].Y = j;
				Nodes[j * MapWidth + i].bObstacle = false;
				Nodes[j * MapWidth + i].Parent = nullptr;
				Nodes[j * MapWidth + i].bVisited = false;
			}
		}

		//Set Node Neighbours
		for (int x = 0; x < MapWidth; x++)
		{
			for (int y = 0; y < MapHeight; y++)
			{
				if (y > 0)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y - 1) * MapWidth + (x + 0)]);
				}
				if (y < MapHeight - 1)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y + 1) * MapWidth + (x + 0)]);
				}

				if (x > 0)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y + 0) * MapWidth + (x - 1)]);
				}
				if (x < MapWidth - 1)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y + 0) * MapWidth + (x + 1)]);
				}

				// Diagonall solution
				if (y > 0 && x > 0)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y - 1) * MapWidth + (x - 1)]);
				}
				if (y < MapHeight - 1 && x > 0)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y + 1) * MapWidth + (x - 1)]);
				}
				if (y > 0 && x < MapWidth - 1)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y - 1) * MapWidth + (x + 1)]);
				}
				if (y < MapHeight - 1 && x < MapWidth - 1)
				{
					Nodes[y * MapWidth + x].VectorNeighbours.push_back(&Nodes[(y + 1) * MapWidth + (x + 1)]);
				}
				
			}
		}

		NodeStart = &Nodes[(MapHeight / 2) * MapWidth + 1];
		NodeEnd = &Nodes[(MapHeight / 2) * MapWidth + MapWidth - 2];

		return true;
	}

	bool SolveAStar()
	{
		//Reset the navigation parameters
		for (int x = 0; x < MapWidth; x++)
		{
			for (int y = 0; y < MapHeight; y++)
			{
				Nodes[x * MapWidth + y].bVisited = false;
				Nodes[x * MapWidth + y].GlobalGoal = INFINITY;
				Nodes[x * MapWidth + y].LocalGoal = INFINITY;
				Nodes[x * MapWidth + y].Parent = nullptr;
			}
		}

		//lambdas
		auto Distance = [](FNode* A, FNode* B)
		{
			return sqrtf(pow((A->X - B->X), 2) + pow((A->Y - B->Y), 2));
		};

		auto Heuristic = [Distance](FNode* A, FNode* B)
		{
			return Distance(A, B);
		};

		FNode* CurrentNode = NodeStart;
		NodeStart->LocalGoal = 0.0f;
		NodeStart->GlobalGoal = Heuristic(NodeStart, NodeEnd);

		std::list<FNode*> ListNotTestedNodes;
		ListNotTestedNodes.push_back(NodeStart);

		while (!ListNotTestedNodes.empty() && CurrentNode != NodeEnd)
		{
			ListNotTestedNodes.sort([](const FNode* Lhs, const FNode* Rhs) {return Lhs->GlobalGoal < Rhs->GlobalGoal; });

			while (!ListNotTestedNodes.empty() && ListNotTestedNodes.front()->bVisited)
			{
				ListNotTestedNodes.pop_front();
			}

			if (ListNotTestedNodes.empty())
			{
				break;
			}

			CurrentNode = ListNotTestedNodes.front();
			CurrentNode->bVisited = true;

			for (auto NodeNeighbour : CurrentNode->VectorNeighbours)
			{
				if (!NodeNeighbour->bVisited && !NodeNeighbour->bObstacle)
				{
					ListNotTestedNodes.push_back(NodeNeighbour);
				}

				float LowerGoal = CurrentNode->GlobalGoal + Distance(CurrentNode, NodeNeighbour);

				if (LowerGoal < NodeNeighbour->LocalGoal)
				{
					NodeNeighbour->Parent = CurrentNode;
					NodeNeighbour->LocalGoal = LowerGoal;

					NodeNeighbour->GlobalGoal = NodeNeighbour->LocalGoal + Heuristic(NodeNeighbour, NodeEnd);
				}
			}
		}
		return true;
	}

	virtual bool OnUserUpdate(float fElapsedTime)
	{
		float NodeSize = 9.0f;
		float NodeBorder = 2.0f;

		int SelectedNodeX = GetMouseX() / NodeSize;
		int SelectedNodeY = GetMouseY() / NodeSize;

		if (GetMouse(0).bReleased)
		{
			if (SelectedNodeX >= 0 && SelectedNodeX < MapWidth)
			{
				if (SelectedNodeY >= 0 && SelectedNodeY < MapHeight)
				{
					if (GetKey(olc::SHIFT).bHeld)
					{
						NodeStart = &Nodes[SelectedNodeY * MapWidth + SelectedNodeX];
					}
					else if(GetKey(olc::CTRL).bHeld)
					{
						NodeEnd = &Nodes[SelectedNodeY * MapWidth + SelectedNodeX];
					}
					else
					{
						Nodes[SelectedNodeY * MapWidth + SelectedNodeX].bObstacle = !Nodes[SelectedNodeY * MapWidth + SelectedNodeX].bObstacle;
					}

					SolveAStar();
				}
			}
		}

		Clear(olc::BLACK);

		//Draw Conections
		for (int x = 0; x < MapWidth; x++)
		{
			for (int y = 0; y < MapHeight; y++)
			{
				for (auto Neighbor : Nodes[y * MapWidth + x].VectorNeighbours)
				{
					DrawLine(x * NodeSize + NodeSize / 2, y * NodeSize + NodeSize / 2,
							 Neighbor->X * NodeSize + NodeSize / 2, Neighbor->Y * NodeSize + NodeSize / 2, 
							 olc::BLUE);
				}
			}
		}

		//Draw Nodes
		for (int i = 0; i < MapWidth; i++)
		{
			for (int j = 0; j < MapHeight; j++)
			{
				olc::vf2d Start = { i * NodeSize + NodeBorder, j * NodeSize + NodeBorder };
				olc::vf2d End = { NodeSize - NodeBorder,  NodeSize - NodeBorder };

				FillRect(Start, End, Nodes[j * MapWidth + i].bObstacle ? olc::WHITE : olc::DARK_BLUE);

				if (Nodes[j * MapWidth + i].bVisited)
				{
					FillRect(Start, End, olc::BLUE);

				}
				if (&Nodes[j * MapWidth + i] == NodeStart)
				{
					FillRect(Start, End, olc::GREEN);
				}
				if (&Nodes[j * MapWidth + i] == NodeEnd)
				{
					FillRect(Start, End, olc::RED);
				}
			}
		}

		//Draw Path
		if (NodeEnd != nullptr)
		{
			FNode* NodePtr = NodeEnd;
			while (NodePtr->Parent != nullptr)
			{
				DrawLine(NodePtr->X * NodeSize + NodeSize / 2, NodePtr->Y * NodeSize + NodeSize / 2,
						 NodePtr->Parent->X * NodeSize + NodeSize / 2, NodePtr->Parent->Y * NodeSize + NodeSize / 2, 
						 olc::YELLOW);

				NodePtr = NodePtr->Parent;
			}
		}
		return true;
	}
};


int main()
{
	FPathFinding Game;
	if (Game.Construct(160, 160, 6, 6))
	{
		Game.Start();
	}

	return 0;
}