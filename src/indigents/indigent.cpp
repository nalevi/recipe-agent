//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/indigents/indigent.h"

namespace recipeagent {
namespace indigent {
  Indigent::Indigent(const std::string &name, IndigentType type) : name{ name }, type{ type } {}
  Indigent::Indigent() : name{}, type{ IndigentType::UNKNOWN } {}
}// namespace indigent
}// namespace recipeagent