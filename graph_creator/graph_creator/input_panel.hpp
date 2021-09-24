#ifndef INPUT_PANEL_HPP_INCLUDED
#define INPUT_PANEL_HPP_INCLUDED

#define DELETE_KEY 8
#define ENTER_KEY 	13
#define ESCAPE_KEY 27
#define CODE_SYMBOL 128
#define BAR_SIZE     30

#include <SFML/Graphics.hpp>

#include <cstddef>
#include <sstream>

class text_input
{
    sf::RenderWindow * main_win_;
    sf::Text text_;
    std::string str_;
    std::ostringstream stream_text_;
    size_t length_ = 10;
    sf::RectangleShape block_;
    void clslch();
    bool is_select_;

public:

    text_input( sf::Font & font, sf::RenderWindow * );

    void update(char);
    void draw();

    bool  get_int(int & ret_val);
    bool  get_flt(float & ret_val) ;
    std::string get_str();

    void cleard();
    void setxy(sf::Vector2f const & pos);

    void set_color( sf::Color const & clr_back,  sf::Color const & clr_outline);
    void set_length(size_t const len);

    void set_mode(bool const mode, sf::Vector2f const & pos, sf::Color const & clr_back,  sf::Color const & clr_outline, size_t const len);

    bool Is_Active() const;
    void Set_Active(bool st);
};

#endif // INPUT_PANEL_HPP_INCLUDED
