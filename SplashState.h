#ifndef SPLASH_H_INCLUDED
#define SPLASH_H_INCLUDED

class StateManager;
class SplashState: public State
{
private:
    StateManager* sm;
    bool isFadingIn;
    bool isOnPause;
    bool isFadingOut;
    float timeCount;
    int logoAlpha;
public:
    SplashState(StateManager* sm);
    void update(float dt);
    void draw(sf::RenderWindow&) const;
    void onActivate(const std::string& accept);
    void onDeactivate();
    void handleInput(int u, int v, const std::string& typed, sf::Event e);
};

#endif // SPLASH_H_INCLUDED
