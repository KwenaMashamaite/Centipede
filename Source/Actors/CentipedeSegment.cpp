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

#include "Source/Actors/CentipedeSegment.h"
#include "Source/Common/Constants.h"
#include <cassert>

namespace centpd {
    ///////////////////////////////////////////////////////////////
    CentipedeSegment::CentipedeSegment(ime::Scene &scene, Type type) :
        GameObject(scene),
        m_type{type},
        m_link{nullptr},
        m_gridMover{nullptr},
        m_isSwitchingRows{false},
        m_isDescending{true},
        m_rowChangeId{-1}
    {
        setTag("centipedeSegment");
        setCollisionGroup("centipedeSegment");

        // Objects centipede cannot collide with
        getCollisionExcludeList().add("invisibleWall");
        getCollisionExcludeList().add("scorpion");

        // Init default texture
        ime::Sprite& sprite = getSprite();
        sprite.setTexture("Spritesheet.png");
        sprite.setTextureRect({1, 0, 7, 8});
        resetSpriteOrigin();
        sprite.setScale(2.0f, 2.0f);

        // Init animations
        createAnimation(Type::Head);
        createAnimation(Type::Body);
        setDirection(ime::Right);

        // Init collision handlers
        onCollision([this](ime::GameObject*, ime::GameObject* other) {
            if (other->getClassName() == "Bullet")
                setActive(false);
            else if (other->getClassName() == "Mushroom")
                changeRow();
        });
    }

