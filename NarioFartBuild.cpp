#include<txlib.h>
namespace graphicsWorker {
    bool createWindow(int height, int width) {
        return txCreateWindow (height, width);
    }

    bool destroyWindow() {
        return txDestroyWindow();
    }

    void drawRectangle(int left, int up, int right, int down, int color) {
        txSetFillColor(color);
        txRectangle(left, up, right, down);
    }
}

class Cube {
    public:
    virtual void draw(int cubeSide, int x, int y) = 0;
};

class AirCube : public Cube {
    private:
    AirCube() {}
    AirCube(const Cube&);
    AirCube& operator=(Cube&);

    public:
    void draw(int cubeSide, int x, int y) {
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 100);
    }

    static AirCube* getInstance() {
        static AirCube instance;
        return &instance;
    }
};

class DirtCube : public Cube {
    private:
    DirtCube() {}
    DirtCube(const Cube&);
    DirtCube& operator=(Cube&);

    public:
    void draw(int cubeSide, int x, int y) {}

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
    static int const mapWidth = 15, mapHeight = 10;
    int cubeSide;
    Cube *_cubes[mapWidth][mapHeight];
    public:
    Map(int gameViewHeight) {
        cubeSide = gameViewHeight / mapHeight;
        //initWithFile
        for(int x = 0; x < mapWidth; x++) {
            for(int y = 0; y < mapHeight; y++) {
                _cubes[x][y] = AirCube::getInstance();
            }
        }
    }

    void draw(float centerX) {
        for(int x = 0; x < mapWidth; x++) {
            for(int y = 0; y < mapHeight; y++) {
                Cube *c = _cubes[x][y];
                c->draw(cubeSide, x + centerX, y);
            }
        }
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
        _cubeMap.draw(0);
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
        Game game = Game(windowWidth, windowHeight, 50);
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
        Sleep(2);
    }
}
