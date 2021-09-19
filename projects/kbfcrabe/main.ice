// MIT license, see LICENSE_MIT in Silice repo root

$$if not ORANGECRAB and not SIMULATION then
$$error([[this project is specific to the OrangeCrab mounted
$$               on a FeatherWing Keyboard from @solderparty]])
$$end

// ===== screen CONFIGURATION, adjust to your setups
$$screen_width  = 240
$$screen_height = 320

// include SPI controller
$include('../common/spi.ice')

// PLL ; NOTE: we're happily overclocking at 100 MHz, passes ~65 MHz
$$if ORANGECRAB then
import('../common/plls/orangecrab_100.v')
$$end

// CPU declaration
riscv cpu_drawer(output uint1  screen_rst,
                 output uint32 screen,
                 output uint1  on_screen,
                 output uint32 rgb,
                 output uint1  on_rgb,
                 input  uint1  rgb_ready,
                 output uint32 leds,
                 output uint1  on_leds
                ) <
                  mem=98304, // we allocate a big chunk for the framebuffer
                  core="ice-v-dual", // dual core please
                  ICEV_FAST_SHIFT=1, // fast shifts (barrel shifter)
                  O=3        // compile with -O3
                > {

  // =============== firmware in C language ===========================
  #include "lcd_ili9351.h" // include LCD screen driver

  //_ framebuffer
  unsigned char tbl[320*240 + 8/*padding*/];

  //_ 32 xRGB entries ('fire' palette)
  unsigned char pal[] = {
    0,0x04,0x04,0x04, 0,0x1c,0x04,0x04, 0,0x2c,0x0c,0x04, 0,0x44,0x0c,0x04,
    0,0x54,0x14,0x04, 0,0x64,0x1c,0x04, 0,0x74,0x1c,0x04, 0,0x9c,0x2c,0x04,
    0,0xac,0x3c,0x04, 0,0xbc,0x44,0x04, 0,0xc4,0x44,0x04, 0,0xdc,0x54,0x04,
    0,0xdc,0x54,0x04, 0,0xd4,0x5c,0x04, 0,0xd4,0x5c,0x04, 0,0xd4,0x64,0x0c,
    0,0xcc,0x74,0x0c, 0,0xcc,0x7c,0x0c, 0,0xcc,0x84,0x14, 0,0xc4,0x84,0x14,
    0,0xc4,0x94,0x1c, 0,0xbc,0x9c,0x1c, 0,0xbc,0x9c,0x1c, 0,0xbc,0xa4,0x24,
    0,0xbc,0xa4,0x24, 0,0xbc,0xac,0x2c, 0,0xb4,0xac,0x2c, 0,0xb4,0xb4,0x2c,
    0,0xcc,0xcc,0x6c, 0,0xdc,0xdc,0x9c, 0,0xec,0xec,0xc4, 0,0xef,0xef,0xef};

  //_ draws fire onto the LCD
  void draw_fire()
  {
    while (1) {
      //leds(0); // uncomment to measure loop cycles in simulation
      for (int u=0;u<320;u++) {
        unsigned char *col = tbl + u;
        for (int v=0;v<240;++v) {
          int clr  = ((*col)>>2)&31;
          col     += 320;
          int *ptr = (int*)(pal) + clr;
          // screen_pix(*ptr++,*ptr++,*ptr++);
          rgb(*ptr);
          // insert a delay to wait for SPI completion
          asm volatile ("nop; nop; nop; nop; nop; nop; nop;");
        }
      }
    }
  }

  //_ update the fire framebuffer
  void update_fire()
  {
    int rng  = 31421;  // random number generator seed
    while (1) {
      // move up
      unsigned char *below   = tbl;
      unsigned char *current = tbl + 320;
      for (int v=320;v<240*320;v++) {
        int clr = 0;
        if ((*below) > 1) {
          clr = (*below)-(rng&1);
        }
        rng = ((rng<<5) ^ 6927) + ((rng>>5) ^ v);
        *(current + (rng&3)) = clr; // NOTE: table padding avoids
                                    //       out of bounds access
        ++ below;
        ++ current;
      }
      // keep the heat on
      unsigned char *ptr = tbl;
      for (int v=0;v<320;v++) {
        rng = ((rng<<5) ^ 6927) ^ (rng>>5);
        (*ptr++) = 120 + (rng&7);
      }
    }
  }

  //_ C main
  void main() {
    if (cpu_id() == 0) {
      screen_init();
      screen_rect(0,240, 0,320);
      draw_fire();
    } else {
      update_fire();
    }
  }

  // =============== end of firmware ==================================
}

