# Reactor模式 
## 三种经典的Reactor模式：  
### 单Ractor单进程/线程  
![alt text](image.png)
可以看到进程里有 Reactor、Acceptor、Handler 这三个对象：  

Reactor 对象的作用是监听和分发事件；  
Acceptor 对象的作用是获取连接；  
Handler 对象的作用是处理业务；  

Reactor 对象通过 select （IO 多路复用接口） 监听事件，收到事件后通过 dispatch 进行分发，具体分发给 Acceptor 对象还是 Handler 对象，还要看收到的事件类型；  
如果是连接建立的事件，则交由 Acceptor 对象进行处理，Acceptor 对象会通过 accept 方法 获取连接，并创建一个 Handler 对象来处理后续的响应事件；  
如果不是连接建立事件， 则交由当前连接对应的 Handler 对象来进行响应；Handler 对象通过 read -> 业务处理 -> send 的流程来完成完整的业务流程；  

### 单Reactor多进程/线程  

# Proactor模式  
![alt text](image.png)  
介绍一下 Proactor 模式的工作流程：  
Proactor Initiator 负责创建 Proactor 和 Handler 对象，并将 Proactor 和 Handler 都通过   
Asynchronous Operation Processor 注册到内核；  
Asynchronous Operation Processor 负责处理注册请求，并处理 I/O 操作;  
Asynchronous Operation Processor 完成 I/O 操作后通知 Proactor；  
Proactor 根据不同的事件类型回调不同的 Handler 进行业务处理；  
Handler 完成业务处理； 