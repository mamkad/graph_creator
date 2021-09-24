#ifndef MES_BOX_HPP_INCLUDED
#define MES_BOX_HPP_INCLUDED

#include <SFML/Graphics.hpp>

class Message_Box
{
	sf::RenderWindow* main_win_ = nullptr;
	sf::Font* font_ = nullptr;
	sf::Text text_[2];
	sf::RectangleShape block_[3];

	bool is_active_ = true;

public:

	Message_Box(sf::RenderWindow* main_win, sf::Vector2f const& pos, sf::Font* font, std::string mes);

	void Set_Xy();
	sf::Vector2f Get_Xy() const;

	void Update(sf::Event& event);
	void Draw();

	bool Is_Active() const;
	void Set_Active(bool st);

	void Set_Text(std::string mes);
};
#endif // MES_BOX_HPP_INCLUDED
