#pragma once

#include <SFML/Graphics.hpp>

typedef float type_t;

#include "graph_base.h"
#include "node_and_edge.h"

#include "input_panel.h"
#include "mes_box.h"

#include <list>
#include <functional>
#include <fstream>
#include <iostream>
#include <string>

using std::string;
using std::list;

class graph_program : public graph_base
{
    //===================================

    sf::RenderWindow* main_win_ = nullptr;
    sf::ContextSettings anti_alias;

    //===================================

    sf::Text text_;
    sf::RectangleShape back_ground_;

    string file_name_;
    sf::Font font_;

    //===================================

    std::ostringstream out_str_stream_;

    //===================================

    text_input * input_ = nullptr;
    //menu menu_;
    //Message_Box mes_box_;
    //SmartMenu smartMenu_;

    //graph
    //===================================

    list<node*> nodes_;
    list<node*>::iterator node_ptr;
    edge* edg_ptr;

    bool is_edge_moves_;
    bool is_nodes_moves_;
    bool is_edge_drawing_;

    matrix adj_matrix;

    size_t count_edge_;

    //===================================

    size_t max_node_count_ = 256;

    //===================================

    bool add_node();
    bool add_edge(node* src_node);

    node* last_node();
    edge* last_edge();

    bool check_links(size_t src_node_id, size_t dst_node_id);

    bool update_graph(sf::Event& event);
    bool update_input(sf::Event& event);

    graph_program(graph_program const& other);
    graph_program& operator = (graph_program& other);

    sf::Vector2f mouse_pos() const;

    //===================================

    bool is_now_edge_drawing() const noexcept
    {
        return is_edge_drawing_;
    }

    bool is_now_edge_moves() const noexcept
    {
        return is_edge_moves_;
    }
     
    bool is_node_moves() const noexcept
    {
        return is_nodes_moves_;
    }

public:

    graph_program(sf::Vector2i const size, string const& title, string const& font_name, size_t max_node_count);
    virtual ~graph_program();

    void clear_graph();

    void update(sf::Event& event);
    void draw();

    void highlight_path();

    bool create_adjmatr();
    void clear_adjmatr();

    void highlight_path(std::vector<size_t> const& path);
    void clear_path();

    bool save_graph(std::string  pth);
    bool open_graph(std::string  pth);

    bool load_from_matrix(std::string  pth);

    bool Is_Active() const;
    void Set_Active_Menu();

    void update_menu(sf::Event& event);

    int exec();

    //===================================

    void set_background();
};

bool String_To_List_Of_Number(std::string const& str, std::vector<size_t>& path);

template<typename type>
static inline void Delete(type* element)
{
    delete element;
    element = nullptr;
}

