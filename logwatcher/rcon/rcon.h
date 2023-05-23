#ifndef RCON_H
#define RCON_H

#include <string>

/* packet type constants */
#define SERVERDATA_AUTH 3
#define SERVERDATA_AUTH_RESPONSE 2
#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_RESPONSE_VALUE 0

/* rcon_packet */
struct rcon_packet {
    int size;
    int id;
    int type;
    std::string body;
};

/* rcon_packet methods */

struct rcon_packet *rp_construct(int size, int id, int type, std::string body);
struct rcon_packet *rp_construct_from_stream(std::string message);
void rp_destroy(struct rcon_packet *packet);
std::string rp_stream_form(struct rcon_packet *packet);

/* data conversion methods */

void bits_to_u32(unsigned &ret, unsigned char *bits);
void u32_to_bits(unsigned char *ret, unsigned u32);

/* rcon connection class */
class rcon_connection {
    const std::string address;
    const int port;
    const std::string password;

    unsigned counter;
    unsigned socket_fd;
    bool connected;
    bool authenticated;

    public:
        rcon_connection(const std::string address, const int port, const std::string password, const double timeout=10.0);
        virtual ~rcon_connection();

        int send(std::string& result, const std::string message, const int type=SERVERDATA_EXECCOMMAND);
        bool is_connected() const;
        bool is_authenticated() const;
        int dest_address(std::string& address, int& port);
    
    private:
        int connect();
        int send_packet(int type, std::string body);
        int recv_packet();
        int authenticate();

};

#endif
