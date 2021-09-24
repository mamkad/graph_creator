#ifndef MENU_HPP_INCLUDED
#define MENU_HPP_INCLUDED

#include <SFML/Graphics.hpp>                     

#include <cstddef>
#include <iostream>

#define RESUME 0
#define LOAD 1
#define SAVE 2
#define HOTKEYS 3
#define HIGHLIGHT 4
#define EXIT 6
#define BRANCH_AND_BOUND 40


class menu
{
    sf::RenderWindow * main_win_;
    sf::RectangleShape block_[3];

    std::vector<std::vector<sf::Text>> text_;
    std::vector<std::vector<sf::Text>*> menu_state_;

    sf::Vertex line_[2];

    bool is_menu_ex_ = true;
    int idx = 0, jdx = 0;
    int cur_idx = 0;
    int offset = 0;

public:
    menu(sf::Font & font, sf::RenderWindow *);

    int update(sf::Event & event);

    void draw();
    void setxy(sf::Vector2f const & pos);

    int get_codeinp();

    bool is_clr_ = false;
    bool is_clicked_ = false;

    bool Is_Active() const;
    void Set_Active(bool st);

    void Setup(size_t i, std::vector<sf::Text> *);
    void Set_Menu(size_t m, std::vector<sf::Text> * text, size_t c_idx, bool is_clk, size_t offs);
};

#endif // MENU_HPP_INCLUDED
