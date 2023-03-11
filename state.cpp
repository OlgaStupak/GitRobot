#include <iostream>
#include <cmath>

using namespace std;

enum States //��������� ���������
{
	State_Waiting, State_Moving,
	State_Rotate, State_MoveForvard,
	State_Clean 
};
//States state = State_Waiting; //��������� ���������

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
			if (det -> there_is_graffiti(bool))//���� �������� ���� (�� ���� �� ���� ����������, ������ ����� ���� ���� 1, ��� 0)
			{
				state = State_Moving;//��������� � ��������� ��������
			}
			break;
		
		case State_Moving://�� ������ ���-�� �������� ���� ����, � ������ ���������� � �������
			if (det -> detectAngle(5))//��������� ����: ���� ��� ���� ������ ���������
			{
				state = State_Rotate;//��������� � ��������� ��������
			}
			else if (det -> detectDistance(5))//��������� ����������
			{
				state = State_MoveForvard;
			}
			else if (det -> graffity_yes(bool))//���� ���� ��������, ����������� ������                              ->
			{
				state = State_Clean;
			}
			/*else if (det->no_graffity(bool))//���� ��� ��������, ������������ � ��������� ��������
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
