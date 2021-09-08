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

#include "Source/Actors/Bullet.h"
#include "Source/Actors/Player.h"

namespace centpd {
    ///////////////////////////////////////////////////////////////
    Bullet::Bullet(ime::Scene &scene) :
        GameObject(scene),
        m_owner{nullptr},
        m_posChangeId{-1},
        m_destId{-1},
        m_isFired{false}
    {
        setTag("bullet");

        ime::Sprite& sprite = getSprite();
        sprite.setTexture("Spritesheet.png");
        sprite.setTextureRect(ime::UIntRect{12, 80, 1, 6});
        resetSpriteOrigin();
        sprite.scale(2.0f, 2.0f);

        // Collision stuff
        setCollisionGroup("bullet");
        getCollisionExcludeList().add("invisibleWall");

        // Destroy bullet
        onCollision([](ime::GameObject* bullet, ime::GameObject* other) {
            std::string hitObjectType = other->getClassName();
            if ((hitObjectType == "Mushroom")
                || (hitObjectType == "Scorpion")
                || (hitObjectType == "Flea")
                || (hitObjectType == "CentipedeSegment"))
            {
                bullet->setActive(false);
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    Bullet::Ptr Bullet::create(ime::Scene &scene) {
        return std::make_unique<Bullet>(scene);
    }

    ///////////////////////////////////////////////////////////////
    std::string Bullet::getClassName() const {
        return "Bullet";
    }

    ///////////////////////////////////////////////////////////////
    void Bullet::setOwner(Player *owner) {
        if (!m_isFired) {
            if (m_owner) {
                m_owner->removeDestructionListener(m_destId);
                m_owner->unsubscribe("position", m_posChangeId);
            }

            // A nullptr is usd to remove the current owner without setting a new one
            if (!owner)
                return;

            m_owner = owner;
            syncPosition();

            // Make the bullet track the position of its owner. This is done
            // because the player must visually show that it has a bullet or not
            m_posChangeId = m_owner->onPropertyChange("position", [this](const ime::Property&) {
                syncPosition();
            });

            // If the owner is destroyed before the bullet
            m_destId = owner->onDestruction([this] {
                m_owner = nullptr;
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    Player *Bullet::getOwner() {
        return m_owner;
    }

    ///////////////////////////////////////////////////////////////
    const Player *Bullet::getOwner() const {
        return m_owner;
    }

    ///////////////////////////////////////////////////////////////
    bool Bullet::fire() {
        if (m_owner && isActive()) {
            m_isFired = true;

            // Stop the bullet from tracking its owners position
            m_owner->unsubscribe("position", m_posChangeId);
            m_posChangeId = -1;

            return true;
        }

        return false;
    }

    ///////////////////////////////////////////////////////////////
    bool Bullet::isFired() const {
        return m_isFired;
    }

    ///////////////////////////////////////////////////////////////
    void Bullet::syncPosition() {
        ime::Vector2f ownerPos = m_owner->getTransform().getPosition();
        static const float offset = 4.0f; // Make the bullet peak out the head of the owner
        getTransform().setPosition(ownerPos.x, ownerPos.y - offset);
    }

    ///////////////////////////////////////////////////////////////
    Bullet::~Bullet() {
        if (m_owner) {
            m_owner->unsubscribe("position", m_posChangeId);
            m_owner->removeDestructionListener(m_destId);
        }
    }
}
