//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/recipe_db/recipe_db.h"

namespace recipeagent {
namespace database {
  RecipeDatabase::RecipeDatabase(const std::shared_ptr<SQLite::Database> &db) : m_db(db) {}
}// namespace database
}// namespace recipeagent