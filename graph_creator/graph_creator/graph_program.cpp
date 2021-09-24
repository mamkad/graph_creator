#include "graph_program.hpp"


//constractors
//==============================================================================================================
//**************************************************************************************************************

graph_program::graph_program(std::string fname, sf::Font font, sf::RenderWindow * main_win)
    : main_win_(main_win),
      file_name_(fname),
      font_(font),
      input_(font_, main_win_),
      menu_(font_, main_win_),
      mes_box_(main_win_, { main_win_->getSize().x / 2.0f - 300.0f / 2.0f, main_win_->getSize().y / 2.0f - 150.0f / 2.0f }, &font_, "")//,
      //smartMenu_(font_, main_win_ )
{
    back_ground_.setSize({1920, 1080});
    back_ground_.setFillColor(sf::Color(250, 250, 250));
    back_ground_.setPosition({0, 0});
    back_ground_.setOutlineThickness(4.0f);
    back_ground_.setOutlineColor(sf::Color(sf::Color::Blue));

    out_str_stream_ << "File: "<< file_name_ << "\nTotal nodes: " << nodes_.size() << "\nTotal edges: " << edges_.size();

    text_.setFont(font_);
    text_.setString(out_str_stream_.str());
    text_.setFillColor(sf::Color::Black);
    text_.setCharacterSize(20);
    text_.setPosition(50, main_win_->getSize().y - 100);

    mes_box_.Set_Active(false);
}

graph_program::~graph_program()
{
    clear_graph();
}

void graph_program::clear_graph()
{
    if(Is_Edge_Drawing() || Is_Node_Moves() || input_.Is_Active())
        return;

    if(nodes_.empty() && edges_.empty())
        return;

    for(auto it = nodes_.begin();  it != nodes_.end(); ++it ) { Delete(*it); }
    for(auto it = edges_.begin();  it != edges_.end(); ++it ) { Delete(*it); }

    nodes_.clear();
    edges_.clear();

    clear_adjmatr();
}

//==============================================================================================================
//**************************************************************************************************************
//matrix

void graph_program::clear_adjmatr()
{
    if(adj_matrix.empty())
        return;

    for(size_t i = 0; i < adj_matrix.size(); ++i)
        adj_matrix[i].clear();

    adj_matrix.clear();
}

bool graph_program::create_adjmatr()
{
    if(Is_Edge_Drawing() || Is_Node_Moves() || Is_Value_Read() )
        return false;

    if(nodes_.empty() && edges_.empty())
        return false;

    if(!adj_matrix.empty())
        clear_adjmatr();

    size_t max_size = 0;

    for(auto it = nodes_.begin();  it != nodes_.end(); ++it)
        if((*it)->get_id() > max_size)
            max_size = (*it)->get_id();

    adj_matrix.resize(max_size + 1, std::vector<type_t>(max_size + 1));

     for(size_t i = 0; i < adj_matrix.size(); ++i)
        std::fill(adj_matrix[i].begin(), adj_matrix[i].end(), -1);

     for(auto it = edges_.begin();  it != edges_.end(); ++it )
     {
         adj_matrix[(*it)->get_src_node()->get_id()][(*it)->get_dst_node()->get_id()] = (*it)->get_value();
     }

    for(size_t i = 0; i < adj_matrix.size(); ++i)
    {
  		   for(size_t j = 0; j < adj_matrix.size(); ++j)
              cout << adj_matrix[i][j] << ' ';
          cout << endl;
    }
    return true;
}

//==============================================================================================================
//**************************************************************************************************************
//open/save file

