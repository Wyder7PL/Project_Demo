#include "Text.hpp"

Demo::Text::Text(const std::string& txt, const unsigned int& fontSize)
{
	text.setFont(infiniteFont);
	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(1.f);
	SetFontSize(fontSize);
	UpdateText(txt);
	UpdatePosition();
}

Demo::Text::~Text()
{
	
}

void Demo::Text::SetPosition(const PointU& loc)
{
	UpdatePosition();
	text.setPosition(loc.x,loc.y);
}

Demo::PointU Demo::Text::GetPosition()
{
	auto bounds = text.getLocalBounds();
	return PointU(bounds.left+bounds.width/2,bounds.top+bounds.height/2);
}

void Demo::Text::SetRotation(const double& angle)
{
	text.setRotation(angle);
}

double Demo::Text::GetRotation()
{
	return text.getRotation();
}

void Demo::Text::UpdateText(const std::string& txt)
{
	text.setString(txt);
	UpdatePosition();
}

void Demo::Text::SetFontSize(const unsigned int& size)
{
	text.setCharacterSize(size);
	UpdatePosition();
}

unsigned int Demo::Text::GetFontSize()
{
	return text.getCharacterSize();
}

unsigned int Demo::Text::GetWidth()
{
	UpdatePosition();
	return text.getLocalBounds().width + text.getLocalBounds().left;
}

void Demo::Text::SetFontColor(const unsigned char& r, const unsigned char& g, const unsigned char& b, const unsigned char& a)
{
	text.setFillColor(sf::Color(r,g,b,a));
}

void Demo::Text::HiddenDraw(sf::RenderTarget& target) const
{
	target.draw(text);
}

void Demo::Text::UpdatePosition()
{
	auto bounds = text.getLocalBounds();
	text.setOrigin(bounds.width/2+bounds.left ,bounds.height/2+bounds.top);
}

sf::Font Demo::Text::ImpactedFontConstruct()
{
	sf::Font impacted;
	impacted.loadFromFile("resources/font/Impacted.ttf");
	return impacted;
}

sf::Font Demo::Text::impactedFont = Demo::Text::ImpactedFontConstruct();

sf::Font Demo::Text::FairJudgeFontConstruct()
{
	sf::Font fair;
	fair.loadFromFile("resources/font/FairJudge.ttf");
	return fair;
}

sf::Font Demo::Text::fairJudgeFont = Demo::Text::FairJudgeFontConstruct();

sf::Font Demo::Text::InfiniteConstruct()
{
	sf::Font infinite;
	infinite.loadFromFile("resources/font/Infinite.ttf");
	return infinite;
}

sf::Font Demo::Text::infiniteFont = Demo::Text::InfiniteConstruct();

sf::Font Demo::Text::IndochineConstruct()
{
	sf::Font indochine;
	indochine.loadFromFile("resources/font/IndochineNF.ttf");
	return indochine;
}

sf::Font Demo::Text::indochineFont = Demo::Text::IndochineConstruct();

