#pragma once

#include <string>
#include <map>


class HttpResponse{
public:
    enum HttpStatusCode{
        kUnknown = 1,
        k100Continue = 100,
        k200Ok = 200,
        k400BadRequest = 400,
        k403Forbidden = 403,
        k404NotFound = 404,
        k500internalServerError = 500,
    };

    HttpResponse(bool close_connection);
    ~HttpResponse();
    
    void SetStatusCode(HttpStatusCode status_code); //设置回应码
    void SetStatusMessage(const std::string& status_message);
    void SetCloseConnection(bool close_connection);

    void SetContentType(const std::string& content_type); 
    void AddHeader(const std::string& key, const std::string& value); //设置回应头

    void SetBody(const std::string& body);

    std::string message(); //将信息加入到buffer中
    
    bool IsCloseConnection() const;

private:
    std::map<std::string, std::string> headers_;
    HttpStatusCode status_code_;
    std::string status_message_;
    std::string body_;
    bool close_connection_;
};