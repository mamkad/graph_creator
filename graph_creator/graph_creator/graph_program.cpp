#include "graph_program.h"

#include <iostream>
using namespace std;

//constractors
//==============================================================================================================
//**************************************************************************************************************

graph_program::graph_program(sf::Vector2i const size, string const& title, string const& font_name, size_t max_node_count)
{
    cout << 1 << endl;

    anti_alias.antialiasingLevel = 8;

    main_win_ = new sf::RenderWindow(sf::VideoMode(size.x, size.y), title, sf::Style::Titlebar, anti_alias);

    if (!font_.loadFromFile(font_name))
        throw std::logic_error(font_name + " is not found");

    input_ = new text_input(font_, main_win_);

    back_ground_.setSize({ (float)size.x, (float)size.y });
    back_ground_.setFillColor(sf::Color(250, 250, 250));
    back_ground_.setPosition({ 0, 0 });
    back_ground_.setOutlineThickness(4.0f);
    back_ground_.setOutlineColor(sf::Color(sf::Color::Blue));

    text_.setFont(font_);
    text_.setString(out_str_stream_.str());
    text_.setFillColor(sf::Color::Black);
    text_.setCharacterSize(20);
    text_.setPosition(50, main_win_->getSize().y - 100);

    max_node_count_ = max_node_count;

    edg_ptr = nullptr;

    is_edge_moves_ = false;
    is_nodes_moves_ = false;;
    is_edge_drawing_ = false;

    count_edge_ = 0;

    out_str_stream_ << "File: " << file_name_ << "\nTotal nodes: " << nodes_.size() << "\nTotal edges: " << count_edge_;
}




graph_program::~graph_program()
{
    clear_graph();

    delete main_win_;
    delete input_;
}

void graph_program::clear_graph()
{
    if (is_now_edge_drawing() || is_node_moves() || input_->Is_Active())
        return;

    if (nodes_.empty() /*&& edges_.empty()*/)
        return;

    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) 
        for (auto it2 = (*it)->begin(0); it2 != (*it)->end(0); ++it2)
            delete (*it2);

    nodes_.clear();
   // clear_adjmatr();
}

//==============================================================================================================
//**************************************************************************************************************
//matrix

void graph_program::clear_adjmatr()
{
    if (adj_matrix.empty())
        return;

    for (size_t i = 0; i < adj_matrix.size(); ++i)
        adj_matrix[i].clear();

    adj_matrix.clear();
}
//====================================================================================================
//**************************************************************************************************************

