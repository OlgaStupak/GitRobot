#include <iostream>
#include <cmath>

using namespace std;

enum States //возможные состояния
{
	State_Waiting, State_Moving,
	State_Rotate, State_MoveForvard,
	State_Clean 
};
//States state = State_Waiting; //начальное состояние

class Robot
{
	States state;
	Camera* det;
	Control* command;

public:
	Robot()
	{
		state = State_Waiting;
	}
	void processEvents()
	{
		switch (state)
		{
		case State_Waiting:
			if (det -> there_is_graffiti(bool))//если граффити есть (не знаю на счет переменной, просто может если есть 1, нет 0)
			{
				state = State_Moving;//переходим в состояние движения
			}
			break;
		
		case State_Moving://мы должны как-то получить этот угол, а дальше сравнивать с искомым
			if (det -> detectAngle(5))//проверяем угол: если наш угол больше заданного
			{
				state = State_Rotate;//переходим в состояние поворота
			}
			else if (det -> detectDistance(5))//проверяем расстояние
			{
				state = State_MoveForvard;
			}
			else if (det -> graffity_yes(bool))//если есть граффити, запускается чистка                              ->
			{
				state = State_Clean;
			}
			/*else if (det->no_graffity(bool))//если нет граффити, возвращаемся в состояние ожидания
			{
				state = State_Waiting;
			}*/
			break;
		
		case State_Rotate:
			command->rotate(5);
			state = State_Moving;
			break;
		
		case State_MoveForvard:
			command->moving(10);
			state = State_Moving;
			break;
		
		case State_Clean:
			command->cleaning();
			state = State_Waiting;
			break;
		}
	}
};
void run()
{
	while (1)
	{
		processEvents();
	}
}
