cmake_minimum_required(VERSION 3.19)
project(cpplexer VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

set(cpplexer_SOURCES
  code/cpplexer.cpp
  code/cpplexer/token_ids.cpp
  code/cpplexer/re2clex/aq.cpp
  code/cpplexer/re2clex/cpp_re.cpp
)
set(cpplexer_HEADERS
  code/cpplexer.hpp
  code/cpplexer/detail/combine_policies.hpp
  code/cpplexer/detail/multi_pass.hpp
  code/cpplexer/detail/no_check_policy.hpp
  code/cpplexer/detail/position_iterator.hpp
  code/cpplexer/detail/ref_counted_policy.hpp
  code/cpplexer/detail/split_functor_input_policy.hpp
  code/cpplexer/detail/split_std_deque_policy.hpp
  code/cpplexer/re2clex/aq.hpp
  code/cpplexer/re2clex/cpp_re.hpp
  code/cpplexer/re2clex/cpp_re2c_lexer.hpp
  code/cpplexer/re2clex/scanner.hpp
  code/cpplexer/re2clex/strict_cpp_re.hpp
  code/cpplexer/boost_iterator_categories.hpp
  code/cpplexer/config.hpp
  code/cpplexer/convert_trigraphs.hpp
  code/cpplexer/cpplexer_exceptions.hpp
  code/cpplexer/cpplexer_gen.hpp
  code/cpplexer/cpp_lex_interface.hpp
  code/cpplexer/cpp_lex_interface_generator.hpp
  code/cpplexer/cpp_lex_iterator.hpp
  code/cpplexer/cpp_lex_token.hpp
  code/cpplexer/equality.hpp
  code/cpplexer/file_position.hpp
  code/cpplexer/identity.hpp
  code/cpplexer/is_iterator.hpp
  code/cpplexer/iterator_adaptor.hpp
  code/cpplexer/iterator_facade.hpp
  code/cpplexer/language_support.hpp
  code/cpplexer/lazy_conditional.hpp
  code/cpplexer/multi_pass.hpp
  code/cpplexer/multi_pass_fwd.hpp
  code/cpplexer/multi_pass_wrapper.hpp
  code/cpplexer/position_iterator.hpp
  code/cpplexer/position_iterator_fwd.hpp
  code/cpplexer/token_cache.hpp
  code/cpplexer/token_ids.hpp
  code/cpplexer/validate_universal_char.hpp
)

add_library(cpplexer STATIC
  ${cpplexer_SOURCES}
  ${cpplexer_HEADERS}
)

install(
  TARGETS cpplexer
  EXPORT cpplexer
)
export(
  TARGETS cpplexer
  NAMESPACE cpplexer::
  FILE "${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/CppLexer/CppLexerTargets.cmake"
)

file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/lib/cmake/CppLexer/CppLexerConfig.cmake"
  "set(CppLexer_FOUND 1)"
)
