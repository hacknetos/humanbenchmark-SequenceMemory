#include <iostream>
#include <Windows.h>
#include <thread>
#include <vector>

bool program_running = true;

POINT P_TOPLEFT;
POINT P_BOTRIGHT;

std::vector<std::pair<double,double>> V_squers;
std::vector<std::pair<double, double>> V_clickOrder;

bool b_canGO = false;
bool b_No_clickting = true;
//Backgrundcolor = (43, 135, 209)
const COLORREF C_backgrundColor = 13731627; //Normaler Hinter grund Kann ignoriert werden
const COLORREF C_normalSquare = 12677925; // Normaler Squerare
const COLORREF C_marktSquare = 16777215; // Markierter Squerare 

bool search = true, building = true;




void WaitSquerRegiester() {
	short anzahl_runns = 0;
	POINT mous_pos;
	while (program_running)
	{
		
		if (GetKeyState('S') & 0x8000)
		{
			GetCursorPos(&mous_pos);

			//Get Infos of the Squer
			if (anzahl_runns > 1)
			{
				std::cout << "the check ranges from " << P_TOPLEFT.x << "," << P_TOPLEFT.y << " To " << P_BOTRIGHT.x << "," << P_BOTRIGHT.y << std::endl;
				Sleep(500);
				continue;
			}
			
			//Set The Bottem RIGHT Corner of the Squer
			if (anzahl_runns == 1)
			{
				
				P_BOTRIGHT = mous_pos;
				std::cout << "second point corresponds to" << P_BOTRIGHT.x << " , " << P_BOTRIGHT.y << std::endl;
				anzahl_runns++;
				Sleep(500);
				continue;
			}
			
			//Set The Top Left Corner Of the Squer
			if (anzahl_runns == 0)
			{
				P_TOPLEFT = mous_pos;
				std::cout << "first point corresponds to" << P_TOPLEFT.x << " , " << P_TOPLEFT.y << std::endl;
				anzahl_runns++;
				Sleep(500);
				continue;
			}

			
		}
		if (GetKeyState('I') & 0x8000)
		{
			HDC display = GetDC(NULL);
			POINT info_mouse_pos;
			GetCursorPos(&info_mouse_pos);
			COLORREF color = GetPixel(display, info_mouse_pos.x, info_mouse_pos.y);
			std::cout << "Color: " << color << std::endl;
			Sleep(500);
		}
		if (GetKeyState('M') & 0x8000)
		{
			if (V_squers.size() > 0) {
				SetCursorPos(V_squers.at(0).first, V_squers.at(0).second);
				POINT info_mouse_pos;
				GetCursorPos(&info_mouse_pos);
				std::cout << "Mous is Movet ot " << info_mouse_pos.x << " " << info_mouse_pos.y <<
				"\nGaven Indo was x: "<< (int)V_squers.at(0).first <<" y: " << (int)V_squers.at(0).second << std::endl;
				Sleep(500);
			}
		}

	}
}
//Press Q To Quit The Programm 
void WaitForQuitClick() {
	while (program_running)
	{
		if (GetKeyState('Q') & 0x8000)
		{
			program_running = false;
		}
	}
}
void Squermessung(int& height, int& width,int& distance,int foundPosX, int foundPosY) {
	HDC display = GetDC(NULL);
	COLORREF color = GetPixel(display, foundPosX, foundPosY);
	int counter = 1;
	std::cout << "Start mess width\n";
	//Bewege dich Pixel nach Rechts bis wieder back grund da ist
	while (color == C_normalSquare && program_running)
	{
		color = GetPixel(display, foundPosX+counter, foundPosY);
		if (color == C_backgrundColor)
		{
			break;
		}
		counter++;
	}
	width = counter;
	counter = 1;
	color = C_normalSquare;

	std::cout << "Start mess height\n";

	//jetzt bewege dich nach unten
	while (color == C_normalSquare && program_running)
	{
		color = GetPixel(display, foundPosX + counter, foundPosY+counter);
		if (color == C_backgrundColor)
		{
			break;
		}
		counter++;
	}

	height = counter;
	counter = 1;
	
	std::cout << "Start mess distance\n";

	while (color != C_normalSquare && program_running)
	{
		color = GetPixel(display, foundPosX + width + counter, foundPosY);
		if (color == C_normalSquare)
		{
			break;
		}
		counter++;
	}
	distance = counter - 1;
	ReleaseDC(NULL, display);
}

