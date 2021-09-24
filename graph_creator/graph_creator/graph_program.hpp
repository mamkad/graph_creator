#ifndef GRAPH_PROGRAM_HPP
#define GRAPH_PROGRAM_HPP

#include <SFML/Graphics.hpp>

typedef float type_t;

#include "graph_base.h"

#include "edge.hpp"
#include "input_panel.hpp"
#include "menu.hpp"
#include "mes_box.hpp"

//#include "smart_menu.hpp"

#include <list>
#include <functional>
#include <fstream>
#include <iostream>

using namespace std;

//scolor BLACK({43, 46, 53});

class graph_program  : public graph_base
{
    //data
    //===================================

    sf::RenderWindow * main_win_ = nullptr;
    std::string file_name_;
    sf::Font font_;

    text_input input_;
    menu menu_;
    Message_Box mes_box_;
    //SmartMenu smartMenu_;

    sf::Text text_;
    sf::RectangleShape back_ground_;

    //graph
    //===================================

    std::list<node*> nodes_;
    std::list<edge*> edges_;
    std::list<node*>::iterator node_ptr;
    std::list<edge*>::iterator edg_ptr;
    matrix adj_matrix;

    //===================================

    bool add_node( );
    bool add_edge( node * src_node );

    node* last_node();
    edge* last_edge();

    bool check_links(size_t src_node_id, size_t dst_node_id);

    bool update_graph(sf::Event & event);
    bool update_input(sf::Event & event);

    graph_program(graph_program const & other);
    graph_program & operator = (graph_program & other);

    std::ostringstream out_str_stream_;

    sf::Vector2f mouse_pos() const;

public:

    graph_program(std::string, sf::Font, sf::RenderWindow *);
    virtual ~graph_program();
    void clear_graph();

    void update( sf::Event & event );
    void draw();

    void highlight_path();

    bool create_adjmatr();
    void clear_adjmatr();

    void highlight_path(std::vector<size_t> const & path);
    void clear_path();

    bool save_graph(std::string  pth);
    bool open_graph(std::string  pth);
    bool load_from_matrix(std::string  pth);

    bool Is_Active() const;
    void Set_Active_Menu();

    void update_menu( sf::Event & event);
};

bool String_To_List_Of_Number(std::string const & str, std::vector<size_t> & path);

template<typename type>
static inline void Delete(type * element)
{
    delete element;
    element = nullptr;
}

#endif // GRAPH_PROGRAM_HPP
