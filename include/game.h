class Game
{
    public:
        int update();
        int getState();
        void setState();
    private:
        int state_;

};

enum State
{
    STATE_PLAYING,
    STATE_QUITING
};