bool graph_program::save_graph(std::string  pth)
{
    if(pth.empty() ||nodes_.empty() )
        return true;

    if(pth.find(".txt") == std::string::npos)
        pth.append(".txt");

    pth = "Saves/" + pth;

    create_adjmatr();

    std::string text_data;
    std::list<node*>::iterator node_it;
    std::list<edge*>::iterator edg_it;
    bool is_first;

    for(node_it = nodes_.begin(); node_it != nodes_.end(); ++node_it)
    {
        text_data.append(std::to_string((*node_it)->get_id()) + " " + std::to_string((*node_it)->getxy().x) + " " + std::to_string((*node_it)->getxy().y) + ";");
        text_data.append("\n");
    }
    text_data.append("#");
    text_data.append("\n");
  
    for(node_it = nodes_.begin(); node_it != nodes_.end(); ++node_it)
    {
        edg_it = edges_.begin();
        edg_it = std::find_if(edg_it, edges_.end(), [&](auto i){return (i->get_src_node()->get_id()) == ((*node_it)->get_id());});
        is_first = true;

        while( edg_it != edges_.end() )
        {
            if(is_first)
            {
                is_first = false;
                text_data.append(std::to_string((*node_it)->get_id()) + " ");
                text_data.append(std::to_string((*edg_it)->get_dst_node()->get_id()) + " " + std::to_string((*edg_it)->get_value()));
            }
            else
            {
                text_data.append(" | " + std::to_string((*edg_it)->get_dst_node()->get_id()) + " "+ std::to_string((*edg_it)->get_value()));
            }

            ++edg_it;
            edg_it = std::find_if(edg_it, edges_.end(), [&](auto i){return (i->get_src_node()->get_id()) == ((*node_it)->get_id());});
        }

        text_data.append(" $;\n");
    }

    std::ofstream file_out(pth, std::ios::out | std::ios::binary);
    file_out.write (&text_data[0] ,text_data.size());
    file_out.close();

    return true;
}

bool graph_program::open_graph(std::string  pth)
{
    if(pth.empty())
        return true;

    if(pth.find(".txt") == std::string::npos)
        pth.append(".txt");

    pth = "Saves/" + pth;

    std::ifstream file_in(pth, std::ios::in | std::ios::binary);

    if(!file_in.is_open())
        return false;

    if(!nodes_.empty())
        clear_graph();

    std::string text_data;
    file_in.seekg(0, std::ios::end);
    text_data.resize(file_in.tellg());
    file_in.seekg(0, std::ios::beg);
    file_in.read(&text_data[0], text_data.size());
    file_in.close();

    std::size_t node_size = 0, row_edge_size = 0;
    size_t i = 0;
    size_t node_id;
    type_t x, y;

    for(; text_data[i] != '#'; ++i)
    {
        if(text_data[i] == ';')
        {
            text_data[i] = ' ';
            ++node_size;
        }
    }

    text_data[i] = ' ';

    for(; text_data[i] != '\0' ; ++i)
    {
        if(text_data[i] == ';')
        {
            text_data[i] = ' ';
            ++row_edge_size;
        }
    }

    std::stringstream str_stream;
    str_stream << text_data;

    for(size_t j = 0; j < node_size; ++j)
    {
        str_stream >> node_id >> x >> y;
        add_node();
        nodes_.back()->set_id(node_id);
        nodes_.back()->setxy({x, y});
    }

    size_t child_node_id;
    char flag;
    type_t value;
    std::list<node*>::iterator node_it;
    std::list<edge*>::iterator edg_it;

    for(size_t j = 0; j < row_edge_size; ++j)
    {
        str_stream >> node_id;
        cout << node_id << endl;

        while(str_stream >> child_node_id >> value >> flag)
        {
            cout << node_id << ' ' << child_node_id << ' ' << value << ' ' << flag << endl;
            node_it = std::find_if(nodes_.begin(), nodes_.end(), [&](auto i){return i->get_id() == node_id;});

            if(node_it != nodes_.end())
            {
                add_edge(*node_it);
                node_it = std::find_if(nodes_.begin(), nodes_.end(), [&](auto i){return i->get_id() == child_node_id;});

                 if(node_it != nodes_.end())
                 {
                    edg_it = std::find_if(edges_.begin(), edges_.end(), [&](auto i){return (i->get_src_node()->get_id() == child_node_id) && (i->get_dst_node()->get_id()  ==node_id);});

                    if(edg_it != edges_.end())
                    {
                        (*edg_it)->set_mode(true, true);
                         edges_.back()->set_mode(true, false);
                    }

                    edges_.back()->set_dst_node(*node_it);
                    edges_.back()->set_value(value);
                    edges_.back()->set_color({43, 46, 53});
                 }
                 else
                 {
                     Delete(edges_.back());
                     auto last_edge_it = edges_.end();
                     --last_edge_it;
                     edges_.erase(last_edge_it);
                     break;
                 }
            }
            else break;

            if(flag == '$')
                break;
        }
    }

    file_name_ = pth;
    return true;
}

