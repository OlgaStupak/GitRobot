#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>
#include <cmath>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define MIN_AREA 350

class Camera
{
private:

	VideoCapture cap;

	tuple<Mat, Mat> getting_a_frame()
	{
		Mat frame;
		//получение кадров с устройства (камеры)
		cap.read(frame);

		imshow("Original", frame);

		Mat thsv;
		cvtColor(frame, thsv, COLOR_BGR2HSV);

		return make_tuple(thsv, frame);
	}
public:

	Camera()
	{
		cap.open(0);
		//VideoCapture vid_capture("Resources/Cars.mp4");

		// Print error message if the stream is invalid
		if (!cap.isOpened())
		{
			cout << "Error opening video stream or file" << endl;
		}
		else
		{
			cout<< "It is Okey"<<endl;
		}

	}
	~Camera()
	{
		//закрываем видеофайл
		cap.release();

		//закрываем все окна
		destroyAllWindows();
	}

	tuple<Mat, Mat> toServer()
	{
		return getting_a_frame();
	}

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

class Graffity
{
private:
	bool presence = false; //наличие (присутствие) граффити 
	int xGreen;
	int yGreen;

public:
	Graffity() {}
	Graffity(int x, int y)
	{
		xGreen = x;
		yGreen = y;
		presence = true;
	}

	void positionGraffity(int x, int y, bool presence)//функции установки положения граффити
	{
		if (x < 0 || y < 0)
		{
			presence = false;
			return;
		}
		else
		{
			xGreen = x;
			yGreen = y;
			presence = true;
		}
	}
	int Pos_X() { return presence ? xGreen : -1; }
	int Pos_Y() { return presence ? yGreen : -1; }
	bool graffityExist()
	{
		return presence;
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
	int xRob;
	int yRob;


public:
	//void start();
	//void stop();
	//void clean();

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
		cout << "Cleaning" << endl;
		return 0;
	}

	Robot()
	{
		xRob = 0;
		yRob = 0;
	}

	void setPos(int x, int y)
	{
		xRob = x;
		yRob = y;
	}

	int Pos_X() { return xRob; }
	int Pos_Y() { return yRob; }

};


class Server
{
private:

	Mat frame;
	Mat hsv;
	Camera &camera;
	Graffity graffity;
	Robot robot;
	Mat graffityRes;
	Mat robotRes;

	double angle = 0;

	double detectAngle(int xB, int xR, int yB, int yR)
	{
		if (xB == 0 || yB == 0 || xR == 0 || yR == 0)
			return 0;
		if (xR == xB && yB < yR)
			return 180;
		if (yB - yR == 0)
			return 90;
		//вычисляем угол, если если есть наклон
		double t = (xB * xR + yB * yR) / (sqrt((double)xB * xB + yB * yB) * sqrt((double)xR * xR + yR * yR));
		if (t < -1) t = -1;
		else if (t > 1) t = 1;
		return acos(t);
	}

	Scalar convert_hsv(Scalar hsv)
	{
		// Получаем H S V парметры
		int h = hsv[0];
		int s = hsv[1];
		int v = hsv[2];
		// Переводим H из формата [0, 360] в [2, 180]
		h = static_cast<int>(h * 180 / 360);
		// Переводим S и V из формата [0, 100] в [0, 255]
		s = static_cast<int>(s * 255 / 100);
		v = static_cast<int>(v * 255 / 100);
		// Возвращаем HSV
		return Scalar(h, s, v);
	}

	Scalar L_Blue = convert_hsv(Scalar(110, 50, 50));//нижняя граница задаваемого цвета
	Scalar U_Blue = convert_hsv(Scalar(260, 100, 100));//верхняя граница цвета

	Scalar L_Red = convert_hsv(Scalar(300, 30, 30));
	Scalar U_Red = convert_hsv(Scalar(340, 100, 100));

	Scalar L_Green = convert_hsv(Scalar(50, 40, 40));
	Scalar U_Green = convert_hsv(Scalar(180, 100, 100));

