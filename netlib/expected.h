// Copyright 2019 Google LLC
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#ifndef CPP_NETLIB_EXPECTED_H_
#define CPP_NETLIB_EXPECTED_H_

#include <cassert>
#include <type_traits>
#include <utility>

namespace cppnetlib {

/// This is a minimal implementation of the proposed P0323R3
/// std::unexpected<...> API.
template <class E>
// requires EqualityComparable<E> && (CopyConstructible<E> ||
// MoveConstructible<E>)
class unexpected {
 public:
  static_assert(!std::is_same_v<E, void>, "unexpected<void> is not supported.");

  unexpected() = delete;
  constexpr explicit unexpected(const E& e) : value_(e) {}
  constexpr explicit unexpected(E&& e) : value_(std::move(e)) {}
  constexpr const E& value() const& { return value_; }
  constexpr E& value() & { return value_; }
  constexpr E&& value() && { return std::move(value_); }
  constexpr const E&& value() const&& { return std::move(value_); }

  // This is a deviation from the proposal which suggests that these functions
  // are namespace-level functions, instead of ADL-only found comparison
  // operators (defined friend free functions).
  template <class F>
  friend constexpr bool operator==(const unexpected<F>& lhs,
                                   const unexpected<F>& rhs) {
    return lhs.value_ == rhs.value_;
  }

  template <class F>
  friend constexpr bool operator!=(const unexpected<F>& lhs,
                                   const unexpected<F>& rhs) {
    return lhs.value_ == rhs.value_;
  }

 private:
  E value_;
};

struct unexpect_t {
  unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};

template <class E>
class bad_expected_access;

template <>
class bad_expected_access<void> {
 public:
  virtual const char* what() const noexcept {
    return "cppnetlib::bad_expected_access";
  }
  virtual ~bad_expected_access() {}
};

template <class E>
class bad_expected_access : public bad_expected_access<void> {
 public:
  explicit bad_expected_access(E e) : val_(e) {}
  const char* what() const noexcept override {
    return "cppnetlib::bad_expected_access<E>";
  }
  const E& error() const&;
  E& error() &;
  E&& error() &&;

 private:
  E val_;
};

/// This is a minimal implementation of the proposed P0323R3
/// std::expected<...> API.
template <class T, class E>
class expected {
 public:
  using value_type = T;
  using error_type = E;
  using unexpected_type = unexpected<E>;

  template <class U>
  struct rebind {
    using type = expected<U, error_type>;
  };

  constexpr expected() : has_value_(false) {}
  constexpr expected(const expected& other)
      : union_storage_(other.union_storage_), has_value_(other.has_value_) {}

  constexpr expected(
      expected&& other,
      std::enable_if_t<std::is_move_constructible_v<T> &&
                       std::is_move_constructible_v<E>>* =
          0) noexcept(std::is_nothrow_move_constructible_v<T>&&
                          std::is_nothrow_move_constructible_v<E>)
      : has_value_(other.has_value_) {
    if (other.has_value_)
      new (union_storage_)
          T(std::move(*reinterpret_cast<T*>(other.union_storage_)));
    else
      new (union_storage_)
          unexpected<E>(std::move(*reinterpret_cast<E*>(other.union_storage_)));
  }

 private:
  template <class U, class G>
  using constructor_helper_t =
      std::enable_if_t<std::is_constructible_v<T, const U&> and
                       std::is_constructible_v<E, const G&> and
                       !std::is_constructible_v<T, expected<U, G>&> and
                       !std::is_constructible_v<T, expected<U, G>&&> and
                       !std::is_constructible_v<T, const expected<U, G>&> and
                       !std::is_constructible_v<T, const expected<U, G>&&> and
                       !std::is_convertible_v<expected<U, G>&, T> and
                       !std::is_convertible_v<expected<U, G>&&, T> and
                       !std::is_convertible_v<const expected<U, G>&, T> and
                       !std::is_convertible_v<const expected<U, G>&&, T>>;

