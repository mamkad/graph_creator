#include "menu.hpp"

static std::vector<std::string> main_menu = {"Resume", "Open File", "Save File", "Hot keys","Highlight the path" , "Algorithms", "Exit"};
static std::vector<std::string> sub_menu_1 = {"Enter file name", "Return"};
static std::vector<std::string> sub_menu_2 = {"Enter path (example 1 2 3 ...)", "Return"};
static std::vector<std::string> info = {"M - Menu", "T - Clear graph", "C - Create Adjency matrix", "O - Clear Adjency matrix", "Return"};
static std::vector<std::string> sub_menu_4 = {"Branch Bound method", "Potential method", "Return"};

menu::menu( sf::Font & font, sf::RenderWindow *main_win )
    :  main_win_(main_win)
{

    menu_state_.resize(5);

    for(size_t i = 1; i < menu_state_.size(); ++i)
        Setup(i, nullptr);

    //block_[0].setSize({600, 400});
    block_[0].setSize({ 1280, 972 });
   // block_[0].setPosition({main_win_->getSize().x / 2 - block_[0].getSize().x/2, main_win_->getSize().y / 2 - block_[0].getSize().y/2});
    block_[0].setPosition({ 0, 0 });
    block_[0].setOutlineThickness(1.0f);
    block_[0].setOutlineColor(sf::Color(0, 191, 255));
    block_[0].setFillColor(sf::Color(43, 46, 53));

    //block_[1].setSize({600, 10});
    block_[1].setSize({ 1280, 10 });
    //block_[1].setPosition(block_[0].getPosition());
    block_[0].setPosition({ 1, 1 });
    block_[1].setFillColor(sf::Color(0, 191, 255));

    text_.resize(5, std::vector<sf::Text>(0));

    for(size_t i = 0; i < main_menu.size(); ++i)
    {
        text_[0].push_back(sf::Text());
        text_[0].back().setFont(font);
        text_[0].back().setFillColor(sf::Color::White);
        text_[0].back().setCharacterSize(20);
        text_[0].back().setPosition(block_[0].getPosition().x + 25, block_[0].getPosition().y + 20 + 50 * i);
        //text_[0].back().setPosition(600 + 20, 400 + 20 + 50 * i);
        text_[0].back().setString(main_menu[i]);
        //text_[0].back().setString("anton");
    }

    for(size_t i = 0; i < sub_menu_1.size(); ++i)
    {
        text_[1].push_back(sf::Text());
        text_[1].back().setFont(font);
        text_[1].back().setFillColor(sf::Color::White);
        text_[1].back().setCharacterSize(20);
        //text_[1].back().setPosition(360 + 200, block_[0].getPosition().y + 20 + 50 * i);
        text_[1].back().setPosition(250, block_[0].getPosition().y + 25 + 100 * i);
        //text_[1].back().setPosition(360 + 200, 400 + 20 + 50 * i);
        text_[1].back().setString(sub_menu_1[i]);
    }

    for(size_t i = 0; i < sub_menu_2.size(); ++i)
    {
        text_[2].push_back(sf::Text());
        text_[2].back().setFont(font);
        text_[2].back().setFillColor(sf::Color::White);
        text_[2].back().setCharacterSize(20);
        //text_[2].back().setPosition(360 + 200, block_[0].getPosition().y + 20 + 50 * i);
        text_[2].back().setPosition(250, block_[0].getPosition().y + 25 + 100 * i);
        //text_[2].back().setPosition(360 + 200, 400 + 20 + 50 * i);
        text_[2].back().setString(sub_menu_2[i]);
    }

    for(size_t i = 0; i < info.size(); ++i)
    {
        text_[3].push_back(sf::Text());
        text_[3].back().setFont(font);
        text_[3].back().setFillColor(sf::Color::White);
        text_[3].back().setCharacterSize(20);
        //text_[3].back().setPosition(360 + 200, block_[0].getPosition().y + 20 + 50 * i);
        text_[3].back().setPosition(250, block_[0].getPosition().y + 25 + 25 * i);
        //text_[3].back().setPosition(360 + 200, 400 + 20 + 50 * i);
        text_[3].back().setString(info[i]);
    }

    for(size_t i = 0; i < sub_menu_4.size(); ++i)
    {
        text_[4].push_back(sf::Text());
        text_[4].back().setFont(font);
        text_[4].back().setFillColor(sf::Color::White);
        text_[4].back().setCharacterSize(20);
       // text_[4].back().setPosition(360 + 200, block_[0].getPosition().y + 20 + 50 * i);
        text_[4].back().setPosition(250, block_[0].getPosition().y + 25 + 25 * i);
        //text_[4].back().setPosition(360 + 200, 400 + 20 + 50 * i);
        text_[4].back().setString(sub_menu_4[i]);
    }

    Setup(0, &text_[0]);

    line_[0].color = sf::Color::White;
    line_[1].color = sf::Color::White;
    line_[0].position = {text_[0][0].getPosition().x + 180, text_[0][0].getPosition().y };
    line_[1].position = {text_[0].back().getPosition().x + 180, text_[0].back().getPosition().y + 20};

    block_[2].setSize({160, 25});
    block_[2].setFillColor(sf::Color(sf::Color::Black));
    block_[2].setOutlineThickness(0.6f);
    block_[2].setOutlineColor(sf::Color(sf::Color::White));
}


