#include <iostream>
#include <Windows.h>
#include <thread>


bool program_running = true;

POINT P_TOPLEFT;
POINT P_BOTRIGHT;

//Backgrundcolor = (43, 135, 209)
const COLORREF C_backgrundColor = 13731627; //Normaler Hinter grund Kann ignoriert werden
const COLORREF C_normalSquare = 13731627; // Normaler Squerare
const COLORREF C_marktSquare = 16777215; // Markierter Squerare 


void WaitSquerRegiester() {
	short anzahl_runns = 0;
	POINT mous_pos;
	while (program_running)
	{
		if (GetKeyState('S') & 0x8000)
		{
			GetCursorPos(&mous_pos);
			std::cout << "Die aktuelle Position ist X: " << mous_pos.x << " Y: " << mous_pos.y << std::endl;
			HDC dc = GetDC(NULL);
			if (anzahl_runns == 0)
			{
				P_TOPLEFT = mous_pos;
				anzahl_runns++;
			}
			if (anzahl_runns == 1)
			{
				P_BOTRIGHT = mous_pos;
				anzahl_runns++;
			}
			if (anzahl_runns > 1)
			{
				std::cout << "the check ranges from "<<P_TOPLEFT.x <<","<< P_TOPLEFT.y<< " To " << P_BOTRIGHT.x << "," << P_BOTRIGHT.y << std::endl;
			}
			Sleep(500);
		}
		if (GetKeyState('R')& 0x8000)
		{
			P_TOPLEFT.x = NULL;
			P_TOPLEFT.y = NULL;

			P_BOTRIGHT.x = NULL;
			P_BOTRIGHT.y = NULL;
			
			anzahl_runns = 0;
		}
	}
}
void WaitForQuitClick() {
	while (program_running)
	{
		if (GetKeyState('Q') & 0x8000)
		{
			program_running = false;
		}
	}
}
int main()
{
	std::thread t_posSaver(WaitSquerRegiester);
	std::thread t_quitWaiter(WaitForQuitClick);
	while (program_running)
	{

	}
	t_posSaver.join();
	t_quitWaiter.join();
}

