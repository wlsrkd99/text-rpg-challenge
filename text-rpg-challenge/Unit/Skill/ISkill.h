#pragma once
#include <string>
#include "../Character.h"

namespace TextRPG
{
    class ISkill
    {
    public:
        virtual ~ISkill() = default;

        virtual std::string GetName() const = 0;
        virtual int GetManaCost() const = 0;

        virtual void Use(Character* caster, Character* target) = 0;
    };
}
