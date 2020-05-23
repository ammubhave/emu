#include <SFML/Graphics.hpp>
#include <iostream>

#include "Bus.h"
#include "GdbServer.h"
#include "Ram.h"
#include "Rom.h"
#include "cpu/Cpu.h"
#include "peripherals/Cmos.h"
#include "peripherals/Ps2.h"

const uint64_t CPU_FREQUENCY = 25000000;

using namespace emu::peripherals::cmos;
using namespace emu::peripherals::ps2;

int main(int /* argc */, const char*[] /* argv */) {
  sf::RenderWindow win(sf::VideoMode(200, 200), "Emu");

  Bus bus;

  Ram<0xa0000> ram;
  bus.addMemoryRange(0, &ram);

  Rom<0x10000> biosRom{"/Users/amol/Documents/code/emu-rs/rom/bios.bin"};
  bus.addMemoryRange(0xf0000, &biosRom);

  Cmos cmos;
  bus.addIoRange(0x70, &cmos);

  Ps2 ps2;
  bus.addIoRange(0x60, &cmos);

  Cpu cpu{bus};
  cpu.enableDebug();

  GdbServer gdbServer{cpu};

  uint64_t cycles = 0;
  while (win.isOpen()) {
    sf::Event event;
    while (win.pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        win.close();
      }
    }

    if (cycles % (CPU_FREQUENCY / 60)) {
      win.clear();
      win.display();
    }

    cpu.step();
    cycles = (cycles + 1) % CPU_FREQUENCY;
  }
  return 0;
}
