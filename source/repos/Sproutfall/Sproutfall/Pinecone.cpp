#include "Pinecone.h"

Pinecone::Pinecone(sf::Texture* texture, Player* player)
{
	m_Sprite = make_unique<sf::Sprite>();
	m_Sprite->setTexture(*texture);
	m_Sprite->setOrigin(16, 16);
}

void Pinecone::Update(float tf)
{
}

bool Pinecone::checkProjectiles()
{
	return false;
}

void Pinecone::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(*m_Sprite);
}
