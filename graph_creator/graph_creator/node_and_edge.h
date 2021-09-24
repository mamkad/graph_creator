#pragma once

#include <SFML/Graphics.hpp>
#include <sstream>
#include <list>
#include <functional>

class edge;

typedef bool (edge::* edge_method)();

class node
{
    size_t idx_;
    sf::RenderWindow* main_win_;
    sf::CircleShape circle_;
    sf::Text text_;

    bool is_moves_ = false;

    std::list<edge*> input_edge_;
    std::list<edge*> out_edge_;

public:
    node(size_t idx, sf::RenderWindow* main_win, sf::Font& font);

    sf::Vector2f getxy() const;
    void setxy(sf::Vector2f const&);

    void update();
    void draw();

    bool contain();

    size_t get_id() const;
    void set_id(size_t);

    float get_radius() const;
    void set_color(sf::Color const& color);

    bool is_moves() const;
    void set_moves(bool st);

    bool is_highlight_ = false;

    void add_inp_edge(edge * edge_ptr);
    void add_out_edge(edge * edge_ptr);

    void del_inp_edge();
    void del_out_edge();

    bool del_inp_edge(node * node_ptr);
    bool del_out_edge(node* node_ptr);

    std::list<edge*>::iterator begin(short);
    std::list<edge*>::iterator end(short);
};

static float const M_PI = 3.141f;
//static float const arrrow_length = 15.0f;
static float const R = 15.0f;

class edge
{
    sf::RenderWindow* main_win_;
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

    node* nodes_[2] = { nullptr, nullptr };
    sf::Vertex line_[2], arrow1[2][2], arrow2[2][2];

    void calc_curve(sf::Vector2f const&);

public:
    edge(sf::RenderWindow* main_win, node* src_node, sf::Font& font_);
    ~edge();

    void  set_src_node(node* src_node);
    node* get_src_node() const;
    void  set_dst_node(node* dst_node);
    node* get_dst_node() const;

    float get_value() const;
    void set_value(float);

    bool contain() const;
    void update();
    void draw();

    void set_color(sf::Color const& color);

    bool get_hight() const;
    void set_hight(bool st);

    void set_mode(bool st, bool w);
    bool get_mode() const;

    void setup_color();
};
