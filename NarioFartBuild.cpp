#include<txlib.h>
namespace graphicsWorker {
    bool createWindow(int height, int width) {
        return txCreateWindow (height, width);
    }

    bool destroyWindow() {
        return txDestroyWindow();
    }

    void drawRectangle(int left, int up, int right, int down, int color) {
        switch(color) {//артефакт
            case 0:
                txSetFillColor(RGB(100, 100, 255));
                txSetColor(RGB(100, 100, 255));
                break;
            case 1:
                txSetFillColor(RGB(0, 200, 0));
                txSetColor(RGB(0, 200, 0));
                break;
        }
        txRectangle(left, up, right, down);
    }
}

class Cube {//прородитель кубиков
    public:
    virtual void draw(int cubeSide, float x, int y) = 0;
};

class AirCube : public Cube {
    private:
    AirCube() {}
    AirCube(const Cube&);//переопределенные конструкторы, создать экземпляр можно только через getInstance()
    AirCube& operator=(Cube&);

    public:
    void draw(int cubeSide, float x, int y) {//переопределенный draw() из Cube
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 0);
    }

    static AirCube* getInstance() {
        static AirCube instance;//синглтон Майерса (инициализация статической переменной происходит только при первом вызове функции)
        return &instance;
    }
};

class DirtCube : public Cube {//тож самое, тока цвет другой
    private:
    DirtCube() {}
    DirtCube(const Cube&);
    DirtCube& operator=(Cube&);

    public:
    void draw(int cubeSide, float x, int y) {
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 1);
    }

    static DirtCube* getInstance() {
        static DirtCube instance;
        return &instance;
    }
};

//class MapFragment {
//    private:
//    static int const fragmentWidth = 50, fragmentHeight = 50;
//    Cube* _cubes[fragmentWidth][fragmentHeight];
//    int deprecatedColor;
//
//    public:
//    MapFragment() {
//        static int Dcolor = 0;
//        Dcolor += 50;
//        deprecatedColor = Dcolor;
//    }
//
//    void draw(int cubeSide, float myNumber, float centerX) {
//        graphicsWorker::drawRectangle(fragmentWidth * cubeSide + fragmentWidth * cubeSide * centerX, 0, myNumber * fragmentWidth * cubeSide  + fragmentWidth * cubeSide * centerX, fragmentHeight * cubeSide, deprecatedColor);
//    }
//};

class Map {
    private:
//    static const int _fragmentsNumber = 3;
//    MapFragment _fragments[_fragmentsNumber];
    public:
    static int const mapWidth = 200, mapHeight = 12;
    private:
    int cubeSide;
    Cube *_cubes[mapWidth][mapHeight];
    public:
    Map(int gameViewHeight) {
        cubeSide = gameViewHeight / mapHeight;
        //initWithFile (в следующей версии будем из файла читать)
        for(int x = 0; x < mapWidth; x++) {
            for(int y = 0; y < mapHeight; y++) {
                if (x < mapWidth - 1 && x > 0 && y < mapHeight - 1) _cubes[x][y] = AirCube::getInstance();
                else _cubes[x][y] = DirtCube::getInstance();
                if (x % 10 == 0 && y == mapHeight - 2) _cubes[x][y] = DirtCube::getInstance();//артефакт
                if (x % 15 == 0 && y == mapHeight - 3) _cubes[x][y] = DirtCube::getInstance();//артефакт
                if (x % 20 == 0 && y == mapHeight - 4) _cubes[x][y] = DirtCube::getInstance();//артефакт
                if (x % 25 == 0 && y == mapHeight - 5) _cubes[x][y] = DirtCube::getInstance();//артефакт
                if (x % 30 == 0 && y == mapHeight - 6) _cubes[x][y] = DirtCube::getInstance();//артефакт
                if (x % 35 == 0 && y == mapHeight - 7) _cubes[x][y] = DirtCube::getInstance();//артефакт
            }
        }
    }

    void draw(float centerX, int gameViewWidth) {
        for(int x = (int)floor(0-centerX); x <= gameViewWidth / cubeSide + (int)floor(0-centerX); x++) {
            if (x == mapWidth) continue;
            for(int y = 0; y < mapHeight; y++) {
                Cube *c = _cubes[x][y];
                c->draw(cubeSide, x + centerX, y);//-> работает, как . , но лучше
            }
        }
    }

    int getCubeSide() {//геттер
        return cubeSide;
    }
};

class Game {
    private:
    int _gameViewWidth, _gameViewHeight, _mobsNumber;
    Map _cubeMap = Map(0);

    public:
    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) {
        _cubeMap = Map(gameViewHeight);
        _gameViewWidth = gameViewWidth;
        _gameViewHeight = gameViewHeight;
    }

    void drawCubes() {
        static float x = 1;//артефакт, в следующей версии будем передавать сюда координату Nарио
        float px = x;
        if(px > 0) px = 0;
        if(px < 0 - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide()) px = 0 - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide();
        _cubeMap.draw(px, _gameViewWidth);
        x-=0.05;
    }

    void drawMobs() {}

    void animateMobs() {}
};

class WinDirector {
    public:
    const int windowHeight = 600, windowWidth = 800;
    private:
        WinDirector() {
            graphicsWorker::createWindow(windowWidth, windowHeight);
        }
        WinDirector(const WinDirector&);
        WinDirector& operator=(WinDirector&);
        Game game = Game(windowWidth, windowHeight, 50);//не знаю, что здесь делает эта строка. потом уберем
    public:
    static WinDirector& getInstance() {
        static WinDirector instance;
        return instance;
    }

    void drawGame() {
        game.drawCubes();
    }
};

int main() {
    while(true) {
        WinDirector::getInstance().drawGame();
    }
}
