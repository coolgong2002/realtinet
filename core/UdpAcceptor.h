
#ifndef UDP_MUDUO_NET_ACCEPTOR_H
#define UDP_MUDUO_NET_ACCEPTOR_H

#include <functional>
#include <map>

#include <net/InetAddress.h>
#include <net/Channel.h>
#include <net/Socket.h>
#include <net/Buffer.h>

namespace muduo
{
	namespace net
	{

		class EventLoop;
		class InetAddress;

		class UdpConnector;
		typedef std::shared_ptr<UdpConnector> UdpConnectorPtr;

		class UdpAcceptor : noncopyable
		{
		public:
			typedef std::function<void( 
				Socket* connectedSocket, const InetAddress&)> NewConnectionCallback;

			UdpAcceptor( EventLoop* loop, const InetAddress& listenAddr, bool reuseport );
			~UdpAcceptor();

			void setNewConnectionCallback( const NewConnectionCallback& cb )
			{ newConnectionCallback_ = cb; }

			bool listenning() const { return listenning_; }
			void listen();

			uint16_t GetListenPort() const { return listenPort_; }
			EventLoop* getLoop() const { return loop_; }
			void RemoveConnector( const InetAddress& peerAddr );
			void EraseConnector( const InetAddress& peerAddr );

		private:
			void handleRead();
			void newConnection( Socket* connectedSocket,
				const InetAddress& peerAddr );

			EventLoop* loop_;
			Socket acceptSocket_;
			Channel acceptChannel_;
			NewConnectionCallback newConnectionCallback_;
			bool listenning_;
			uint16_t listenPort_;
			InetAddress listenAddr_;

			typedef std::map<InetAddress, UdpConnectorPtr> InetAddressToUdpConnectorMap;
			InetAddressToUdpConnectorMap peerAddrToUdpConnectors_;

			static const size_t krecvfromBufSize = 1500;
			char recvfromBuf_[krecvfromBufSize];
		};

	}
}

#endif  // MUDUO_NET_ACCEPTOR_H
