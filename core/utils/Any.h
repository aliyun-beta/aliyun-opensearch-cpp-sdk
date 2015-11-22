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

#ifndef CORE_UTILS_ANY_H_
#define CORE_UTILS_ANY_H_

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
  Any(const ValueType& value)
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
    Any(rhs).swap(*this);
    return *this;
  }

  Any& operator=(Any& rhs) {
    Any(rhs).swap(*this);
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

    Holder(const ValueType& value)
        : held_(value) {
    }

    virtual const std::type_info& type() const {
      return typeid(ValueType);
    }

    virtual PlaceHolder* clone() const {
      return new ValueType(held_);
    }

    virtual std::string toString() const {
      utils::StringUtils::ToString(held_);
    }

   private:
    Holder& operator=(const Holder& rhs);
  };

 private:

  template<typename ValueType>
  friend ValueType* any_cast(Any*);

  template<typename ValueType>
  friend ValueType* unsafe_any_cast(Any*);

  PlaceHolder* content_;
};

inline void swap(Any& lhs, Any& rhs) {
  lhs.swap(rhs);
}

class bad_any_cast : public std::bad_cast {
 public:
  virtual const char * what() const throw () {
    return "aliyun::Any: failed conversion using aliyun::any_cast";
  }
};

template<typename ValueType>
ValueType* any_cast(Any* operand) {
  return
      operand && operand->type() == typeid(ValueType) ?
          &static_cast<Any::Holder<ValueType>*>(operand->content_)->held_ : 0;
}

template<typename ValueType>
inline const ValueType* any_cast(const Any* operand) {
  return any_cast<ValueType>(const_cast<Any*>(operand));
}

template<typename ValueType>
inline ValueType any_cast(Any& operand) {
  ValueType* result = any_cast<ValueType>(&operand);
  if (!result) {
    throw bad_any_cast();
  }
  return *result;
}

template<typename ValueType>
inline ValueType any_cast(const Any& operand) {
  return any_cast<ValueType>(const_cast<Any&>(operand));
}

template<typename ValueType>
inline ValueType* unsafe_any_cast(Any* operand) throw () {
  return &static_cast<Any::Holder<ValueType>*>(operand->content_)->held_;
}

template<typename ValueType>
inline const ValueType* unsafe_any_cast(const Any* operand) throw () {
  return unsafe_any_cast<ValueType>(const_cast<Any*>(operand));
}

}  // namespace utils
}  // namespace aliyun

#endif  // CORE_UTILS_ANY_H_
