#include "../../header/app/app.hpp"

App::App(GLFWwindow *window, int *windowWidth, int *windowHeight, bool devMode)
    : DrawLoop(window, windowWidth, windowHeight)
{
    this->devMode = devMode;

    poker::CardGUI::createTexture();
    poker::ChipsGUI::createTexture();
    poker::SpecialChipsGUI::createTexture();

    numPlayers = 6;
    startMoney = 400;
    bigBlindMoney = 10;
    blindsIncrementRounds = 8;

    _gui = new AppGUI(window, windowWidth, windowHeight, false, numPlayers);
    sceneGUI["gui"] = _gui;

    game = new poker::Game(_gui->table, devMode, numPlayers, startMoney, bigBlindMoney, blindsIncrementRounds);
}
App::~App()
{
    delete game;

    poker::CardGUI::deleteTexture();
    poker::ChipsGUI::deleteTexture();
    poker::SpecialChipsGUI::deleteTexture();
}

void App::init()
{
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void App::inputs()
{
    _gui->inputs(window, *windowWidth, *windowHeight);

    if (glfwGetTime() > game->getNextActionTime())
    {
        switch (game->getNextAction())
        {
        case poker::Game::START:
        {
            if (_gui->startGame)
            {
                _gui->updateGameInfo(game->getRoundIndex(), game->getSmallBlindMoney(), game->getBigBlindMoney(), game->getBlindsIncrementRounds(), game->getPrevBlindsIncrement(), game->getTotalBettingMoney(), game->getCommunityPots());
                game->startGame();
            }
        }
        break;
        case poker::Game::CONTINUE:
        {
            _gui->updateGameInfo(game->getRoundIndex(), game->getSmallBlindMoney(), game->getBigBlindMoney(), game->getBlindsIncrementRounds(), game->getPrevBlindsIncrement(), game->getTotalBettingMoney(), game->getCommunityPots());

            if (game->showPlayerHands() && !_gui->playerHandsVisible)
                _gui->showPlayerHands(game->getPlayerCards(), game->getCommunityCards(), game->getPlayerHands());

            game->continueGame();
        }
        break;
        case poker::Game::PAUSE:
        {
            if (_gui->continuePausedGame)
            {
                _gui->continuePausedGame = false;
                game->continueGame();

                if (!game->showPlayerHands() && _gui->playerHandsVisible)
                    _gui->hidePlayerHands();
                else if (_gui->playerHandsVisible)
                    _gui->pauseGame(game->getWinnerText());
            }
            else if (!_gui->isPaused)
                _gui->pauseGame(game->getWinnerText());
        }
        break;
        case poker::Game::PLAYER_INPUT:
        {
            if (!_gui->playerInput)
            {
                _gui->getPlayerInput(game->getMinBetMoney(), game->getPrevRaiseMoney(), game->getMaxBetMoney(), game->getCurrentBetMoney());
            }
            else if (_gui->bet)
            {
                game->bet(_gui->betMoney);
                _gui->playerInput = false;
            }
            else if (_gui->fold)
            {
                game->fold();
                _gui->playerInput = false;
            }
        }
        break;
        case poker::Game::END:
        {
            if (!_gui->isGameOver)
                _gui->gameOver(game->getWinnerText());
        }
        break;
        }
    }

    if (_gui->restartGame)
    {
        delete _gui;
        delete game;

        _gui = new AppGUI(window, windowWidth, windowHeight, true, numPlayers);
        sceneGUI["gui"] = _gui;

        game = new poker::Game(_gui->table, devMode, numPlayers, startMoney, bigBlindMoney, blindsIncrementRounds);
    }
}