bool graph_program::add_node()
{
    if (Is_Edge_Drawing())
        return true;

    for (auto it = nodes_.begin(); it != nodes_.end(); ++it)
        if ((*it)->contain())
            return true;

    node* node_ptr = new (std::nothrow) node(counter_, main_win_, font_);

    if (!node_ptr)
        return false;

    nodes_.push_back(node_ptr);
    ++counter_;

    return true;
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::Is_Active() const
{
    return input_->Is_Active() /*|| smartMenu_.IsActive() || mes_box_.Is_Active();*/;
}

//==============================================================================================================
//**************************************************************************************************************

sf::Vector2f graph_program::mouse_pos() const
{
    return { float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y) };
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::update_graph(sf::Event& event)
{
    if (nodes_.empty() && (event.type == sf::Event::MouseButtonPressed) && (event.key.code == sf::Mouse::Left))
        return add_node();

    if (event.type == sf::Event::MouseButtonPressed)
    {
        if (!is_now_edge_drawing() && (event.key.code == sf::Mouse::Left))
        {
            auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i) { return i->contain(); });

            if (node_it != nodes_.end())
            {
                (*node_it)->setxy(mouse_pos());
                (*node_it)->set_color({ 31, 101, 218 });
                (*node_it)->set_moves(true);

                node_ptr = node_it;

                for (auto it2 = (*node_ptr)->begin(0); it2 != (*node_ptr)->end(0); ++it2)
                    (*it2)->set_color({ 31, 101, 218 });

                for (auto it2 = (*node_ptr)->begin(1); it2 != (*node_ptr)->end(1); ++it2)
                    (*it2)->set_color({ 255, 99, 71 });

                is_nodes_moves_ = true;
            }
            else
            {
                for(auto it = nodes_.begin(); it != nodes_.end(); ++it)
                    for (auto it2 = (*it)->begin(0); it2 != (*it)->end(0); ++it2)
                        if ((*it2)->contain())
                        {
                            edg_ptr = (*it2);
                            edg_ptr->set_color({ 205, 133, 63 });
                            edg_ptr->get_src_node()->set_color({ 205, 133, 63 });
                            edg_ptr->get_dst_node()->set_color({ 205, 133, 63 });

                            is_edge_moves_ = true;

                            return true;
                        }
  
                return add_node();
            }
        }
        else if (!is_node_moves() && (event.key.code == sf::Mouse::Right))
        {
            if (!is_now_edge_drawing())
            {
                auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i) { return i->contain(); });

                if (node_it != nodes_.end())
                {
                    node_ptr = node_it;

                    (*node_it)->set_color({ 31, 101, 218 });

                    edg_ptr = new edge(main_win_, *node_it, font_);
                    (*node_it)->add_out_edge(edg_ptr);

                    is_edge_drawing_ = true;

                    ++count_edge_;

                    return true;
                }
            }
            else
            {
                auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i) { return i->contain(); });
                edg_ptr->get_src_node()->set_color({ 43, 46, 53 });

                if (node_it != nodes_.end() && edg_ptr->get_src_node()->get_id() != (*node_it)->get_id())
                {
                    (*node_it)->add_inp_edge(edg_ptr);

                    /*
                    for (auto it = edg_ptr->get_src_node()->begin(1); it != edg_ptr->get_src_node()->end(1); ++it)
                        if ((*it)->get_dst_node()->get_id() == (*node_it)->get_id())
                        {
                            edg_ptr->get_src_node()->del_out_edge();
                            delete edg_ptr;

                            --count_edge_;

                            is_edge_drawing_ = false;
                            edg_ptr = nullptr;

                            return true;
                        }*/


                    //проверяем выходящие узлы
                    for (auto it = (*node_it)->begin(1); it != (*node_it)->end(1); ++it)
                        if ((*it)->get_dst_node()->get_id() == edg_ptr->get_src_node()->get_id())
                        {
                            edg_ptr->set_mode(true, false);
                            break;
                        }

                    edg_ptr->set_dst_node(*node_it);
                    edg_ptr->set_color({ 43, 46, 53 });
                }
                else
                {
                    edg_ptr->get_src_node()->del_out_edge();
                    delete edg_ptr;

                    --count_edge_;
                }

                is_edge_drawing_ = false;
                edg_ptr = nullptr;

                return true;
            }
        }
    }
    else if (event.type == sf::Event::MouseButtonReleased)
    {
        if (is_node_moves())
        {
            (*node_ptr)->set_moves(false);
            (*node_ptr)->set_color({ 43, 46, 53 });

            for (auto it2 = (*node_ptr)->begin(0); it2 != (*node_ptr)->end(0); ++it2)
                (*it2)->set_color({ 43, 46, 53 });

            for (auto it2 = (*node_ptr)->begin(1); it2 != (*node_ptr)->end(1); ++it2)
                (*it2)->set_color({ 43, 46, 53 });

            is_nodes_moves_ = false;
        }
        else if (is_now_edge_drawing())
        {
            //!= nullptr ?
            edg_ptr->update();
        }
    }
    else if (event.type == sf::Event::KeyPressed)
    {
        if (is_node_moves())
        {
            if (event.key.code == sf::Keyboard::D)
            {        
                for (auto it = (*node_ptr)->begin(0); it != (*node_ptr)->end(0); ++it)
                    if ((*it)->get_src_node()->del_out_edge(*node_ptr))
                    {
                        delete (*it);
                        --count_edge_;
                    }
                   
                for (auto it = (*node_ptr)->begin(1); it != (*node_ptr)->end(1); ++it)
                    if ((*it)->get_dst_node()->del_inp_edge(*node_ptr))
                    {
                        delete (*it);
                        --count_edge_;
                    }
                        
                
                delete (*node_ptr);
                nodes_.erase(node_ptr);

                is_nodes_moves_ = false;
                --counter_;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                Set_Read(true);
                Set_Work_With_Node_Edge(true);
                Set_Object(false);

                input_->set_mode(true, { (*node_ptr)->getxy().x + 25.0f, (*node_ptr)->getxy().y }, sf::Color(43, 46, 53), sf::Color::Black, 10);

                (*node_ptr)->set_color({ 31, 101, 218 });
                (*node_ptr)->set_moves(false);
                (*node_ptr) = nullptr;

                input_->cleard();
            }
        }
        else if (is_now_edge_moves())
        {
            if (event.key.code == sf::Keyboard::D)
            {
                edg_ptr->setup_color();

                edg_ptr->get_src_node()->del_out_edge(edg_ptr->get_dst_node());
                edg_ptr->get_dst_node()->del_inp_edge(edg_ptr->get_src_node());

                delete edg_ptr;
                edg_ptr = nullptr;
 
                is_edge_moves_ = false;

                --count_edge_;
            }
            else if (event.key.code == sf::Keyboard::S)
            {
                Set_Read(true);
                Set_Work_With_Node_Edge(true);
                Set_Object(true);

                input_->set_mode(true, { edg_ptr->get_dst_node()->getxy().x, edg_ptr->get_dst_node()->getxy().y }, sf::Color(43, 46, 53), sf::Color::Black, 10);
                
                is_edge_moves_ = false;
            }
            else if (event.key.code == sf::Keyboard::Q)
            {
                edg_ptr->setup_color();
            }
        }
    }
    return true;
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::update_input(sf::Event& event)
{
    if (event.type == sf::Event::TextEntered)
        input_->update(event.text.unicode);
    return true;
}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::update_menu(sf::Event& event)
{

}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::update(sf::Event& event)
{
   // cout << "=" << endl;
    /*if (mes_box_.Is_Active())
    {
        cout << "1" << endl;
        mes_box_.Update(event);
    }
    else*/ if (input_->Is_Active())
    {
        //cout << "2" << endl;
        update_input(event);
    }
    else if (Is_Value_Read())
    {
        //cout << "3" << endl;
        bool result = true;
        std::string mes;

        if (Is_Work_With_Node_Edge())
        {
            if (!What_Is_Object())
            {
                int int_ret_val;

                if (!input_->get_int(int_ret_val))
                {
                    result = false;
                    mes = "Error: Uncorrect input";
                    int_ret_val = 0;
                }

                (*node_ptr)->set_id(int_ret_val);
                (*node_ptr)->set_color({ 43, 46, 53 });
            }
            else
            {
                float flt_ret_val;

                if (!input_->get_flt(flt_ret_val))
                {
                    result = false;
                    mes = "Error: Uncorrect input";
                    flt_ret_val = 0.0f;
                }

                edg_ptr->set_value(flt_ret_val);
                edg_ptr->setup_color();
            }
            Set_Work_With_Node_Edge(false);
        }
        else if (Is_Read_Inp())
        {
           // cout << "4" << endl;
            /*
            if (!open_graph(input_->get_str()))
            {
                result = false;
                mes = "Error: File not open";
            }
            */

            Set_Read_Inp(false);
        }
        else if (Is_Write_Inp())
        {
            //cout << "5" << endl;
            /*
            if (!save_graph(input_->get_str()))
            {
                result = false;
                mes = "Error: File not saved";
            }*/

            Set_Write_Inp(false);
        }
        else if (Is_Hight_Light())
        {
            /*
            cout << "6" << endl;
            std::vector<size_t> path;

            if (!String_To_List_Of_Number(input_->get_str(), path))
            {
                result = false;
                mes = "Error: Uncorrect input\n";
            }

            highlight_path(path);*/
            Set_Hight_Light(false);
        }

        if (!result)
        {
            //mes_box_.Set_Active(true);
            //mes_box_.Set_Text(mes, { 500, 300 });
        }

        Set_Read(false);
    }
    else
    {   
        update_graph(event);

        for (auto it = nodes_.begin(); it != nodes_.end(); ++it)
            (*it)->update();
    }

    out_str_stream_.str("");
    out_str_stream_.clear();
    out_str_stream_ << "File: " << file_name_ << "\nTotal nodes: " << nodes_.size() << "\nTotal edges: " << count_edge_;

    text_.setString(out_str_stream_.str());
}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::draw()
{
    main_win_->draw(back_ground_);

    if (is_now_edge_drawing())
    {
        edg_ptr->update();
        edg_ptr->draw();
    }
       
    for (auto it = nodes_.begin(); it != nodes_.end(); ++it) { (*it)->draw(); }
 
    main_win_->draw(text_);

    if (input_->Is_Active())
        input_->draw();

    if (Is_Value_Read())
    {
    }


}

