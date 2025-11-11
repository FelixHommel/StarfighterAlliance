#include "Websocket.hpp"

/*
 * @brief Set up client and configure all callback functions needed
*/
Websocket::Websocket()
    : m_connected(false)
    , m_requestSent(false)
    , m_responseRecieved(false)
{
    m_client.set_access_channels(websocketpp::log::alevel::all);
    m_client.clear_access_channels(websocketpp::log::alevel::frame_payload);
    m_client.set_error_channels(websocketpp::log::alevel::all);

    m_client.init_asio();

    m_client.set_tls_init_handler([this](websocketpp::connection_hdl){ return onTlsInit(); });
    m_client.set_open_handler([this](websocketpp::connection_hdl hdl)
    {
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_connected = true;
            m_connectionHandle = hdl;
        }
        m_cv.notify_all();
    });
    m_client.set_message_handler([this](websocketpp::connection_hdl hdl, client::message_ptr msg)
    {
        if(!m_responseRecieved)
        {
            m_response = msg->get_payload();
            m_responseRecieved = true;
        }
    });
    m_client.set_close_handler([](websocketpp::connection_hdl hdl){});
    m_client.set_fail_handler([](websocketpp::connection_hdl hdl){ std::clog << "TLS connection failed" << std::endl; });

    m_client.set_error_channels(websocketpp::log::elevel::all);
}

Websocket::~Websocket()
{
    if(m_connected)
        close();

    stop();
}

/*
 * @brief try to establish a connection to the server
*/
void Websocket::connect()
{
    m_clientThread = std::thread([this]()
    {
        websocketpp::lib::error_code error;
        auto connection{ m_client.get_connection(uri, error) };
        if(error)
        {
            std::cerr << "Connection error: " << error.message() << std::endl;
            return;
        }
        m_client.connect(connection);
        m_client.run();
    });
}

/*
 * @brief try to send a request to the server
 *
 * @return bool, whether or not the sending was successfull
*/
bool Websocket::send()
{
    if(m_connectionHandle.lock())
    {
        m_client.send(m_connectionHandle, payload, websocketpp::frame::opcode::text);
        m_requestSent = true;
    }
    else
    {
        std::clog << "Can not send message, no open connection" << std::endl;
        return false;
    }

    return true;
}

/*
 * @brief try to close a open connection to the server
*/
void Websocket::close()
{
    if(m_connectionHandle.lock())
    {
        m_client.close(m_connectionHandle, websocketpp::close::status::normal, "Closing connection manually");
        m_connected = false;
    }
}

/*
 * @brief try to stop the server if possible
*/
void Websocket::stop()
{
    if(m_clientThread.joinable())
    {
        m_client.stop();
        m_clientThread.join();
    }
}

/*
 * @brief gives the response back to the user of the class, if there was one recieved yet
 *
 * @param msg, output parameter, which will contain the response if there was one recieved yet
 *
 * @return bool, whether or not a response will be returned
*/
bool Websocket::getResponse(std::string& msg)
{
    if(m_responseRecieved)
    {
        msg = m_response;
        return true;
    }

    return false;
}

/*
 * @brief Initialize the TLS context
*/
context_ptr Websocket::onTlsInit()
{
	context_ptr ctx {websocketpp::lib::make_shared<boost::asio::ssl::context>(boost::asio::ssl::context::tlsv12)};

	try
    {
        ctx->set_default_verify_paths();
        ctx->set_verify_mode(boost::asio::ssl::verify_peer);
    }
    catch(const std::exception& e)
    {
        std::cerr << "TLS initializeation error: " << e.what() << std::endl;
    }

    return ctx;
}
