#include <iostream>
#include <SFML/Graphics.hpp>
#include <random>
#include <chrono>

#define WIDTH 800
#define HEIGHT 600
#define CANVAS_WIDTH 100
#define CANVAS_HEIGHT 75
#define SIZE 1
#define FPS 20


struct State {
    
    sf::RenderWindow window;
    sf::RenderTexture canvas;
    State(){
        window.create(sf::VideoMode({WIDTH, HEIGHT}, 8), "Sanke");
        window.setFramerateLimit(FPS);
        if (!canvas.create({CANVAS_WIDTH, CANVAS_HEIGHT})){
            std::cerr << "Unable to create render texture" << std::endl;
        }
    }
};
State state;
class GameObject {
protected:
    sf::Vector2f pos;
    sf::RectangleShape rect;
public:
    GameObject(sf::Vector2f pos, sf::Color color): pos(pos), rect(sf::Vector2f(SIZE, SIZE)){
        rect.setFillColor(color);
        rect.setPosition(pos);
    }
    sf::Vector2f getPos(void) { return pos; }
    void setPos(sf::Vector2f newPos) { 
        pos = newPos; 
        rect.setPosition(pos);
    }
    virtual void draw(void) { state.canvas.draw(rect); }
};
class Segment: public GameObject {
private:
    Segment* next;
public:
    Segment(sf::Vector2f pos): GameObject(pos, sf::Color::White), next(nullptr){ }
    void setNext(Segment* segment){ next = segment; }

    void extend() {
        if (next == nullptr){
            next = new Segment(pos);
            return;
        }
        next->extend();
    }
    bool overlaps(sf::Vector2f oPos){
        if (next == nullptr){
            return false;
        } else if (next->pos == oPos){
            return true;
        } 
        return next->overlaps(oPos);
    
    }  

    void removeLast(void){
        if (next == nullptr){
            return;
        } else if (next->next == nullptr){
            delete next;
            next = nullptr;
            return;
        } else {
            next->removeLast();
        }
    }

    void draw() override {
        state.canvas.draw(rect);
        if (next != nullptr) next->draw();
    }
};

class Snake { 
private: 
    Segment* head;
    sf::Vector2f dir;

public:
    Snake(sf::Vector2f pos): head(new Segment(pos)), dir({1,0}) { }

    Segment* getHead() { return head; } 

    void setDir(sf::Vector2f newDir){
        // Can't make 180 degree turn.
        float dot = newDir.x * dir.x + newDir.y * dir.y;
        if (dot >= 0 && dot <= std::sqrt(3)/2) {
            dir = newDir;
        }
    }

    bool isColliding(void){
        return head->overlaps(head->getPos());
    }   

    void move(){
        sf::Vector2f pos = head->getPos() + sf::Vector2f({ dir.x, dir.y });
        if (pos.x >= CANVAS_WIDTH) pos.x = 0;
        if (pos.y >= CANVAS_HEIGHT) pos.y = 0;
        if (pos.x < 0) pos.x = CANVAS_WIDTH - 1;
        if (pos.y < 0) pos.y = CANVAS_HEIGHT - 1;
        Segment* newHead = new Segment(pos);
        newHead->setNext(head);
        head = newHead;
        head->removeLast();
    }
    void draw() { head->draw(); } 
    void extend(void){ head->extend(); }
};


sf::Vector2f getDir(void){
    float dX = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) ? -1 : (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) ? 1 : 0.0f));
    float dY = (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) ? 1 : (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) ? -1 : 0.0f));
    return sf::Vector2f(dX, dY);
}



float GetRandom(float min, float max) {
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    static std::default_random_engine generator(seed);
    std::uniform_int_distribution<int> distribution(static_cast<int>(min), static_cast<int>(max));
    return static_cast<float>(distribution(generator));
}

int main(void){

    Snake snake({40, 40});
    float fX = GetRandom(0, CANVAS_WIDTH);
    float fY = GetRandom(0, CANVAS_HEIGHT);
    GameObject food({fX, fY}, sf::Color::Green);

    while (state.window.isOpen()){
        sf::Event event;
        while (state.window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                state.window.close();
            }
        }

        // Clear window with gray
        state.canvas.clear(sf::Color(20,20,20));



        // Game logic
        sf::Vector2f dir = getDir();
        if (dir.x != 0 || dir.y != 0) {
            snake.setDir(dir);
        }
        snake.move();

        if (snake.isColliding()) break;

        if (snake.getHead()->getPos() == food.getPos()){
            snake.extend();
            float nX = GetRandom(0, CANVAS_WIDTH);
            float nY = GetRandom(0, CANVAS_HEIGHT);
            std::cout << nX << ", " << nY << std::endl;
            food.setPos({nX, nY});
        }

        food.draw();
        snake.draw();
        sf::Sprite canvasSprite(state.canvas.getTexture());
        canvasSprite.setScale({
            static_cast<float>(WIDTH) / static_cast<float>(CANVAS_WIDTH), 
            static_cast<float>(HEIGHT) / static_cast<float>(CANVAS_HEIGHT)
        });

        state.window.draw(canvasSprite);
        state.window.display();

    }
    
}
