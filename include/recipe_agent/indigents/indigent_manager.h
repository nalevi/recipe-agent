//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_INDIGENT_MANAGER_H
#define RECIPE_AGENT_INDIGENT_MANAGER_H

#include "indigent.h"

#include "recipe_agent/recipe_db/recipe_db.h"

namespace recipeagent {
namespace indigent {

  namespace db = recipeagent::database;

  class IndigentManager
  {
  public:
    IndigentManager() = default;
    explicit IndigentManager(const std::shared_ptr<db::RecipeDatabase> &mDb);

  private:
    std::shared_ptr<db::RecipeDatabase> m_db;
  };

}// namespace indigent
}// namespace recipeagent

#endif// RECIPE_AGENT_INDIGENT_MANAGER_H
