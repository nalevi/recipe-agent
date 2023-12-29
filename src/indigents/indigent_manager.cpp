//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/indigents/indigent_manager.h"

namespace recipeagent {
namespace indigent {
  IndigentManager::IndigentManager(const std::shared_ptr<db::RecipeDatabase> &mDb) : m_db(mDb) {}


}// namespace indigent
}// namespace recipeagent