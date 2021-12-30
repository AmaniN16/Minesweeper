//Here we go...
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "TextureManager.h"
#include "Tile.h"
#include "Mine.h"
using namespace std;

void LoadBoard(string fileName, vector<Board>& allBoards) {
    int width;
    int height;
    int mines;

    ifstream infile(fileName);

    if (infile.is_open()) {
        infile >> width;
        infile >> height;
        infile >> mines;

        Board playBoard(width, height, mines);
        allBoards.push_back(playBoard);
    }
}

void CreateMines(Board& board, vector<vector<Tile>>& tiles, vector<Mine>& mines, sf::RenderWindow& window) {

    for (int i = 0; i <= board.mineCount; i++) {
        float randomX = rand() % ((board.windowHeight - 88) / 32);
        float randomY = rand() % (board.windowWidth / 32);
        auto tilePos = tiles[randomX][randomY].sprite.getPosition();

        Mine currMine(tilePos.x, tilePos.y, false);
        mines.push_back(currMine);
    }

}

void InitBoard(Board& board, vector<vector<Tile>>& tiles, sf::RenderWindow& window) {

    sf::RectangleShape background(sf::Vector2f(board.windowWidth, 88));
    background.setPosition(0, board.windowHeight - 88);
    window.draw(background);
    //Draw tiles
    for (int i = 0; i < (board.windowHeight - 88) / 32; i++) {
        tiles.push_back(vector<Tile>());
        for (int j = 0; j < board.windowWidth / 32; j++) {
            bool hidden = true;
            Tile currTile((float)j, (float)i, hidden);
            tiles[i].push_back(currTile);

        }
    }

}

void DrawTiles(bool& toggle, Board& board, vector<vector<Tile>>& tiles, vector<Mine>& mines, sf::RenderWindow& window)
{

    sf::Sprite revTile(TextureManager::GetTexture("tile_revealed"));
    sf::Sprite flag(TextureManager::GetTexture("flag"));

    //Checking for loss
    for (int m = 0; m < board.mineCount; m++) {
        if (mines[m].lost == true) {
            sf::Sprite faceSad(TextureManager::GetTexture("face_lose"));
            faceSad.setPosition(((board.windowWidth - 32.f) / 2.f), (board.windowHeight - 88.f));
            window.draw(faceSad);

            //Draw losing tiles
            
            for (int x = 0; x < (board.windowHeight - 88) / 32; x++) {
                for (int y = 0; y < board.windowWidth / 32; y++) {
                    for (int l = 0; l < board.mineCount; l++) {
                        if (tiles[x][y].sprite.getGlobalBounds().contains(mines[l].xPos, mines[l].yPos)) {
                            revTile.setPosition(tiles[x][y].xPos, tiles[x][y].yPos);
                            window.draw(revTile);
                        }

                        else if (tiles[x][y].hidden == false && !tiles[x][y].flagged) {
                            revTile.setPosition(tiles[x][y].xPos, tiles[x][y].yPos);
                            window.draw(revTile);
                        }

                        else
                            window.draw(tiles[x][y].sprite);
                    }
                }
            }
            
            for (int m = 0; m < board.mineCount; m++) {
                window.draw(mines[m].mine);
            }
            return;
        }
    }
    
    if (toggle == true) {
        for (int i = 0; i < (board.windowHeight - 88) / 32; i++) {
            for (int j = 0; j < board.windowWidth / 32; j++)
                for (int m = 0; m < board.mineCount; m++) {
                    bool containsMine = true;
                    window.draw(mines[m].mine);
                    
                    if (!tiles[i][j].sprite.getGlobalBounds().contains(mines[m].xPos, mines[m].yPos)) 
                        containsMine = false;

                    if (tiles[i][j].hidden == true && !containsMine && !tiles[i][j].flagged)
                        window.draw(tiles[i][j].sprite);

                    else if (tiles[i][j].hidden == true && tiles[i][j].flagged) {
                        window.draw(tiles[i][j].sprite);
                        flag.setPosition(tiles[i][j].xPos, tiles[i][j].yPos);
                        window.draw(flag);
                    }

                    else if(tiles[i][j].hidden == false && !containsMine && !tiles[i][j].flagged) {
                        revTile.setPosition(tiles[i][j].xPos, tiles[i][j].yPos);
                        window.draw(revTile);
                    }

                    
                    
                }
        }
    }
    
    else {
        for (int i = 0; i < (board.windowHeight - 88) / 32; i++) {
            for (int j = 0; j < board.windowWidth / 32; j++) {
                if (tiles[i][j].hidden == true && tiles[i][j].flagged) {
                    window.draw(tiles[i][j].sprite);
                    flag.setPosition(tiles[i][j].xPos, tiles[i][j].yPos);
                    window.draw(flag);
                }

                else if (tiles[i][j].hidden == true)
                    window.draw(tiles[i][j].sprite);

                else if (tiles[i][j].hidden == false && !tiles[i][j].flagged) {
                    revTile.setPosition(tiles[i][j].xPos, tiles[i][j].yPos);
                    window.draw(revTile);
                }


            }
        }

    }


}

