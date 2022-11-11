// Copyright 2022 Forrest Boyer

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Window.hpp>

typedef enum {up, left, down, right} direction;
sf::RenderWindow window(sf::VideoMode(1, 1), "Sokoban");

sf::Texture block_06;
sf::Texture crate_03;
sf::Texture environment_03;
sf::Texture ground_01;
sf::Texture ground_04;
sf::Texture player_05;
sf::Texture player_08;
sf::Texture player_17;
sf::Texture player_20;

class Sokoban: public sf::Drawable {
    friend std::istream& operator>>(std::istream& in, Sokoban& rhs);
    friend std::ostream& operator<<(std::ostream& out, Sokoban& rhs);

 public:
    void undo() {
        if (curMove >= 1) {
            leveldata = levelhistory[curMove - 1];
            levelhistory.pop_back();
            curMove--;
        }
    }
    void reset() {
        leveldata = levelhistory[0];
        while (levelhistory.size() > 1) {
            levelhistory.pop_back();
        }
        playerTexture = player_05;
        curMove = 0;
    }
    bool isWon() const {
        int boxCount = 0;
        int zoneCount = 0;
        for (auto row : leveldata) {
            boxCount += std::count_if(row.begin(), row.end(), [](char c) {return c == 'O';});
            zoneCount += std::count_if(row.begin(), row.end(), [](char c) {return (c == '*' || c == '!');});
        }

        if (boxCount == 0 || zoneCount == 0) {
            return true;
        } else {
            return false;
        }
    }
    void moveplayer(direction d) {
        if (!isWon()) {
            for (size_t row = 0; row < leveldata.size() - 1; row++) {
                for (size_t col = 0; col < leveldata[col].size() - 1; col++) {
                    if (leveldata.at(row).at(col) == '@' || leveldata.at(row).at(col) == '!') {
                        char leave;
                        // Checks if player is moving from normal ground or end zone chooses what tile to leave behind when player moves
                        if (leveldata.at(row).at(col) == '@') {
                            leave = '.';
                        } else {
                            leave = '*';
                        }

                        size_t useRow;
                        size_t useCol;
                        size_t useRow2;
                        size_t useCol2;
                        switch (d) {
                            case up:
                                useRow = row - 1;
                                useRow2 = row - 2;
                                useCol = col;
                                useCol2 = col;
                                playerTexture = player_08;
                                break;
                            case left:
                                useRow = row;
                                useRow2 = row;
                                useCol = col - 1;
                                useCol2 = col - 2;
                                playerTexture = player_20;
                                break;
                            case down:
                                useRow = row + 1;
                                useRow2 = row + 2;
                                useCol = col;
                                useCol2 = col;
                                playerTexture = player_05;
                                break;
                            case right:
                                useRow = row;
                                useRow2 = row;
                                useCol = col + 1;
                                useCol2 = col + 2;
                                playerTexture = player_17;
                                break;
                            default:
                                break;
                        }

                        // Moving into wall
                        if (leveldata[useRow][useCol] == '#') {
                            break;
                        // Moving into box
                        } else if (leveldata[useRow][useCol] == 'O') {
                            // Box is moving into open space
                            if (leveldata[useRow2][useCol2] == '.') {
                                leveldata[useRow2][useCol2] = 'O';
                                leveldata[useRow][useCol] = '@';
                                leveldata[row][col] = leave;
                            // Box is moving into end zone
                            } else if (leveldata[useRow2][useCol2] == '*') {
                                leveldata[useRow2][useCol2] = '$';
                                leveldata[useRow][useCol] = '@';
                                leveldata[row][col] = leave;
                            }
                        // Moving into end zone
                        } else if (leveldata[useRow][useCol] == '*') {
                            leveldata[useRow][useCol] = '!';
                            leveldata[row][col] = leave;
                        // Moving into box on top of end zone
                        } else if (leveldata[useRow][useCol] == '$') {
                            // Box is moving into open space
                            if (leveldata[useRow2][useCol2] == '.') {
                                leveldata[useRow2][useCol2] = 'O';
                                leveldata[useRow][useCol] = '!';
                                leveldata[row][col] = leave;
                            }
                        // Moving into open space
                        } else {
                            leveldata[useRow][useCol] = '@';
                            leveldata[row][col] = leave;
                        }
                        // If the loop was entered that means movement happens and needs to exit to avoid chains
                        levelhistory.push_back(leveldata);
                        curMove++;
                        goto end;
                    }
                }
            }
            end:
                return;
        }
    }

    int getMapWidth() {
        return mapWidth;
    }
    int getMapHeight() {
        return mapHeight;
    }