bool graph_program::load_from_matrix(std::string pth)
{
    if(pth.empty())
        return true;

    if(pth.find(".txt") == std::string::npos)
        pth.append(".txt");

    std::ifstream file_in(pth, std::ios::in | std::ios::binary);

    if(!file_in.is_open())
        return false;

    if(!nodes_.empty())
        clear_graph();

    std::string text_data;
    file_in.seekg(0, std::ios::end);
    text_data.resize(file_in.tellg());
    file_in.seekg(0, std::ios::beg);
    file_in.read(&text_data[0], text_data.size());
    file_in.close();

    std::size_t Size = 0;

    for(size_t i = 0; text_data[i] != '\0'; ++i){
        if((!isdigit(text_data[i])) && text_data[i] != '.' && text_data[i] != '-' && text_data[i] != ';' && !isspace(text_data[i]))
            return false;

        if(text_data[i] == ';')
        {
            text_data[i] = ' ';
            ++Size;
        }
    }

    if(!Size)
        return false;

    adj_matrix.resize(Size, std::vector<type_t>(Size));

    std::stringstream str_stream;
    type_t number;
    str_stream << text_data;

    for(size_t i = 0; i < adj_matrix.size(); ++i)
    {
        for(size_t j = 0; j < adj_matrix[i].size(); ++j)
        {
            str_stream >> number;
            adj_matrix[i][j] = number;
        }
    }

    srand(time(NULL));

    for(size_t i = 0; i < Size; ++i)
    {
        add_node();
        nodes_.back()->set_id(i);
        nodes_.back()->setxy({float((rand() + 20) % 500) + 1, float(rand() % 500) + 1 });
    }

    for(auto node_it = nodes_.begin(); node_it != nodes_.end(); ++node_it)
    {
        for(size_t j = 0; j < adj_matrix[(*node_it)->get_id()].size(); ++j)
        {
            if(adj_matrix[(*node_it)->get_id()][j] != -1)
            {
                auto node_find = std::find_if(nodes_.begin(), nodes_.end(), [&](auto i){return i->get_id() == j;});

                add_edge(*node_it);
                edges_.back()->set_dst_node(*node_find);
                edges_.back()->set_value(adj_matrix[(*node_it)->get_id()][j]);
                edges_.back()->set_color({43, 46, 53});

                auto edge_find = std::find_if(edges_.begin(), edges_.end(), [&](auto i){return (i->get_src_node()->get_id() == j) && (i->get_dst_node()->get_id()  == (*node_it)->get_id());});

                if(edge_find != edges_.end())
                {
                    (*edge_find)->set_mode(true, true);
                     edges_.back()->set_mode(true, false);

                }
            }
        }
    }

    file_name_ = pth;
    return true;
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::add_node()
{
    if(Is_Edge_Drawing())
        return true;

    for(auto it = nodes_.begin(); it != nodes_.end(); ++it )
        if((*it)->contain())
            return true;

    node * node_ptr = new (std::nothrow) node( counter_, main_win_ , font_);

    if(!node_ptr)
        return false;

    nodes_.push_back( node_ptr );
    ++counter_;

    return true;
}

bool graph_program::add_edge( node * src_node )
{
    edge * edge_ptr = new (std::nothrow) edge( main_win_, src_node, font_);

    if(!edge_ptr)
        return false;

    edges_.push_back( edge_ptr );
    edges_.back()->set_color({31, 101, 218});
    return true;
}

bool graph_program::check_links(size_t src_node_id, size_t dst_node_id)
{
    if(dst_node_id == src_node_id)
        return false;

    auto edge_it = std::find_if(edges_.begin(), edges_.end() , [&](auto i){ return (i != edges_.back()) &&
                                                            (i->get_dst_node()->get_id() == dst_node_id) && (i->get_src_node()->get_id() == src_node_id);});
    if(edge_it != edges_.end())
        return false;

    edge_it = std::find_if(edges_.begin(), edges_.end() , [&](auto i){ return (i != edges_.back()) &&
                                                           (i->get_src_node()->get_id() == dst_node_id) && (i->get_dst_node()->get_id() == src_node_id);});
    if(edge_it != edges_.end())
    {
         (*edge_it)->set_mode(true, true);
         Set_Draw_Two_Edge(true);
    }

    return true;
}


//==============================================================================================================
//**************************************************************************************************************




//==============================================================================================================
//**************************************************************************************************************

bool graph_program::Is_Active() const
{
    return input_.Is_Active() ||menu_.Is_Active() ||mes_box_.Is_Active();
}

//==============================================================================================================
//**************************************************************************************************************

sf::Vector2f graph_program::mouse_pos() const
{
    return {float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y)};
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::update_graph(sf::Event & event)
{
    if(nodes_.empty() && (event.type == sf::Event::MouseButtonPressed) && (event.key.code == sf::Mouse::Left))
        return add_node();

    if(event.type == sf::Event::MouseButtonPressed)
    {
        if( !Is_Edge_Drawing() && (event.key.code == sf::Mouse::Left) )
        {
            auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i){ return i->contain();});

            if(node_it != nodes_.end())
            {
                (*node_it)->setxy(mouse_pos());
                (*node_it)->set_color({31, 101, 218});
                (*node_it)->is_moves_ = true;

                node_ptr = node_it;
                Set_Node_Moves(true);

                for(auto it2 = edges_.begin(); it2 != edges_.end(); ++it2)
                {
                    if((*it2)->get_src_node()->get_id() == (*node_ptr)->get_id())      { (*it2)->set_color({31, 101, 218}); }
                    else if((*it2)->get_dst_node()->get_id() == (*node_ptr)->get_id()) { (*it2)->set_color({255, 99, 71});  }
                }
            }
            else
            {
                auto edge_it = std::find_if(edges_.begin(), edges_.end(), [](auto i){ return i->contain();});

                if(edge_it != edges_.end())
                {
                    edg_ptr = edge_it;
                    Set_Edge_Moves(true);

                    (*edg_ptr)->set_color({205, 133, 63});
                    (*edg_ptr)->get_src_node()->set_color({205, 133, 63});
                    (*edg_ptr)->get_dst_node()->set_color({205, 133, 63});
                }
                else return add_node();
            }
        }
        else if( !Is_Node_Moves() && (event.key.code == sf::Mouse::Right) )
        {
            if(!Is_Edge_Drawing())
            {
                auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i){ return i->contain();});

                if(node_it != nodes_.end())
                {
                    (*node_it)->set_color({31, 101, 218});
                    Set_Edge_Draw(true);
                    return add_edge((*node_it));
                }
            }
            else
            {
                auto node_it = std::find_if(nodes_.begin(), nodes_.end(), [](auto i){ return i->contain();});
                edges_.back()->get_src_node()->set_color({43, 46, 53});

                if(node_it != nodes_.end() && check_links(edges_.back()->get_src_node()->get_id(), (*node_it)->get_id()))
                {
                    edges_.back()->set_dst_node(*node_it);
                    edges_.back()->set_color({43, 46, 53});

                    if(Is_Draw_Two_Edge())
                    {
                        edges_.back()->set_mode(true, false);
                        Set_Draw_Two_Edge(false);
                    }
                }
                else
                {
                    Delete(edges_.back());
                    edges_.pop_back();
                }

                Set_Edge_Draw(false);
            }
        }
    }
    else if(event.type == sf::Event::MouseButtonReleased)
    {
        if(Is_Node_Moves())
        {
            Set_Node_Moves(false);
            (*node_ptr)->is_moves_ = false;
            (*node_ptr)->set_color({43, 46, 53});

            for(auto it2 = edges_.begin(); it2 != edges_.end(); ++it2)
            {
                if((*it2)->get_src_node()->get_id() == (*node_ptr)->get_id())      { (*it2)->set_color({43, 46, 53}); }
                else if((*it2)->get_dst_node()->get_id() == (*node_ptr)->get_id()) { (*it2)->set_color({43, 46, 53});  }
            }
        }
    }
    else if(event.type == sf::Event::KeyPressed)
    {
        if(Is_Node_Moves())
        {
            if(event.key.code == sf::Keyboard::D)
            {
                for(auto it = edges_.begin();  it != edges_.end(); ++it )
                {
                    if((*it)->get_src_node()->get_id() == (*node_ptr)->get_id() || (*it)->get_dst_node()->get_id() == (*node_ptr)->get_id())
                    {
                        Delete(*it);
                        it = edges_.erase(it);
                        --it;
                    }
                }

                Delete(*node_ptr);
                nodes_.erase(node_ptr);
                Set_Node_Moves(false);
                --counter_;
            }
            else if(event.key.code == sf::Keyboard::S)
            {
                Set_Read(true);
                Set_Work_With_Node_Edge(true);
                Set_Object(false);

                input_.set_mode(true, {(*node_ptr)->getxy().x  + 25.0f, (*node_ptr)->getxy().y}, sf::Color(43, 46, 53),  sf::Color::Black, 10);

                Set_Node_Moves(false);
                (*node_ptr) ->set_color({31, 101, 218});
            }
        }
        else if(!Is_Edge_Drawing() && Is_Edge_Moves())
        {
            if( event.key.code == sf::Keyboard::D)
            {
                (*edg_ptr)->setup_color();

                Delete(*edg_ptr);
                edges_.erase(edg_ptr);
                Set_Edge_Moves(false);
            }
            else if(event.key.code == sf::Keyboard::S)
            {
                Set_Read(true);
                Set_Work_With_Node_Edge(true);
                Set_Object(true);

                input_.set_mode(true, {(*edg_ptr)->get_dst_node()->getxy().x, (*edg_ptr)->get_dst_node()->getxy().y}, sf::Color(43, 46, 53),  sf::Color::Black, 10 );
                Set_Edge_Moves(false);

            }
            else if(event.key.code == sf::Keyboard::Q)
            {


                (*edg_ptr)->setup_color();
            }
        }
    }
    return true;
}

