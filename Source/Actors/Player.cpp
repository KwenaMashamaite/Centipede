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
#include "Source/Scenes/GameplayScene.h"
#include <cassert>

namespace centpd {
    Player::Player(ime::Scene &scene, int lives) :
        GameObject(scene),
        m_numLives{lives},
        m_posChangeId{-1},
        m_bullet{nullptr}
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
    void Player::setBullet(Bullet *bullet) {
        assert(bullet && "Bullet cannot be a nullptr");
        assert(!bullet->isFired() && "Cannot assign player a bullet that's already fired");

        if (!m_bullet) {
            m_bullet = bullet;
            m_bullet->setOwner(this);
            m_bullet->getCollisionExcludeList().add(getCollisionGroup()); // Player is invulnerable to bullets
        }
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

    ///////////////////////////////////////////////////////////////
    bool Player::canShoot() const {
        return m_bullet != nullptr;
    }

    ///////////////////////////////////////////////////////////////
    Bullet* Player::shoot() {
        if (m_bullet) {
            m_bullet->fire();
            Bullet* temp = m_bullet;
            m_bullet = nullptr;
            return temp;
        }

        return nullptr;
    }

    Player::~Player() {
        if (m_bullet)
            m_bullet->setOwner(nullptr);
    }
}
