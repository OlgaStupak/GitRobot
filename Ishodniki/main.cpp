#include <iostream>
#include <cmath>
#include <stdio.h>
#include <stdlib.h>
//#include <unistd.h>
#include <thread>
#include <mosquitto.h>
#include <string.h>
//#include <json-c/json.h> //сделать клон с гитхаба?

#define NUM_MESSAGES    30
#define THREAD          10
#define MQTT_SERVER     "localhost"
#define KEEP_ALIVE      60
#define MQTT_PORT       1883
#define MQTT_PUB_TOPIC  "/funmqqt"
#define MQTT_QOS_LEVEL  2
#define MSG_MAX_SIZE    512

#define mqtt_host "192.168.1.4"
#define mqtt_port 1883

#pragma comment(lib, "C:/Users/Olga/source/repos/OlgaStupak/Ishodniki/lib/mosquitto/build/lib/Debug/mosquitto.lib")
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
	//Control();
	//virtual void left();
	//virtual void rigt();
	//virtual void forward();

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

class Control_mqtt: public Control
{
	bool clean_session = true;
	struct mosquitto* mosq = NULL;


public:

	Control_mqtt()
	{
		mosq = mosquitto_new(NULL, clean_session, NULL);
	}

	~Control_mqtt()
	{
		mosquitto_destroy(mosq);
	}

	void connect() 
	{
		mosquitto_connect(mosq, mqtt_host, mqtt_port, KEEP_ALIVE);
	}

	void sendtoserver(const char* data)
	{
		mosquitto_publish(mosq, NULL, MQTT_PUB_TOPIC, strlen(data), data, 0, 0);
	}

	void left()
	{
		sendtoserver("{\"cmd\":\"left\", \"val\":0.1, \"spd\":0.3}");
	}
		
	void right()
	{
		sendtoserver("{\"cmd\":\"right\", \"val\":0.1, \"spd\":0.3}");
	}
	void forward()
	{
		sendtoserver("{\"cmd\":\"forward\", \"val\":0.1, \"spd\":0.3}");
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