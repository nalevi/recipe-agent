//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/indigents/indigent_manager.h"

#include <spdlog/spdlog.h>

#include <source_location>

namespace recipeagent {
namespace indigent {

  namespace db = recipeagent::database;

  IndigentManager::IndigentManager(const std::shared_ptr<SQLite::Database> &db) : m_db{ db } {}

  bool IndigentManager::add_indigent(Indigent indigent)
  {
    auto loc = std::source_location::current();
    spdlog::info("[{}]", loc.function_name());

    db::QueryBuilder query(m_db);

    query.insert("indigents", "name, type").values("?, ?").bind(1, indigent.name).bind(2, indigent.type);
    
    return query.execute();
  }
}// namespace indigent
}// namespace recipeagent