#include "mes_box.hpp"


Message_Box::Message_Box(sf::RenderWindow* main_win, sf::Vector2f const& pos, sf::Font* font, std::string mes)
    :
    main_win_(main_win), font_(font)
{
    block_[0].setSize({ 300, 150 });
    //block_[0].setPosition({main_win_->getSize().x / 2 - block_[0].getSize().x/2, main_win_->getSize().y / 2 - block_[0].getSize().y/2});
    block_[0].setPosition(pos);
    block_[0].setOutlineThickness(1.0f);
    block_[0].setOutlineColor(sf::Color(0, 191, 255));
    block_[0].setFillColor(sf::Color(43, 46, 53));

    block_[1].setSize({ 300, 10 });
    block_[1].setPosition(block_[0].getPosition());
    block_[1].setFillColor(sf::Color(0, 191, 255));

    block_[2].setSize({ 50, 20 });
    block_[2].setPosition(block_[0].getPosition().x + block_[0].getSize().x / 2.0f - block_[2].getSize().x / 2.0f,
        block_[0].getPosition().y + 120.0f - block_[2].getSize().y / 2.0f);

    block_[2].setOutlineThickness(1.0f);
    block_[2].setOutlineColor(sf::Color::White);
    block_[2].setFillColor(sf::Color(43, 46, 53));

    text_[0].setFont(*font_);
    text_[0].setFillColor(sf::Color::White);
    text_[0].setCharacterSize(20);
    text_[0].setString(mes.c_str());
    text_[0].setOrigin(text_[0].getLocalBounds().left + text_[0].getLocalBounds().width / 2.0f, text_[0].getLocalBounds().top + text_[0].getLocalBounds().height / 2.0f);
    text_[0].setPosition({ block_[0].getPosition().x + block_[0].getSize().x / 2.0f, block_[0].getPosition().y + block_[0].getSize().y / 2.0f });

    text_[1].setFont(*font_);
    text_[1].setFillColor(sf::Color::White);
    text_[1].setCharacterSize(15);
    text_[1].setString("OK");
    text_[1].setOrigin(text_[1].getLocalBounds().left + text_[1].getLocalBounds().width / 2.0f, text_[1].getLocalBounds().top + text_[1].getLocalBounds().height / 2.0f);
    text_[1].setPosition({ block_[2].getPosition().x + block_[2].getSize().x / 2.0f,  block_[2].getPosition().y + block_[2].getSize().y / 2.0f });
}

void Message_Box::Update(sf::Event& event)
{
    sf::Vector2f pos = { float(sf::Mouse::getPosition(*main_win_).x),  float(sf::Mouse::getPosition(*main_win_).y) };

    if (
        (pos.x >= block_[2].getPosition().x && pos.x <= block_[2].getPosition().x + block_[2].getSize().x) &&
        (pos.y >= block_[2].getPosition().y && pos.y <= block_[2].getPosition().y + block_[2].getSize().y)
        )
    {
        block_[2].setFillColor(sf::Color(0, 191, 255));

        if (event.type == sf::Event::MouseButtonPressed && event.key.code == sf::Mouse::Left)
        {
            Set_Active(false);
            return;
        }
    }
    else
    {
        block_[2].setFillColor(sf::Color(43, 46, 53));
    }
}

void Message_Box::Draw()
{
    main_win_->draw(block_[0]);
    main_win_->draw(block_[1]);
    main_win_->draw(block_[2]);
    main_win_->draw(text_[0]);
    main_win_->draw(text_[1]);
}

bool Message_Box::Is_Active() const
{
    return is_active_;
}

void Message_Box::Set_Active(bool st)
{
    is_active_ = st;
}

void Message_Box::Set_Text(std::string mes)
{
    block_[0].setSize({ 300, 150 });
    //block_[0].setPosition({main_win_->getSize().x / 2 - block_[0].getSize().x/2, main_win_->getSize().y / 2 - block_[0].getSize().y/2});
    block_[0].setPosition({ main_win_->getSize().x / 2.0f - 300.0f / 2.0f, main_win_->getSize().y / 2.0f - 150.0f / 2.0f });
    block_[0].setOutlineThickness(1.0f);
    block_[0].setOutlineColor(sf::Color(0, 191, 255));
    block_[0].setFillColor(sf::Color(43, 46, 53));

    block_[1].setSize({ 300, 10 });
    block_[1].setPosition(block_[0].getPosition());
    block_[1].setFillColor(sf::Color(0, 191, 255));

    block_[2].setSize({ 50, 20 });
    block_[2].setPosition(block_[0].getPosition().x + block_[0].getSize().x / 2.0f - block_[2].getSize().x / 2.0f,
        block_[0].getPosition().y + 120.0f - block_[2].getSize().y / 2.0f);

    block_[2].setOutlineThickness(1.0f);
    block_[2].setOutlineColor(sf::Color::White);
    block_[2].setFillColor(sf::Color(43, 46, 53));

    text_[0].setFont(*font_);
    text_[0].setFillColor(sf::Color::White);
    text_[0].setCharacterSize(20);
    text_[0].setString(mes.c_str());
    text_[0].setOrigin(text_[0].getLocalBounds().left + text_[0].getLocalBounds().width / 2.0f, text_[0].getLocalBounds().top + text_[0].getLocalBounds().height / 2.0f);
    text_[0].setPosition({ block_[0].getPosition().x + block_[0].getSize().x / 2.0f, block_[0].getPosition().y + block_[0].getSize().y / 2.0f });

    text_[1].setFont(*font_);
    text_[1].setFillColor(sf::Color::White);
    text_[1].setCharacterSize(15);
    text_[1].setString("OK");
    text_[1].setOrigin(text_[1].getLocalBounds().left + text_[1].getLocalBounds().width / 2.0f, text_[1].getLocalBounds().top + text_[1].getLocalBounds().height / 2.0f);
    text_[1].setPosition({ block_[2].getPosition().x + block_[2].getSize().x / 2.0f,  block_[2].getPosition().y + block_[2].getSize().y / 2.0f });
}
