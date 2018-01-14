//TODO Gen map with init file; synchronize map movement with Nario's; create frames generation for
//TODO Nario's movement;

#include <txlib.h>

#define if_DEBUG

//Global constants
const COLORREF RED = RGB        (128,   0,   0);
const COLORREF SKY_BLUE = RGB   (100, 100, 255);
const COLORREF GRASS_GREEN = RGB(  0, 200,   0);

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
    static const int width = 20, x0 = 200, height =  40, ay = 3;
    int xCenter = x0, yCenter = 0, dx = 0, dy = 0, y0 = 0;
    bool onGround = true;

    public:

    Nario(int cubeSide) : y0(cubeSide * 11 - height / 2), yCenter(y0) {}

    inline int getXCenter() {
        return xCenter;
    }

    inline int getX0() {
        return x0;
    }

    inline void setDx(int speed) {
        dx = speed;
    }

    inline void setDy(int speed) {
        dy = speed;
    }

    inline void setOnGround(bool statement) {
        onGround = statement;
    }

    inline bool getOnGround() {
        return onGround;
    }

    void draw() {
            graphicsWorker::drawRectangle(xCenter - width / 2 , yCenter - height / 2, xCenter + width / 2, yCenter + height / 2, 2);
    }

    inline bool checkIfOnGround() {
        for(int i = xCenter - width / 2; i <= xCenter + width / 2; i ++)
            if(graphicsWorker::getPixel(i, yCenter + 1) == GRASS_GREEN)
                return true;
        return false;
    }

    bool checkIfWallLeft() {
        int flag = false;
        for(int x = xCenter + dx - width / 2; x < xCenter - width / 2; x ++) {
            assert( xCenter + dx - width / 2 <= x && x < xCenter - width / 2);

            for(int y = yCenter - height / 2; y <= yCenter + height / 2 - 1; y ++) {
                assert(yCenter - height / 2 <= y && y <= yCenter + height / 2 - 1);

                if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                    flag = true;
                    break;
                }
            }
        }

        return flag;
    }

    bool checkIfWallRight() {
        int flag = false;
        for(int x = xCenter + dx + width / 2; x > xCenter + width / 2; x --) {
            assert( xCenter + dx + width / 2 >= x && x > xCenter + width / 2);

            for(int y = yCenter - height / 2; y <= yCenter + height / 2 - 1; y ++) {
                assert(yCenter - height / 2 <= y && y <= yCenter + height / 2 - 1);

                if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                    flag = true;
                    break;
                }
            }
        }

        return flag;
    }

    void update() {
        bool flag = true;

        if(! checkIfOnGround())
            onGround = false;

        if(dx < 0) {
            if(! checkIfWallLeft())
                xCenter += dx;
            else {
                flag = true;

                for(int x = xCenter - width / 2; x >= xCenter - width / 2 + dx; x --) {
                assert(xCenter - width / 2 >= x && x >= xCenter - width / 2 + dx);

                    for(int y = yCenter + height / 2 - 1; y >= yCenter - height / 2; y --) {
                        assert(yCenter + height / 2 - 1 >= y && y >= yCenter - height / 2);

                        if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                            xCenter = x + width / 2 + 1;
                            flag = false;
                            break;
                        }

                    }

                    if(! flag)
                        break;
                }
            }
        }

        if(dx > 0) {
            if(! checkIfWallRight()) {
                xCenter += dx;
            }
            else {
                flag = true;

                for(int x = xCenter + width / 2; x <= xCenter + width / 2 + dx; x ++) {
                assert(xCenter + width / 2 <= x && x <= xCenter + width / 2 + dx);

                    for(int y = yCenter + height / 2 - 1; y >= yCenter - height / 2; y --) {
                        assert(yCenter + height / 2 - 1 >= y && y >= yCenter - height / 2);

                        if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                            xCenter = x - width / 2 - 1;
                            flag = false;
                            break;
                        }
                    }

                    if(! flag)
                        break;
                }
            }
        }




        if(! onGround) {
            dy += ay;
            yCenter += dy;

            if(dy < 0) {
                for(int y = yCenter - height / 2 - dy; y >= yCenter - height / 2 + dy; y --) {
                    assert(yCenter - height / 2 - dy >= y && y >= yCenter - height / 2 + dy);

                    for(int x = xCenter - width / 2; x < xCenter + width / 2; x ++) {
                        assert(xCenter - width / 2 <= x && x < xCenter + width / 2);

                        if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                            yCenter = y + height / 2 + 1;
                            dy = 0;
                            break;
                        }
                    }
                }
            }

            if(dy > 0) {
                for(int y = yCenter + height / 2 - dy; y <= yCenter + height / 2 + dy; y ++) {
                    assert(yCenter + height / 2 - dy <= y && y <= yCenter + height / 2 + dy);

                    for(int x = xCenter - width / 2; x < xCenter + width / 2; x ++) {
                        assert(xCenter - width / 2 <= x && x < xCenter + width / 2);

                        if(graphicsWorker::getPixel(x, y) == GRASS_GREEN) {
                            yCenter = y - height / 2;
                            dy = 0;
                            onGround = true;
                            break;
                        }
                    }
                }
            }
        }

        if(yCenter + height / 2 > y0 + height / 2) {
            yCenter = y0;
            dy = 0;
            onGround = true;
        }

        dx = 0;
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
    void draw(int cubeSide, float x, int y) {//переопределенный draw() из Cube
        graphicsWorker::drawRectangle(cubeSide * x, cubeSide * y, cubeSide * (x + 1), cubeSide * (y + 1), 0);
    }

    static AirCube* getInstance() {
        static AirCube instance;//синглтон Майерса (инициализация статической переменной происходит только при первом вызове функции)
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
    Map(int gameViewHeight) : cubeSide(gameViewHeight / mapHeight) {
        //initWithFile (read from file in next version)
        
        for(int x = 0; x < mapWidth; x++) {
            assert(0 <= x && x < mapWidth);

            for(int y = 0; y < mapHeight; y++) {
                assert(0 <= y && y < mapHeight);

                if (x < mapWidth - 1 && x > 0 && y < mapHeight - 1) _cubes[x][y] = AirCube::getInstance();
                else _cubes[x][y] = DirtCube::getInstance();
                if (x % 10 == 0 && y == mapHeight - 2) _cubes[x][y] = DirtCube::getInstance();//artefact
                if (x % 15 == 0 && y == mapHeight - 3) _cubes[x][y] = DirtCube::getInstance();//artefact
                if (x % 20 == 0 && y == mapHeight - 4) _cubes[x][y] = DirtCube::getInstance();//artefact
                if (x % 25 == 0 && y == mapHeight - 5) _cubes[x][y] = DirtCube::getInstance();//artefact
                if (x % 30 == 0 && y == mapHeight - 6) _cubes[x][y] = DirtCube::getInstance();//artefact
                if (x % 35 == 0 && y == mapHeight - 7) _cubes[x][y] = DirtCube::getInstance();//artefact
            }
        }
    }

    void draw(float centerX, int gameViewWidth) {
        for(int x = (int)floor(centerX); x <= gameViewWidth / cubeSide + (int)floor(centerX); x++) {
            assert((int)floor(centerX) <= x && x <= gameViewWidth / cubeSide + (int)floor(centerX));

            if (x == mapWidth) continue;

            for(int y = 0; y < mapHeight; y++) {
                assert(0 <= y && y < mapHeight);

                Cube *c = _cubes[x][y];
                c->draw(cubeSide, x - centerX, y);
            }
        }
    }

    int getCubeSide() {//getter
        return cubeSide;
    }
};