void CheackMySquer(int x, int y) {
	COLORREF lastColor = C_normalSquare;
	HDC display = GetDC(NULL);
	while (program_running)
	{
		COLORREF color = GetPixel(display, x, y);
		if (color != lastColor && b_No_clickting)
		{
			lastColor = color;

			if (color == C_marktSquare)
			{
				lastColor == C_marktSquare;
				V_clickOrder.push_back(std::pair<double, double>(x, y));
				b_canGO = false;
				std::cout << "a Markt Square\n";

			}
			else if(color == C_normalSquare && color != lastColor)
			{
				std::cout << "a Normal Square\n";
				lastColor = C_normalSquare;
				b_canGO = false;
				
			}
			else {
				lastColor = color;
				b_canGO = false;
			}
			
		}
		Sleep(10);
	}
	ReleaseDC(NULL, display);
};



void CheackFild() {
	
	std::vector<std::thread> V_threads;


	int firstfundX, FirstfundY = 0;
	
	HDC display = GetDC(NULL);
	
	int height = 0, width = 0, distance = 0;

	

	while (program_running)
	{
		while (building)
		{

		
			if (P_BOTRIGHT.y != NULL && program_running)
			{
				int counter = 0;
				while (search)
				{
					for (int x = P_TOPLEFT.x; x < P_TOPLEFT.x+(P_TOPLEFT.x*-1 - P_BOTRIGHT.x*-1)/4; x++)
					{
						if (!program_running)
						{
							break;
						}

						COLORREF color = GetPixel(display, x, P_TOPLEFT.y + counter);
						//std::cout << color << " = " << C_normalSquare << "\n";

						if (color == C_normalSquare)
						{
							std::cout << "Fund start of an squer POS: " << x << "," << P_TOPLEFT.y + counter << std::endl;
							Squermessung(height, width, distance, x, P_TOPLEFT.y + counter);
							std::cout << "der Squer ist Hoch: " << height << " und hat eine Breite von: " << width << " der Abstand zwichen den sqers ist: " << distance << std::endl;
							search = false;
							firstfundX = x;
							FirstfundY = P_TOPLEFT.y + counter;
							break;
						}
					}

					if (!program_running)
					{
						break;
					}
					counter++;

					
				}
				int anzahl_squers_on_x = (P_TOPLEFT.x*-1 - P_BOTRIGHT.x*-1) / width;
				int anzahl_squers_on_y = (P_BOTRIGHT.y - P_TOPLEFT.y) / height;
				std::cout << "die anzahl an squers ist: " << anzahl_squers_on_x * anzahl_squers_on_y << "\n";
				
				if (!program_running)
				{
					break;
				}

				
				int last_cord_x = firstfundX + (width / 2);
				int last_cord_y = FirstfundY + (height / 2);
				for (int y = 0; y < anzahl_squers_on_y; y++)
				{
					last_cord_x = firstfundX + (width / 2);

					for (int x = 0; x < anzahl_squers_on_x; x++)
					{
						if (!program_running)
						{
							break;
						}
						V_squers.push_back(std::pair<double, double>(last_cord_x, last_cord_y));
						last_cord_x = last_cord_x + width + distance;
						std::cout << "Derzeige cord x>> " << last_cord_x << "\n";
						std::cout << "Derzeige cord y>> " << last_cord_y << "\n";

					}
					if (!program_running)
					{
						break;
					}
					last_cord_y = last_cord_y + height + distance;

				}


				for(int i = 0;i<V_squers.size();i++)
				{
					std::cout << "x: " << V_squers.at(i).first << " y: " << V_squers.at(i).second << "\n";
				}
				
				building = false;
				//Starte Die Überwachung
				for (int i = 0; i < V_squers.size(); i++)
				{
					V_threads.push_back(std::move(std::thread(CheackMySquer, V_squers.at(i).first, V_squers.at(i).second)));
				}
				Sleep(2000);
			}
		}
	}
	ReleaseDC(NULL, display);

	for (int i = 0; i < V_threads.size(); i++)
	{
		V_threads.at(i).join();
	}
}

void StartClicks() {
	while (program_running)
	{
		Sleep(500);
		if (b_canGO && V_clickOrder.size() > 0) {
			b_No_clickting = false;
			std::cout << "Start Clicking\n";
			for (int i = 0; i < V_clickOrder.size(); i++)
			{
				
				SetCursorPos(V_clickOrder.at(i).first, V_clickOrder.at(i).second);
				Sleep(50);
				mouse_event(MOUSEEVENTF_LEFTDOWN, 0, 0, 0, 0);
				mouse_event(MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
			}
			V_clickOrder.clear();
			b_No_clickting = true;
			std::cout << "\nFinished Clicking\n";
		}
		else
		{
			Sleep(500);
			b_canGO = true;
		}
	}
}

int main()
{
	//Start The Threads 
	std::thread t_posSaver(WaitSquerRegiester);
	std::thread t_quitWaiter(WaitForQuitClick);
	std::thread t_FieldBuilder(CheackFild);
	std::thread t_startClick(StartClicks);
	while (program_running)
	{
		
	}
	t_posSaver.join();
	t_quitWaiter.join();
	t_FieldBuilder.join();
	t_startClick.join();
}

