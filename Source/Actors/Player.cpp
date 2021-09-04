////////////////////////////////////////////////////////////////////////////////
// Centipede clone
//
// Copyright (c) 2021 Kwena Mashamaite (kwena.mashamaite1@gmail.com)
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
////////////////////////////////////////////////////////////////////////////////

#include "Source/Actors/Player.h"

namespace centpd {
    Player::Player(ime::Scene &scene, int lives) :
        GameObject(scene),
        m_numLives{lives}
    {
        setCollisionGroup("player");
        setTag("player");

        ime::Sprite& sprite = getSprite();
        sprite.setTexture("Spritesheet.png");
        sprite.setTextureRect(ime::UIntRect{0, 80, 7, 8});
        resetSpriteOrigin();
        sprite.scale(2.0f, 2.0f);
    }

    ///////////////////////////////////////////////////////////////
    Player::Ptr Player::create(ime::Scene &scene, int lives) {
        return std::make_unique<Player>(scene, lives);
    }

    ///////////////////////////////////////////////////////////////
    std::string Player::getClassName() const {
        return "Player";
    }

    ///////////////////////////////////////////////////////////////
    void Player::setLives(int lives) {
        if (m_numLives != lives) {
            m_numLives = lives;

            if (m_numLives == 0)
                setActive(false);

            emitChange(ime::Property{"lives", m_numLives});
        }
    }

    ///////////////////////////////////////////////////////////////
    int Player::getLives() const {
        return m_numLives;
    }
}
