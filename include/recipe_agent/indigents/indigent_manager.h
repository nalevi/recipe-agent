//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_INDIGENT_MANAGER_H
#define RECIPE_AGENT_INDIGENT_MANAGER_H

#include "indigent.h"

#include "recipe_agent/recipe_db/recipe_db.h"

#include "SQLiteCpp/SQLiteCpp.h"

#include <memory>

namespace recipeagent {
namespace indigent {

  class IndigentManager
  {
  public:
    IndigentManager() = delete;

    explicit IndigentManager(const std::shared_ptr<SQLite::Database> &db);

    IndigentManager(const IndigentManager &other) = delete;
    IndigentManager &operator=(const IndigentManager &other) = delete;

    IndigentManager(IndigentManager &&other) noexcept = default;
    IndigentManager &operator=(IndigentManager &&other) noexcept = default;

    ~IndigentManager() = default;

    bool add_indigent(const Indigent &indigent);

  private:
    std::shared_ptr<SQLite::Database> m_db;
  };

}// namespace indigent
}// namespace recipeagent

#endif// RECIPE_AGENT_INDIGENT_MANAGER_H
