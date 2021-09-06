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

#include "Source/Actors/Mushroom.h"

namespace centpd {
    namespace {
        const unsigned int MAX_BULLET_HITS = 4;
    }

    ///////////////////////////////////////////////////////////////
    Mushroom::Mushroom(ime::Scene &scene) :
        GameObject(scene),
        m_isPoisoned{false},
        m_hitCount{0}
    {
        // Initialized shared mushroom textures
        if (!m_isSpriteSheetCreated) {
            m_spriteSheet.create("Spritesheet.png",ime::Vector2u{8, 8}, ime::Vector2u{0, 0}, ime::UIntRect{64, 0, 32, 16});
            m_isSpriteSheetCreated = true;
        }

        setCollisionGroup("mushroom");
        setAsObstacle(true);

        // Since a mushroom is an obstacle object, by default, other objects
        // cannot overlap with it. However, Scorpions and Bullets must be able
        // to collide with it by overlapping
        getObstacleCollisionFilter().add("scorpion");
        getObstacleCollisionFilter().add("bullet");

        // Set initial mushroom (full) texture
        getSprite() = m_spriteSheet.getSprite(ime::Index{0, 0});
        getSprite().scale(2.0f, 2.0f);
        resetSpriteOrigin();

        // Automatically update the mushroom texture on bullet collision
        onCollision([this](ime::GameObject*, ime::GameObject* other) {
            if (other->getClassName() == "Bullet") {
                m_hitCount++;

                // Inactive objects are destroyed at the end of the current frame
                if (m_hitCount == MAX_BULLET_HITS)
                    setActive(false);
                else
                    getSprite().setTextureRect(*m_spriteSheet.getFrame(ime::Index{m_isPoisoned ? 1 : 0, static_cast<int>(m_hitCount)}));
            }
        });
    }

    ///////////////////////////////////////////////////////////////
    Mushroom::Ptr Mushroom::create(ime::Scene &scene) {
        return std::make_unique<Mushroom>(scene);
    }

    ///////////////////////////////////////////////////////////////
    void Mushroom::setPoisoned(bool poison) {
        if (m_isPoisoned != poison) {
            m_isPoisoned = poison;
            getSprite().setTextureRect(*m_spriteSheet.getFrame(ime::Index{1, static_cast<int>(m_hitCount)}));
            emitChange(ime::Property{"poisoned", m_isPoisoned});
        }
    }

    ///////////////////////////////////////////////////////////////
    bool Mushroom::isPoisoned() const {
        return m_isPoisoned;
    }

    ///////////////////////////////////////////////////////////////
    unsigned int Mushroom::getHitCount() const {
        return m_hitCount;
    }

    ///////////////////////////////////////////////////////////////
    std::string Mushroom::getClassName() const {
        return "Mushroom";
    }
}
