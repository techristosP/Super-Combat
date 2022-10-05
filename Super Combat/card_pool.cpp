#include "card_pool.h"

CardPool* CardPool::m_instance = nullptr;

CardPool* CardPool::getInstance()
{
    if (!m_instance) {
        m_instance = new CardPool();
    }

    return m_instance;
}

void CardPool::releaseInstance()
{
    if (m_instance)
        delete m_instance;

    m_instance = nullptr;
}

// Returns a copy of a random card from the card pool.
Card* CardPool::drawCard()
{
    float test = RAND0TO1();
    int index{};

    // 13% chance to draw shield, 13% chance to draw heal, 74% chance to draw attack card.
    // uniform chance to draw any of shield/heal/attack card, add 0.5 to round.
    if (test <= 0.13) { index = static_cast<int>(RAND0TO1() + 6 + 0.5); } 
    else if (test <= 0.26) { index = static_cast<int>(RAND0TO1() + 8 + 0.5); } 
    else { index = static_cast<int>(RAND0TO1() * 5 + 0.5); }

    Card* newCard = m_instance->m_pool[index];

    // Determine card type, and use corresponding constructor.
    if (typeid(*newCard) == typeid(AttackCard))
        return new AttackCard(*(dynamic_cast<AttackCard*>(newCard)));

    if (typeid(*newCard) == typeid(ShieldCard))
        return new ShieldCard(*(dynamic_cast<ShieldCard*>(newCard)));

    if (typeid(*newCard) == typeid(HealCard))
        return new HealCard(*(dynamic_cast<HealCard*>(newCard)));

    return nullptr;
}

CardPool::~CardPool()
{
    for (auto card : m_pool)
    {
        delete card;
    }
}

