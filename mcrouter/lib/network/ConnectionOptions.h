/*
 *  Copyright (c) 2014-present, Facebook, Inc.
 *
 *  This source code is licensed under the MIT license found in the LICENSE
 *  file in the root directory of this source tree.
 *
 */
#pragma once

#include <chrono>
#include <memory>

#include <folly/io/async/AsyncSocket.h>

#include "mcrouter/lib/CompressionCodecManager.h"
#include "mcrouter/lib/mc/protocol.h"
#include "mcrouter/lib/network/AccessPoint.h"

namespace folly {
class SSLContext;
} // folly

namespace facebook {
namespace memcache {

/**
 * A struct for storing all connection related options.
 */
struct ConnectionOptions {
  using SocketOptions = folly::AsyncSocket::OptionMap;

  ConnectionOptions(
      folly::StringPiece host_,
      uint16_t port_,
      mc_protocol_t protocol_)
      : accessPoint(std::make_shared<AccessPoint>(host_, port_, protocol_)) {}

  explicit ConnectionOptions(std::shared_ptr<const AccessPoint> ap)
      : accessPoint(std::move(ap)) {}

  /**
   * Access point of the destination.
   */
  std::shared_ptr<const AccessPoint> accessPoint;

  /**
   * Number of TCP KeepAlive probes to send before considering connection dead.
   *
   * Note: Option will be enabled iff it is supported by the OS and this
   *       value > 0.
   */
  int tcpKeepAliveCount{0};

  /**
   * Interval between last data packet sent and the first TCP KeepAlive probe.
   */
  int tcpKeepAliveIdle{0};

  /**
   * Interval between two consequent TCP KeepAlive probes.
   */
  int tcpKeepAliveInterval{0};

  /**
   * Write/connect timeout in ms.
   */
  std::chrono::milliseconds writeTimeout{0};

  /**
   * Informs whether QoS is enabled.
   */
  bool enableQoS{false};

  /*
   * QoS class to use in packages.
   */
  unsigned int qosClass{0};

  /*
   * QoS path to use in packages.
   */
  unsigned int qosPath{0};

  /**
   * SSLContext provider callback. If null, then unsecured connections will be
   * established, else it will be called for each attempt to establish
   * connection.
   */
  std::function<std::shared_ptr<folly::SSLContext>()> sslContextProvider;

  /**
   * Path of the debug fifo.
   * If empty, debug fifo is disabled.
   */
  std::string debugFifoPath;

  /**
   * Name of the router that owns this connection.
   * NOTE: Must be be a literal (constexpr), and shouldn't be used
   * outside of mcrouter.
   */
  folly::StringPiece routerInfoName;

  /**
   * enable ssl session caching
   */
  bool sessionCachingEnabled{false};

  /**
   * enable ssl handshake offload to a separate thread pool
   */
  bool sslHandshakeOffload{false};

  /**
   * Use JemallocNodumpAllocator
   */
  bool useJemallocNodumpAllocator{false};

  /**
   * Map of codecs to use for compression.
   * If nullptr, compression will be disabled.
   */
  const CompressionCodecMap* compressionCodecMap{nullptr};

  /**
   * Service identity of the destination service when SSL is used.
   */
  std::string sslServiceIdentity;

  /**
   * Whether TFO is enabled for SSL connections
   */
  bool tfoEnabledForSsl{false};
};
}
} // facebook::memcache
