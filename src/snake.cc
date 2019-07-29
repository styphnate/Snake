#include <SFML/Graphics.hpp>
#include <iostream>
#include <random>
#include <vector>

#define WIDTH 1000
#define HEIGHT 1000

template <typename T> void updatePosition(const T &xSpeed, const T &ySpeed, std::vector<sf::RectangleShape> &snake) {
  for (uint32_t i = snake.size() - 1; i >= 1; --i) {
    snake.at(i).setPosition(snake.at(i - 1).getPosition().x, snake.at(i - 1).getPosition().y);
  }
  snake.at(0).setPosition(snake.at(0).getPosition().x + xSpeed, snake.at(0).getPosition().y + ySpeed);
  if (snake.at(0).getPosition().x < 0) {
    snake.at(0).setPosition(WIDTH - WIDTH / 10, snake.at(0).getPosition().y);
  }
  if (snake.at(0).getPosition().x > WIDTH - WIDTH / 10) {
    snake.at(0).setPosition(0, snake.at(0).getPosition().y);
  }
  if (snake.at(0).getPosition().y < 0) {
    snake.at(0).setPosition(snake.at(0).getPosition().x, HEIGHT - HEIGHT / 10);
  }
  if (snake.at(0).getPosition().y > HEIGHT - HEIGHT / 10) {
    snake.at(0).setPosition(snake.at(0).getPosition().x, 0);
  }
  for(uint32_t i = 1 ; i < snake.size() ; ++i) {
    if(snake.at(0).getPosition() == snake.at(i).getPosition()) {
      snake.erase(snake.begin()+3, snake.end());
    }
  }
}

void addBody(std::vector<sf::RectangleShape> &snake) {
  constexpr static uint32_t spacing = 5;
  snake.push_back(sf::RectangleShape(sf::Vector2f(WIDTH / 10 - spacing, HEIGHT / 10 - spacing)));
  snake.at(snake.size() - 1).setFillColor(sf::Color::White);
  if (snake.size() > 1) {
    snake.at(snake.size() - 1).setPosition(snake.at(snake.size() - 2).getPosition());
  }
}

template <typename T> void checkInput(T &xSpeed, T &ySpeed, const sf::Event &event) {
  if (event.key.code == sf::Keyboard::Left) {
    if (xSpeed == 0) {
      xSpeed = -WIDTH / 10;
      ySpeed = 0;
    }
  }
  if (event.key.code == sf::Keyboard::Right) {
    if (xSpeed == 0) {
      xSpeed = WIDTH / 10;
      ySpeed = 0;
    }
  }
  if (event.key.code == sf::Keyboard::Up) {
    if (ySpeed == 0) {
      xSpeed = 0;
      ySpeed = -HEIGHT / 10;
    }
  }
  if (event.key.code == sf::Keyboard::Down) {
    if (ySpeed == 0) {
      xSpeed = 0;
      ySpeed = HEIGHT / 10;
    }
  }
}

void generateFoodLocation(sf::RectangleShape *food) {
  std::random_device dev;
  std::mt19937 rng(dev());
  std::uniform_int_distribution<std::mt19937::result_type> dis(0, 9);
  food->setPosition(dis(rng) * WIDTH / 10, dis(rng) * HEIGHT / 10);
}

void checkCollision(std::vector<sf::RectangleShape> &snake, sf::RectangleShape *food) {
  if (snake.at(0).getPosition() == food->getPosition()) {
    generateFoodLocation(food);
    addBody(snake);
  }
}

int main() {
  sf::RenderWindow window(sf::VideoMode(WIDTH, HEIGHT), "Fourier");
  window.setPosition(sf::Vector2i(sf::VideoMode::getDesktopMode().width / 2 - WIDTH / 2,
                                  sf::VideoMode::getDesktopMode().height / 2 - HEIGHT / 2));
  window.setFramerateLimit(5);

  int32_t xSpeed = WIDTH / 10;
  int32_t ySpeed = 0;
  constexpr static uint32_t spacing = 5;

  std::vector<sf::RectangleShape> snake;
  std::unique_ptr<sf::RectangleShape> food =
      std::make_unique<sf::RectangleShape>(sf::Vector2f(WIDTH / 10 - spacing, HEIGHT / 10 - spacing));
  food->setFillColor(sf::Color::Red);
  generateFoodLocation(food.get());
  addBody(snake);
  addBody(snake);
  addBody(snake);

  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        window.close();

      if (event.type == sf::Event::KeyPressed) {
        checkInput(xSpeed, ySpeed, event);
      }
    }

    updatePosition(xSpeed, ySpeed, snake);
    checkCollision(snake, food.get());

    window.clear();
    for (auto &s : snake) {
      window.draw(s);
    }
    window.draw(*food.get());
    window.display();
  }

  return 0;
}