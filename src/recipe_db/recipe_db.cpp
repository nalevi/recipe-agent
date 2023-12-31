//
// Created by Nagy Levente on 29/12/2023.
//

#include "recipe_agent/recipe_db/recipe_db.h"

#include <spdlog/spdlog.h>

#include <assert.h>
#include <source_location>

namespace recipeagent {
namespace database {

  QueryBuilder::QueryBuilder(const std::shared_ptr<SQLite::Database> &db, const std::shared_ptr<spdlog::logger> &logger)
    : m_logger{ logger }, m_stmt{}, m_stmt_str{}, m_type{ Type::SELECT }, m_db(db)
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
          auto res = m_stmt->exec();
          m_logger->info("[{}] {} rows updated", loc.function_name(), res);
          success = true;
        }
      } else {
        m_logger->error("[{}] Can not execute query, as it was not yet prepared!", loc.function_name());
        success = false;
      }
    } catch (std::exception &ex) {
      m_logger->error("[{}] Query execution failed: {}", loc.function_name(), ex.what());
      success = false;
    } catch (...) {
      m_logger->error("[{}] Unknown exception caught during query execution!", loc.function_name());
      success = false;
    }

    return success;
  }

  QueryBuilder &QueryBuilder::select(std::string_view columns) noexcept
  {
    if (columns.empty()) {
      m_logger->error("Input columns can not be empty!");
      return *this;
    }
    // SELECT should be in the beginning of the statement
    if (!m_stmt_str.empty()) { m_stmt_str.clear(); }

    m_type = Type::SELECT;
    m_stmt_str.append("SELECT ");
    m_stmt_str.append(columns);

    return *this;
  }


  QueryBuilder &QueryBuilder::insert(std::string_view table, std::string_view columns) noexcept
  {
    assert(!columns.empty());

    if (!m_stmt_str.empty()) { m_stmt_str.clear(); }

    m_type = Type::INSERT;
    m_stmt_str.append("INSERT INTO ");
    m_stmt_str.append(table);
    m_stmt_str.append(" (");
    m_stmt_str.append(columns);
    m_stmt_str.append(") ");

    return *this;
  }

  QueryBuilder &QueryBuilder::delete_from(std::string_view table_name) noexcept
  {
    assert(!table_name.empty());

    if (!m_stmt_str.empty()) { m_stmt_str.clear(); }

    m_type = Type::DELETE;
    m_stmt_str.append("DELETE FROM ");
    m_stmt_str.append(table_name);

    return *this;
  }

  QueryBuilder &QueryBuilder::from(std::string_view table_name) noexcept
  {
    assert(m_stmt_str.empty() == false);
    assert(table_name.empty() == false);

    m_stmt_str.append("FROM ");
    m_stmt_str.append(table_name);
    return *this;
  }

  QueryBuilder &QueryBuilder::where(std::string_view query_str) noexcept
  {
    assert(!m_stmt_str.empty());
    assert(!query_str.empty());

    m_stmt_str.append("WHERE ");
    m_stmt_str.append(query_str);

    return *this;
  }

  QueryBuilder &QueryBuilder::update(std::string_view table_name) noexcept
  {
    assert(!table_name.empty());

    if (!m_stmt_str.empty()) { m_stmt_str.clear(); }

    m_type = Type::UPDATE;
    m_stmt_str.append("UPDATE ");
    m_stmt_str.append(table_name);
    return *this;
  }

  template<typename T> T QueryBuilder::get_column(const int index) { return m_stmt->getColumn(index); }

  QueryBuilder &QueryBuilder::set(std::string_view columns) noexcept
  {
    assert(!columns.empty());
    //    static_assert(!m_stmt_str.empty());

    m_stmt_str.append(" SET ");
    m_stmt_str.append(columns);

    return *this;
  }

  QueryBuilder &QueryBuilder::values(std::string_view values) noexcept
  {
    assert(!m_stmt_str.empty());

    m_stmt_str.append(" VALUES (");
    m_stmt_str.append(values);
    m_stmt_str.append(") ");

    return *this;
  }

  template<typename T> QueryBuilder &QueryBuilder::bind(const int index, T value)
  {
    assert(!m_stmt_str.empty());
    auto loc = std::source_location::current();

    try {
      if (!m_stmt.has_value()) { m_stmt = SQLite::Statement(*m_db, m_stmt_str); }
      m_stmt->bind(index, value);
    } catch (std::exception &ex) {
      m_logger->error("[{}] Bind failed: {}", loc.function_name(), ex.what());
    } catch (...) {
      m_logger->error("[{}] Unknown exception caught during binding!", loc.function_name());
    }

    return *this;
  }

  std::shared_ptr<SQLite::Database> create_db_connection(std::string_view db_name,
    std::shared_ptr<spdlog::logger> logger)
  {

    std::shared_ptr<SQLite::Database> p_db = nullptr;
    try {
      p_db = std::make_shared<SQLite::Database>(db_name, SQLite::OPEN_READWRITE | SQLite::OPEN_CREATE);

      SQLite::Transaction transaction(*p_db);

      p_db->exec(
        "CREATE TABLE IF NOT EXISTS indigents ("
        "indigent_id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "type INTEGER NOT NULL)");

      p_db->exec(
        "CREATE TABLE IF NOT EXISTS recipes ("
        "recipe_id INTEGER PRIMARY KEY,"
        "name TEXT NOT NULL UNIQUE,"
        "type INTEGER NOT NULL)");

      p_db->exec(
        "CREATE TABLE IF NOT EXISTS recipe_steps ("
        "id INTEGER PRIMARY KEY,"
        "recipe_id INTEGER,"
        "description TEXT NOT NULL,"
        "FOREIGN KEY (recipe_id)"
        "  REFERENCES recipes (recipe_id)"
        "   ON DELETE CASCADE"
        "   ON UPDATE NO ACTION)");

      p_db->exec(
        "CREATE TABLE IF NOT EXISTS indigent_to_recipe ("
        "recipe_id INTEGER,"
        "indigent_id INTEGER,"
        "PRIMARY KEY (recipe_id, indigent_id),"
        "FOREIGN KEY (recipe_id)"
        "  REFERENCES recipes (recipe_id)"
        "   ON DELETE CASCADE"
        "   ON UPDATE NO ACTION,"
        "FOREIGN KEY (indigent_id)"
        "  REFERENCES indigents (indigent_id)"
        "    ON DELETE CASCADE"
        "    ON UPDATE NO ACTION)");

      transaction.commit();
      logger->info("Tables created!");
    } catch (std::exception &ex) {
      logger->error("Could not build connection to database, exception: {} ", ex.what());
    } catch (...) {
      logger->error("Could not build connection to database, exiting ... ");
      std::terminate();
    }
    return p_db;
  }
}// namespace database
}// namespace recipeagent