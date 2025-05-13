#include "HttpRequest.h"
#include "HttpContext.h"

#include <string>
#include <iostream>

HttpRequest::HttpRequest():method_(Method::kInvaild),version_(Version::kUnknown){
};

HttpRequest::~HttpRequest(){};

void HttpRequest::SetVersion(const std::string& ver){
    if(ver == "1.0"){
        version_ = Version::kHttp10;
    }
    else if(ver == "1.1"){
        version_ = Version::kHttp11;
    }
    else{
        version_ = Version::kUnknown;
    }
}

HttpRequest::Version HttpRequest::version() const{
    return version_;
}

std::string HttpRequest::GetVersionString() const{
    if(version_ == Version::kHttp10){
        return "1.0";
    }
    else if(version_ == Version::kHttp11){
        return "1.1";
    }
    else{
        return "Unknown";
    }
}

bool HttpRequest::SetMethod(const std::string& method){
    if(method == "GET"){
        method_ = Method::kGet;
    }
    else if(method == "POST"){
        method_ = Method::kPost;
    }
    else if(method == "HEAD"){
        method_ = Method::kHead;
    }
    else if(method == "PUT"){
        method_ = Method::kPut;
    }
    else if(method == "DELETE"){
        method_ = Method::kDelete;
    }
    else{
        method_ = Method::kInvaild;
    }
}

HttpRequest::Method HttpRequest::method() const{
    
}

std::string GetMethodString() const; 


void SetUrl(const std::string& url); //设置请求路径
const std::string& url() const; //获取请求路径

void SetRequestParams(const std::string &key, const std::string &value); //设置请求参数
std::string GetRequestValue(const std::string& key) const; //获取请求值
const std::map<std::string, std::string> & headers() const; //获取请求头
void SetProtocol(const std::string& str); //设置协议
const std::string& protocol() const; //获取协议
void AddHeader(const std::string& key, const std::string& value); //添加请求头
std::string GetHeader(const std::string& field) const;
const std::map<std::string, std::string> & header() const; 
void SetBody(const std::string &str); //设置请求体
const std::string& body() const; //获取请求体