//==============================================================================================================
//**************************************************************************************************************

/*
void graph_program::highlight_path(std::vector<size_t> const& path)
{
    for (size_t i = 0; i < path.size(); ++i)
    {
        for (auto it = edges_.begin(); it != edges_.end(); ++it)
        {
            if ((*it)->get_src_node()->get_id() == path[i] && (*it)->get_dst_node()->get_id() == path[i + 1])
            {
               
                (*it)->get_src_node()->set_color(sf::Color::Red);
                (*it)->get_dst_node()->set_color(sf::Color::Red);
                (*it)->set_color(sf::Color::Red);
                (*it)->get_src_node()->is_highlight_ = true;
                (*it)->get_dst_node()->is_highlight_ = true;
                (*it)->set_hight(true);

                break;
            }
        }
    }
}

void graph_program::clear_path()
{
    for (auto it = edges_.begin(); it != edges_.end(); ++it)
    {
        if ((*it)->get_hight())
        {
            (*it)->get_src_node()->is_highlight_ = false;
            (*it)->get_dst_node()->is_highlight_ = false;
            (*it)->set_hight(false);
            (*it)->get_src_node()->set_color({ 43, 46, 53 });
            (*it)->get_dst_node()->set_color({ 43, 46, 53 });
            (*it)->set_color({ 43, 46, 53 });
        }
    }
}
*/

