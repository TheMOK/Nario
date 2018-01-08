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
                txSetFillColor(SKY_BLUE);
                txSetColor    (SKY_BLUE);
                break;
            case 1:
                txSetFillColor(GRASS_GREEN);
                txSetColor    (GRASS_GREEN);
                break;
            case 2:
                txSetFillColor(RED);
                txSetColor    (RED);
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

class Mario {
    private:
    static const int marioHeight =  40, marioWidth = 20, marioXSpeed = 10, marioYSpeed = 30, MARIO_COLOR = 2;
    int marioXCenter = 65, marioYCenter = 0;

    public:
    Mario(int cubeSide) {
        marioYCenter = cubeSide * 11 - marioHeight / 2;
    }

//{    a temp crutch so that mario sprite doesn't get covered by cubes and vice versa
//    bool checkIfShouldDrawMario(int x, int y) {
//        int flag = 1;
//        for(x -= marioWidth / 2; x <= x + marioWidth ; x ++)
//            for(y -= marioHeight / 2; y <= y + marioHeight; y ++)
//                if(graphicsWorker::getPixel(x, y) == RED) flag = 0;
//        return flag;
//    }
//}

    void draw() {
        //if(checkIfShouldDrawMario(marioXCenter, marioYCenter))
            graphicsWorker::drawRectangle(marioXCenter - marioWidth / 2 , marioYCenter - marioHeight / 2, marioXCenter + marioWidth / 2, marioYCenter + marioHeight / 2, 2);
    }

    void move() {
        if(GetAsyncKeyState(VK_LEFT) && graphicsWorker::getPixel(marioXCenter - marioXSpeed - marioWidth / 2, marioYCenter) != GRASS_GREEN)
            marioXCenter -= marioXSpeed;
        else if(GetAsyncKeyState(VK_LEFT) && graphicsWorker::getPixel(marioXCenter - marioXSpeed - marioWidth / 2, marioYCenter) == GRASS_GREEN)
        {
            for(int x = marioXCenter - marioWidth / 2 - marioXSpeed + 1; x < marioXCenter - marioWidth / 2; x ++)
            {
                assert(marioXCenter - marioWidth / 2 - marioXSpeed + 1 <= x && x < marioXCenter - marioWidth / 2);
                if(graphicsWorker::getPixel(x, marioYCenter) != GRASS_GREEN)
                {
                    marioXCenter = x + marioWidth / 2;
                    break;
                }
            }
        }

        if(GetAsyncKeyState(VK_RIGHT) && graphicsWorker::getPixel(marioXCenter + marioXSpeed + marioWidth / 2, marioYCenter) != GRASS_GREEN)
            marioXCenter += marioXSpeed;
        else if(GetAsyncKeyState(VK_RIGHT) && graphicsWorker::getPixel(marioXCenter + marioXSpeed + marioWidth / 2, marioYCenter) == GRASS_GREEN)
        {
            for(int x = marioXCenter + marioWidth / 2 + marioXSpeed - 1; x > marioXCenter + marioWidth / 2; x --)
            {
                assert(marioXCenter + marioWidth / 2 + marioXSpeed - 1 >= x && x > marioXCenter + marioWidth / 2);
                if(graphicsWorker::getPixel(x, marioYCenter) != GRASS_GREEN)
                {
                    marioXCenter = x - marioWidth / 2;
                    break;
                }
            }
        }
    }
};

class Cube {
    public:
    virtual void draw(int cubeSide, float x, int y) = 0;
};

class AirCube : public Cube {
    private:
    AirCube() {}
    AirCube(const Cube&);
    AirCube& operator=(Cube&);

    public:
    void draw(int cubeSide, float x, int y) {
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 0);
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
        //initWithFile
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

//{    a temp crutch so that mario sprite doesn't get covered by cubes and vice versa. do we need it?
//    bool checkIfShouldDrawCube(int x, int y, int cubeSide) {
//        int flag = 1;
//        for(int _x = x * cubeSide; _x <= (x + 1) * cubeSide; _x ++)
//        {
//            for(int _y = y * cubeSide; _y <= (y + 1) * cubeSide; _y ++) {
//                if(graphicsWorker::getPixel(_x, _y) == RED) flag = 0;
//            }
//        }
//
//        return flag;
//    }
//}

    void draw(float centerX, int gameViewWidth) {
        for(int x = (int)floor(- centerX); x <= gameViewWidth / cubeSide + (int)floor(- centerX); x ++) {
            assert((int)floor(- centerX) <= x && x <= gameViewWidth / cubeSide + (int)floor(- centerX));
            if (x == mapWidth) continue;
            for(int y = 0; y < mapHeight; y ++) {
                assert(0 <= y && y < mapHeight);
                Cube *c = _cubes[x][y];
                //if(checkIfShouldDrawCube(x + centerX, y, cubeSide))  see crutch
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
    Mario mario = Mario(0);

    public:
    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) {
        _cubeMap = Map(gameViewHeight);
        mario = Mario(_cubeMap.getCubeSide());
        _gameViewWidth = gameViewWidth;
        _gameViewHeight = gameViewHeight;

    }

    private:
    //Mario mario = Mario(0); //crutch, dunno how to get mapViewHeight ?

    public:
    void drawCubes() {
        static float x = 1;
        float px = x;
        if(px > 0) px = 0;
        if(px < - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide()) px = - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide();
        _cubeMap.draw(px, _gameViewWidth);
        //x -= 0.05;
    }

    void drawMario() {
        mario.draw();
    }

    void moveMario() {
        mario.move();
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
        game.drawMario();

    }

    void moveObjs() {
        game.moveMario();
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