class Game {
    private:
    int _gameViewWidth, _gameViewHeight, _mobsNumber;
    Map _cubeMap = Map(0);
    Nario nario = Nario(0);

    public:
    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) : _gameViewWidth(gameViewWidth), _gameViewHeight(gameViewHeight) {
        _cubeMap = Map(gameViewHeight);
        nario = Nario(_cubeMap.getCubeSide());
    }

    void drawCubes() {
        //TODO Make a working map scroll

        float offSetX = (nario.getXCenter() - 3 * _cubeMap.getCubeSide()) / _cubeMap.getCubeSide();
        float poffSetX = 0;
        if(offSetX > 3)
            poffSetX = offSetX - 3;
        if(poffSetX > Map::mapWidth - _gameViewWidth / _cubeMap.getCubeSide())
            poffSetX = Map::mapWidth - _gameViewWidth / _cubeMap.getCubeSide();
        _cubeMap.draw(0, _gameViewWidth);
    }

    void drawNario() {
        nario.draw();
    }

    void updateNario() {
        if(GetAsyncKeyState(VK_LEFT))
            nario.setDx(-10);

        if(GetAsyncKeyState(VK_RIGHT))
            nario.setDx(10);

        if(GetAsyncKeyState(VK_UP) && nario.getOnGround()) {
            nario.setDy(-20);
            nario.setOnGround(false);
        }

        nario.update();
//         static float x = 1;//артефакт, в следующей версии будем передавать сюда координату Nарио
//         float px = x;
//         if(px > 0) px = 0;
//         if(px < 0 - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide()) px = 0 - Map::mapWidth + _gameViewWidth / _cubeMap.getCubeSide();
//         _cubeMap.draw(px, _gameViewWidth);
//         x-=0.05;
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
        game.drawNario();
    }

    void updateObjs() {
        game.updateNario();
    }
};

int main() {
    int t = 100;
    while(true) {
        WinDirector::getInstance().updateObjs();
        WinDirector::getInstance().drawGame();
        if(GetAsyncKeyState(VK_DOWN)) t = 10000;
        graphicsWorker::freezeWindow(t);
    }
}