//==============================================================================================================
//**************************************************************************************************************

void graph_program::Set_Active_Menu()
{
    /*
    if (smartMenu_.IsActive())
        smartMenu_.SetActive(false);
    else
    {

        smartMenu_.SetActive(true);
    }*/

}

//==============================================================================================================
//**************************************************************************************************************

int graph_program::exec()
{
    while (main_win_->isOpen())
    {
        sf::Event events;

        while (main_win_->pollEvent(events))
        {
            if (events.type == sf::Event::Closed)
            {
                main_win_->close();
            }
            else if (events.type == sf::Event::Resized)
            {
                float w = static_cast<float>(events.size.width);
                float h = static_cast<float>(events.size.height);

                main_win_->setView(sf::View(sf::Vector2f(w / 2.0, h / 2.0), sf::Vector2f(w, h)));
            }

            if (!Is_Active())
            {
                switch (events.type)
                {
                case sf::Event::KeyPressed:

                    switch (events.key.code)
                    {
                    case sf::Keyboard::T: clear_graph();          break;
              
                    default: break;
                    }

                default: break;
                }
            }

            update(events);  
        }

        main_win_->clear();
        draw();
        main_win_->display();
    }

    return 0;
}





bool String_To_List_Of_Number(std::string const& str, std::vector<size_t>& path)
{
    if (!str.size())
        return false;

    for (char ch : str)
        if (!isdigit(ch) && ch != ' ')
            return false;

    std::stringstream str_stream;
    size_t number;

    str_stream << str;

    for (; str_stream >> number; str_stream.clear())
        path.push_back(number);

    return true;
}
