#include <SFML/Graphics.hpp>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <cstdio>
#include "StateManager.h"
#include "OptionsState.h"

OptionsState::OptionsState(StateManager* sm): State(sm), sm(sm),
    back(sf::Vector2f(Universal::window_width/2.0-250, Universal::window_height/2.0+250), sf::Vector2f(100, 20), "Back")
{
    state = 0;
    font.loadFromFile("assets/fonts/black_jack.ttf");

    upLabel = sf::Text("Up: ", font);
    upInput = sf::Text(upt, font);

    downLabel = sf::Text("Down: ", font);
    downInput = sf::Text(downt, font);

    leftLabel = sf::Text("Left: ", font);
    leftInput = sf::Text(leftt, font);

    rightLabel = sf::Text("Right: ", font);
    rightInput = sf::Text(rightt, font);

    actionLabel = sf::Text("Action: ", font);
    actionInput = sf::Text(actiont, font);

    shiftLabel = sf::Text("Shift: ", font);
    shiftInput = sf::Text(shiftt, font);

    upLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 - 150);
    upInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 - 150);
    upInput.setColor(sf::Color::Green);

    downLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 - 100);
    downInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 - 100);
    downInput.setColor(sf::Color::Green);

    leftLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 - 50);
    leftInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 - 50);
    leftInput.setColor(sf::Color::Green);

    rightLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 - 0);
    rightInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 - 0);
    rightInput.setColor(sf::Color::Green);

    actionLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 + 50);
    actionInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 + 50);
    actionInput.setColor(sf::Color::Green);

    shiftLabel.setPosition(Universal::window_width/2.0 - 80, Universal::window_height/2.0 + 100);
    shiftInput.setPosition(Universal::window_width/2.0 + 10, Universal::window_height/2.0 + 100);
    shiftInput.setColor(sf::Color::Green);

    highlight.setFillColor(sf::Color::White);
    highlight.setSize(sf::Vector2f(150, 50));
}

void OptionsState::update(float dt)
{
    upInput.setString(upt);
    downInput.setString(downt);
    leftInput.setString(leftt);
    rightInput.setString(rightt);
    actionInput.setString(actiont);
    shiftInput.setString(shiftt);


    if(state==0)
    {
        upLabel.setColor(sf::Color::Black);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 - 155);
    }
    else if(state==1)
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::Black);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 - 105);
    }
    else if(state==2)
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::Black);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 - 55);
    }
    else if(state==3)
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::Black);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 - 5);
    }
    else if(state==4)
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::Black);
        shiftLabel.setColor(sf::Color::White);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 +45);
    }
    else if(state==5)
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::Black);
        highlight.setPosition(Universal::window_width/2.0 - 90, Universal::window_height/2.0 +95);
    }
    else
    {
        upLabel.setColor(sf::Color::White);
        downLabel.setColor(sf::Color::White);
        leftLabel.setColor(sf::Color::White);
        rightLabel.setColor(sf::Color::White);
        actionLabel.setColor(sf::Color::White);
        shiftLabel.setColor(sf::Color::White);
        // highlight.setPosition(Universal::window_width/2.0 - 75, Universal::window_height/2.0 +95);
    }
    /*
    upInput.setString(up);
    downInput.setString(down);
    leftInput.setString(left);
    rightInput.setString(right);
    actionInput.setString(action);
    shiftInput.setString(shift);
    */

}

