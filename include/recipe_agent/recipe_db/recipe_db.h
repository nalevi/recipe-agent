//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_RECIPE_DB_H
#define RECIPE_AGENT_RECIPE_DB_H

#include "SQLiteCpp/SQLiteCpp.h"

#include <memory>
#include <spdlog/logger.h>
#include <type_traits>

namespace recipeagent {
namespace database {

  template<typename... Args>
  concept String = (std::is_same_v<std::remove_cvref_t<Args>, std::string> && ...);

  /// @brief This function is used to create the whole database from scratch.
  /// It will only create the database from scratch, when it is not present.
  /// @param db_name The name of the database file.
  /// @return A shared pointer to the database connection.
  [[maybe_unused]] std::shared_ptr<SQLite::Database> create_db_connection(std::string_view db_name,
    std::shared_ptr<spdlog::logger> logger);


  /// @brief You can build any kind of SQL queries with this class and then execute it on a database.
  ///
  /// @example
  /// @code
  /// QueryBuilder query(db);
  /// query.select("*");
  /// query.from("indigents");
  /// query.where("name = ?");
  /// query.bind(1, "carrot");
  ///
  /// query.execute();
  /// // Get results
  /// Indigent element;
  /// query.get_column(1, element);
  /// @endcode
  class QueryBuilder
  {
  public:
    QueryBuilder() = delete;
    explicit QueryBuilder(const std::shared_ptr<SQLite::Database> &db, const std::shared_ptr<spdlog::logger> &logger);

    /// @brief After assembling the statement, you can execute it on the database as one transaction.
    /// In case of a DQL (e.g.: SELECT) transaction, it can be recalled until it returns false, so
    /// you can return the rows step-by-step.
    /// @example
    /// @code
    /// // DQL
    /// int breed_type{};
    /// QueryBuilder q(db);
    /// q.select("cat_breed").from("animals").where("name = ?").bind(1, "mikkamakka");
    /// while(q.execute()) {
    ///   breed_type = q.get_column(1);
    /// }
    /// @endcode
    /// @return Returns true in case the execution  was successful or false, if not.
    bool execute();

    /// @brief Add the column names in std::strings to be returned.
    ///
    /// @param fields - The column names.
    /// @return the QueryBuilder itself as a reference.
    QueryBuilder &select(std::string_view columns) noexcept;

    /// @brief The table to be updated.
    ///
    /// @param table_name - The table name.
    /// @return Itself.
    [[maybe_unused]] QueryBuilder &update(std::string_view table_name) noexcept;

    [[maybe_unused]] QueryBuilder &insert(std::string_view table, std::string_view columns) noexcept;

    // TODO: This is not really user-friendly.
    [[maybe_unused]] QueryBuilder &values(std::string_view values) noexcept;

    /// @brief The columns to be updated.
    ///
    [[maybe_unused]] QueryBuilder &set(std::string_view columns) noexcept;

    [[maybe_unused]] QueryBuilder &delete_from(std::string_view table_name) noexcept;

    [[maybe_unused]] QueryBuilder &from(std::string_view table_name) noexcept;
    [[maybe_unused]] QueryBuilder &where(std::string_view query_str) noexcept;

    template<typename T> [[maybe_unused]] QueryBuilder &bind(const int index, T value);

    template<typename T> [[maybe_unused]] T get_column(const int index);

  private:
    enum class Type { SELECT, INSERT, UPDATE, DELETE };

    std::shared_ptr<spdlog::logger> m_logger;
    std::optional<SQLite::Statement> m_stmt;
    std::string m_stmt_str;
    Type m_type;
    std::shared_ptr<SQLite::Database> m_db;
  };

}// namespace database
}// namespace recipeagent

#endif// RECIPE_AGENT_RECIPE_DB_H
