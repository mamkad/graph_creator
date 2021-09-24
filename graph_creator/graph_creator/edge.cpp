#include "edge.hpp"
#include <iostream>
using namespace std;

edge::edge(sf::RenderWindow * main_win , node * src_node, sf::Font & font)
    : main_win_(main_win), curve_(sf::LinesStrip, 11)
{
    set_src_node(src_node);

    edge_stat_ = {false, false, false, false};

    text_.setFont(font);
    text_.setCharacterSize(18.f);
    text_.setFillColor(sf::Color::White);

    block_.setFillColor(sf::Color(43, 46, 53));

    set_value(0);

    set_color({43, 46, 53});
}

void  edge::set_src_node(node * src_node) { nodes_[0] = src_node; }
node* edge::get_src_node() const          { return nodes_[0];     }
void  edge::set_dst_node(node * dst_node)
{
    nodes_[1] = dst_node;
    edge_stat_.is_linkd_ = true;
}
node* edge::get_dst_node() const { return nodes_[1]; }

float edge::get_value() const
{
    return value_;
}

void edge::set_value(float v)
{
    value_ = v;
    std::ostringstream out_str_stream_;
    out_str_stream_ << v;

    text_.setString( out_str_stream_.str());
    text_.setOrigin(text_.getLocalBounds().left + text_.getLocalBounds().width/2.0f,  text_.getLocalBounds().top + text_.getLocalBounds().height/2.0f);

    block_.setSize({text_.getLocalBounds().left + text_.getLocalBounds().width,  text_.getLocalBounds().top + text_.getLocalBounds().height});
    block_.setOrigin({block_.getLocalBounds().left + block_.getLocalBounds().width/2.0f,  block_.getLocalBounds().top + block_.getLocalBounds().height/2.0f});
    block_.setPosition(text_.getPosition());
}

bool edge::contain() const
{
    sf::Vector2f pos = {float(sf::Mouse::getPosition(*main_win_).x), float(sf::Mouse::getPosition(*main_win_).y)};

    float k = (line_[1].position.y - line_[0].position.y) / (line_[1].position.x - line_[0].position.x);
    float y = k * (pos.x - line_[0].position.x) + line_[0].position.y;
    return (fabs(y - pos.y) <= 6.f);
}


static float corr_val(float v)
{
    if(fabs(v) < 0.1f)
    {
        if(v < 0.0f) v = -0.1f;
        else         v =  0.1f;
    }

    return v;
}

void edge::calc_curve(sf::Vector2f const & midPoint)
{
    float dt, mt, st;

    curve_[0].position = line_[0].position;
    curve_[10].position =  line_[1].position;

    float t[11] = {0.0f, 0.1f, 0.2f, 0.3f, 0.4f, 0.5f, 0.6f, 0.7f, 0.8f, 0.9f, 1.0f };

    for(size_t i = 1; i < 10; ++i)
    {
        dt = (1.0f - t[i]);
        mt =  2 * t[i];
        st = t[i] * t[i];

        sf::Vector2f np = {dt * line_[0].position.x + mt * dt * midPoint.x + st * line_[1].position.x,
                           dt * line_[0].position.y + mt * dt * midPoint.y + st * line_[1].position.y};
        curve_[i].position = np;

    }
}


