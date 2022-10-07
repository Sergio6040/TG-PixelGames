#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"

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

protected:
	bool OnUserCreate() override
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
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		float NodeSize = 9.0f;
		float NodeBorder = 2;

		Clear(olc::BLACK);

		for (int i = 0; i < MapWidth; i++)
		{
			for (int j = 0; j < MapHeight; j++)
			{
				olc::vf2d Start = { i * NodeSize + NodeBorder, j * NodeSize + NodeBorder };
				olc::vf2d End = { (i + 1) * NodeSize - NodeBorder, (j + 1) * NodeSize - NodeBorder};
				FillRect(Start, End, olc::BLUE);
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