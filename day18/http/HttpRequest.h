#pragma once
#include <string>
#include <map>

class HttpRequest{
public:
    enum Method{
        kInvaild = 0,
        kGet,
        kPost,
        kHead,
        kPut,
        kDelete
    };

    enum Version{
        kUnknown = 0,
        kHttp10,
        kHttp11
    };

    HttpRequest();
    ~HttpRequest();

    void SetVersion(const std::string& ver); //http版本
    Version version() const;
    std::string GetVersionString() const;

    bool SetMethod(const std::string& method); //设定请求方法
    Method method() const; //获取请求方法
    std::string GetMethodString() const; //获取请求方法字符串

    void SetUrl(const std::string& url); //设置请求路径
    const std::string& url() const; //获取请求路径

    void SetRequestParams(const std::string &key, const std::string &value); //设置请求参数
    std::string GetRequestValue(const std::string& key) const; //获取请求值
    const std::map<std::string, std::string> & headers() const; //获取请求头

    void SetBody(const std::string &str); //设置请求体
    const std::string& body() const; //获取请求体

private:
    Method method_; //请求方法
    Version version_; //版本

    std::map<std::string, std::string> request_params_; //请求参数

    std::string url_; //请求路径

    std::string protocol_; //协议

    std::map<std::string, std::string> headers_; //请求头

    std::string body_; //请求体


};