#include <string>

#include "rcon.h"

/* rcon_packet methods */

/*
 * Constructs a rcon_packet struct directly from given values for each field.
 */
struct rcon_packet *rp_construct(int id, int type, std::string body) {
    struct rcon_packet *rp = new rcon_packet;

    rp->size = 10 + body.length();
    rp->id = id;
    rp->type = type;
    rp->body = body;

    return rp;
}

/*
 * Destroy a rcon_packet struct.
 */
void rp_destroy(struct rcon_packet *&packet) {
    delete packet;
    packet = nullptr;
}

/*
 * Obtain the 'stream form' of a rcon_packet, i.e. a std::string representing
 * the packet suitable for sending over the internet.
 */
std::string rp_stream_form(struct rcon_packet *packet) {
    std::string ret = std::string();

    //size field
    unsigned char *ubuffer = new unsigned char[4];
    u32_to_bits(ubuffer, (unsigned)packet->size);
    for (unsigned i = 0; i < 4; i++) ret.push_back(ubuffer[i]);

    //id field 
    u32_to_bits(ubuffer, (unsigned)packet->id);
    for (unsigned i = 0; i < 4; i++) ret.push_back(ubuffer[i]);

    //type field
    u32_to_bits(ubuffer, (unsigned)packet->type);
    for (unsigned i = 0; i < 4; i++) ret.push_back(ubuffer[i]);

    //body
    ret.append(packet->body);

    //null terminators
    ret.append('\0\0');

    return ret;
}

/* data conversion methods */

/*
 * Given a pointer (unsigned char *)bits to at least 4 chars (8 bytes total)
 * that make up an integer stored in little-endian format, convert this
 * back into an unsigned integer and store it in the ret parameter. 
 */
void bits_to_u32(unsigned &ret, unsigned char *bits) {
    ret = (bits[0]) | (bits[1] << 8) | (bits[2] << 16) | (bits[3] << 24);
}

/*
 * Given a buffer of unsigned chars ret (which should contain at least 4 chars,
 * exactly 4 preferred), store the unsigned integer u32 into this buffer in a 
 * little-endian format.
 */
void u32_to_bits(unsigned char ret[], unsigned u32) {
    ret[0] = (u32 & 0x000000FF) >> 0;
    ret[1] = (u32 & 0x0000FF00) >> 8;
    ret[2] = (u32 & 0x00FF0000) >> 16;
    ret[3] = (u32 & 0xFF000000) >> 24;
}