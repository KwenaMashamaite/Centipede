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

#include "Source/Actors/Scorpion.h"
#include "Source/Actors/Mushroom.h"

namespace centpd {
    ///////////////////////////////////////////////////////////////
    Scorpion::Scorpion(ime::Scene &scene) :
        GameObject(scene)
    {
        setTag("scorpion");
        setCollisionGroup("scorpion");
        getCollisionExcludeList().add("invisibleWall");

        static auto spritesheet = ime::SpriteSheet("Spritesheet.png", ime::Vector2u{16, 8}, ime::Vector2u{0, 0}, ime::UIntRect{1, 55, 64, 8});

        // Init default texture
        ime::Sprite& sprite = getSprite();
        sprite = spritesheet.getSprite(ime::Index{0, 0}); // Set the first animation frame as the default texture
        resetSpriteOrigin();
        sprite.setScale(2.0f, 2.0f);

        // Init animation
        ime::Animation::Ptr movementAnim = ime::Animation::create("moving", spritesheet, ime::milliseconds(400));
        movementAnim->addFrames(ime::Index{0, 0}, 4);
        movementAnim->setLoop(true);
        sprite.getAnimator().addAnimation(std::move(movementAnim));
        sprite.getAnimator().startAnimation("moving");

        // Init collision response
        onCollision([this](ime::GameObject*, ime::GameObject* other) {
            if (other->getClassName() == "Bullet") {
                setActive(false);
            } else if (other->getClassName() == "Mushroom")
                static_cast<Mushroom*>(other)->setPoisoned(true);
        });
    }

    ///////////////////////////////////////////////////////////////
    Scorpion::Ptr Scorpion::create(ime::Scene &scene) {
        return std::make_unique<Scorpion>(scene);
    }

    ///////////////////////////////////////////////////////////////
    std::string Scorpion::getClassName() const {
        return "Scorpion";
    }
}
