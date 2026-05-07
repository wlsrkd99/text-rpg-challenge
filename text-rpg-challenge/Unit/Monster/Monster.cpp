#include "Monster.h"

namespace TextRPG
{
    Monster::Monster(const std::string& name) : Character(name), m_DropExp(0), m_DropGold(0), m_DropItem(nullptr)
    {

    }

    Monster::~Monster()
    {
        if (m_DropItem)
        {
            delete m_DropItem;
            m_DropItem = nullptr;
        }
    }

    ItemBase* Monster::ReleaseDropItem()
    {
        ItemBase* item = m_DropItem;
        m_DropItem = nullptr;
        return item;
    }
}
