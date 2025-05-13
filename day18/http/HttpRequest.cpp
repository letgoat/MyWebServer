#include "HttpRequest.h"
#include "HttpContext.h"

#include <string>
#include <iostream>

HttpRequest::HttpRequest():method_(Method::kInvaild),version_(Version::kUnknown){
};

HttpRequest::~HttpRequest(){};

