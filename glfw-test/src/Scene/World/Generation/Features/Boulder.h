#pragma once

namespace Engine {
	class Boulder
	{
	public:
		static const int MAX_SIZE = 64;

		int Height = 0;
		
		Boulder();

		void* GetData();
		char m_Blocks[MAX_SIZE][MAX_SIZE][MAX_SIZE];
	private:
		

	};
}