//
// Created by Nagy Levente on 29/12/2023.
//

#ifndef RECIPE_AGENT_INDIGENT_H
#define RECIPE_AGENT_INDIGENT_H

#include <concepts>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace recipeagent {
namespace indigent {

  /// @brief All the possible categories of the indigents in the database.
  enum class IndigentType {
    UNKNOWN,
    SEED,
    FRUIT,
    VEGETABLE,
    DAIRY,
    MEAT,
    FAT,
    BREAD,
    SNACK,
    CONDIMENT,
  };

  const std::vector<std::string> INDIGENT_TYPES{
    "UNKNOWN",
    "SEED",
    "FRUIT",
    "VEGETABLE",
    "DAIRY",
    "MEAT",
    "FAT",
    "BREAD",
    "SNACK",
    "CONDIMENT",
  };

  template<class IndType>
    requires std::is_enum_v<IndType>
  inline static constexpr std::string convert_type_to_str(IndType type) noexcept
  {
    switch (type) {
    case IndigentType::UNKNOWN:
      return "UNKNOWN";
    case IndigentType::SEED:
      return "SEED";
    case IndigentType::FRUIT:
      return "FRUIT";
    case IndigentType::VEGETABLE:
      return "VEGETABLE";
    case IndigentType::DAIRY:
      return "DAIRY";
    case IndigentType::MEAT:
      return "MEAT";
    case IndigentType::FAT:
      return "FAT";
    case IndigentType::BREAD:
      return "BREAD";
    case IndigentType::SNACK:
      return "SNACK";
    case IndigentType::CONDIMENT:
      return "CONDIMENT";
    default:
      return "UNKNOWN";
    }

    return "";
  }

  struct Indigent
  {
    std::string name;
    IndigentType type;

    Indigent();
    Indigent(std::string_view name, IndigentType type);
  };

}// namespace indigent
}// namespace recipeagent

#endif// RECIPE_AGENT_INDIGENT_H
