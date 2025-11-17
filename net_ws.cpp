#include "net_client.h"
#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

typedef websocketpp::client<websocketpp::config::asio_client> ws_client;

class WSNet : public NetClient {
public:
    ws_client client;
    websocketpp::connection_hdl hdl;

    bool connect(const std::string& url) override {
        try {
            client.init_asio();
            client.set_message_handler([this](websocketpp::connection_hdl, ws_client::message_ptr msg) {
                if (onMessage) onMessage(msg->get_payload());
            });

            auto con = client.get_connection(url, websocketpp::lib::error_code());
            hdl = con->get_handle();
            client.connect(con);

            std::thread([this]() { client.run(); }).detach();
            online = true;
            return true;
        }
        catch (...) {
            online = false;
            return false;
        }
    }

    void send(const std::string& msg) override {
        if (online)
            client.send(hdl, msg, websocketpp::frame::opcode::text);
    }

    void poll() override { /* unused */ }

    void close() override {
        if (online)
            client.close(hdl, websocketpp::close::status::normal, "bye");
        online = false;
    }
};
