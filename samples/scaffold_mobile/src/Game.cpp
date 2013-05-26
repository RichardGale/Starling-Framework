

#include "Game.h"
#include "starling/animation/Transitions.h"
#include "starling/core/Starling.h"
#include "starling/display/Image.h"
#include "starling/display/Sprite.h"
#include "starling/events/TouchEvent.h"
#include "starling/events/TouchPhase.h"
#include "starling/utils/deg2rad.h"

    /** The Game class represents the actual game. In this scaffold, it just displays a 
     *  Starling that moves around fast. When the user touches the Starling, the game ends. */

using namespace starling::animation;
using namespace starling::core;
using namespace starling::display;
using namespace starling::events;
using namespace starling::utils;



        const std::string Game::GAME_OVER = "gameOver";

                    

        Game::Game()
        {
            init();
        }

        void Game::init()
        {
            mBird = new Image(Root()->assets()->getTexture("starling_rocket"));
            mBird->pivotX ( mBird->width() / 2);
            mBird->pivotY ( mBird->height() / 2);
            mBird->x ( Constants()->STAGE_WIDTH() / 2);
            mBird->y ( Constants()->STAGE_HEIGHT() / 2);
            mBird->addEventListener(TouchEvent::TOUCH, onBirdTouched);
            addChild(mBird);

            moveBird();
        }

        void Game::moveBird()
        {
            float scale = Math::random() * 0.8 + 0.2;

            Starling::juggler()->tween(mBird, Math::random() * 0.5 + 0.5, {
                x: Math::random() * Constants()->STAGE_WIDTH(),
                y: Math::random() * Constants()->STAGE_HEIGHT(),
                scaleX: scale,
                scaleY: scale,
                rotation: Math::random() * deg2rad(180) - deg2rad(90),
                transition: Transitions::EASE_IN_OUT,
                onComplete: moveBird
            });
        }

        void Game::onBirdTouched(TouchEvent* event)
        {
            if (event->getTouch(mBird, TouchPhase::BEGAN))
            {
                Root()->assets()->playSound("click");
                Starling::juggler()->removeTweens(mBird);
                dispatchEventWith(GAME_OVER, true, 100);
            }
        }

