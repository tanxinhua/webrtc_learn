/**
* Copyright 2022 xinhuatan  All rights reserved.
* Author: tanxinhuaxt@163.com
*/

#pragma once
#include <map>
#include <string>
#include <vector>

#define META_COMPARE_FOURCC(x, y) (x[0] == y[0] && x[1] == y[1] && x[2] == y[2] && x[3] == y[3])

namespace wgbase {
class MetaData {
 public:
  explicit MetaData(const std::string& str);
  const std::string& GetUri() const;

 private:
  std::string uri_;
};
}  // namespace wgbase