bool ContainsHunsDigit(int value, int digit) {
    value /= 100;
    int tempDigit = value % 10;
    if (tempDigit == digit)
        return true;
    
    return false;
    
}

bool ContainsTensDigit(int value, int digit) {
    value /= 10;
    int tempDigit = value % 10;
    if (tempDigit == digit)
        return true;

    return false;
}

bool ContainsOnesDigit(int value, int digit) {
    int tempDigit = value % 10;
    if (tempDigit == digit)
        return true;

    return false;
}

void DrawDigits(int numMines, Board& board, sf::RenderWindow& window) {
    //To draw the digits
    float menuHeight = board.windowHeight - 88;
    

    if (numMines < 100) {
        sf::Sprite zero(TextureManager::GetTexture("digits"));
        zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
        zero.setPosition(25.f, menuHeight);
        window.draw(zero);
    }

    //Draw digit in the 100's place
    else {
        if (ContainsHunsDigit(numMines, 1)) {
            sf::Sprite one(TextureManager::GetTexture("digits"));
            one.setTextureRect(sf::IntRect(21, 0, 21, 32));
            one.setPosition(25.f, menuHeight);
            window.draw(one);
        }

        else if (ContainsHunsDigit(numMines, 2)) {
            sf::Sprite two(TextureManager::GetTexture("digits"));
            two.setTextureRect(sf::IntRect(42, 0, 21, 32));
            two.setPosition(25.f, menuHeight);
            window.draw(two);
        }

        else if (ContainsHunsDigit(numMines, 3)) {
            sf::Sprite three(TextureManager::GetTexture("digits"));
            three.setTextureRect(sf::IntRect(63, 0, 21, 32));
            three.setPosition(25.f, menuHeight);
            window.draw(three);
        }

        else if (ContainsHunsDigit(numMines, 4)) {
            sf::Sprite four(TextureManager::GetTexture("digits"));
            four.setTextureRect(sf::IntRect(84, 0, 21, 32));
            four.setPosition(25.f, menuHeight);
            window.draw(four);
        }

        else if (ContainsHunsDigit(numMines, 5)) {
            sf::Sprite five(TextureManager::GetTexture("digits"));
            five.setTextureRect(sf::IntRect(105, 0, 21, 32));
            five.setPosition(25.f, menuHeight);
            window.draw(five);
        }

        else if (ContainsHunsDigit(numMines, 6)) {
            sf::Sprite six(TextureManager::GetTexture("digits"));
            six.setTextureRect(sf::IntRect(126, 0, 21, 32));
            six.setPosition(25.f, menuHeight);
            window.draw(six);
        }

        else if (ContainsHunsDigit(numMines, 7)) {
            sf::Sprite seven(TextureManager::GetTexture("digits"));
            seven.setTextureRect(sf::IntRect(147, 0, 21, 32));
            seven.setPosition(25.f, menuHeight);
            window.draw(seven);
        }

        else if (ContainsHunsDigit(numMines, 8)) {
            sf::Sprite eight(TextureManager::GetTexture("digits"));
            eight.setTextureRect(sf::IntRect(168, 0, 21, 32));
            eight.setPosition(25.f, menuHeight);
            window.draw(eight);
        }

        else if (ContainsHunsDigit(numMines, 9)) {
            sf::Sprite nine(TextureManager::GetTexture("digits"));
            nine.setTextureRect(sf::IntRect(189, 0, 21, 32));
            nine.setPosition(25.f, menuHeight);
            window.draw(nine);
        }


    }

    //Draw digit in the ten's place
    
        if (ContainsTensDigit(numMines, 0)) {
            sf::Sprite zero(TextureManager::GetTexture("digits"));
            zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
            zero.setPosition(46.f, menuHeight);
            window.draw(zero);
        }

        else if (ContainsTensDigit(numMines, 1)) {
            sf::Sprite one(TextureManager::GetTexture("digits"));
            one.setTextureRect(sf::IntRect(21, 0, 21, 32));
            one.setPosition(46.f, menuHeight);
            window.draw(one);
        }

        else if (ContainsTensDigit(numMines, 2)) {
            sf::Sprite two(TextureManager::GetTexture("digits"));
            two.setTextureRect(sf::IntRect(42, 0, 21, 32));
            two.setPosition(46.f, menuHeight);
            window.draw(two);
        }

        else if (ContainsTensDigit(numMines, 3)) {
            sf::Sprite three(TextureManager::GetTexture("digits"));
            three.setTextureRect(sf::IntRect(63, 0, 21, 32));
            three.setPosition(46.f, menuHeight);
            window.draw(three);
        }

        else if (ContainsTensDigit(numMines, 4)) {
            sf::Sprite four(TextureManager::GetTexture("digits"));
            four.setTextureRect(sf::IntRect(84, 0, 21, 32));
            four.setPosition(46.f, menuHeight);
            window.draw(four);
        }

        else if (ContainsTensDigit(numMines, 5)) {
            sf::Sprite five(TextureManager::GetTexture("digits"));
            five.setTextureRect(sf::IntRect(105, 0, 21, 32));
            five.setPosition(46.f, menuHeight);
            window.draw(five);
        }

        else if (ContainsTensDigit(numMines, 6)) {
            sf::Sprite six(TextureManager::GetTexture("digits"));
            six.setTextureRect(sf::IntRect(126, 0, 21, 32));
            six.setPosition(46.f, menuHeight);
            window.draw(six);
        }

        else if (ContainsTensDigit(numMines, 7)) {
            sf::Sprite seven(TextureManager::GetTexture("digits"));
            seven.setTextureRect(sf::IntRect(147, 0, 21, 32));
            seven.setPosition(46.f, menuHeight);
            window.draw(seven);
        }

        else if (ContainsTensDigit(numMines, 8)) {
            sf::Sprite eight(TextureManager::GetTexture("digits"));
            eight.setTextureRect(sf::IntRect(168, 0, 21, 32));
            eight.setPosition(46.f, menuHeight);
            window.draw(eight);
        }

        else if (ContainsTensDigit(numMines, 9)) {
            sf::Sprite nine(TextureManager::GetTexture("digits"));
            nine.setTextureRect(sf::IntRect(189, 0, 21, 32));
            nine.setPosition(46.f, menuHeight);
            window.draw(nine);
        }
    

    //Draw digit in the one's place
    
        if (ContainsOnesDigit(numMines, 0)) {
            sf::Sprite zero(TextureManager::GetTexture("digits"));
            zero.setTextureRect(sf::IntRect(0, 0, 21, 32));
            zero.setPosition(67.f, menuHeight);
            window.draw(zero);
        }

        else if (ContainsOnesDigit(numMines, 1)) {
            sf::Sprite one(TextureManager::GetTexture("digits"));
            one.setTextureRect(sf::IntRect(21, 0, 21, 32));
            one.setPosition(67.f, menuHeight);
            window.draw(one);
        }

        else if (ContainsOnesDigit(numMines, 2)) {
            sf::Sprite two(TextureManager::GetTexture("digits"));
            two.setTextureRect(sf::IntRect(42, 0, 21, 32));
            two.setPosition(67.f, menuHeight);
            window.draw(two);
        }

        else if (ContainsOnesDigit(numMines, 3)) {
            sf::Sprite three(TextureManager::GetTexture("digits"));
            three.setTextureRect(sf::IntRect(63, 0, 21, 32));
            three.setPosition(67.f, menuHeight);
            window.draw(three);
        }

        else if (ContainsOnesDigit(numMines, 4)) {
            sf::Sprite four(TextureManager::GetTexture("digits"));
            four.setTextureRect(sf::IntRect(84, 0, 21, 32));
            four.setPosition(67.f, menuHeight);
            window.draw(four);
        }

        else if (ContainsOnesDigit(numMines, 5)) {
            sf::Sprite five(TextureManager::GetTexture("digits"));
            five.setTextureRect(sf::IntRect(105, 0, 21, 32));
            five.setPosition(67.f, menuHeight);
            window.draw(five);
        }

        else if (ContainsOnesDigit(numMines, 6)) {
            sf::Sprite six(TextureManager::GetTexture("digits"));
            six.setTextureRect(sf::IntRect(126, 0, 21, 32));
            six.setPosition(67.f, menuHeight);
            window.draw(six);
        }

        else if (ContainsOnesDigit(numMines, 7)) {
            sf::Sprite seven(TextureManager::GetTexture("digits"));
            seven.setTextureRect(sf::IntRect(147, 0, 21, 32));
            seven.setPosition(67.f, menuHeight);
            window.draw(seven);
        }

        else if (ContainsOnesDigit(numMines, 8)) {
            sf::Sprite eight(TextureManager::GetTexture("digits"));
            eight.setTextureRect(sf::IntRect(168, 0, 21, 32));
            eight.setPosition(67.f, menuHeight);
            window.draw(eight);
        }

        else if (ContainsOnesDigit(numMines, 9)) {
            sf::Sprite nine(TextureManager::GetTexture("digits"));
            nine.setTextureRect(sf::IntRect(189, 0, 21, 32));
            nine.setPosition(67.f, menuHeight);
            window.draw(nine);
        }
    

}

