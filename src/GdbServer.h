#pragma once

#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iostream>
#include <optional>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>

#include "cpu/Cpu.h"

using emu::cpu::Cpu;

namespace {
std::string u8ToHex(uint8_t number) {
  char hex[3];
  sprintf(hex, "%02x", number);
  return std::string(hex);
}
std::string u32ToHex(uint32_t number) {
  std::stringstream ss;
  for (int i = 0; i < 4; i++) {
    ss << u8ToHex(number & 0xff);
    number >>= 8;
  }
  return ss.str();
}
uint8_t hexToU8(std::string hex) {
  return (uint8_t)strtoul(hex.c_str(), NULL, 16);
}
uint32_t hexToU32(std::string hex) {
  return (uint32_t)strtoul(hex.c_str(), NULL, 16);
}
}  // namespace

class GdbServer {
 public:
  GdbServer(Cpu& cpu) : cpu_{cpu} {
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    assert(sockfd != -1);

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = INADDR_ANY;
    sockaddr.sin_port = htons(9999);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt,
               sizeof(int));
    assert(bind(sockfd, reinterpret_cast<struct sockaddr*>(&sockaddr),
                sizeof(sockaddr)) >= 0);
    assert(listen(sockfd, 10) >= 0);

    listenThread = std::thread([this]() { loop(); });
  }

  ~GdbServer() {
    close(sockfd);
    listenThread.join();
  }

  void loop() {
    while (true) {
      auto addrlen = sizeof(sockaddr);
      int connection =
          accept(sockfd, reinterpret_cast<struct sockaddr*>(&sockaddr),
                 reinterpret_cast<socklen_t*>(&addrlen));
      if (connection < 0) {
        return;
      }

      auto readChar = [&]() {
        char buffer[1];
        while (true) {
          auto bytesRead = read(connection, buffer, 1);
          if (bytesRead == -1) throw std::string("");
          if (bytesRead == 0) continue;
          return buffer[0];
        }
      };
      auto readCommand = [&]() {
        while (true) {
          auto begin = readChar();
          if (begin == '+') continue;
          if (begin != '$') {
            std::cout << "unsupported char: " << begin << std::endl;
          }
          assert(begin == '$');
          break;
        }

        std::stringstream ss;
        uint8_t checksum = 0;
        while (true) {
          auto ch = readChar();
          if (ch == '#') break;
          checksum += (uint8_t)ch;
          ss << ch;
        }

        char receivedChecksum[3] = {0, 0, 0};
        receivedChecksum[0] = readChar();
        receivedChecksum[1] = readChar();

        assert(checksum == (uint8_t)strtoul(receivedChecksum, NULL, 16));

        return ss.str();
      };
      auto sendReply = [&](std::string reply) {
        send(connection, "+$", 2, 0);
        send(connection, reply.c_str(), reply.size(), 0);
        send(connection, "#", 1, 0);

        uint8_t checksum = 0;
        for (size_t i = 0; i < reply.size(); i++) {
          checksum += (uint8_t)reply[i];
        }
        char checksumString[3];
        sprintf(checksumString, "%02x", checksum);
        send(connection, checksumString, 2, 0);

        std::cout << checksumString << std::endl;
      };

      std::unordered_set<std::string> supportedFeatures;

      while (true) {
        // try {
        auto command = readCommand();

        if (command.find("qSupported") == 0) {
          command.erase(0, 11);
          size_t pos = 0;
          while ((pos = command.find(";")) != std::string::npos) {
            auto feature = command.substr(0, pos);
            supportedFeatures.emplace(feature);
            command.erase(0, pos + 1);
          }
          sendReply("+");
        } else if (command.find("qTStatus") == 0) {
          sendReply("T0");
        } else if (command.find("qTfV") == 0) {
          sendReply("");
        } else if (command.find("?") == 0) {
          sendReply("S05");
        } else if (command.find("qfThreadInfo") == 0) {
          sendReply("m0");
        } else if (command.find("qsThreadInfo") == 0) {
          sendReply("l");
        } else if (command.find("qAttached") == 0) {
          sendReply("1");
        } else if (command.find("qC") == 0) {
          sendReply("QC0");
        } else if (command.find("g") == 0) {
          std::stringstream ss;
          for (uint8_t i = 0; i < 16; i++) {
            ss << u32ToHex(getRegister(i));
          }
          sendReply(ss.str());
        } else if (command.find("p") == 0) {
          command.erase(0, 1);
          auto registerNumber = (int)strtoul(command.c_str(), NULL, 16);
          sendReply(u32ToHex(getRegister(registerNumber)));
        } else if (command.find("m") == 0) {
          command.erase(0, 1);
          auto addr = hexToU32(command.substr(0, command.find(',')));
          command.erase(0, command.find(',') + 1);
          auto len = hexToU32(command);
          std::stringstream ss;
          for (size_t i = 0; i < len; i++) {
            ss << u8ToHex(cpu_.bus_.readMemory(addr + i));
          }
          sendReply(ss.str());
        } else if (command.find("v") == 0) {
          command.erase(0, 1);
          std::cout << "Unrecognized v command: " << command << std::endl;
          sendReply("");
        } else if (command.find("qTfP") == 0) {
          sendReply("");
        } else if (command.find("H") == 0) {
          sendReply("OK");
        } else {
          std::cout << "Unsupported GDB command: " << command << std::endl;
        }
      };
      close(connection);
    }
  }

 private:
  uint32_t getRegister(uint8_t index) {
    switch (index) {
      case 0:
        return cpu_.regs_.ax;
      case 1:
        return cpu_.regs_.bx;
      case 2:
        return cpu_.regs_.cx;
      case 3:
        return cpu_.regs_.dx;
      case 4:
        return cpu_.regs_.sp;
      case 5:
        return cpu_.regs_.bp;
      case 6:
        return cpu_.regs_.si;
      case 7:
        return cpu_.regs_.di;
      case 8:
        return cpu_.regs_.ip;
      case 9:
        return cpu_.regs_.flags.value;
      case 10:
        return cpu_.regs_.cs.selector.value;
      case 11:
        return cpu_.regs_.ss.selector.value;
      case 12:
        return cpu_.regs_.ds.selector.value;
      case 13:
        return cpu_.regs_.es.selector.value;
      case 14:
        return 0;
      case 15:
        return 0;
    }
    return 0;
  }

  Cpu& cpu_;

  int sockfd;
  sockaddr_in sockaddr;
  std::thread listenThread;
};