 public:
  // TODO: c++20 has support for conditional explicit, use that instead of the
  // duplication.
  template <class U, class G,
            std::enable_if_t<!(std::is_convertible_v<const U&, T> and
                               std::is_convertible_v<const G&, E>),
                             bool> = false>
  explicit constexpr expected(const expected<U, G>& other,
                              constructor_helper_t<U, G>* = 0)
      : has_value_(other.has_value_) {
    if (other.has_value_)
      new (&union_storage_) T(*other);
    else
      new (&union_storage_) unexpected<E>(other.error());
  }

  template <class U, class G,
            std::enable_if_t<(std::is_convertible_v<const U&, T> and
                              std::is_convertible_v<const G&, E>),
                             bool> = false>
  constexpr expected(const expected<U, G>& other,
                     constructor_helper_t<U, G>* = 0)
      : has_value_(other.has_value_) {
    if (other.has_value_)
      new (&union_storage_) T(*other);
    else
      new (&union_storage_) unexpected<E>(other.error());
  }

  template <class U, class G,
            std::enable_if_t<!(std::is_convertible_v<const U&, T> and
                               std::is_convertible_v<const G&, E>),
                             bool> = false>
  explicit constexpr expected(const expected<U, G>&& other,
                              constructor_helper_t<U, G>* = 0)
      : has_value_(other.has_value_) {
    if (other.has_value_)
      new (&union_storage_) T(std::move(*other));
    else
      new (&union_storage_) unexpected<E>(std::move(unexpected(other.error())));
  }

  template <class U = T,
            std::enable_if_t<!std::is_convertible_v<U&&, T>, bool> = false>
  explicit constexpr expected(
      U&& value,
      std::enable_if_t<std::is_constructible_v<T, U&&> and
                       !std::is_same_v<std::decay_t<U>, std::in_place_t> and
                       !std::is_same_v<expected<T, E>, std::decay_t<U>> and
                       !std::is_same_v<unexpected<E>, std::decay_t<U>>>* = 0)
      : has_value_(true) {
    new (&union_storage_) U(std::forward<U>(std::move(value)));
  }

  template <class U = T,
            std::enable_if_t<std::is_convertible_v<U&&, T>, bool> = false>
  constexpr expected(
      U&& value,
      std::enable_if_t<std::is_constructible_v<T, U&&> and
                       !std::is_same_v<std::decay_t<U>, std::in_place_t> and
                       !std::is_same_v<expected<T, E>, std::decay_t<U>> and
                       !std::is_same_v<unexpected<E>, std::decay_t<U>>>* = 0)
      : has_value_(true) {
    new (&union_storage_) U(std::forward<U>(value));
  }

  template <class G = E,
            std::enable_if_t<!std::is_convertible_v<const G&, E>, bool> = false>
  explicit constexpr expected(unexpected<G> const& e) : has_value_(false) {
    new (&union_storage_) unexpected<E>(e);
  }

  template <class G = E,
            std::enable_if_t<std::is_convertible_v<const G&, E>, bool> = false>
  constexpr expected(unexpected<G> const& e) : has_value_(false) {
    new (&union_storage_) unexpected<E>(e);
  }

  template <class G = E,
            std::enable_if_t<!std::is_convertible_v<G&&, E>, bool> = false>
  explicit constexpr expected(unexpected<G>&& e) noexcept(
      std::is_nothrow_move_constructible_v<E, G&&>)
      : has_value_(false) {
    new (&union_storage_) unexpected<E>(std::move(e.value()));
  }

  template <class G = E,
            std::enable_if_t<std::is_convertible_v<G&&, E>, bool> = false>
  constexpr expected(unexpected<G>&& e) noexcept(
      std::is_nothrow_constructible_v<E, G&&>)
      : has_value_(false) {
    new (&union_storage_) unexpected<E>(std::move(e.value()));
  }

  // Destructor.
  ~expected() {
    if constexpr (!std::is_trivially_destructible_v<T>) {
      if (has_value_)
        reinterpret_cast<T*>(&union_storage_)->~T();
      else
        reinterpret_cast<unexpected_type*>(&union_storage_)->~unexpected_type();
    }
  }

