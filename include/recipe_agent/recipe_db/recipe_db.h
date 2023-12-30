//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_RECIPE_DB_H
#define RECIPE_AGENT_RECIPE_DB_H

#include "SQLiteCpp/SQLiteCpp.h"

#include <memory>
#include <type_traits>

namespace recipeagent {
namespace database {

  template <typename... Args>
  concept String = (std::is_same_v<std::remove_cvref_t<Args>, std::string> && ...);

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
  class QueryBuilder
  {
  public:
    QueryBuilder() = delete;
    explicit QueryBuilder(const std::shared_ptr<SQLite::Database> &db);

    /// @brief After assembling the statement, you can execute it on the database as one transaction.
    void execute() const;

    /// @brief Add the column names in std::strings to be returned.
    ///
    /// @tparam Args - Must be std::string.
    /// @param args - The column names.
    /// @return the QueryBuilder itself as a reference.
    template<String...Args>
    QueryBuilder& select(Args... args);

    /// @brief The table to be updated.
    ///
    /// @param table_name - The table name.
    /// @return Itself.
    QueryBuilder& update(const std::string &table_name);

    template<String...Args>
    QueryBuilder& insert(Args... args);

    /// @brief The columns to be updated.
    ///
    /// @tparam Args - Must be std::string.
    /// @param args - The column names.
    template<String...Args>
    QueryBuilder& set(Args... args);

    QueryBuilder& delete_from(const std::string &table_name);

    QueryBuilder& from(const std::string &table_name);
    QueryBuilder& where(const std::string &query_str);

    template<typename T>
    QueryBuilder& bind(std::size_t index, T value);

    template<typename T>
    void get_column(const int index, T& column);

  private:
    enum class Type {
      SELECT,
      INSERT,
      UPDATE,
      DELETE
    };

    std::optional<SQLite::Statement> m_stmt;
    std::string m_stmt_str;
    Type m_type;
    std::shared_ptr<SQLite::Database> m_db;
  };

}// namespace database
}// namespace recipeagent

#endif// RECIPE_AGENT_RECIPE_DB_H
