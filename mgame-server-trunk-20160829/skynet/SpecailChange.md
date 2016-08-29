## Change
修改skynet源码处：
1. 日志模块：
	service_logger.c 本身skynet的日志，不支持日志文件跨天处理
2. httpc.lua
	skynet的http client，对于Get请求的发送的格式 入   http://127.0.0.1/a  data=1(这个放在 http body中) , 而不是一般的  http://127.0.0.1/a?data=1

3. socketchannel模块增加了一个request_ex接口，更新mysql/redis驱动，使用新接口在网络断开的情况下自动重连
