#include "../../header/app/app_gui.hpp"

AppGUI::AppGUI(GLFWwindow *window, int *windowWidth, int *windowHeight, bool skipStartGamePopup, int numPlayers)
{
    this->window = window;
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;

    createCallbacks();

    Arial = new gui::Font(filePath("fonts/Arial.ttf"));
    saveConstraints("fill", new gui::Constraints(gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 1, gui::RELATIVE_CONSTRAINT, 1));
    saveConstraints("btnText", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .85, gui::RELATIVE_CONSTRAINT, .9));

    saveConstraints("wrapper", new gui::Constraints(gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 1, gui::RELATIVE_CONSTRAINT, 1));
    saveNamedChild("wrapper", new gui::native::Container({.05, .1, .05, 1}, 0, getConstraints("wrapper")), true);

    saveConstraints("tableContainer", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .6, gui::RELATIVE_CONSTRAINT, 1));
    saveNamedChild("tableContainer", new gui::native::Container(glm::vec4(0), 0, getConstraints("tableContainer")));
    getNamedChild("wrapper")->addChild(getNamedChild("tableContainer"));

    table = new poker::TableGUI(Arial, this, getConstraints("fill"), windowWidth, windowHeight);
    getNamedChild("tableContainer")->addChild(table);

    saveConstraints("playerInputContainer", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT_INVERSE, 0, gui::RELATIVE_CONSTRAINT, 1, gui::RELATIVE_CONSTRAINT, .08));
    saveNamedChild("playerInputContainer", new gui::native::Container({.2, .2, .2, .9}, 10, getConstraints("playerInputContainer")));
    getNamedChild("playerInputContainer")->draw_b = false;
    getNamedChild("tableContainer")->addChild(getNamedChild("playerInputContainer"));

    saveConstraints("playerInputFold", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .005, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .14, gui::RELATIVE_CONSTRAINT, .9));
    buttons["playerInputFold"] = new gui::native::Button({.1, .1, .1, 1}, 10, clickCallbacks["fold"], getConstraints("playerInputFold"));
    buttons["playerInputFold"]->setClickColor({.4, .25, .25, 1});
    buttons["playerInputFold"]->addChild(new gui::native::Text(Arial, "Fold", 40, {1, 0, 0}, false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("playerInputContainer")->addChild(buttons["playerInputFold"]);

    saveConstraints("playerInputBet", new gui::Constraints(gui::RELATIVE_CONSTRAINT, getConstraints("playerInputFold")->getX() + getConstraints("playerInputFold")->getW() + .01, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .14, gui::RELATIVE_CONSTRAINT, .9));
    buttons["playerInputBet"] = new gui::native::Button({.1, .1, .1, 1}, 10, clickCallbacks["bet"], getConstraints("playerInputBet"));
    buttons["playerInputBet"]->setClickColor({.25, .25, .4, 1});
    variableText["playerInputBet"] = new gui::native::Text(Arial, "Bet", 40, {0, .5, 1}, false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText"));
    buttons["playerInputBet"]->addChild(variableText["playerInputBet"]);
    getNamedChild("playerInputContainer")->addChild(buttons["playerInputBet"]);

    saveConstraints("playerInputMoney", new gui::Constraints(gui::RELATIVE_CONSTRAINT, getConstraints("playerInputBet")->getX() + getConstraints("playerInputBet")->getW() + .01, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .14, gui::RELATIVE_CONSTRAINT, .9));
    variableText["playerInputMoney"] = new gui::native::Text(Arial, "", 50, {0, .5, 1}, false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("playerInputMoney"));
    getNamedChild("playerInputContainer")->addChild(variableText["playerInputMoney"]);

    saveConstraints("playerInputSlider", new gui::Constraints(gui::RELATIVE_CONSTRAINT_INVERSE, .02, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 1 - (getConstraints("playerInputMoney")->getX() + getConstraints("playerInputMoney")->getW() + .04), gui::RELATIVE_CONSTRAINT, .5));
    sliders["playerInput"] = new customGUI::Slider(0, 100, 0, sliderCallbacks["playerInput"], getConstraints("playerInputSlider"));
    sliders["playerInput"]->callbackOnMouseDown = true;
    getNamedChild("playerInputContainer")->addChild(sliders["playerInput"]);

    saveConstraints("startGame", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .2, gui::ASPECT_CONSTRAINT, .5));
    buttons["startGame"] = new gui::native::Button({.8, .8, .8, 1}, 10, clickCallbacks["startGame"], getConstraints("startGame"));
    buttons["startGame"]->setClickColor({.6, 1, .6, 1});
    buttons["startGame"]->addChild(new gui::native::Text(Arial, "Start game", 40, glm::vec3(.2), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("tableContainer")->addChild(buttons["startGame"]);

    saveConstraints("pauseContainer", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT_INVERSE, .57, gui::RELATIVE_CONSTRAINT, .6, gui::RELATIVE_CONSTRAINT, .15));
    saveNamedChild("pauseContainer", new gui::native::Container({0, 0, 0, .5}, 10, getConstraints("pauseContainer")));
    getNamedChild("pauseContainer")->draw_b = false;
    getNamedChild("tableContainer")->addChild(getNamedChild("pauseContainer"));

    saveConstraints("pauseContainerTitle", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .9, gui::RELATIVE_CONSTRAINT, .5));
    variableText["pauseContainerTitle"] = new gui::native::Text(Arial, "", 40, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("pauseContainerTitle"));
    getNamedChild("pauseContainer")->addChild(variableText["pauseContainerTitle"]);

    saveConstraints("pauseContainerClose", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT_INVERSE, .05, gui::RELATIVE_CONSTRAINT, .5, gui::RELATIVE_CONSTRAINT, .4));
    buttons["pauseContainerClose"] = new gui::native::Button({.8, .8, .8, 1}, 10, clickCallbacks["continuePausedGame"], getConstraints("pauseContainerClose"));
    buttons["pauseContainerClose"]->setClickColor({.6, 1, .6, 1});
    buttons["pauseContainerClose"]->addChild(new gui::native::Text(Arial, "Continue game", 40, glm::vec3(.2), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("pauseContainer")->addChild(buttons["pauseContainerClose"]);

    saveConstraints("gameOverContainer", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .6, gui::RELATIVE_CONSTRAINT, .15));
    saveNamedChild("gameOverContainer", new gui::native::Container({0, 0, 0, .5}, 10, getConstraints("gameOverContainer")));
    getNamedChild("gameOverContainer")->draw_b = false;
    getNamedChild("tableContainer")->addChild(getNamedChild("gameOverContainer"));

    saveConstraints("gameOverContainerTitle", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .9, gui::RELATIVE_CONSTRAINT, .5));
    variableText["gameOverContainerTitle"] = new gui::native::Text(Arial, "", 40, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("gameOverContainerTitle"));
    getNamedChild("gameOverContainer")->addChild(variableText["gameOverContainerTitle"]);

    saveConstraints("gameOverContainerClose", new gui::Constraints(gui::CENTER_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT_INVERSE, .05, gui::RELATIVE_CONSTRAINT, .5, gui::RELATIVE_CONSTRAINT, .4));
    buttons["gameOverContainerClose"] = new gui::native::Button({.8, .8, .8, 1}, 10, clickCallbacks["restartGame"], getConstraints("gameOverContainerClose"));
    buttons["gameOverContainerClose"]->setClickColor({.6, 1, .6, 1});
    buttons["gameOverContainerClose"]->addChild(new gui::native::Text(Arial, "Start a new game", 40, glm::vec3(.2), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("gameOverContainer")->addChild(buttons["gameOverContainerClose"]);

    // Player hands
    saveConstraints("playerHandsContainer", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .5 + getConstraints("tableContainer")->getW() / 2, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .4, gui::RELATIVE_CONSTRAINT, 1));
    saveNamedChild("playerHandsContainer", new gui::native::Container({.05, .05, .1, 1}, 0, getConstraints("playerHandsContainer")));
    getNamedChild("playerHandsContainer")->draw_b = false;
    getNamedChild("wrapper")->addChild(getNamedChild("playerHandsContainer"));

    saveConstraints("playerHandsText", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .05, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .95, gui::RELATIVE_CONSTRAINT, .3));
    for (int i = 0; i < 7; i++)
        saveConstraints("playerHandsCard_" + std::to_string(i), new gui::Constraints(gui::RELATIVE_CONSTRAINT, i / 7.f, gui::RELATIVE_CONSTRAINT, .3, gui::RELATIVE_CONSTRAINT, .9f / 7.f, gui::RELATIVE_CONSTRAINT, .7f));

    for (int p = 0; p < numPlayers; p++)
    {
        std::string playerStr = std::to_string(p);
        saveConstraints("playerHands_" + playerStr, new gui::Constraints(gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, p / (float)numPlayers, gui::RELATIVE_CONSTRAINT, 1, gui::RELATIVE_CONSTRAINT, .99f / numPlayers));
        saveNamedChild("playerHands_" + playerStr, new gui::native::Container(glm::vec4(0), 0, getConstraints("playerHands_" + playerStr)));
        getNamedChild("playerHandsContainer")->addChild(getNamedChild("playerHands_" + playerStr));

        variableText["playerHandsText_" + playerStr] = new gui::native::Text(Arial, "", 30, glm::vec3(1), false, gui::ALIGNMENT_LEFT_H, gui::ALIGNMENT_CENTER_V, getConstraints("playerHandsText"));
        getNamedChild("playerHands_" + playerStr)->addChild(variableText["playerHandsText_" + playerStr]);

        for (int i = 0; i < 7; i++)
        {
            std::string iStr = std::to_string(i);
            cards["playerHands_" + playerStr + "_Card_" + iStr] = new poker::CardGUI(nullptr, getConstraints("playerHandsCard_" + iStr), windowWidth, windowHeight);
            getNamedChild("playerHands_" + playerStr)->addChild(cards["playerHands_" + playerStr + "_Card_" + iStr]);
        }
    }

    saveConstraints("playerHandsCardOpacity", new gui::Constraints(gui::RELATIVE_CONSTRAINT, 5 / 7.f, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 2 / 7.f, gui::RELATIVE_CONSTRAINT, 1));
    getNamedChild("playerHandsContainer")->addChild(new gui::native::Container({0, 0, 0, .5}, 0, getConstraints("playerHandsCardOpacity")));

    // Game Info
    saveConstraints("gameInfoLabel", new gui::Constraints(gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, 0, gui::RELATIVE_CONSTRAINT, .2, gui::RELATIVE_CONSTRAINT, .07));
    saveNamedChild("gameInfoLabel", new gui::native::Text(Arial, "Game info", 40, glm::vec3(1), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("gameInfoLabel")));
    getNamedChild("gameInfoLabel")->draw_b = false;
    getNamedChild("wrapper")->addChild(getNamedChild("gameInfoLabel"));

    saveConstraints("gameInfo", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .0125, gui::RELATIVE_CONSTRAINT, .07, gui::RELATIVE_CONSTRAINT, .175, gui::RELATIVE_CONSTRAINT, 1 - .07));
    variableText["gameInfo"] = new gui::native::Text(Arial, "", 25, glm::vec3(1), true, gui::ALIGNMENT_LEFT_H, gui::ALIGNMENT_TOP_V, getConstraints("gameInfo"));
    getNamedChild("wrapper")->addChild(variableText["gameInfo"]);

    // Menu Buttons
    saveConstraints("quitGameBtn", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .01, gui::RELATIVE_CONSTRAINT, .01, gui::ASPECT_CONSTRAINT, 3, gui::RELATIVE_CONSTRAINT, .07));
    buttons["quitGame"] = new gui::native::Button(glm::vec4(1), 10, clickCallbacks["quitGame"], getConstraints("quitGameBtn"));
    buttons["quitGame"]->setClickColor({1, .6, .6, 1});
    buttons["quitGame"]->addChild(new gui::native::Text(Arial, "Quit application", 40, glm::vec3(0), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("tableContainer")->addChild(buttons["quitGame"]);

    saveConstraints("restartGameBtn", new gui::Constraints(gui::RELATIVE_CONSTRAINT, .01, gui::RELATIVE_CONSTRAINT, .09, gui::ASPECT_CONSTRAINT, 3, gui::RELATIVE_CONSTRAINT, .07));
    buttons["restartGame"] = new gui::native::Button(glm::vec4(1), 10, clickCallbacks["restartGame"], getConstraints("restartGameBtn"));
    buttons["restartGame"]->setClickColor({.6, 1, .6, 1});
    buttons["restartGame"]->draw_b = false;
    buttons["restartGame"]->addChild(new gui::native::Text(Arial, "Restart game", 40, glm::vec3(0), false, gui::ALIGNMENT_CENTER_H, gui::ALIGNMENT_CENTER_V, getConstraints("btnText")));
    getNamedChild("tableContainer")->addChild(buttons["restartGame"]);

    if (skipStartGamePopup)
        clickCallbacks["startGame"]();
}

void AppGUI::createCallbacks()
{
    clickCallbacks["startGame"] = [this]()
    {
        startGame = true;
        buttons["startGame"]->draw_b = false;
        buttons["restartGame"]->draw_b = true;
        getNamedChild("gameInfoLabel")->draw_b = true;
    };
    clickCallbacks["continuePausedGame"] = [this]()
    {
        continuePausedGame = true;
        isPaused = false;
        getNamedChild("pauseContainer")->draw_b = false;
    };
    clickCallbacks["restartGame"] = [this]()
    {
        restartGame = true;
    };
    clickCallbacks["quitGame"] = [this]()
    {
        glfwSetWindowShouldClose(window, true);
    };

    clickCallbacks["fold"] = [this]()
    {
        fold = true;
        getNamedChild("playerInputContainer")->draw_b = false;
    };
    clickCallbacks["bet"] = [this]()
    {
        bet = true;
        getNamedChild("playerInputContainer")->draw_b = false;
    };

    sliderCallbacks["playerInput"] = [this](double &value)
    {
        int iValue = round(value);

        if (iValue < minBetMoney + prevRaiseMoney - 3)
            iValue = minBetMoney;
        else if (iValue < minBetMoney + prevRaiseMoney)
            iValue = minBetMoney + prevRaiseMoney;

        variableText["playerInputMoney"]->setText(std::to_string(iValue - currentBetMoney) + '$', variableText["playerInputMoney"]->getFontSize(), variableText["playerInputMoney"]->getColor(), *windowWidth, *windowHeight);

        std::string betType = "Call";
        if (iValue == currentBetMoney)
            betType = "Check";
        else if (iValue == maxBetMoney)
            betType = "All-In";
        else if (iValue > minBetMoney)
            betType = minBetMoney == 0 ? "Bet" : "Raise";

        variableText["playerInputBet"]->setText(betType, variableText["playerInputBet"]->getFontSize(), variableText["playerInputBet"]->getColor(), *windowWidth, *windowHeight);

        betMoney = iValue;
    };
}

void AppGUI::getPlayerInput(int minBetMoney, int prevRaiseMoney, int maxBetMoney, int currentBetMoney)
{
    if (minBetMoney > maxBetMoney)
        minBetMoney = maxBetMoney;

    if (minBetMoney + prevRaiseMoney > maxBetMoney)
        prevRaiseMoney = maxBetMoney - minBetMoney;

    this->minBetMoney = minBetMoney;
    this->prevRaiseMoney = prevRaiseMoney;
    this->maxBetMoney = maxBetMoney;
    this->currentBetMoney = currentBetMoney;

    playerInput = true;
    fold = bet = false;

    sliders["playerInput"]->setMin(minBetMoney == maxBetMoney ? minBetMoney : minBetMoney + prevRaiseMoney - 6);
    sliders["playerInput"]->setMax(minBetMoney == maxBetMoney ? maxBetMoney + 1 : maxBetMoney);
    sliders["playerInput"]->setValue(currentBetMoney);
    double value = sliders["playerInput"]->getValue();
    sliderCallbacks["playerInput"](value);
    if (value == maxBetMoney)
    {
        sliders["playerInput"]->setValue(maxBetMoney + 1);
        sliders["playerInput"]->inputs_b = false;
    }
    else
        sliders["playerInput"]->inputs_b = true;

    getNamedChild("playerInputContainer")->draw_b = true;
}
void AppGUI::pauseGame(std::string title)
{
    isPaused = true;
    continuePausedGame = false;

    variableText["pauseContainerTitle"]->setText(title, variableText["pauseContainerTitle"]->getFontSize(), variableText["pauseContainerTitle"]->getColor(), *windowWidth, *windowHeight);
    getNamedChild("pauseContainer")->draw_b = true;
}
void AppGUI::gameOver(std::string title)
{
    variableText["gameOverContainerTitle"]->setText(title, variableText["gameOverContainerTitle"]->getFontSize(), variableText["gameOverContainerTitle"]->getColor(), *windowWidth, *windowHeight);
    getNamedChild("gameOverContainer")->draw_b = true;

    buttons["restartGame"]->draw_b = false;
    getNamedChild("gameInfoLabel")->draw_b = false;
    variableText["gameInfo"]->draw_b = false;
}

void AppGUI::updateGameInfo(int roundIndex, int smallBlindMoney, int bigBlindMoney, int blindsIncrementRounds, int prevBlindsIncrement, int totalBettingMoney, std::vector<poker::CommunityPot> *communityPots)
{
    std::string str = "Round: " + std::to_string(roundIndex + 1) +
                      "\nSmall blinds: " + std::to_string(smallBlindMoney) + '$' +
                      "\nBig blinds: " + std::to_string(bigBlindMoney) + '$' +
                      "\nBlinds increase in " + std::to_string(blindsIncrementRounds - (roundIndex - prevBlindsIncrement)) + " rounds..." +
                      "\nTotal betting money: " + std::to_string(totalBettingMoney) + "$\n";

    for (int i = 0; i < communityPots->size(); i++)
        str += "Community pot " + std::to_string(i + 1) + ": " + std::to_string(communityPots->at(i).money) + "$\n";

    variableText["gameInfo"]->setText(str, variableText["gameInfo"]->getFontSize(), variableText["gameInfo"]->getColor(), *windowWidth, *windowHeight);
}