void edge::update()
{
    float ang;

    if(edge_stat_.is_linkd_)
    {
        float dx = corr_val(get_dst_node()->getxy().x - get_src_node()->getxy().x);
        float dy = get_dst_node()->getxy().y - get_src_node()->getxy().y;
        float L = corr_val(sqrtf(dx * dx + dy * dy));

        ang = atan2(-dy , dx);

        if(edge_stat_.mode_)
        {
            if(edge_stat_.mode_of_calk_)
            {
                line_[0].position = get_src_node()->getxy() + sf::Vector2f( sin( ang + M_PI - M_PI / 3 ) * 15.0f, cos( ang + M_PI- M_PI / 3 ) * 15.0f );
                line_[1].position = get_dst_node()->getxy() + sf::Vector2f( sin( ang - M_PI + M_PI / 3 ) * 15.0f, cos( ang - M_PI + M_PI / 3 ) * 15.0f );

                float x = (line_[0].position.x + line_[1].position.x)/ 2.0f;
                float y = (line_[0].position.y + line_[1].position.y)/ 2.0f;

                calc_curve({x, y});
            }
            else
            {
               line_[0].position = get_src_node()->getxy() + sf::Vector2f( sin( ang - M_PI - M_PI / 3 ) * 15.0f, cos( ang - M_PI - M_PI / 3 ) * 15.0f );
               line_[1].position = get_dst_node()->getxy() + sf::Vector2f( sin( ang - M_PI + M_PI / 3 ) * 15.0f, cos( ang - M_PI + M_PI / 3 ) * 15.0f );

               float x = (line_[0].position.x + line_[1].position.x)/ 2.0f;
               float y = (line_[0].position.y + line_[1].position.y)/ 2.0f;

               cout << line_[0].position.x << ' ' << line_[1].position.x << endl;
               cout << line_[0].position.y << ' ' << line_[1].position.y << endl;
               cout << x << ' ' << y << endl;

               calc_curve({x, y });
            }
        }
        else
        {
            line_[0].position = {get_src_node()->getxy().x + R * (dx / L), get_src_node()->getxy().y + R * (dy / L) };

            dx =  get_src_node()->getxy().x - get_dst_node()->getxy().x;
            dy =  get_src_node()->getxy().y - get_dst_node()->getxy().y;

            line_[1].position = {get_dst_node()->getxy().x + R * (dx / L), get_dst_node()->getxy().y + R * (dy / L) };
        }

        #define length 10

        arrow1[0][0].position =  line_[1].position;
        arrow1[0][1].position =  line_[1].position + sf::Vector2f( sin( ang - M_PI / 3 ) * length, cos( ang - M_PI / 3 ) * length ); //15 arrow length //низ
        arrow1[1][0].position =  line_[1].position;
        arrow1[1][1].position =  line_[1].position + sf::Vector2f( sin( ang - M_PI + M_PI / 3 ) * length, cos( ang - M_PI + M_PI / 3 ) * length ); //верх

        text_.setPosition(  { (line_[1].position.x + line_[0].position.x) / 2.0f, (line_[1].position.y + line_[0].position.y) / 2.0f + 10.0f} );
        block_.setPosition(text_.getPosition());
    }
    else
    {
        if(get_src_node()->contain())
            return;

        float dx = corr_val( float(sf::Mouse::getPosition(*main_win_).x) - get_src_node()->getxy().x);
        float dy = float(sf::Mouse::getPosition(*main_win_).y) - get_src_node()->getxy().y;
        float L = corr_val(sqrtf(dx * dx + dy * dy));

        ang = atan2(-dy , dx);

        line_[0].position = {get_src_node()->getxy().x  + R * (dx / L), get_src_node()->getxy().y + R * (dy / L)};
        line_[1].position = {float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y)};

        arrow1[0][0].position =  line_[1].position;
        arrow1[0][1].position =  line_[1].position + sf::Vector2f( sin( ang - M_PI / 3 ) * length, cos( ang - M_PI / 3 ) * length ); //15 arrow length
        arrow1[1][0].position =  line_[1].position;
        arrow1[1][1].position =  line_[1].position + sf::Vector2f( sin( ang - M_PI + M_PI / 3 ) * length, cos( ang - M_PI + M_PI / 3 ) * length );
    }
}

void edge::draw()
{
    if(edge_stat_.mode_)
        main_win_->draw(curve_);
    else
        main_win_->draw(line_, 2, sf::Lines);

    if(edge_stat_.is_linkd_)
    {
        main_win_->draw(block_);
        main_win_->draw(text_);
    }

    main_win_->draw(arrow1[0], 2, sf::Lines);
    main_win_->draw(arrow1[1], 2, sf::Lines);
}


void edge::set_color(sf::Color const & color)
{
    if(!edge_stat_.is_highlight_)
    {
        line_[0].color = color;
        line_[1].color = color;

        arrow1[0][0].color = color;
        arrow1[0][1].color = color;
        arrow1[1][0].color = color;
        arrow1[1][1].color = color;

        arrow2[0][0].color = color;
        arrow2[0][1].color = color;
        arrow2[1][0].color = color;
        arrow2[1][1].color = color;

        for(size_t i = 0; i < 10; ++i)
            curve_[i].color = color;
    }
}

bool edge::get_hight() const
{
    return edge_stat_.is_highlight_;
}

void edge::set_hight(bool st)
{
    edge_stat_.is_highlight_ = st;
}

void edge::set_mode(bool st, bool w)
{
    edge_stat_.mode_ = st;
    edge_stat_.mode_of_calk_ = w;
}

bool edge::get_mode() const
{
    return edge_stat_.mode_;
}

void edge::setup_color()
{
    get_src_node()->set_color({43, 46, 53});
    get_dst_node()->set_color({43, 46, 53});
    set_color({43, 46, 53});
}
