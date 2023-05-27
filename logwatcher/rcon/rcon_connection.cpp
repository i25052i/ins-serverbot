#include <string>
#include <stdio.h>
#include <iostream>
#include <boost/asio.hpp>

#include "rcon.h"

using boost::asio::ip::tcp;

/* rcon connection class implementation */

rcon_connection::rcon_connection(const std::string address, const int port, const std::string password):
    address(address),
    port(port),
    password(password),
    counter(0),
    connected(false),
    authenticated(false),
    socket(nullptr),
    context(nullptr)
{
    int result;

    result = connect();
    if (result) {
        //Connection Error
        return;
    }

    result = authenticate();
    if (result) {
        //Wrong password
        return;
    }
}

rcon_connection::~rcon_connection() {
    socket->shutdown(tcp::socket::shutdown_send);
    socket->close();
    if (socket != nullptr) delete socket;
    socket = nullptr;
    if (context != nullptr) delete context;
    context = nullptr;
}

int rcon_connection::send(std::string& response, const std::string message, const int type) {
    if (!connected || !authenticated) return 1;
    
    int result;
    int sent_id;
    struct rcon_packet *received;

    result = send_packet(type, message, sent_id);
    if (result) {
        return result;
    }

    while (true) {
        result = recv_packet(received);
        if (result) {
            return result;
        }

        if (received->id == sent_id) {
            break;
        }
    }

    response = received->body;

    return 0;
}

bool rcon_connection::is_connected() const {
    return connected;
}

bool rcon_connection::is_authenticated() const {
    return authenticated;
}

int rcon_connection::dest_address(std::string& address, int& port) {
    address = this->address;
    port = this->port;
    return 0;
}

int rcon_connection::connect() {
    context = new boost::asio::io_context;
    tcp::resolver resolver(*context);
    tcp::resolver::results_type endpoints = resolver.resolve(address, std::to_string(port));
    socket = new tcp::socket(*context);

    boost::system::error_code ec;
    boost::asio::connect(*socket, endpoints, ec);
    if (ec) {
        std::cout << ec.what() << std::endl;
        return 1;
    }

    connected = true;
    return 0;
}

int rcon_connection::send_packet(int type, std::string body, int &id) {
    if (!connected) return 1;

    //construct and send packet
    id = counter++;
    struct rcon_packet *to_send = rp_construct(id, type, body);
    boost::system::error_code ec;
    boost::asio::write(*socket, boost::asio::buffer(rp_stream_form(to_send)), ec);

    //cleanup
    rp_destroy(to_send);
    if (ec) {
        std::cout << ec.what() << std::endl;
        connected = false;
        authenticated = false;
        return 1;
    }
    return 0;
}

int rcon_connection::recv_packet(struct rcon_packet *&received) {
    //read in the packet length
    std::string buf({'\0', '\0', '\0', '\0'});
    boost::system::error_code ec;
    boost::asio::read(*socket, boost::asio::buffer(buf, 4), ec);
    if (ec) {
        std::cout << ec.what() << std::endl;
        connected = false;
        authenticated = false;
        return 1;
    }
    
    unsigned length;
    unsigned char *length_buffer = new unsigned char[4]{0};
    for (int i = 0; i < 4; i++) length_buffer[i] = buf[i];
    bits_to_u32(length, length_buffer);
    delete[] length_buffer;

    //read in the rest of the packet
    buf.resize(length, '\0');
    boost::asio::read(*socket, boost::asio::buffer(buf, length));

    received = rp_construct_from_stream(buf);

    return 0;
}

int rcon_connection::authenticate() {
    int result;
    int sent_id;
    struct rcon_packet *response;

    result = send_packet(SERVERDATA_AUTH, password, sent_id);
    if (result) {
        return result;
    }

    while (true) {
        result = recv_packet(response);
        if (result) {
            return result;
        }

        if (response->id == -1) {
            return 1;
        }

        if (response->id == sent_id && response->type == SERVERDATA_AUTH_RESPONSE) {
            break;
        }
    }
    authenticated = true;
    return 0;
}