  // Assignment.
  expected& operator=(const expected& other) {
    if (has_value_ and other.has_value_) {
      **this = *other;
      return *this;
    }

    if (!has_value_ and !other.has_value_) {
      (*reinterpret_cast<unexpected_type*>(&union_storage_)) =
          unexpected(other.error());
      return *this;
    }

    if (has_value_ and !other.has_value_) {
      reinterpret_cast<T*>(&union_storage_)->~T();
      new (&union_storage_) unexpected_type(unexpected(other.error()));
      has_value_ = false;
      return *this;
    }

    if (!has_value_ and other.has_value_) {
      if constexpr (std::is_nothrow_copy_constructible_v<T>) {
        reinterpret_cast<unexpected_type*>(&union_storage_)->~unexpected_type();
        new (&union_storage_) T(other.value());
        has_value_ = true;
      } else if constexpr (std::is_nothrow_move_constructible_v<T>) {
        T tmp = *other;
        reinterpret_cast<unexpected_type*>(&union_storage_)->~unexpected_type();
        new (&union_storage_) T(tmp);
        has_value_ = true;
      } else if constexpr (std::is_nothrow_move_constructible_v<E>) {
        unexpected_type tmp = unexpected(std::move(error()));
        reinterpret_cast<unexpected_type*>(&union_storage_)->~unexpected_type();
        try {
          new (&union_storage_) T(*other);
          has_value_ = true;
        } catch (...) {
          new (&union_storage_) unexpected_type(std::move(tmp));
        }
      }
    }

    return *this;
  }

  // Observer functions.
  constexpr const T* operator->() const {
    if (!has_value_) throw bad_expected_access<E>(error());
    return reinterpret_cast<const T*>(&union_storage_);
  }

  constexpr T* operator->() {
    if (!has_value_) throw bad_expected_access<E>(error());
    return reinterpret_cast<T*>(&union_storage_);
  }

  constexpr const T& operator*() const& {
    if (!has_value_) throw bad_expected_access<E>(error());
    return *reinterpret_cast<const T*>(&union_storage_);
  }

  constexpr T& operator*() & {
    if (!has_value_) throw bad_expected_access<E>(error());
    return *reinterpret_cast<T*>(&union_storage_);
  }

  constexpr const T&& operator*() const&& {
    if (!has_value_) throw bad_expected_access<E>(error());
    return std::move(*reinterpret_cast<const T*>(&union_storage_));
  }

  constexpr T&& operator*() && {
    if (!has_value_) throw bad_expected_access<E>(error());
    return std::move(*reinterpret_cast<T*>(&union_storage_));
  }

  constexpr explicit operator bool() const noexcept { return has_value_; }
  constexpr bool has_value() const noexcept { return has_value_; }

  constexpr const T& value() const& { return **this; }
  constexpr T& value() & { return **this; }
  constexpr const T&& value() const&& { return std::move(**this); }
  constexpr T&& value() && { return std::move(**this); }

  constexpr const E& error() const& {
    assert(!has_value_ &&
           "expected<T, E> must not have a value when taking an error!");
    return reinterpret_cast<const unexpected<E>*>(&union_storage_)->value();
  }

  constexpr E& error() & {
    assert(!has_value_ &&
           "expected<T, E> must not have a value when taking an error!");
    return reinterpret_cast<unexpected<E>*>(&union_storage_)->value();
  }

  constexpr const E&& error() const&& {
    assert(!has_value_ &&
           "expected<T, E> must not have a value when taking an error!");
    return std::move(*reinterpret_cast<const unexpected<E>*>(&union_storage_))
        ->value();
  }

  constexpr E&& error() && {
    assert(!has_value_ &&
           "expected<T, E> must not have a value when taking an error!");
    return std::move(*reinterpret_cast<unexpected<E>*>(&union_storage_))
        ->value();
  };

 private:
  std::aligned_union_t<8, value_type, unexpected_type> union_storage_;
  bool has_value_;
};

}  // namespace cppnetlib

#endif  // CPP_NETLIB_EXPECTED_H_
