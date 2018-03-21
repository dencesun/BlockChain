//
// Created by dencesun on 20/03/2018.
//

#ifndef BLOCKCHAIN_CONNECTION_H
#define BLOCKCHAIN_CONNECTION_H

#include <boost/asio.hpp>
#include <boost/tuple/tuple.hpp>
#include <iostream>
#include <iomanip>
#include <boost/bind.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/asio/io_service.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/list.hpp>

class Connection {
public:
    Connection(boost::asio::io_service& ioService)
            : mSocket(ioService) {

    }

    boost::asio::ip::tcp::socket& socket() {
        return mSocket;
    }

    template  <typename T, typename Handler>
    void async_write(const T& t, Handler handler) {
        std::ostringstream archiveStream;
        boost::archive::text_oarchive archive(archiveStream);
        archive << t;
        mOutboundData = archiveStream.str();

        std::ostringstream headerStream;
        headerStream << std::setw(headerLength)
                     << std::hex << mOutboundData.size();

        if (!headerStream || headerStream.str() != headerLength) {
            boost::system::error_code error(boost::asio::error::invalid_argument);
            mSocket.get_io_service().post(boost::bind(handler, error));
            return ;
        }

        mOutboundHeader = headerStream.str();

        std::vector<boost::asio::const_buffer> buffers;
        buffers.push_back(boost::asio::buffer(mOutboundHeader));
        buffers.push_back(boost::asio::buffer(mOutboundData));
        boost::asio::async_write(mSocket, buffers, handler);
    }

    template <typename T, typename Handler>
    void async_read(T& t, Handler handler)
    {
        // On récupère le header (10)
        void (Connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>)
        = &Connection::handle_read_header<T, Handler>;
        boost::asio::async_read(m_socket, boost::asio::buffer(m_inbound_header),
                                boost::bind(f,
                                            this, boost::asio::placeholders::error, boost::ref(t),
                                            boost::make_tuple(handler)));
    }

    template <typename T, typename Handler>
    void handler_read_haeder(const boost::system::error_code& e, T& t, boost::tuple<Handler> handler) {
        if (e) {
            boost::get<0>(handler)(e);
        } else {
            std::istringstream is(std::string(mInboundHeader, headerLength));
            std::size_t mInboundDataSize = 0;
            if (!(is >> std::hex >> mInboundDataSize)) {
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return ;
            }

            mInboundData.resize(mInboundDataSize);
            void (Connection::*f)(const boost::system::error_code&, T&, boost::tuple<Handler>)
                    = &Connection::handle_read_data<T, handler>;

            boost::asio::async_read(mSocket, boost::asio::buffer(mInboundData),
                                    boost::bind(f, this,
                                            boost::asio::placeholders::error, boost::ref(t), handler));

        }
    }

    template <typename T, typename Handler>
    void handle_read_data(const boost::system::error_code& e,
                            T& t, boost::tuple<Handler> handler) {
        if (e) {
            boost::get<0>(handler)(e)
        } else {
            try {
                std::string archiveData(&mInboundData[0], mInboundData.size());
                std::istringstream archiveStream(archiveData);
                boost::archive::text_iarchive archive(archiveStream);
                archive >> t;
            } catch (...) {
                boost::system::error_code error(boost::asio::error::invalid_argument);
                boost::get<0>(handler)(error);
                return ;
            }

            boost::get<0>(handler)(e);
        }

    }

private:
    boost::asio::ip::tcp::socket mSocket;

    enum {headerLength = 8};

    std::string mOutboundHeader;
    std::string mOutboundData;
    char mInboundHeader[headerLength];
    std::vector<char> mInboundData;
};
#endif //BLOCKCHAIN_CONNECTION_H