    ///////////////////////////////////////////////////////////////
    CentipedeSegment::Ptr CentipedeSegment::create(ime::Scene &scene, Type type) {
        return std::make_unique<CentipedeSegment>(scene, type);
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::setType(CentipedeSegment::Type type) {
        if (m_type != type) {
            m_type = type;
            emitChange(ime::Property{"type", m_type});
        }
    }

    ///////////////////////////////////////////////////////////////
    CentipedeSegment::Type CentipedeSegment::getType() const {
        return m_type;
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::setGridMover(ime::GridMover *gridMover) {
        assert(gridMover && "A CentipedeSegment grid mover must not be a nullptr");

        m_gridMover = gridMover;

        // Keep segment moving in its current direction
        m_gridMover->onAdjacentMoveEnd([this](ime::Index index) {
            if (!m_isSwitchingRows /*&& m_type == Type::Head*/)
                m_gridMover->requestDirectionChange(m_dir);
        });

        m_gridMover->onPropertyChange("direction", [this](const ime::Property& property) {
            setDirection(property.getValue<ime::Vector2i>());
        });

        m_gridMover->onGridBorderCollision([this] {
            changeRow();
        });

        m_gridMover->requestDirectionChange(m_dir);
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::attachSegment(CentipedeSegment *segment) {
        assert(segment && "A nullptr cannot be attached to a CentipedeSegment");
        if (m_link && this->isSameObjectAs(*segment))
            return;
        else {
            m_link = segment;
            m_link->setType(Type::Body);

            m_link->onDestruction([this] {
                m_link = nullptr;
            });

            onPropertyChange("active", [this](const ime::Property& property) {
                auto isActive = property.getValue<bool>();
                if (!isActive && m_link) {
                    m_link->setType(Type::Head);
                    m_link->updateAnimation();
                }
            });
        }
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::setDirection(const ime::Vector2i &dir) {
        if (m_dir != dir) {
            m_dir = dir;
            updateAnimation();
        }
    }

    ///////////////////////////////////////////////////////////////
    const ime::Vector2i &CentipedeSegment::getDirection() const {
        return m_dir;
    }

    ///////////////////////////////////////////////////////////////
    std::string CentipedeSegment::getClassName() const {
        return "CentipedeSegment";
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::changeRow() {
        updateVerticalMovement();
        getCollisionExcludeList().add("mushroom");
        m_isSwitchingRows = true;
        ime::Vector2i dirOnRowSwitch = m_dir * -1;

        if (m_isDescending) {
            if (m_dir == ime::Right)
                m_gridMover->requestDirectionChange(ime::DownLeft);
            else
                m_gridMover->requestDirectionChange(ime::DownRight);
        } else {
            if (m_dir == ime::Right)
                m_gridMover->requestDirectionChange(ime::UpLeft);
            else
                m_gridMover->requestDirectionChange(ime::UpRight);
        }

        // Resume normal movement after reaching row
        m_rowChangeId = m_gridMover->onAdjacentMoveEnd([this, dirOnRowSwitch](ime::Index) {
            getCollisionExcludeList().remove("mushroom");
            m_isSwitchingRows = false;
            m_gridMover->unsubscribe(m_rowChangeId);
            m_rowChangeId = -1;
            m_gridMover->requestDirectionChange(dirOnRowSwitch);
        });
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::updateVerticalMovement() {
        ime::Index curIndex = m_gridMover->getCurrentTileIndex();
        unsigned int numRows = m_gridMover->getGrid().getSizeInTiles().y;
        if (curIndex.row == numRows - 1)
            m_isDescending = false;
        else if (curIndex.row == numRows - Constants::PLAYER_AREA_HEIGHT)
            m_isDescending = true;
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::updateAnimation() {
        ime::Sprite& sprite = getSprite();
        ime::Animator& animator = sprite.getAnimator();

        // Note: The animation texture has one directional frames only (By default
        // On the texture, the centipede is facing left for horizontal movement,
        // facing down for vertical movement and facing down-left for diagonal
        // movement. So we have to flip the texture horizontally, vertically or
        // both to get the other directions

        // Reset the texture to the default orientation, so we can flip it properly below.
        auto [xScale, yScale] = sprite.getScale();
        sprite.scale(xScale < 0.0f ? -1.0f : 1.0f, yScale < 0.0f ? -1.0f : 1.0f);

        // Update animation
        if (m_dir == ime::Left || m_dir == ime::Right) {
            animator.startAnimation(std::string("movingHor") + ((m_type == Type::Head) ? "Head" : "Body"));
            if (m_dir == ime::Right)
                sprite.scale(-1.0f, 1.0f);
        } else if (m_dir == ime::Up || m_dir == ime::Down) {
            animator.startAnimation(std::string("movingVert") + ((m_type == Type::Head) ? "Head" : "Body"));
            if (m_dir == ime::Up)
                sprite.scale(1.0f, -1.0f);
        } else {
            animator.startAnimation(std::string("movingDiag") + ((m_type == Type::Head) ? "Head" : "Body"));
            if (m_dir == ime::DownRight)
                sprite.scale(-1.0f, 1.0f);
            else if (m_dir == ime::UpLeft)
                sprite.scale(1.0f, -1.0f);
            else if (m_dir == ime::UpRight)
                sprite.scale(-1.0f, -1.0f);
        }
    }

    ///////////////////////////////////////////////////////////////
    void CentipedeSegment::createAnimation(Type type) {
        ime::Sprite& sprite = getSprite();

        // Horizontal movement animations
        static auto horSpritesheet = ime::SpriteSheet("Spritesheet.png", ime::Vector2u{7, 8}, ime::Vector2u{1, 0}, ime::UIntRect{0, 0, 33, 32});
        ime::Animation::Ptr horMoveAnim = ime::Animation::create(std::string("movingHor") + ((type == Type::Head) ? "Head" : "Body"), horSpritesheet, ime::milliseconds(200));
        horMoveAnim->addFrame(ime::Index{type == Type::Head ? 0 : 2, 3});
        horMoveAnim->addFrame(ime::Index{type == Type::Head ? 1 : 3, 3});
        horMoveAnim->addFrame(ime::Index{type == Type::Head ? 0 : 2, 2});
        horMoveAnim->addFrame(ime::Index{type == Type::Head ? 1 : 3, 1});
        horMoveAnim->addFrame(ime::Index{type == Type::Head ? 0 : 2, 1});
        horMoveAnim->setLoop(true);
        sprite.getAnimator().addAnimation(std::move(horMoveAnim));

        // Diagonal animations
        static auto diagSpritesheet = ime::SpriteSheet("Spritesheet.png", ime::Vector2u{8, 8}, ime::Vector2u{0, 0}, ime::UIntRect{33, 0, 8, 32});
        ime::Animation::Ptr diaMoveAnim = ime::Animation::create(std::string("movingDiag") + ((type == Type::Head) ? "Head" : "Body"), diagSpritesheet, ime::milliseconds(200));
        diaMoveAnim->addFrames(ime::Index{type == Type::Head ? 0 : 2, 0}, 2, ime::FrameArrangement::Vertical);
        diaMoveAnim->setLoop(true);
        sprite.getAnimator().addAnimation(std::move(diaMoveAnim));

        // Vertical animations
        static auto vertSpritesheet = ime::SpriteSheet("Spritesheet.png", ime::Vector2u{8, 7}, ime::Vector2u{0, 0}, ime::UIntRect{49, 1, 16, 28});
        ime::Animation::Ptr vertMoveAnim = ime::Animation::create(std::string("movingVert") + ((type == Type::Head) ? "Head" : "Body"), vertSpritesheet, ime::milliseconds(200));
        vertMoveAnim->addFrame(ime::Index{type == Type::Head ? 1 : 3, 1});
        vertMoveAnim->addFrame(ime::Index{type == Type::Head ? 0 : 2, 1});
        vertMoveAnim->addFrame(ime::Index{type == Type::Head ? 1 : 3, 0});
        vertMoveAnim->setLoop(true);
        sprite.getAnimator().addAnimation(std::move(vertMoveAnim));
    }
}
