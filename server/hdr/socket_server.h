#include <netinet/in.h>
#include <string>

class SocketServer {
public:
        SocketServer() {}
        ~SocketServer() {}

        //创建socket
        int Socket();

        //绑定
        int Bind(std::string ip, std::string port);
        int Bind(int ip, int port, int type);
        
        //监听
        int Listen(int maxlink);

private:
        //创建socket的文件描述符
        int sockfd;
        //服务端绑定监听ip
        struct sockaddr_in serveraddr;
        
};