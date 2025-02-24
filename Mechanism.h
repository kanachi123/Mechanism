#pragma once
#include "SFML/Graphics.hpp"
#include <vector>
#include <memory>
#include <cmath>

namespace mech {

    struct JointPoint {
        sf::Vector2f position;

        JointPoint(float x, float y) : position(x, y) {}

        void update(sf::Vector2i& mousePos) {
            position = sf::Vector2f(mousePos.x, mousePos.y);
        }
        
    };


    class MechanismPart {
    public:
        virtual void update() = 0;
        virtual void draw(sf::RenderWindow& window) const = 0;
        virtual ~MechanismPart() = default;
    };

    class BaseLink : public MechanismPart {
    protected:
        float length = 0;
        float alpha = 0;
        JointPoint* joint = nullptr;
        JointPoint* newJoint = nullptr;

    public:
        BaseLink(float len, float ang, JointPoint* jnt) : length(len), alpha(ang), joint(jnt) {}

        JointPoint* getFirstJoint() const {
            return joint;
        }

        JointPoint* getNewJoint() const {
            return newJoint;
        }

        void setJointPosition(JointPoint* jnt) {
            newJoint = jnt;
        }
    };


    class Crank : public BaseLink {
        sf::Vertex line[2];

    public:
        Crank(float len, float ang, JointPoint* jnt) : BaseLink(len, ang, jnt) {
            if (joint != nullptr) {
                line[0].position = joint->position;
            }
            line[0].color = sf::Color::Black;
            line[1].color = sf::Color::Black;
        }

        void update() override {
            if (joint != nullptr) {
                float dx = joint->position.x - line[0].position.x;
                float dy = joint->position.y - line[0].position.y;

                alpha = std::atan2(dy, dx);

                sf::Vector2f newPosition(line[0].position.x + std::cos(alpha) * length,
                    line[0].position.y + std::sin(alpha) * length);

                line[1].position = newPosition;
                joint->position = newPosition;

                newJoint = new JointPoint(line[1].position.x, line[1].position.y);

            }
        }


        void draw(sf::RenderWindow& window) const override {
            window.draw(line, 2, sf::Lines);
        }
    };

    class Rod : public BaseLink {
        sf::Vertex line[2];

        float crankAngle = 0;
    public:
        Rod(float len,float ang, JointPoint* jnt) : BaseLink(len, 0, jnt) {
            if (joint != nullptr) {
                line[0].position = joint->position;
            }
            line[0].color = sf::Color::Red;
            line[1].color = sf::Color::Red;
        }

        void update() override {
            if (joint) {
                line[0].position = joint->position;

                crankAngle = std::atan2(line[1].position.y - joint->position.y, line[1].position.x - joint->position.x);

                line[1].position = sf::Vector2f(line[0].position.x + std::cos(crankAngle) * length,
                    line[0].position.y + std::sin(crankAngle) * length);
                newJoint = new JointPoint(line[1].position.x, line[1].position.y);
            }
        }
        void draw(sf::RenderWindow& window) const override {
            window.draw(line, 2, sf::Lines);
        }
    };



    class MechanicSystemFabric {
    protected:
        std::vector<std::unique_ptr<BaseLink>> parts;

    public:
        JointPoint jointPoint;

        MechanicSystemFabric(float x, float y) : jointPoint(x, y) {}

        template <typename T>
        void push_mech(std::unique_ptr<T> part) {
            if (parts.empty()) {
                parts.push_back(std::move(part));
            }
            else {
                
                auto& lastPart = parts.back();
                JointPoint* newJoint = lastPart->getNewJoint();
                

                /*    C++17
                
                if constexpr (std::is_same<T, Crank>::value) {
                    parts.push_back(std::move(part));
                }
                */
                part->setJointPosition(newJoint);
                parts.push_back(std::move(part));
            }
        }

        void updateParts() {
            for (auto& part : parts) {
                part->update();
            }
        }
        inline std::vector<std::unique_ptr<BaseLink>>&  getParts() {
            return parts;
        }

        void drawParts(sf::RenderWindow& window) const {
            for (const auto& part : parts) {
                part->draw(window);
            }
        }


        
    /*
        sf::Vector2f getJointPosition() const {
            return sf::Vector2f(jointPoint.position.x, jointPoint.position.y);
        }   
    */  
        
        
        
    };
}
