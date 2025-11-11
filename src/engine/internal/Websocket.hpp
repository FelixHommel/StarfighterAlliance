#ifndef INTERNAL_WEBSOCKET_HPP
#define INTERNAL_WEBSOCKET_HPP

#include "websocketpp/roles/client_endpoint.hpp"
#include "websocketpp/config/asio_client.hpp"

#include <thread>

using client = websocketpp::client<websocketpp::config::asio_tls_client>;
using context_ptr = websocketpp::lib::shared_ptr<websocketpp::lib::asio::ssl::context>;

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1; //NOLINT
using websocketpp::lib::placeholders::_2; //NOLINT

/*
 * @file Websocket.hpp
 * @brief This class can be used to communicate with the server that provides spaceship colors on request
 *
 * @details This class sets up a TLS session, which can be connected on demand, send messages on demand, be closed on demand
 *          and be stopped on demand. The uri is hardwired into the code.
 *
 * @author Felix Hommel
 * @date Nov 25, 2024
*/
class Websocket
{
	public:
        /** Constructor */
		Websocket();
        ~Websocket();

        /** Public member functions */
		void connect();
        bool send();
        void close();
        void stop();

        bool isConnected() const { return m_connected; }
        bool requestSent() const { return m_requestSent; }
        bool getResponse(std::string& msg);

	private:
        /** member variables */
        client m_client;
        websocketpp::connection_hdl m_connectionHandle;
        std::thread m_clientThread;

        std::mutex m_mutex;
        std::condition_variable m_cv;
        bool m_connected;

        bool m_requestSent;
        std::string m_response;
        bool m_responseRecieved;

        /** Private member functions */
		context_ptr onTlsInit();

        /** static constant members */
		static constexpr auto payload {R"({ "messageType": "COLOR_REQUEST" })"};
		static constexpr auto uri {""};
};

#endif //!INTERNAL_WEBSOCKET_HPP