// now we are creating the hardware hosting the CPU
algorithm main(
  output uint8  leds,
$$if SIMULATION then
) {
$$else
  inout  uint14 G,
  inout  uint6  A,
  output uint1  sck,
  output uint1  sda,
  output uint1  scl,
  output uint1  mosi,
  input  uint1  miso,
// PLL
) <@fast_clock> {
  uint1 fast_clock(0);
  pll main_pll(
    clkin   <: clock,
    clkout0 :> fast_clock
  );
$$end
  // instantiates our CPU as defined above
  cpu_drawer cpu;

$$if SIMULATION then
  uint32 cycle(0);
  uint32 prev(0);
$$end

  // sending a full pixel
  uint24 pix_data = uninitialized;
  uint3  pix_sending(3b000);
  uint1  pix_wait(0);
  uint1  pix_send_ready  <:: displ.ready & ~pix_wait; // can we send?
  //                         ^^^ SPI ready  ^^^ did not send just before
  // We have to check we did not send on cycle before as it takes one cycle
  // for the SPI controller to lower displ.ready after a disp.enable

  // screen driver
  uint1 displ_en         <:: pix_sending[0,1] ? pix_send_ready
                                              : cpu.on_screen;
  uint1 displ_dta_or_cmd <:: pix_sending[0,1] ? 1b1           : cpu.screen[10,1];
  uint8 displ_byte       <:: pix_sending[0,1] ? pix_data[0,8] : cpu.screen[0,8];
  spi_mode3_send displ(
    enable          <: displ_en,
    data_or_command <: displ_dta_or_cmd,
    byte            <: displ_byte,
  );

  always {
    leds          = cpu.leds;
    // CPU requests RGB write
    if (cpu.on_rgb) {
$$if SIMULATION then
      if (pix_sending[0,1]) {
        __display("[ERROR] still sending previous pixel");
      }
$$end
      // grab pixel data
      pix_data    = cpu.rgb[8,24];
      // initiate sending RGB
      pix_sending = 3b111;
    } else {
      // if we send the next pixel, shift to next RGB component
      pix_data    = pix_send_ready ? pix_data>>8    : pix_data;
      pix_sending = pix_send_ready ? pix_sending>>1 : pix_sending;
    }
    // did we send this cycle?        vvv something to send    vvv SPI ready
    pix_wait      = pix_wait ? 1b0 : (pix_sending[0,1] & displ.ready);
$$if not SIMULATION then
    G.oenable = 14b11111111111111;
    G.o       = {
      /*13:        */ 1b0,
      /*12:        */ 1b0,
      /*11:        */ 1b0,
      /*10:  lcd_dc*/ displ.spi_dc,
      /* 9:  lcd_cs*/ 1b0,
      /* 8:        */ 1b0,
      /* 7:        */ 1b0,
      /* 6: stmp_cs*/ 1b1,
      /* 5: card_cs*/ 1b1,
      /* 4:        */ 1b0,
      /* 3:        */ 1b0,
      /* 2:        */ 1b0,
      /* 1:        */ 1b0,
      /* 0:        */ 1b0
    };
    A.oenable = 6b111111;
    A.o       = {5b0,cpu.screen_rst};
    mosi      = displ.spi_mosi;
    sck       = displ.spi_clk;
$$else
    if (cpu.on_leds) {
      __display("%d] elapsed: %d cycles",cpu.leds,cycle - prev);
      prev = cycle;
    }
    // if (cycle == 128) { __finish(); }
    cycle     = cycle + 1;
$$end
  }

}
