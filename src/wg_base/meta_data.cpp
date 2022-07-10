/**
* Copyright 2022 xinhuatan  All rights reserved.
* Author: tanxinhuaxt@163.com
*/


#include "meta_data.h"
#include <assert.h>
#include <utility>
#include <string>
#include <vector>
#include <map>

namespace wgbase {
MetaData::MetaData(const std::string& str):uri_(str) {

}
const std::string& MetaData::GetUri() const { return uri_; }
}  // namespace wgbases