 private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
        int x = 0;
        int y = 0;
        for (auto row : leveldata) {
            x = 0;
            for (auto col : row) {
                // Symbol for wall
                if (col == '#') {
                    sf::Sprite sprite;
                    sprite.setTexture(block_06);
                    sprite.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                }
                // Symbol for player
                if (col == '@') {
                    sf::Sprite sprite;
                    sf::Sprite sprite2;
                    sprite.setTexture(ground_01);
                    sprite2.setTexture(playerTexture);
                    sprite.setPosition(sf::Vector2f(x, y));
                    sprite2.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                    target.draw(sprite2);
                }
                // Symbol for normal ground
                if (col == '.') {
                    sf::Sprite sprite;
                    sprite.setTexture(ground_01);
                    sprite.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                }
                // Symbol for box
                if (col == 'O') {
                    sf::Sprite sprite;
                    sf::Sprite sprite2;
                    sprite.setTexture(ground_01);
                    sprite2.setTexture(crate_03);
                    sprite.setPosition(sf::Vector2f(x, y));
                    sprite2.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                    target.draw(sprite2);
                }
                // Symbol for end zone
                if (col == '*') {
                    sf::Sprite sprite;
                    sprite.setTexture(ground_04);
                    sprite.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                }
                // Symbol for box on end zone
                if (col == '$') {
                    sf::Sprite sprite;
                    sf::Sprite sprite2;
                    sprite.setTexture(ground_04);
                    sprite2.setTexture(crate_03);
                    sprite.setPosition(sf::Vector2f(x, y));
                    sprite2.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                    target.draw(sprite2);
                }
                // Symbol for player on end zone
                if (col == '!') {
                    sf::Sprite sprite;
                    sf::Sprite sprite2;
                    sprite.setTexture(ground_04);
                    sprite2.setTexture(playerTexture);
                    sprite.setPosition(sf::Vector2f(x, y));
                    sprite2.setPosition(sf::Vector2f(x, y));
                    target.draw(sprite);
                    target.draw(sprite2);
                }
                x += 64;
            }
            y += 64;
        }
    }
    std::vector<std::vector<char>> leveldata;
    std::vector<std::vector<std::vector<char>>> levelhistory;
    int curMove = 0;
    int mapWidth;
    int mapHeight;
    sf::Texture playerTexture = player_05;
};

std::istream& operator>>(std::istream& in, Sokoban& rhs) {
    std::string str;
    std::string width;
    std::string height;
    getline(in, str);
    rhs.mapWidth = stoi(str.substr(0, str.find(' ')));
    rhs.mapHeight = stoi(str.substr(str.find(' '), str.find('\n')));

    while (getline(in, str)) {
        rhs.leveldata.push_back(std::vector<char>(str.begin(), str.end()));
    }

    rhs.levelhistory.push_back(rhs.leveldata);

    return in;
}

std::ostream& operator<<(std::ostream& out, Sokoban& rhs) {
    for (size_t x = 0; x < rhs.leveldata.size(); x++) {
            for (size_t y = 0; y < rhs.leveldata[x].size(); y++) {
            std::cout << rhs.leveldata[x][y];
        }
        std::cout << std::endl;
    }
    return out;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Incorrect number of command line arguments" << std::endl;
        return -1;
    }

    block_06.loadFromFile("block_06.png");
    crate_03.loadFromFile("crate_03.png");
    environment_03.loadFromFile("environment_03.png");
    ground_01.loadFromFile("ground_01.png");
    ground_04.loadFromFile("ground_04.png");
    player_05.loadFromFile("player_05.png");
    player_08.loadFromFile("player_08.png");
    player_17.loadFromFile("player_17.png");
    player_20.loadFromFile("player_20.png");

    Sokoban sokoban{};

    std::ifstream file(argv[1]);
    file >> sokoban;

    window.create(sf::VideoMode(sokoban.getMapWidth() * 64, sokoban.getMapHeight() * 64), "Sokoban");
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyPressed:
                    switch (event.key.code) {
                        case sf::Keyboard::W:
                            sokoban.moveplayer(up);
                            break;
                        case sf::Keyboard::A:
                            sokoban.moveplayer(left);
                            break;
                        case sf::Keyboard::S:
                            sokoban.moveplayer(down);
                            break;
                        case sf::Keyboard::D:
                            sokoban.moveplayer(right);
                            break;
                        case sf::Keyboard::R:
                            sokoban.reset();
                            break;
                        case sf::Keyboard::Z:
                            sokoban.undo();
                            break;
                        default:
                            break;
                    }
                // Drawing
                // std::cout << sokoban << std::endl;
                default:
                    break;
            }
        }

        // CLOCK STUFF
        sf::Time time;
        int seconds = clock.getElapsedTime().asSeconds();
        int minutes = seconds / 60;
        seconds -= minutes * 60;
        std::string timeStr;
        if (minutes < 10) {
            timeStr = '0';
        }
        if (seconds < 10) {
            timeStr += std::to_string(minutes) + ":0" + std::to_string(seconds);
        } else {
        timeStr += std::to_string(minutes) + ":" + std::to_string(seconds);
        }
        sf::Font font;
        font.loadFromFile("arial.ttf");
        sf::Text text(timeStr, font);
        text.setScale(0.8, 0.8);
        text.setPosition(sf::Vector2f(2, 15));

        // WIN MESSAGE
        sf::Text winText("Congratulations You Won!", font);
        winText.setPosition(sf::Vector2f(128, 128));

        // DRAWING
        window.clear();
        window.draw(sokoban);
        window.draw(text);
        if (sokoban.isWon()) {
            window.draw(winText);
        }
        window.display();
    }

    return 0;
}
