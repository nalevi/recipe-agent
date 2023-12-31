//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/recipe_db/recipe_db.h"

#include <spdlog/spdlog.h>

#include <source_location>
#include <sstream>
#include <assert.h>

namespace recipeagent {
namespace database {
  QueryBuilder::QueryBuilder(const std::shared_ptr<SQLite::Database> &db)
    : m_stmt{}, m_stmt_str{}, m_type{ Type::SELECT }, m_db(db)
  {}

  bool QueryBuilder::execute()
  {
    auto success = false;
    auto loc = std::source_location::current();
    try {
      if (m_stmt.has_value()) {
        if (m_type == Type::SELECT) {
          success = m_stmt->executeStep();
        } else {
          m_stmt->exec();
        }
      } else {
        spdlog::error("[{}] Can not execute query, as it was not yet prepared!", loc.function_name());
        success = false;
      }
    } catch (std::exception &ex) {
      spdlog::error("[{}] Query execution failed: {}", loc.function_name(), ex.what());
      success = false;
    } catch (...) {
      spdlog::error("[{}] Unknown exception caught during query execution!", loc.function_name());
      success = false;
    }

    return success;
  }

  QueryBuilder &QueryBuilder::select(std::string_view fields) {
    assert(fields.empty() == false);
    // SELECT should be in the beginning of the statement
    if (!m_stmt_str.empty()) {
      m_stmt_str.clear();
    }

    m_type = Type::SELECT;
    m_stmt_str.append(fields);

    return *this;
  }


  QueryBuilder &QueryBuilder::insert(std::string_view fields) { return *this; }

  QueryBuilder &QueryBuilder::delete_from(const std::string &table_name) { return *this; }

  QueryBuilder &QueryBuilder::from(std::string_view table_name) {
    assert(m_stmt_str.empty() == false);
    assert(table_name.empty() == false);

    m_stmt_str.append(table_name);
    return *this;
  }

  QueryBuilder &QueryBuilder::where(std::string_view query_str) {
    assert(!m_stmt_str.empty());
    assert(!query_str.empty());

    m_stmt_str.append(query_str);

    return *this;
  }

  QueryBuilder &QueryBuilder::update(std::string_view table_name) { return *this; }

  template<typename T> T QueryBuilder::get_column(const int index) { return m_stmt->getColumn(index); }

  template<String... Args> QueryBuilder &QueryBuilder::set(Args... args) { return *this; }

  template<typename T> QueryBuilder &QueryBuilder::bind(std::size_t index, T value) {
    assert(!m_stmt_str.empty());
    auto loc = std::source_location::current();

    try {
      m_stmt = SQLite::Statement(*m_db, m_stmt_str);
      m_stmt->bind(index, value);
    } catch (std::exception &ex) {
      spdlog::error("[{}] Bind failed: {}", loc.function_name(), ex.what());
    } catch (...) {
      spdlog::error("[{}] Unknown exception caught during binding!", loc.function_name());
    }

    return *this;
  }

}// namespace database
}// namespace recipeagent