//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/recipe_db/recipe_db.h"

#include <spdlog/spdlog.h>

#include <source_location>
#include <sstream>

namespace recipeagent {
namespace database {
  QueryBuilder::QueryBuilder(const std::shared_ptr<SQLite::Database> &db) : m_stmt{}, m_stmt_str{}, m_type{Type::SELECT}, m_db(db) {}

  void QueryBuilder::execute() const {
    auto loc = std::source_location::current();
    try {
      if (m_stmt.has_value()) {
        if (m_type != Type::SELECT) {
          // TODO
//          m_stmt->exec();
        }
      }
    } catch (std::exception& ex) {
      spdlog::error("[{}] Query execution failed: {}", loc.function_name(), ex.what());
    } catch (...) {

      spdlog::error("[{}] Unknown exception caught during query execution!", loc.function_name());
    }
  }

  template<String... Args> QueryBuilder &QueryBuilder::select(Args... args) { return *this; }


  template<String... Args> QueryBuilder &QueryBuilder::insert(Args... args) { return *this; }

  QueryBuilder &QueryBuilder::delete_from(const std::string &table_name) { return *this; }

  QueryBuilder &QueryBuilder::from(const std::string &table_name) { return *this; }

  QueryBuilder &QueryBuilder::where(const std::string &query_str) { return *this; }
  QueryBuilder &QueryBuilder::update(const std::string &table_name) { return *this; }

  template<typename T> void QueryBuilder::get_column(const int index, T &column) {
    column = m_stmt->getColumn(index);
  }

  template<String... Args> QueryBuilder &QueryBuilder::set(Args... args) { return *this; }

  template<typename T> QueryBuilder &QueryBuilder::bind(std::size_t index, T value) { return *this; }

}// namespace database
}// namespace recipeagent