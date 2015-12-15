/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef ALIYUN_UTILS_ANY_H_
#define ALIYUN_UTILS_ANY_H_

#include <algorithm>
#include <typeinfo>
#include <string>
#include "StringUtils.h"

namespace aliyun {
namespace utils {

class Any {
 public:
  Any()
      : content_(0) {
  }

  // ValueType must copyable
  template<typename ValueType>
  explicit Any(const ValueType& value)
      : content_(new Holder<ValueType>(value)) {
  }

  Any(const Any& rhs)
      : content_(rhs.content_ ? rhs.content_->clone() : 0) {
  }

  ~Any() {
    delete content_;
  }

  Any& swap(Any& rhs) {
    std::swap(content_, rhs.content_);
    return *this;
  }

  template<typename ValueType>
  Any& operator=(const ValueType& rhs) {
    Any tmp(rhs);
    tmp.swap(*this);
    return *this;
  }

  Any& operator=(const Any& rhs) {
    Any tmp(rhs);
    tmp.swap(*this);
    return *this;
  }

  const std::type_info& type() const {
    return content_ ? content_->type() : typeid(void);
  }

  std::string toString() const {
    return content_ ? content_->toString() : "";
  }

  bool empty() const {
    return !content_;
  }

 private:
  struct PlaceHolder {
    virtual ~PlaceHolder() {
    }

    virtual const std::type_info& type() const = 0;

    virtual PlaceHolder* clone() const = 0;

    virtual std::string toString() const = 0;
  };

  template<typename ValueType>
  struct Holder : public PlaceHolder {
    ValueType held_;

    explicit Holder(const ValueType& value)
        : held_(value) {
    }

    virtual const std::type_info& type() const {
      return typeid(ValueType);
    }

    virtual PlaceHolder* clone() const {
      return new Holder(held_);
    }

    virtual std::string toString() const {
      return utils::StringUtils::ToString(held_);
    }

   private:
    Holder& operator=(const Holder& rhs);
  };

  template<typename ValueType>
  friend ValueType* AnyCast(Any* operand);

  template<typename ValueType>
  friend ValueType* UnsafeAnyCast(Any* operand);

 private:
  PlaceHolder* content_;
};

inline void swap(Any& lhs, Any& rhs) {
  lhs.swap(rhs);
}

class BadAnyCast : public std::bad_cast {
 public:
  virtual const char * what() const throw() {
    return "aliyun::Any: failed conversion using aliyun::AnyCast";
  }
};

template<typename ValueType>
ValueType* AnyCast(Any* operand) {
  return operand && operand->type() == typeid(ValueType) ?
          &(static_cast<Any::Holder<ValueType>*>(operand->content_)->held_) : 0;
}

template<typename ValueType>
inline const ValueType* AnyCast(const Any* operand) {
  return AnyCast<ValueType>(const_cast<Any*>(operand));
}

template <typename T>
struct TypeTraits {
  typedef T Type;
};

template <typename T>
struct TypeTraits<const T> {
  typedef T Type;
};

template <typename T>
struct TypeTraits<T&> {
  typedef T Type;
};

template <typename T>
struct TypeTraits<const T&> {
  typedef T Type;
};

template<typename ValueType>
inline ValueType AnyCast(const Any& operand) {
  Any& nonconst = const_cast<Any&>(operand);
  typedef typename TypeTraits<ValueType>::Type NonRef;
  NonRef* result = AnyCast<NonRef>(&nonconst);
  if (!result) {
    throw BadAnyCast();
  }
  return *result;
}

template<typename ValueType>
inline ValueType* UnsafeAnyCast(Any* operand) {
  return &(static_cast<Any::Holder<ValueType>*>(operand->content_)->held_);
}

template<typename ValueType>
inline const ValueType* UnsafeAnyCast(const Any* operand) {
  return UnsafeAnyCast<ValueType>(const_cast<Any*>(operand));
}

}  // namespace utils
}  // namespace aliyun

#endif  // ALIYUN_UTILS_ANY_H_
