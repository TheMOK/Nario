#include<txlib.h>
namespace graphicsWorker {
    bool createWindow(int height, int width) {
        return txCreateWindow (height, width);
    }

    bool destroyWindow() {
        return txDestroyWindow();
    }

    void fillRed() {
        txSetFillColor(TX_RED);
        txClear();
    }
}

class Cube {};

class AirCube : public Cube {
    public:
    static Cube* getInstance(){};
};

class DirtCube : public Cube {
    public:
    static Cube* getInstance(){};
};

class Map {
    private:
    static const int _fragmentsNumber = 4;
    //MapFragment _fragments[_fragmentsNumber];
    public:
    Map() {
    }

//    void init() {}

    void draw(int cubeSide) {
        graphicsWorker::fillRed();
    }
};

class Game {
    private:
    int _gameViewWidth, _gameViewHeight, _mobsNumber, _cubeSide;
    Map _cubeMap;

    public:
    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) {
        _gameViewWidth = gameViewWidth;
        _gameViewHeight = gameViewHeight;
        _cubeSide = _gameViewHeight / cubesVerticalNumber;
    }

    void drawCubes() {
        _cubeMap.draw(_cubeSide);
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
    WinDirector::getInstance().drawGame();
}
