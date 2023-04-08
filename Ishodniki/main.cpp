#include <iostream>
#include <cmath>
using namespace std;

class Server
{
public:
	void Rob();
	void Graf();
	void Marshrut();
};

class Control
{ 
public:
	//void rigth();
	//void left();
	//void back();
	//void straight();
	int rotate(float angle)
	{
		return 0;
	}
	int moving(float distance)
	{
		return 0;
	}
	int cleaning()
	{
		return 0;
	}
};

class Graffity
{
private:
	int x1;
	int x2;
};

class Camera
{
public:
	//void Rob(Robot* p);
	//void Graf(Graffity* q);
	bool there_is_graffiti()
	{
		return 0;
	}
	bool detectAngle(float angle)
	{
		return 0;
	}
	bool detectDistance(float distance)
	{
		return 0;
	}
	bool graffity_yes()
	{
		return 0;
	}

};

enum States //возможные состояния
{
	State_Waiting, State_Moving,
	State_Rotate, State_MoveForvard,
	State_Clean
};

class Robot
{
private:
	int x;
	int y;
	States state;
	Camera* det;
	Control* command;

public:
	void start();
	void stop();
	void clean();

	Robot()
	{
		state = State_Waiting;
	}
	void processEvents()
	{
		switch (state)
		{
		case State_Waiting:
			if (det->there_is_graffiti())//если граффити есть 
			{
				state = State_Moving;//переходим в состояние движения
			}
			break;

		case State_Moving:
			if (det->detectAngle(5))//проверяем угол
			{
				state = State_Rotate;//переходим в состояние поворота
			}
			else if (det->detectDistance(5))//проверяем расстояние
			{
				state = State_MoveForvard;
			}
			else if (det->graffity_yes())//если есть граффити, запускается чистка                              ->
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
	void run()
	{
		while (1)
		{
			processEvents();
		}
	}

};

int main()
{
	setlocale(LC_ALL, "Russian");
	Robot r;
	r.run();
	return 0;
}