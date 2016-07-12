class Game
{
    public:
        static int update();
        static int getState();
        static void setState(int newState);
    private:
        static int state_;

};

enum State
{
    STATE_PLAYING,
    STATE_QUITING
};

