#pragma once

namespace Engine {
	class SimpleTree
	{
	public:
		static const int MAX_SIZE = 32;

		int Height = 0;

		SimpleTree();

		void* GetData();
		char m_Blocks[MAX_SIZE][MAX_SIZE][MAX_SIZE];
	private:


	};
}