void setConfig(int which,  Board& board, vector<vector<Tile>>& tiles, vector<Mine>& mines, sf::RenderWindow& window) {

}

void LeftMouseClicks(bool& toggle, int& click, Board& board, vector<vector<Tile>>& tiles, vector<Mine>& mines, vector<sf::Sprite>& menu, sf::RenderWindow& window){
    sf::RectangleShape tileSpace(sf::Vector2f(board.windowWidth, (board.windowHeight - 88)));
    sf::RectangleShape menuSpace(sf::Vector2f(board.windowWidth, 88));
    menuSpace.setPosition(0, board.windowHeight - 88);
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
   

    //Click mouse in tile space
    if (tileSpace.getGlobalBounds().contains(mousePos)) {
        sf::Sprite revTile;
        
        for (int i = 0; i < (board.windowHeight - 88) / 32; i++) {
            for (int j = 0; j < board.windowWidth / 32; j++) {;
                if (tiles[i][j].sprite.getGlobalBounds().contains(mousePos)) {
                    //Check if there is a mine there

                    if (tiles[i][j].hidden = true) {
                        tiles[i][j].hidden = false;

                        for (int k = 0; k < board.mineCount; k++) {

                            if (tiles[i][j].sprite.getGlobalBounds().contains(mines[k].xPos, mines[k].yPos)) {
                                mines[k].lost = true;
                               
                            }

                        }


                    }
                }


            }

        }
    }

    //Click mouse in menu space
    if (menu[1].getGlobalBounds().contains(mousePos))
        click = 1;

    else if (menu[2].getGlobalBounds().contains(mousePos))
        click = 2;

    else if (menu[3].getGlobalBounds().contains(mousePos))
        click = 3;

    else if (menu[4].getGlobalBounds().contains(mousePos))
        click = 4;

    else if (menu[0].getGlobalBounds().contains(mousePos))
        click = 5;


    switch (click) {
    case 1:
        toggle = true;
        break;

    case 2:
        mines.clear();
        setConfig(click, board, tiles, mines, window);
        break;

    case 3:
        mines.clear();
        setConfig(click, board, tiles, mines, window);
        break;

    case 4:
        mines.clear();
        setConfig(click, board, tiles, mines, window);
        break;

    case 5:
        for (int m = 0; m < board.mineCount; m++) {
            mines[m].flagged = false;
        }
      
        break;
        

    }
}

