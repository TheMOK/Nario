
class Cube {
    static Cube* getInstance(){}
};

class AirCube : public Cube {};

class DirtCube : public Cube {};

class Mob {};

class MapFragment {
    private:
    static int const fragmentWidth = 50, fragmentHeight = 50;
    Cube* _cubes[fragmentWidth][fragmentHeight];
    public:
    MapFragment() {
        initWithGround();
    }

    void initWithGround() {
        for (int i = 0; i < fragmentWidth; i++) {
            for (int j = 0; j < fragmentHeight - 1; j++) {
                _cubes[i][j] = AirCube.getInstance(); //singleton, лучшая вещь в проге
            }
        }
        for (int j = 0; j < fragmentWidth; j++) {
            _cubes[j][fragmentHeight] = DirtCube.getInstance(); //singleton, просто офигенный, серьезно
        }
    }

    void draw(int cubeSide) {
        for (int i = 0; i < fragmentWidth; i++) {
            for (int j = 0; j < fragmentHeight - 1; j++) {
                (*(_cubes[i][j])).draw(i, j, cubeSide);
            }
        }
    }
};

class Map {
    private:
    static const int _fragmentsNumber = 4;
    MapFragment _fragments[_fragmentsNumber];
    public:
    Map() {
        //написать сложную инициализацию фрагментов
        init();
    }

    void init() {
        //прочитать уровень из файла //////в отдельном потоке
        for (int i = 0; i < _fragmentsNumber; i++) {
            //распределить уровень по фрагментам
            //пока что здесь будет затычка
            _fragments[i].initWithGround();
        }
    }

    void draw(int cubeSide) {
        for (int i = 0; i < _fragmentsNumber; i++) {
            _fragments[i].draw(cubeSide);
        }
    }
};

class Game {
    private:
    int _gameViewWidth, _gameViewHeight, _mobsNumber, _cubeSide;
    Map _cubeMap;
    Mob _mobs[];//Нарио хранит свои координаты в кубиках, как числа с плавающей точкой. Все мобы их так хранят

    Game(int gameViewWidth, int gameViewHeight, int cubesVerticalNumber) {
        _gameViewWidth = gameViewWidth;
        _gameViewHeight = gameViewHeight;
        _cubeSide = _gameViewHeight / cubesVerticalNumber;
        _cubeMap.init();
    }

    public:
    void drawCubes() {
        _cubeMap.draw(_cubeSide);
    }

    void drawMobs() {}

    void animateMobs() {}
};
