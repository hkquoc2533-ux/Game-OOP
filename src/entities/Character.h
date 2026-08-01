#pragma once
#include <string>
#include <memory>
#include <SFML/Graphics.hpp>
#include "../interfaces/IGameObject.h"
#include "../interfaces/IMoveable.h"

#include "../systems/Animator.h"

// Lop truu tuong (Abstraction) dai dien cho MOI nhan vat nguoi choi co the
// dieu khien (Dau si, Phap su, Xa thu, Do don deu ke thua tu day).
//
// - Dong goi (Encapsulation): du lieu (mau, no...) la private/protected,
//   chi thao tac qua ham cong khai (takeDamage, heal, gainRage...).
// - Da hinh (Polymorphism): basicAttack() va useSkill() la ham ao thuan tuy,
//   moi lop con tu dinh nghia cach danh rieng cua minh.
class Character : public IGameObject, public IMoveable {
public:
    Character(std::string name, int maxHp, int maxRage, float moveSpeed);
    virtual ~Character() override = default;

    // ---- IGameObject ----
    void update(float deltaTime) override;
    void draw(sf::RenderWindow& window) override;

    // ---- IMoveable ----
    void move(const sf::Vector2f& direction, float deltaTime) override;
    sf::Vector2f getPosition() const override;
    void setPosition(const sf::Vector2f& position) override;

    // ---- Combat: da hinh, moi nhan vat cai dat rieng ----
    virtual void basicAttack(Character& target) = 0;      // Danh thuong: hoi mot phan no
    virtual void useSkill(int skillIndex, Character& target) = 0; // skillIndex: 0..2 (3 chieu)
    virtual std::string getClassName() const = 0;

    // ---- Truy cap trang thai (da dong goi) ----
    virtual void takeDamage(int amount);
    void heal(int amount);
    void gainRage(int amount);
    bool spendRage(int amount); // tra ve false neu khong du no
    void restoreState(int hp, int rage); // nap lai mau/no da luu tu GameSession
    void scaleMaxHp(float factor);
    void setCurrentFloor(int floor) { currentFloor = floor; }
    int getCurrentFloor() const { return currentFloor; }

    bool isAlive() const { return currentHp > 0; }
    int getHp() const { return currentHp; }
    int getMaxHp() const { return maxHp; }
    int getRage() const { return currentRage; }
    int getMaxRage() const { return maxRage; }
    const std::string& getName() const { return name; }

    // Danh cho hien thi tren ban do (chon nhan vat / di chuyen giua cac tang)
    void setTexture(const sf::Texture& texture);

    // ---- Animation System ----
    void addAnimation(AnimState state, const sf::Texture* texture, int frameCount = 1, float frameDuration = 0.12f, bool loop = true);
    void playAnimation(AnimState state);

protected:
    std::string name;
    int maxHp;
    int currentHp;
    int maxRage;
    int currentRage;
    float moveSpeed;

    Animator animator;
    float animOffsetTimer = 0.f;
    sf::Vector2f visualOffset = {0.f, 0.f};

    std::unique_ptr<sf::Sprite> sprite; // tao trong setTexture, tranh phai load anh khi chua co
    sf::Vector2f position;
    int currentFloor = 1;
};