void RightMouseClicks(int& value, Board& board, vector<vector<Tile>>& tiles, vector<Mine>& mines, sf::RenderWindow& window) {
    sf::RectangleShape tileSpace(sf::Vector2f(board.windowWidth, (board.windowHeight - 88)));
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));

    
    //Click mouse in tile space
    if (tileSpace.getGlobalBounds().contains(mousePos)) {
        for (int i = 0; i < (board.windowHeight - 88) / 32; i++) {
            for (int j = 0; j < board.windowWidth / 32; j++) {
                if (tiles[i][j].sprite.getGlobalBounds().contains(mousePos) && tiles[i][j].hidden && !tiles[i][j].flagged) {
                    tiles[i][j].flagged = true;
                    value--;
                    

                }

                else if (tiles[i][j].sprite.getGlobalBounds().contains(mousePos) && tiles[i][j].hidden && tiles[i][j].flagged) {
                    tiles[i][j].flagged = false;
                    value++;
                }
            }
        }
    }
}

int main()
{
    string configFile = "boards/config.cfg";
    vector<Board> boards;
    LoadBoard(configFile, boards);
    int bc = 0;
    sf::RenderWindow board(sf::VideoMode(boards[bc].windowWidth, boards[bc].windowHeight), "Minesweeper");
    vector<vector<Tile>> tiles;
    vector<Tile*> adjacentTiles;
    vector<Mine> mines;
    vector<sf::Sprite> menu;
    sf::Sprite debugS(TextureManager::GetTexture("debug"));
    sf::Sprite test1S(TextureManager::GetTexture("test_1"));
    sf::Sprite test2S(TextureManager::GetTexture("test_2"));
    sf::Sprite test3S(TextureManager::GetTexture("test_3"));
    float menuHeight = boards[bc].windowHeight - 88.f;
    float boardWidth = boards[bc].windowWidth;

    int mouseClick = 0;
    
    //Menu initialization
    sf::Sprite faceMid(TextureManager::GetTexture("face_happy"));
    faceMid.setPosition(((boards[bc].windowWidth - 32.f) / 2.f), (boards[bc].windowHeight - 88.f));
    //menu[0]
    menu.push_back(faceMid);

    //menu[1]
    debugS.setPosition((boardWidth - 290.f), menuHeight);
    menu.push_back(debugS);

    //menu[2]
    test1S.setPosition((boardWidth - 226.f), menuHeight);
    menu.push_back(test1S);

    //menu[3]
    test2S.setPosition((boardWidth - 162.f), menuHeight);
    menu.push_back(test2S);

    //menu[4]
    test3S.setPosition((boardWidth - 98.f), menuHeight);
    menu.push_back(test3S);

    bool toggle = false;
    int numMines = boards[bc].mineCount;
    

    while (board.isOpen())
    {

        // check all the window's events that were triggered since the last iteration of the loop
        sf::Event event;
        if (board.isOpen()) {
            board.clear();
            InitBoard(boards[bc], tiles, board);
            
            //Draw everything else
            //Draw smiley

            board.draw(faceMid);

            //Draw other buttons              
            board.draw(debugS);
            board.draw(test1S);
            board.draw(test2S);
            board.draw(test3S);

            DrawDigits(numMines, boards[bc], board);


            //Create mines (not drawing yet)
            CreateMines(boards[bc], tiles, mines, board);
            DrawTiles(toggle, boards[bc], tiles, mines, board);

            while (board.pollEvent(event))
            {
                // "close requested" event: we close the window
                if (event.type == sf::Event::Closed)
                    board.close();

                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        LeftMouseClicks(toggle, mouseClick, boards[bc], tiles, mines, menu, board);
                    }

                    else if (event.mouseButton.button == sf::Mouse::Right) {
                        RightMouseClicks(numMines, boards[bc], tiles, mines, board);
                    }
                }
            }

            
        }
        board.display();
    }

    TextureManager::Clear();

    return 0;
}
