#include "HttpResponse.h"

HttpResponse::HttpResponse(bool close_connection):status_code_(kUnknown), status_message_(nullptr), body_(nullptr), close_connection_(close_connection){
    
}

HttpResponse::~HttpResponse(){}

void HttpResponse::SetStatusCode(HttpStatusCode status_code){
    status_code_ = status_code;
}

void HttpResponse::SetStatusMessage(const std::string& status_message){
    status_message_ = status_message;
}

void HttpResponse::SetCloseConnection(bool close_connection){
    close_connection_ = close_connection;
}

void HttpResponse::SetContentType(const std::string& content_type){
    AddHeader("Content-Type", content_type);
}

void HttpResponse::AddHeader(const std::string& key, const std::string& value){
    headers_[key] = value;
}

void HttpResponse::SetBody(const std::string& body){
    body_ = body;
}

bool HttpResponse::IsCloseConnection() const{
    return close_connection_;
}

std::string HttpResponse::message(){
    std::string message;
    message += ("HTTP/1.1 " + std::to_string(status_code_) + " " + status_message_ + "\r\n");
    if(close_connection_){
        message += "Connection: close\r\n";
    }
    else{
        message += ("Cotent-Length: " + std::to_string(body_.size()) + "\r\n");
        message += "Connection: keep-alive\r\n";
    }

    message += "\r\n";
    message += body_;
    
    return message;
}