	tuple<int, int> findGraffity()
	{
		Mat Gmask;
		inRange(hsv, L_Green, U_Green, Gmask);

		//Mat mask = Gmask;
		//Mat result;

		//bitwise_and(frame, frame, result, mask);
		graffityRes = Mat::zeros(frame.size(), CV_8UC3);
		frame.copyTo(graffityRes, Gmask);

		imshow("Camera", graffityRes);

		int Gx = 0;
		int Gy = 0;

		vector<vector<Point>> contoursG;
		findContours(Gmask, contoursG, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

		//если найден хотя бы один контур

		if (contoursG.size() > 0)
		{
			auto contourMax = max_element(contoursG.begin(), contoursG.end(), [](auto& a, auto& b)
				{
					return contourArea(a) < contourArea(b);
				});
			if (contourArea(*contourMax) > MIN_AREA)//если контур больше минимальной площади
			{
				Moments M = moments(*contourMax);//ищем центр масс
				if (M.m00 != 0)
				{
					Gx = static_cast<int>(M.m10 / M.m00);
					Gy = static_cast<int>(M.m01 / M.m00);

					circle(graffityRes, Point(Gx, Gy), 7, (0, 0, 255), -1);//рисуем круг в месте центра масс

					return make_tuple(Gx, Gy);
				}
			}
		}
		else
		{
			return make_tuple(-1, -1);
		}
	}

	void AddingObjects(int xB, int yB, int xR, int yR, double angle, bool contoursB, bool contoursR, Graffity graffity)
	{
		if (contoursB)
		{
			circle(robotRes, Point(xB, yB), 7, (0, 255, 255), -1);
		}
		if (contoursR)
		{
			circle(robotRes, Point(xR, yR), 7, (255, 255, 0), -1);
		}
		if (graffity.graffityExist())
		{
			circle(robotRes, Point(graffity.Pos_X(), graffity.Pos_Y()), 7, (0, 0, 255), -1);
		}
		if (contoursB && contoursR)
		{
			// Линия, соединяющая  центр робота с пятном
			line(robotRes, Point(robot.Pos_X(), robot.Pos_Y()), Point(graffity.Pos_X(), graffity.Pos_Y()), Scalar(256, 256, 256), 1);

			// Линия, соединяющая центры масс
			line(robotRes, Point(robot.Pos_X(), robot.Pos_Y()), Point(xR, yR), Scalar(256, 256, 256), 1);
			
			//Выод на экран значения
			putText(robotRes, to_string(angle), Point(robot.Pos_X(), robot.Pos_Y()), FONT_HERSHEY_SIMPLEX, 1, Scalar(256, 256, 256), 2);
		}
	}
	
	void foundRobot(Graffity graffity)
	{
		Mat Bmask, Rmask;
		inRange(hsv, L_Blue, U_Blue, Bmask);
		inRange(hsv, L_Red, U_Red, Rmask);

		Mat mask = Bmask | Rmask;
		//Mat result;

		//bitwise_and(frame, frame, result, mask);
		robotRes = Mat::zeros(frame.size(), CV_8UC3);
		frame.copyTo(graffityRes, mask);

		int xB = 0;
		int yB = 0;
		int xR = 0;
		int yR = 0;

		vector<vector<Point>> contoursB;
		findContours(Bmask, contoursB, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//если найден хотя бы один контур
		if (contoursB.size() > 0)
		{
			auto contourMax = max_element(contoursB.begin(), contoursB.end(), [](auto& a, auto& b)
				{
					return contourArea(a) < contourArea(b);
				});
			if (contourArea(*contourMax) > MIN_AREA)//если контур больше минимальной площади
			{
				Moments M = moments(*contourMax);//ищем центр масс
				if (M.m00 != 0)
				{
					xB = static_cast<int>(M.m10 / M.m00);
					yB = static_cast<int>(M.m01 / M.m00);
					
					//circle(robotRes, Point(xB, yB), 7, (0, 255, 255), -1);//рисуем круг в месте центра масс (синим цветом)
				}
			}
		}

		vector<vector<Point>> contoursR;
		findContours(Rmask, contoursR, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
		//если найден хотя бы один контур
		if (contoursR.size() > 0)
		{
			auto contourMax = max_element(contoursR.begin(), contoursR.end(), [](auto& a, auto& b)
				{
					return contourArea(a) < contourArea(b);
				});
			if (contourArea(*contourMax) > MIN_AREA)//если контур больше минимальной площади
			{
				Moments M = moments(*contourMax);//ищем центр масс
				if (M.m00 != 0)
				{
					xR = static_cast<int>(M.m10 / M.m00);
					yR = static_cast<int>(M.m01 / M.m00);
			
					//circle(robotRes, Point(xR, yR), 7, (255, 255, 0), -1);//рисуем круг в месте центра масс (красным цветом)
				}
			}
		}

		if (xB == 0 || yB == 0 || xR == 0 || yR == 0)
		{
			robot.setPos(0, 0);
			return;
		}

		if (contoursB.size() > 0 && contoursR.size() > 0)
		{
			angle = detectAngle(robot.Pos_X(), graffity.Pos_X(), robot.Pos_Y(), graffity.Pos_Y());
		}

		AddingObjects(xB, yB, xR, yR, angle, contoursB.size() > 0, contoursR.size() > 0, graffity);

		robot.setPos((xB + xR) / 2, (yB + yR) / 2);
		return;
	}

public:
	Server(Camera & cmr):camera(cmr) {}

	void message()
	{
		tie(hsv, frame) = camera.toServer();

		int xGraff, yGraff;
		tie(xGraff, yGraff) = findGraffity();

		if (xGraff != -1 && yGraff != -1)
		{
			graffity.positionGraffity(xGraff, yGraff, true);
			//return 1;
		}
		else
		{
			graffity.positionGraffity(-1, -1, false);
			//return 0;
		}

		foundRobot(graffity);
	}
	
	bool graff()
	{
		tie(hsv, frame) = camera.toServer();

		int xGraff, yGraff;
		//imshow("Original", frame);

		tie(xGraff, yGraff) = findGraffity();

		if (xGraff != -1 && yGraff != -1)
		{
			graffity.positionGraffity(xGraff, yGraff, true);
			return 1;
		}
		else
		{
			graffity.positionGraffity(-1, -1, false);
			return 0;
		}
	}

};

class Control
{

private:
	States state;
	Camera* det;
	Server* st;
	Robot* command;
public:
	Control()
	{
		state = State_Waiting;


		command = new Robot();

		det = new Camera();

		st = new Server(*det);
	}

	void processEvents()
	{
		switch (state)
		{
		case State_Waiting:
			if (st->graff())//если граффити есть 
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
			else if (det->graffity_yes())//если есть граффити, запускается чистка                              
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
	//Server server;
	//server.message();
	Control r;
	r.run();
	return 0;
}