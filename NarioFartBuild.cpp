//TODO Gen map with init file; synchronize map movement with Nario's; create frames generation for
//TODO Nario's movement, esp jump;

#include <txlib.h>
#include <conio.h>

#define if_DEBUG

//Global constants
const COLORREF RED = RGB(128,   0,   0);
const COLORREF SKY_BLUE = RGB(100, 100, 255);
const COLORREF GRASS_GREEN = RGB(  0, 200,   0);

namespace graphicsWorker {
    bool createWindow(int height, int width) {
        return txCreateWindow (height, width);
    }

    bool destroyWindow() {
        return txDestroyWindow();
    }

    void drawRectangle(int left, int up, int right, int down, int color) {
        switch(color) {
            case 0:
                txSetFillColor (  SKY_BLUE);
                txSetColor     (  SKY_BLUE);
                break;
            case 1:
                txSetFillColor(GRASS_GREEN);
                txSetColor    (GRASS_GREEN);
                break;
            case 2:
                txSetFillColor(        RED);
                txSetColor    (        RED);
                break;
        }

        txRectangle(left, up, right, down);
    }

    COLORREF getPixel(int x, int y) {
        COLORREF color = txGetPixel(x, y);
        return color;
    }

    void freezeWindow(int time) {
        txSleep(time);
    }
}

class Nario {
    private:
    static const int narioHeight =  40, narioWidth = 20, narioXSpeed = 10, narioYSpeed = 20;

    public:
    int narioXo = 65, narioYo = 0;
    int narioXCenter = narioXo, narioYCenter;

    Nario(int cubeSide) {
        narioYo = cubeSide * 11 - narioHeight / 2;
        narioYCenter = narioYo;
    }

    void draw() {
            graphicsWorker::drawRectangle(narioXCenter - narioWidth / 2 , narioYCenter - narioHeight / 2, narioXCenter + narioWidth / 2, narioYCenter + narioHeight / 2, 2);
    }

    bool checkIfOnGround(int y) {
        if(graphicsWorker::getPixel(narioXCenter, y) == GRASS_GREEN)
            return true;
        else
            return false;
    }

    bool checkIfWallLeft() {
        int flag = 0;
        for(int y = narioYCenter - narioHeight / 2; y <= narioYCenter + narioHeight / 2; y ++) {
            assert(narioYCenter - narioHeight / 2 <= y && y <= narioYCenter + narioHeight / 2);

            for(int x = narioXCenter - narioXSpeed - narioWidth / 2; x < narioXCenter - narioWidth / 2; x ++) {
                assert( narioXCenter - narioXSpeed - narioWidth / 2 <= x && x < narioXCenter - narioWidth / 2);

                if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                    flag = 1;
                    break;
                }
            }
        }
        return flag;
    }

    bool checkIfWallRight() {
        int flag = 0;
        for(int y = narioYCenter - narioHeight / 2; y <= narioYCenter + narioHeight / 2; y ++) {
            assert(narioYCenter - narioHeight / 2 <= y && y <= narioYCenter + narioHeight / 2);

            for(int x = narioXCenter + narioXSpeed + narioWidth / 2; x > narioXCenter + narioWidth / 2; x --) {
                assert( narioXCenter + narioXSpeed + narioWidth / 2 >= x && x > narioXCenter + narioWidth / 2);

                if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                    flag = 1;
                    break;
                }
            }
        }
        return flag;
    }

    void move() {
        if(checkIfOnGround(narioYCenter + narioHeight / 2 + 1)) {
            if(GetAsyncKeyState(VK_UP)) {
                narioYCenter -= narioYSpeed * 3;
                for(int y = narioYCenter; y <= narioYo + narioHeight / 2; y ++) {
                    assert(narioYCenter <= y && y <= narioYo + narioHeight / 2);

                    if(checkIfOnGround(y + narioHeight / 2)) {
                        narioYCenter = y;
                        break;
                    }
                }
            }
        }

        if(GetAsyncKeyState(VK_LEFT) && (!checkIfWallLeft())) {
            if(checkIfOnGround(narioYCenter + narioHeight / 2 + 1))
                if(GetAsyncKeyState(VK_UP))
                    narioYCenter -= narioYSpeed * 3;

            narioXCenter -= narioXSpeed;
        }
        else if (GetAsyncKeyState (VK_LEFT) && checkIfWallLeft()) {
            if(checkIfOnGround(narioYCenter + narioHeight / 2 + 1))
                if(GetAsyncKeyState(VK_UP))
                    narioYCenter -= narioYSpeed * 3;

            for (int x = narioXCenter - narioWidth / 2 - narioXSpeed + 1; x < narioXCenter - narioWidth / 2; x ++) {
                assert (narioXCenter - narioWidth / 2 - narioXSpeed + 1 <= x && x < narioXCenter - narioWidth / 2);

                if (graphicsWorker::getPixel (x, narioYCenter) != GRASS_GREEN) {
                    narioXCenter = x + narioWidth / 2;
                    break;
                }
            }
        }

        if(GetAsyncKeyState(VK_RIGHT) && (!checkIfWallRight())) {
            if(checkIfOnGround(narioYCenter + narioHeight / 2 + 1))
                if(GetAsyncKeyState(VK_UP))
                    narioYCenter -= narioYSpeed * 3;

            narioXCenter += narioXSpeed;

        }
        else if (GetAsyncKeyState (VK_RIGHT) && checkIfWallRight()) {
            if(checkIfOnGround(narioYCenter + narioHeight / 2 + 1))
                if(GetAsyncKeyState(VK_UP))
                    narioYCenter -= narioYSpeed * 3;

            for (int x = narioXCenter + narioWidth / 2 + narioXSpeed - 1; x > narioXCenter + narioWidth / 2; x --) {
                assert (narioXCenter + narioWidth / 2 + narioXSpeed - 1 >= x && x > narioXCenter + narioWidth / 2);

                if (graphicsWorker::getPixel (x, narioYCenter) != GRASS_GREEN) {
                    narioXCenter = x - narioWidth / 2;
                    break;
                }
            }
        }

        for(int y = narioYCenter; y <= narioYo + narioHeight / 2; y ++) {
            assert(narioYCenter <= y && y <= narioYo + narioHeight / 2);

            if(checkIfOnGround(y + narioHeight / 2)) {
                narioYCenter = y;
                break;
            }
        }
    }
};

