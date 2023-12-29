//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_RECIPE_DB_H
#define RECIPE_AGENT_RECIPE_DB_H

#include <SQLiteCpp/SQLiteCpp.h>

#include <memory>

namespace recipeagent {
namespace database {

  /// @brief Encapsulates the whole database management of the application.
  class RecipeDatabase
  {
  public:
    RecipeDatabase() = default;
    explicit RecipeDatabase(const std::shared_ptr<SQLite::Database> &db);


  private:
    std::shared_ptr<SQLite::Database> m_db;
  };

  class QueryBuilder
  {
  };

}// namespace database
}// namespace recipeagent

#endif// RECIPE_AGENT_RECIPE_DB_H
