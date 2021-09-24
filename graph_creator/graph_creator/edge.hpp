#ifndef EDGE_H
#define EDGE_H

#include <SFML/Graphics.hpp>

#include <cstddef>
#include "node.hpp"

static float const M_PI = 3.141f;
//static float const arrrow_length = 15.0f;
static float const R = 15.0f;

class edge
{
    sf::RenderWindow * main_win_;
    sf::VertexArray curve_;

    struct edge_stat
    {
        bool is_linkd_ : 1;
        bool mode_ : 1;
        bool is_highlight_ : 1;
        bool mode_of_calk_ : 1;
    } edge_stat_;

    float value_ = 0;

    sf::Text text_;
    sf::RectangleShape block_;

    node* nodes_[2] = {nullptr, nullptr};
    sf::Vertex line_[2], arrow1[2][2], arrow2[2][2];

    void calc_curve(sf::Vector2f const &);

public:
     edge(sf::RenderWindow * main_win, node * src_node, sf::Font & font_);

     void  set_src_node(node * src_node);
     node* get_src_node() const;
     void  set_dst_node(node * dst_node);
     node* get_dst_node() const;

     float get_value() const;
     void set_value(float);

     bool contain() const;
     void update();
     void draw();

     void set_color(sf::Color const & color);

     bool get_hight() const;
     void set_hight(bool st);

     void set_mode(bool st, bool w);
     bool get_mode() const;

     void setup_color();
};

#endif // EDGE_H