void menu::draw()
{
    main_win_->draw(block_[0]);
    main_win_->draw(block_[1]);

    if(is_clicked_)
    {
        main_win_->draw(block_[2]);
        main_win_->draw(line_, 2, sf::Lines);
    }

    for(size_t i = 0; i < menu_state_.size(); ++i)
    {
        if(menu_state_[i] != nullptr)
        {
            for(size_t j = 0; j < text_[i].size(); ++j)
            {
                  main_win_->draw(text_[i][j]);
            }
        }
    }
}

void menu::Set_Menu(size_t m, std::vector<sf::Text> * text, size_t c_idx, bool is_clk, size_t offs)
{
    Setup(m, text);
    cur_idx = c_idx;
    is_clicked_ = is_clk;
    offset = offs;
}

int menu::update(sf::Event & event)
{
    sf::Vector2f pos = {float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y)};

    if(this->is_clr_)
    {
        if(!(pos.x >= text_[idx][jdx].getPosition().x && pos.x <= text_[idx][jdx].getPosition().x + text_[idx][jdx].getLocalBounds().width && pos.y >= text_[idx][jdx].getPosition().y && pos.y <= text_[idx][jdx].getPosition().y + text_[idx][jdx].getLocalBounds().height))
        {
            this->is_clr_ = false;
        }
        else if(event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
        {
             size_t total = 10 * idx + jdx;

             std::cout << total << std::endl;

             switch(total)
             {
                 //open/save
             case 1:
                 Set_Menu(1, &text_[1], 1, true, 1);
                 block_[2].setPosition(text_[0][jdx].getPosition());
                 break;

             case 2:
                 Set_Menu(1, &text_[1], 1, true, 1);
                 block_[2].setPosition(text_[0][jdx].getPosition());
                 break;

                //hot keys
             case 3:
                Set_Menu(3, &text_[3], 3, true, 4);
                block_[2].setPosition(text_[0][jdx].getPosition());
                break;

                //highlight
             case 4:
                Set_Menu(2, &text_[2], 2, true, 1);
                block_[2].setPosition(text_[0][jdx].getPosition());
                break;

                //algo
            case 5:
                Set_Menu(4, &text_[4], 4, true, 0);
                block_[2].setPosition(text_[0][jdx].getPosition());
                break;

            case BRANCH_AND_BOUND:

                break;

            case 11: case 34 : case 21: case 42:
                Set_Menu(cur_idx, nullptr, 0, false, 0);
                break;
             }

             return total;
        }
    }
    else
    {
        //text_[idx][jdx].setFillColor(sf::Color::White);
    }

    for(size_t j = offset; j < text_[cur_idx].size(); ++j)
    {
        if(pos.x >= text_[cur_idx][j].getPosition().x && pos.x <= text_[cur_idx][j].getPosition().x + text_[cur_idx][j].getLocalBounds().width
        && pos.y >= text_[cur_idx][j].getPosition().y && pos.y <= text_[cur_idx][j].getPosition().y + text_[cur_idx][j].getLocalBounds().height )
        {
            text_[cur_idx][j].setFillColor(sf::Color::Yellow);

            this->is_clr_ = true;

            idx = cur_idx;
            jdx = j;

            break;
        }
        else
        {
            text_[cur_idx][j].setFillColor(sf::Color::White);
        }
    }

    return -1;
}

bool menu::Is_Active() const
{
	return is_menu_ex_;
}

void menu::Set_Active(bool st)
{
	is_menu_ex_ = st;
}

void menu::Setup(size_t i, std::vector<sf::Text> * ptr)
{
    menu_state_[i] = ptr;
}