//==============================================================================================================
//**************************************************************************************************************

bool graph_program::update_input(sf::Event & event)
{
    if(event.type == sf::Event::TextEntered)
        input_.update(event.text.unicode);
    return true;
}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::update_menu(sf::Event& event)
{
    switch (menu_.update(event))
    {
    case RESUME:
        Set_Active_Menu();
        break;

    case LOAD:
        Set_Read(true);
        Set_Read_Inp(true);
        input_.set_mode(true, { 250, 75 }, sf::Color(43, 46, 53), sf::Color::White, 20);
        break;

    case SAVE:
        Set_Read(true);
        Set_Write_Inp(true);
        input_.set_mode(true, { 250, 75 }, sf::Color(43, 46, 53), sf::Color::White, 20);
        break;

    case HIGHLIGHT:
        Set_Read(true);
        Set_Hight_Light(true);
        input_.set_mode(true, { 250, 75 }, sf::Color(43, 46, 53), sf::Color::White, 20);
        break;

    case BRANCH_AND_BOUND:
        Set_Read(true);
        Set_Bbm(true);
        break;

    case EXIT:
        main_win_->close(); break;
    }
}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::update( sf::Event & event )
{
    //cout << "=" << endl;
    if(mes_box_.Is_Active())
    {
        mes_box_.Update(event);
    }
    else if(input_.Is_Active())
    {
        //cout << "2" << endl;
        update_input(event);
    }
    else if (menu_.Is_Active())
    {
        update_menu(event);
    }
    else if(Is_Value_Read())
    {
        //cout << "3" << endl;
        bool result = true;
        std::string mes;

        if(Is_Work_With_Node_Edge())
        {
            if(!What_Is_Object())
            {
                int int_ret_val;

                if( !input_.get_int(int_ret_val) )
                {
                    result = false;
                    mes = "Error: Uncorrect input";
                    int_ret_val = 0;
                }

                (*node_ptr)->set_id(int_ret_val);
                (*node_ptr)->set_color({43, 46, 53});
            }
            else
            {
                float flt_ret_val;

                if(!input_.get_flt(flt_ret_val))
                {
                    result = false;
                    mes = "Error: Uncorrect input";
                    flt_ret_val = 0.0f;
                }

                (*edg_ptr)->set_value(flt_ret_val);
                (*edg_ptr)->setup_color();
            }
            Set_Work_With_Node_Edge(false);
        }
        else if(Is_Read_Inp())
        {
            //cout << "4" << endl;
            if(!open_graph(input_.get_str()))
            {
                result = false;
                mes = "Error: File not open";
            }

            Set_Read_Inp(false);
        }
        else if(Is_Write_Inp())
        {
            //cout << "5" << endl;
            if(!save_graph(input_.get_str()))
            {
                result = false;
                mes = "Error: File not saved";
            }

            Set_Write_Inp(false);
        }
        else if(Is_Hight_Light())
        {
            //cout << "6" << endl;
            std::vector<size_t> path;

            if(!String_To_List_Of_Number(input_.get_str(), path))
            {
                result = false;
                mes = "Error: Uncorrect input\n";
            }

            highlight_path(path);
            Set_Hight_Light(false);
        }

        if(!result)
        {
            mes_box_.Set_Active(true);
            mes_box_.Set_Text(mes);
        }

        Set_Read(false);
    }
    else
    {
        update_graph(event);

        for(auto it = nodes_.begin();  it != nodes_.end(); ++it ) { (*it)->update();};
        for(auto it = edges_.begin();  it != edges_.end(); ++it ) { (*it)->update();};
    }

    out_str_stream_.str("");
    out_str_stream_.clear();
    out_str_stream_ << "File: "<< file_name_  << "\nTotal nodes: " << nodes_.size() << "\nTotal edges: " << edges_.size();

    text_.setString(out_str_stream_.str());
}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::draw()
{
    if(!menu_.Is_Active())
    {
        main_win_->draw(back_ground_);

        for(auto it = nodes_.begin();  it != nodes_.end(); ++it ) { (*it)->draw(); }
        for(auto it = edges_.begin();  it != edges_.end(); ++it ) { (*it)->draw(); }

        main_win_->draw(text_);
    }
    else menu_.draw();

    if(input_.Is_Active())
        input_.draw();

    if (mes_box_.Is_Active())
        mes_box_.Draw();

    if(Is_Value_Read())
    {}


}

