#include "input_panel.hpp"

#include <iostream>
text_input::text_input( sf::Font & font, sf::RenderWindow * main_win )
    : main_win_(main_win)
{
     text_.setFont(font);
     text_.setFillColor(sf::Color(sf::Color::White));
     text_.setCharacterSize(20);

     is_select_ = false;
     set_length(10);

     block_.setOutlineThickness(1.0f);

     set_color( sf::Color(43, 46, 53),  sf::Color::White);
     setxy({0, 0});
}

void text_input::draw()
{
    main_win_->draw(block_);
    text_.setString(stream_text_.str() + "_");
    main_win_->draw(text_);
}

void text_input::clslch()
{
    if(!(stream_text_.str().length() > 0))
        return;

    std::string tmp = stream_text_.str();
    tmp.pop_back();
    stream_text_.str("");
    stream_text_ << tmp;
}

void text_input::update(char ch)
{
    std::cout << "sdsd" << std::endl;
     if (!is_select_  || ch > CODE_SYMBOL)
        return;

     switch(ch)
     {
     case DELETE_KEY:
         clslch();
         break;

     case ENTER_KEY:
         str_ = stream_text_.str();
         is_select_ = false;
         //this->cleard();
         break;

     case ESCAPE_KEY:
         is_select_ = false;
         cleard();
         break;

     default:
         if (stream_text_.str().length() < length_)
            stream_text_ << ch;
         break;
     }
}

bool text_input::get_int(int & ret_val)
{
    try
    {
        ret_val = stoi(str_);
        cleard();
    }
    catch (std::invalid_argument const & ia)
    {
        //std::cerr << "Invalid argument: " << ia.what() << std::endl;
        return false;
    }

    return true;
}

bool text_input::get_flt(float & ret_val)
{
    try
    {
        ret_val = stof(str_);
        cleard();
    }
    catch (std::invalid_argument const & ia)
    {
        //std::cerr << "Invalid argument: " << ia.what() << std::endl;
        return false;
    }

    return true;
}

std::string text_input::get_str()
{
    std::string s = str_;
    cleard();
    return s;
}

void text_input::cleard()
{
    stream_text_.clear();
    stream_text_.str("");
    str_.clear();
    text_.setString("");
}

 void text_input::setxy(sf::Vector2f const & pos)
{
    text_.setPosition(pos.x + BAR_SIZE / 2, pos.y);
    block_.setPosition(pos.x, pos.y);
}

void text_input::set_color( sf::Color const & clr_back,  sf::Color const & clr_outline)
{
    block_.setFillColor(clr_back);
    block_.setOutlineColor(clr_outline);
}

void text_input::set_length(size_t const len)
{
    length_ = len;
    block_.setSize({13.0f * length_ + BAR_SIZE, 13.0f + BAR_SIZE / 2.0f});
}

void text_input::set_mode(bool const mode, sf::Vector2f const & pos, sf::Color const & clr_back,  sf::Color const & clr_outline, size_t const len)
{
    is_select_ = mode;
    setxy(pos);
    set_color(clr_back,  clr_outline);
    set_length(len);

    cleard();
}


bool text_input::Is_Active() const
{
    return is_select_;
}

void text_input::Set_Active(bool st)
{
    is_select_ = st;
}