// Cubes base class
class Cube {
    public:
    virtual void draw(int cubeSide, float x, int y) = 0;
};

// Singleton class derived from Cube
class AirCube : public Cube {
    private:
    AirCube() {}

    AirCube(const Cube&) = delete;

    AirCube& operator=(const Cube&) = delete;

    public:
    void draw(int cubeSide, float x, int y) {
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 0);
    }

    static AirCube* getInstance() {
        static AirCube instance;
        return &instance;
    }
};

// Singleton class derived from Cube
class DirtCube : public Cube {
    private:
    DirtCube() {}

    DirtCube(const Cube&) = delete;

    DirtCube& operator=(const Cube&) = delete;

    public:
    void draw(int cubeSide, float x, int y) {
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 1);
    }

    static DirtCube* getInstance() {
        static DirtCube instance;
        return &instance;
    }
};

//{ class MapFragment
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
//}

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
        //initWithFile (read from file in next version)
        for(int x = 0; x < mapWidth; x++) {
            assert(0 <= x && x < mapWidth);

            for(int y = 0; y < mapHeight; y++) {
                assert(0 <= y && y < mapHeight);

                if (x < mapWidth - 1 && x > 0 && y < mapHeight - 1) _cubes[x][y] = AirCube::getInstance();
                else _cubes[x][y] = DirtCube::getInstance();
                if (x % 10 == 0 && y == mapHeight - 2) _cubes[x][y] = DirtCube::getInstance();
                if (x % 15 == 0 && y == mapHeight - 3) _cubes[x][y] = DirtCube::getInstance();
                if (x % 20 == 0 && y == mapHeight - 4) _cubes[x][y] = DirtCube::getInstance();
                if (x % 25 == 0 && y == mapHeight - 5) _cubes[x][y] = DirtCube::getInstance();
                if (x % 30 == 0 && y == mapHeight - 6) _cubes[x][y] = DirtCube::getInstance();
                if (x % 35 == 0 && y == mapHeight - 7) _cubes[x][y] = DirtCube::getInstance();
            }
        }
    }

    void draw(float centerX, int gameViewWidth) {
        for(int x = (int)floor(- centerX); x <= gameViewWidth / cubeSide + (int)floor(- centerX); x++) {
            assert((int)floor(- centerX) <= x && x <= gameViewWidth / cubeSide + (int)floor(- centerX));

            if (x == mapWidth) continue;
            for(int y = 0; y < mapHeight; y++) {
                assert(0 <= y && y < mapHeight);

                Cube *c = _cubes[x][y];
                c->draw(cubeSide, x + centerX, y);
            }
        }
    }

    int getCubeSide() {
        return cubeSide;
    }
};

class Game {
    private:
    int _gameViewWidth, _gameViewHeight, _mobsNumber;
    Map _cubeMap = Map(0);
    Nario nario = Nario(0);

    public:
    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) {
        _cubeMap = Map(gameViewHeight);
        nario = Nario(_cubeMap.getCubeSide());
        _gameViewWidth = gameViewWidth;
        _gameViewHeight = gameViewHeight;
    }

    void drawCubes() {
        static float x = 1;
        float px = x;
        if(px > 0) px = 0;
        if(px < - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide()) px = - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide();
        _cubeMap.draw(px, _gameViewWidth);
//        x -= 0.05;
    }

    void drawnario() {
        nario.draw();
    }

    void movenario() {
        nario.move();
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

        WinDirector(const WinDirector&) = delete;

        WinDirector& operator=(const WinDirector&) = delete;

        Game game = Game(windowWidth, windowHeight, 50);

    public:
    static WinDirector& getInstance() {
        static WinDirector instance;
        return instance;
    }

    void drawGame() {
        game.drawCubes();
        game.drawnario();
    }

    void moveObjs() {
        game.movenario();
    }
};

int main() {
    while(true) {
        WinDirector::getInstance().drawGame();
        if(_kbhit())
            WinDirector::getInstance().moveObjs();
        graphicsWorker::freezeWindow(100);
    }
}
