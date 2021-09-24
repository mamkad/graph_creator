#ifndef NODE_HPP_INCLUDED
#define NODE_HPP_INCLUDED

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <sstream>

class node
{
    size_t idx_;
    sf::RenderWindow * main_win_;
    sf::CircleShape circle_;
    sf::Text text_;

public:
    node( size_t idx, sf::RenderWindow * main_win, sf::Font & font );

    sf::Vector2f getxy() const;
    void setxy(sf::Vector2f const &);

    void update();
    void draw();

    bool contain();

    size_t get_id() const;
    void set_id(size_t);

    float get_radius() const;
    void set_color(sf::Color const & color);

    bool is_highlight_ = false;
    bool is_moves_ = false;
};

#endif // NODE_HPP_INCLUDED
