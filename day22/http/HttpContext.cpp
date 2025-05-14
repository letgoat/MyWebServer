#include "HttpContext.h"
#include "HttpRequest.h"

#include <memory>
#include <string>
#include <algorithm>
#include <iostream>

HttpContext::HttpContext():state_(HttpRequestParaseState::START){
    request_ = std::make_unique<HttpRequest>();
}

HttpContext::~HttpContext(){

}

bool HttpContext::GetCompleteRequest(){
    return state_ == HttpRequestParaseState::COMPLETE;
}

void HttpContext::ResetContextState(){
    state_ = HttpRequestParaseState::START;
}

HttpRequest* HttpContext::request(){
    return request_.get();
}

bool HttpContext::ParseRequest(const char* begin, int size){
    char *start = const_cast<char *>(begin);
    char *end = start;
    char *colon = end;
    while(state_ != HttpRequestParaseState::kINVALID && state_ != HttpRequestParaseState::COMPLETE && end - begin <= size){
        char ch = *end; //当前字符
        switch(state_){
            //解析开始
            case HttpRequestParaseState::START:{ 
                if(ch == CR || ch == LF || isblank(ch)){
                    //遇到空格，换行，回车都继续
                }
                else if(isupper(ch)){
                    //遇到大写字母，说明是解析方法
                    state_ = HttpRequestParaseState::METHOD;
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID;
                }
                break;
            }

            //解析请求方法
            case HttpRequestParaseState::METHOD:{
                if(isupper(ch)){ //大写字母继续，说明还是METHOD

                }
                else if(isblank(ch)){ //遇到空格，说明METHOD解析完成
                    request_->SetMethod(std::string(start, end)); //设置请求方法
                    state_ = HttpRequestParaseState::BEFORE_URL;  //转到解析URL之前
                    start = end + 1; //跳过空格，更新下一个指标的位置
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法请求方法  
                }
                break;
            }

            //解析URL之前
            case HttpRequestParaseState::BEFORE_URL:{
                if(ch == '/'){ //URL以'/'开头
                    state_ = HttpRequestParaseState::IN_URL; //转到解析URL
                }
                else if(isblank(ch)){
                    //遇到空格，继续
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法URL
                }
                break;
            }

            //解析URL           
            case HttpRequestParaseState::IN_URL:{
                if(ch == '?'){ //遇到?说明后面是URL参数
                    state_ = HttpRequestParaseState::BEFORE_URL_PARAM_KEY; //转到解析URL参数键之前
                    request_->SetUrl(std::string(start, end));
                    start = end + 1; //跳过'?'，更新下一个指标的位置
                }
                else if(isblank(ch)){
                    //说明没有参数，URL解析完成
                    request_->SetUrl(std::string(start, end));
                    start = end + 1; //跳过空格，更新下一个指标的位置
                    state_ = HttpRequestParaseState::BEFORE_PROTOCOL; //转到解析协议之前
                }
                break;
            }

            //解析URL参数键之前
            case HttpRequestParaseState::BEFORE_URL_PARAM_KEY:{
                if(ch == CR || ch == LF || isblank(ch)){
                    state_ = HttpRequestParaseState::kINVALID;
                }
                else{
                    state_ = HttpRequestParaseState::URL_PARAM_KEY; //转到解析URL参数键
                }
                break;
            }

            //解析URL参数键
            case HttpRequestParaseState::URL_PARAM_KEY:{
                if(ch == '='){ //遇到=说明后面是URL参数值
                    state_ = HttpRequestParaseState::BEFORE_URL_PARAM_VALUE; //转到解析URL参数值之前
                    colon = end; //作为参数和值的分隔符，便于使用std::string构建
                }
                else if(isblank(ch)){
                    state_ = HttpRequestParaseState::kINVALID; //非法URL参数键
                }
                break;
                }
            

            case HttpRequestParaseState::BEFORE_URL_PARAM_VALUE:{
                if(ch == CR || ch == LF || isblank(ch)){
                    state_ = HttpRequestParaseState::kINVALID;
                }
                else{
                    state_ = HttpRequestParaseState::URL_PARAM_VALUE; //转到解析URL参数值
                }
                break;
                }

            case HttpRequestParaseState::URL_PARAM_VALUE:{
                if(ch == '&'){ //说明遇到下一个参数
                    state_ = HttpRequestParaseState::BEFORE_URL_PARAM_KEY; //转到解析URL参数键之前
                    request_->SetRequestParams(std::string(start, colon), std::string(colon + 1, end));
                    start = end + 1; //跳过'&', 更新下一个参数的开始位置
                }
                else if(isblank(ch)){ //遇到空格，说明解析结束
                    state_ = HttpRequestParaseState::BEFORE_PROTOCOL; //转到解析协议之前
                    request_->SetRequestParams(std::string(start, colon), std::string(colon + 1, end));
                    start = end + 1; //跳过空格，更新下一个指标的位置
                }
                else{

                }
                break;
            }

            //解析协议之前
            case HttpRequestParaseState::BEFORE_PROTOCOL:{
                if(isblank(ch)){
                    //nothing
                }
                else{
                    state_ = HttpRequestParaseState::PROTOCOL; //转到解析协议
                }
                break;
            }

            //解析协议
            case HttpRequestParaseState::PROTOCOL:{
                if(ch == '/'){
                    state_ = HttpRequestParaseState::BEFORE_VERSION; //转到解析版本之前
                    request_ -> SetProtocol(std::string(start, end));
                    start = end + 1; //跳过'/'，更新下一个指标的位置
                }
                else {

                }
                break;
            }

            //解析版本之前
            case HttpRequestParaseState::BEFORE_VERSION:{
                if(isdigit(ch)){
                    state_ = HttpRequestParaseState::VERSION;//转到解析版本
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法版本
                }
                break;
            }

            //解析版本
            case HttpRequestParaseState::VERSION:{
                if(ch == CR){ //遇到回车说明结束
                    request_->SetVersion(std::string(start, end));
                    start = end + 1; //跳过回车，更新下一个指标的位置
                    state_ = HttpRequestParaseState::WHEN_CR; //转到遇到CR
                }
                else if(isdigit(ch) || ch == '.'){

                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法版本
                }
                break;
            }

            //解析头键之前
            case HttpRequestParaseState::HEADER_KEY:{
                if(ch == ':'){
                    state_ = HttpRequestParaseState::HEADER_KEY; //转到解析头值之前
                    colon = end; //作为键和值的分隔符，便于使用std::string构建
                }
                break;
            }

            //解析头值
            case HttpRequestParaseState::HEADER_VALUE:{
                if(ch == CR){
                    request_->AddHeader(std::string(start, colon), std::string(colon + 1, end));
                    start = end + 1; //跳过回车，更新下一个指标的位置
                    state_ = HttpRequestParaseState::WHEN_CR; //转到遇到CR
                }
                else if(isblank(ch)){

                }
                break;
            }

            //遇到CR
            case HttpRequestParaseState::WHEN_CR:{
                if(ch == LF){
                    state_ = HttpRequestParaseState::CR_LF; //转到回车换行
                    start = end + 1; //跳过LF，更新下一个指标的位置
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法请求
                }
                break;
            }
            

            //回车换行
            case HttpRequestParaseState::CR_LF:{
                if(ch == CR){
                    state_ = HttpRequestParaseState::CR_LF_CR; //转到回车换行后
                }
                else if(isblank(ch)){
                    state_ = HttpRequestParaseState::kINVALID; //非法请求
                }
                else{
                    state_ = HttpRequestParaseState::HEADER_KEY; //转到解析头键之前
                }
                break;
            }

            //回车换行后
            case HttpRequestParaseState::CR_LF_CR:{
                if(ch == LF){ //表明遇到空行，请求头结束，要解析请求体了
                    if(request_->headers().count("Content-Length")){ //如果有Content-Length头
                        if(atoi(request_->GetHeader("Content-Length").c_str()) > 0){ //如果Content-Length大于0，说明请求体有数据，继续解析
                            state_ = HttpRequestParaseState::BODY; 
                        }
                        else{ //如果Content-Length等于0，说明请求体没有数据，解析完成
                            state_ = HttpRequestParaseState::COMPLETE; 
                        }
                    }
                    else{ //如果没有Content-Length头
                        if(end - begin < size){ //如果还有数据，说明请求体没有结束，继续解析
                            state_ = HttpRequestParaseState::BODY; //转到解析BODY
                        }
                        else{ //如果没有数据，说明请求体结束，解析完成
                            state_ = HttpRequestParaseState::COMPLETE; //解析完成
                        }
                    }
                }
                else{
                    state_ = HttpRequestParaseState::kINVALID; //非法请求
                }
            }

            //解析BODY
            case HttpRequestParaseState::BODY:{
                int body_length = size - (end - begin); //计算请求体长度
                request_->SetBody(std::string(start, start + body_length));
                state_ = HttpRequestParaseState::COMPLETE; //解析完成
                break;
            }

            default:{
                state_ = HttpRequestParaseState::kINVALID; //非法状态
                break;
            }
        }
        end++;

    }
    return state_ == HttpRequestParaseState::COMPLETE;
}