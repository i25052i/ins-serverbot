#ifndef RCON_H
#define RCON_H

#include <string>
#include <boost/asio.hpp>

/* packet type constants */
#define SERVERDATA_AUTH 3
#define SERVERDATA_AUTH_RESPONSE 2
#define SERVERDATA_EXECCOMMAND 2
#define SERVERDATA_RESPONSE_VALUE 0

/* rcon connection error codes */
#define ENCONN 1 /* Not connected */
#define ENAUTH 2 /* Not authorized */
#define ESENDP 3 /* Packet send error */
#define ERECVP 4 /* Packet receive error */
#define ECCONN 5 /* Connection creation error */
#define EPASSW 6 /* Wrong password */

/* rcon_packet */
struct rcon_packet {
    size_t size;
    int id;
    int type;
    std::string body;
};

/* rcon_packet methods */

struct rcon_packet *rp_construct(int id, int type, std::string body);
struct rcon_packet *rp_construct_from_stream(std::string stream);
void rp_destroy(struct rcon_packet *&packet);
std::string rp_stream_form(struct rcon_packet *packet);

/* data conversion methods */

void bits_to_u32(unsigned &ret, unsigned char *bits);
void u32_to_bits(unsigned char ret[], unsigned u32);

/* rcon connection class */
class rcon_connection {
    const std::string address;
    const int port;
    const std::string password;

    unsigned counter;
    bool connected;
    bool authenticated;
    boost::asio::ip::tcp::socket *socket;
    boost::asio::io_context *context;

    public:
        rcon_connection(const std::string address, const int port, const std::string password);
        virtual ~rcon_connection();

        int send(std::string& response, const std::string message, int type=SERVERDATA_EXECCOMMAND);
        bool is_connected() const;
        bool is_authenticated() const;
        int dest_address(std::string& address, int& port);
    
    private:
        int connect();
        int send_packet(int type, std::string body, int &id);
        int recv_packet(struct rcon_packet *&received);
        int authenticate();

};

#endif