void OptionsState::handleInput(int u, int v, const std::string& accept,sf::Event e)
{


    bool leftClick = sf::Mouse::isButtonPressed(sf::Mouse::Left);

    bool upCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
    static bool upPreviouslyPressed = false;

    bool dnCurrentlyPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
    static bool dnPreviouslyPressed = false;

    bool upReleased = !upCurrentlyPressed && upPreviouslyPressed;
    bool dnReleased = !dnCurrentlyPressed && dnPreviouslyPressed;

    if(back.checkCollision(u, v) && leftClick)
        popSelf(1, "");

    if(state==0)
    {
        if(dnReleased)
            state++;

        if(e.type+1 == sf::Event::KeyPressed)
        {

            UPKEY = e.key.code - 97;
            Universal::appendTextInput(upt, accept);
            if(upt.length() >= 2)
            {
                upt = upt[1];
            }

        }
    }
    else if( state==1 )
    {
        if(upReleased)
            state--;
        else if (dnReleased)
        {
            state++;
        }

        if(e.type+1 == sf::Event::KeyPressed)
        {
            DOWNKEY = e.key.code - 97;
            Universal::appendTextInput(downt, accept);
            if(downt.length() >= 2)
            {
                downt = downt[1];
            }

        }
    }
    else if( state==2 )
    {
        if(upReleased)
            state--;
        else if (dnReleased)
        {
            state++;
        }

        if(e.type+1 == sf::Event::KeyPressed)
        {
            LEFTKEY = e.key.code - 97;
            Universal::appendTextInput(leftt, accept);
            if(leftt.length() >= 2)
            {
                leftt = leftt[1];
            }
        }
    }

    else if( state==3 )
    {
        if(upReleased)
            state--;
        else if (dnReleased)
        {
            state++;
        }

        if(e.type+1 == sf::Event::KeyPressed)
        {
            RIGHTKEY = e.key.code - 97;
            Universal::appendTextInput(rightt, accept);
            if(rightt.length() >= 2)
            {
                rightt = rightt[1];
            }
        }
    }

    else if( state==4 )
    {
        if(upReleased)
            state--;
        else if (dnReleased)
        {
            state++;
        }

        if(e.type+1 == sf::Event::KeyPressed)
        {
            ACTIONKEY = e.key.code - 97;
            Universal::appendTextInput(actiont, accept);
            if(actiont.length() >= 2)
            {
                actiont = actiont[1];
            }
        }
    }

    else if( state==5 )
    {
        if(upReleased)
            state--;


        if(e.type+1 == sf::Event::KeyPressed)
        {
            SHIFTKEY = e.key.code - 97;
            Universal::appendTextInput(shiftt, accept);
            if(shiftt.length() >= 2)
            {
                shiftt = shiftt[1];
            }
        }

    }


    upPreviouslyPressed = upCurrentlyPressed;
    dnPreviouslyPressed = dnCurrentlyPressed;

}

void OptionsState::draw(sf::RenderWindow& rw) const
{
    back.draw(rw);
    /*
    up.draw(rw);
    down.draw(rw);
    left.draw(rw);
    right.draw(rw);
    action.draw(rw);
    shift.draw(rw);
    */

    if(state<6)
        rw.draw(highlight);
    rw.draw(upLabel);
    rw.draw(downLabel);
    rw.draw(leftLabel);
    rw.draw(rightLabel);
    rw.draw(actionLabel);
    rw.draw(shiftLabel);
    rw.draw(upInput);
    rw.draw(downInput);
    rw.draw(leftInput);
    rw.draw(rightInput);
    rw.draw(actionInput);
    rw.draw(shiftInput);


}

void OptionsState::onActivate()
{
    state = 0;

}


void OptionsState::onDeactivate()
{
    state = 0;

    std::ofstream configfile;
    configfile.open("config.txt");
    std::string buffer;
    int binding = 0;
    while(binding < 6)
    {
        buffer = "";


        switch(binding)
        {
        case 0:
        {

            configfile << UPKEY;
            configfile<< '\n';
            break;
        }
        case 1:
        {

            configfile << DOWNKEY;
            configfile<< '\n';
            break;
        }
        case 2:
        {

            configfile << LEFTKEY;
            configfile<< '\n';
            break;
        }
        case 3:
        {

            configfile << RIGHTKEY;
            configfile<< '\n';
            break;
        }
        case 4:
        {

            configfile << ACTIONKEY;
            configfile<< '\n';
            break;
        }
        case 5:
        {

            configfile << SHIFTKEY;
            configfile<< '\n';
            break;
        }

        }

        binding++;
    }
    configfile.flush();
    configfile.close();
}


