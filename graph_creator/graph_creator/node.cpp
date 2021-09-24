#include "node.hpp"

node::node(size_t idx, sf::RenderWindow * main_win, sf::Font & font)
    : idx_(idx), main_win_(main_win)
{
    circle_.setRadius(15.0f);
    circle_.setOrigin(circle_.getRadius(), circle_.getRadius());
    circle_.setPosition({float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y)});
    circle_.setFillColor(sf::Color(242, 242, 242));
    circle_.setOutlineThickness(2.0f);
    circle_.setOutlineColor(sf::Color(43, 46, 53));
    circle_.setPointCount(100);

    std::ostringstream out_str_stream_;
    out_str_stream_ << idx_;

    text_.setFont(font);
    text_.setString( out_str_stream_.str());
    text_.setFillColor(sf::Color(25, 25, 31));
    text_.setCharacterSize(17);
    text_.setOrigin(text_.getLocalBounds().left + text_.getLocalBounds().width/2.0f,  text_.getLocalBounds().top + text_.getLocalBounds().height/2.0);
    text_.setPosition(this->getxy());
}

sf::Vector2f node::getxy() const
{
    return circle_.getPosition();
}

void node::setxy(sf::Vector2f const & pos)
{
    circle_.setPosition(pos);
    text_.setPosition(pos);
}

void node::update()
{
    if(is_moves_)
    {
        setxy({float(sf::Mouse::getPosition(*main_win_).x),float(sf::Mouse::getPosition(*main_win_).y)});
        text_.setPosition(getxy());
    }
}

void node::draw()
{
    main_win_->draw(circle_);
    main_win_->draw(text_);
}

bool node::contain()
{
    float x = float(sf::Mouse::getPosition(*main_win_).x);
    float y = float(sf::Mouse::getPosition(*main_win_).y);

    return ( ( (x - circle_.getPosition().x) * (x - circle_.getPosition().x) + (y - circle_.getPosition().y) * (y - circle_.getPosition().y) ) <= circle_.getRadius() * circle_.getRadius() );
}

size_t node::get_id() const
{
    return idx_;
}

void node::set_id(size_t v)
{
    //==========

     this->idx_ = v;
/*
     int a = v / 10;
     int b = v % 10;*/

    //==========

    //this->idx_ = v;

    std::ostringstream out_str_stream_;
    out_str_stream_ << this->idx_;

    //out_str_stream_ << "(" << a << "," << b <<")";

    text_.setCharacterSize(15);
    text_.setString( out_str_stream_.str());
    text_.setOrigin(text_.getLocalBounds().left + text_.getLocalBounds().width/2.0f,  text_.getLocalBounds().top + text_.getLocalBounds().height/2.0);
    text_.setPosition(this->getxy());
}

float node::get_radius() const
{
    return circle_.getRadius();
}

void node::set_color(sf::Color const & color)
{
    if(!is_highlight_)
        circle_.setOutlineColor(color);
}
