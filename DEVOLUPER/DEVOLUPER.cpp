#include "stdafx.h"
#include "iostream"
#include "conio.h"
#include "time.h"
#include "windows.h"
#include "vector"

using namespace std;

void searchPath();
void initWaveMap();
void initPathMap();
void printWave();
void printPath();
void startGame();
void printGame();
void writeCODE();
void checkControl();
void checkCollision(int x, int y);
void movePlayer(char step);

COORD position;
HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	
struct Player
{
	int x;
	int y;
	double live;
};

struct Cod
{
	int x;
	int y;
};

const int SPACE = 0;
const int WALL = 1;
const int CODE = 2;
const int PLAY = 3;
const int PATH = 4;

bool isGameOver;
Player player;
Cod cod;

const int dx[] = { 0, 1, 0, -1 };
const int dy[] = { -1, 0, 1, 0 };

double score = 0;
const int W = 15;
const int H = 15;

int mapWave[W][H];
int map[W][H] =	{ {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
				  {1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 1},
				  {1, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1},
				  {1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1},
				  {1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 1, 1},
				  {1, 0, 1, 0, 1, 0, 1, 1, 0, 1, 1, 1, 0, 1, 1},
				  {1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 1, 1},
				  {1, 0, 1, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 1},
				  {1, 0, 0, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 1},
				  {1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 1},
				  {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1},
				  {1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1},
				  {1, 0, 1, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1},
				  {1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1},
				  {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1} };


vector<pair<int, int>> path;

int main()
{
	srand(time(NULL));

	player.x = 1;
	player.y = 1;
	player.live = 100;

	writeCODE();

	isGameOver = false;
	startGame();

    return 0;
}

void startGame()
{
	while (!isGameOver)
	{
		initWaveMap();
		searchPath();
		initPathMap();
		printGame();
		checkControl();
	}
}

void printGame()
{
	position.X = 0;
	position.Y = 0;
	SetConsoleCursorPosition(hConsole, position);
	
	map[player.y][player.x] = PLAY;

	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			switch (map[i][j])
			{
			case SPACE: cout << "   "; break;
			case WALL: cout << "[=]"; break;
			case CODE: cout << "COD"; break;
			case PLAY: cout << "C++"; break;
			case PATH: cout << " . "; break;
			default: cout << "WaT"; break;
			}
		}
		cout << endl;
	}
	cout << score << endl;
}

void movePlayer(char step)
{	
	int x = player.x;
	int y = player.y;

	map[player.y][player.x] = 0;
	switch (step)
	{
	case 'U': y -= 1; break;
	case 'D': y += 1; break;
	case 'L': x -= 1; break;
	case 'R': x += 1; break;
	}

	checkCollision(x, y);
}

void checkControl()
{
	switch (_getch())
	{
	case 72: movePlayer('U'); break;//UP
	case 80: movePlayer('D'); break;//DOWN
	case 75: movePlayer('L'); break;//LEFT
	case 77: movePlayer('R'); break;//RIGHT
	case 113: isGameOver = true; break;//QUIT
	}
}

void checkCollision(int x, int y)
{
	switch (map[y][x])
	{
	case WALL: x = player.x, y = player.y; break;
	case CODE: score += 10; writeCODE(); break;
	}

	player.x = x;
	player.y = y;
}

void writeCODE()
{
	do
	{
		cod.x = rand() % (W - 3) + 1;
		cod.y = rand() % (H - 3) + 1;
	} while (map[cod.y][cod.x] == WALL || map[cod.y][cod.x] == PLAY);

	map[cod.y][cod.x] = CODE;
}

void searchPath()
{
	int nstep = 2;
	vector<pair<int, int>> wave;
	vector<pair<int, int>> oldWave;

	oldWave.push_back(pair<int, int>(player.x, player.y));
	
	while (oldWave.size() > 0)
	{
		wave.clear();
		for (vector<pair<int, int>> :: iterator i = oldWave.begin(); i != oldWave.end(); ++i)
		{
			for (int d = 0; d < 4; ++d)
			{
				int nx = i->first + dx[d];
				int ny = i->second + dy[d];

				if (mapWave[ny][nx] == SPACE)
				{
					wave.push_back(pair<int, int>(nx, ny));
					mapWave[ny][nx] = nstep;

					if (nx == cod.x && ny == cod.y)
						goto done;
				}
			}
		}
		oldWave = wave;
		nstep++;
	}

done:
	
	int x = cod.x;
	int y = cod.y;
	path.clear();
	
	while (mapWave[y][x] != 2)
	{
		for (int d = 0; d < 4; ++d)
		{
			int nx = x + dx[d];
			int ny = y + dy[d];

			if (mapWave[y][x] - mapWave[ny][nx] == 1)
			{
				x = nx;
				y = ny;
				path.push_back(pair<int, int>(x, y));
				break;
			}
		}
	}
}

void initWaveMap()
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			if (map[i][j] == WALL)
				mapWave[i][j] = map[i][j];
			else
				mapWave[i][j] = SPACE;
		}
		cout << endl;
	}
}

void initPathMap()
{
	for (int i = 0; i < H; i++)
		for (int j = 0; j < W; j++)
			if (map[i][j] == PATH)
				map[i][j] = SPACE;

	for (int i = 0; i < path.size(); i++)
	{
		pair<int, int> xy = path[i];
		map[xy.second][xy.first] = PATH;
	}
	cout << endl;
}

void printWave()
{
	for (int i = 0; i < H; i++)
	{
		for (int j = 0; j < W; j++)
		{
			mapWave[i][j] >= 10 ? cout << " " << mapWave[i][j] : cout << " " << mapWave[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void printPath()
{
	for (int i = 0; i < path.size(); i++)
	{
		pair<int, int> xy = path[i];
		cout << i << " X = " << xy.first << " : Y = " << xy.second << endl;
	}
	cout << endl;
}