//==============================================================================================================
//**************************************************************************************************************

void graph_program::highlight_path(std::vector<size_t> const & path)
{
    for(size_t i = 0; i < path.size(); ++i)
    {
        for(auto it = edges_.begin();  it != edges_.end(); ++it)
        {
                if((*it)->get_src_node()->get_id() == path[i] && (*it)->get_dst_node()->get_id() == path[i + 1])
                {
                    /*
                    (*it)->get_src_node()->set_color({255, 165, 0});
                    (*it)->get_dst_node()->set_color({255, 165, 0});
                    (*it)->set_color({255, 165, 0});
                    (*it)->get_src_node()->is_highlight_ = true;
                    (*it)->get_dst_node()->is_highlight_ = true;
                    (*it)->set_hight(true);*/

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
    for(auto it = edges_.begin();  it != edges_.end(); ++it)
    {
        if((*it)->get_hight())
        {
            (*it)->get_src_node()->is_highlight_ = false;
            (*it)->get_dst_node()->is_highlight_ = false;
            (*it)->set_hight(false);
            (*it)->get_src_node()->set_color({43, 46, 53});
            (*it)->get_dst_node()->set_color({43, 46, 53});
            (*it)->set_color({43, 46, 53});
        }
    }
}


//==============================================================================================================
//**************************************************************************************************************

void graph_program::Set_Active_Menu()
{
    if (menu_.Is_Active())
    {
        menu_.Set_Active(false);

        if (input_.Is_Active())
            input_.Set_Active(false);
    }
    else
    {

        menu_.Set_Active(true);
    }

}

//==============================================================================================================
//**************************************************************************************************************

bool String_To_List_Of_Number(std::string const & str, std::vector<size_t> & path)
{
    if(!str.size())
        return false;

    for(char ch : str)
        if(!isdigit(ch) && ch != ' ')
            return false;

    std::stringstream str_stream;
    size_t number;

    str_stream << str;

    for(; str_stream >> number; str_stream.clear())
        path.push_back(number);

    return true;
}
