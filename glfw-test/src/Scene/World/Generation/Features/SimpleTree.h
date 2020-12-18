#pragma once

namespace Engine {
	class SimpleTree
	{
	public:
		static const int MAX_SIZE = 32;

		int Height = 0;

		SimpleTree();

		void GenerateLeafBall(int x, int y, int z, int leafType);

		void* GetData();
		char m_Blocks[MAX_SIZE][MAX_SIZE][MAX_SIZE];
	private:


	};
}