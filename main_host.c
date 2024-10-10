/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Ha Thach (tinyusb.org)
 *                    sekigon-gonnoc
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

// This example runs both host and device concurrently. The USB host receive
// reports from HID device and print it out over USB Device CDC interface.
// For TinyUSB roothub port0 is native usb controller, roothub port1 is
// pico-pio-usb.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "pico/stdlib.h"
#include "pico/multicore.h"
#include "pico/bootrom.h"

#include "pio_usb.h"
#include "tusb.h"
#include "usb_descriptors.h"

//--------------------------------------------------------------------+
// KEYBOARD ATTACHMENT HEADERS
//--------------------------------------------------------------------+

#include "kbdattchmnt.h"
#include "datamanager.h"
uint8_t const numpadkey[10]={0x62,0x59,0x5a,0x5b,0x5c,0x5d,0x5e,0x5f,0x60,0x61};


//--------------------------------------------------------------------+
// MACRO CONSTANT TYPEDEF PROTYPES
//--------------------------------------------------------------------+



static uint8_t const keycode2ascii[128][2] =  { HID_KEYCODE_TO_ASCII };


/*------------- MAIN -------------*/

#define PIO_USB_CONFIG                                                 \
  {                                                                            \
    PIO_USB_DP_PIN_DEFAULT, PIO_USB_TX_DEFAULT, PIO_SM_USB_TX_DEFAULT,         \
        PIO_USB_DMA_TX_DEFAULT, PIO_USB_RX_DEFAULT, PIO_SM_USB_RX_DEFAULT,     \
        PIO_SM_USB_EOP_DEFAULT, NULL, PIO_USB_DEBUG_PIN_NONE,                  \
        PIO_USB_DEBUG_PIN_NONE                                                 \
  }

// core1: handle host events
void core1_main() {
  sleep_ms(10);

  // Use tuh_configure() to pass pio configuration to the host stack
  // Note: tuh_configure() must be called before
  pio_usb_configuration_t pio_cfg = PIO_USB_CONFIG;
  tuh_configure(1, TUH_CFGID_RPI_PIO_USB_CONFIGURATION, &pio_cfg);

  // To run USB SOF interrupt in core1, init host stack for pio_usb (roothub
  // port1) on core1
  tuh_init(1);

  while (true) {
    tuh_task(); // tinyusb host task
  }
}

//--------------------------------------------------------------------+
// Host HID
//--------------------------------------------------------------------+

// Invoked when device with hid interface is mounted
// Report descriptor is also available for use. tuh_hid_parse_report_descriptor()
// can be used to parse common/simple enough descriptor.
// Note: if report descriptor length > CFG_TUH_ENUMERATION_BUFSIZE, it will be skipped
// therefore report_desc = NULL, desc_len = 0
void tuh_hid_mount_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* desc_report, uint16_t desc_len)
{
  (void)desc_report;
  (void)desc_len;

  // Interface protocol (hid_interface_protocol_enum_t)
  const char* protocol_str[] = { "None", "Keyboard", "Mouse" };
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  uint16_t vid, pid;
  tuh_vid_pid_get(dev_addr, &vid, &pid);

  char tempbuf[256];
  int count = sprintf(tempbuf, "[%04x:%04x][%u] HID Interface%u, Protocol = %s\r\n", vid, pid, dev_addr, instance, protocol_str[itf_protocol]);

  tud_cdc_write(tempbuf, count);
  tud_cdc_write_flush();

  // Receive report from boot keyboard & mouse only
  // tuh_hid_report_received_cb() will be invoked when report is available
  if (itf_protocol == HID_ITF_PROTOCOL_KEYBOARD || itf_protocol == HID_ITF_PROTOCOL_MOUSE)
  {
    if ( !tuh_hid_receive_report(dev_addr, instance) )
    {
      tud_cdc_write_str("Error: cannot request report\r\n");
    }
  }
}

// Invoked when device with hid interface is un-mounted
void tuh_hid_umount_cb(uint8_t dev_addr, uint8_t instance)
{
  char tempbuf[256];
  int count = sprintf(tempbuf, "[%u] HID Interface%u is unmounted\r\n", dev_addr, instance);
  tud_cdc_write(tempbuf, count);
  tud_cdc_write_flush();
}

// look up new key in previous keys
static inline bool find_key_in_report(hid_keyboard_report_t const *report, uint8_t keycode)
{
  for(uint8_t i=0; i<6; i++)
  {
    if (report->keycode[i] == keycode)  return true;
  }

  return false;
}

char returncharofnum(uint8_t n)
{
  switch(n)
  {
    case 0: return '0';
    case 1: return '1';
    case 2: return '2';
    case 3: return '3';
    case 4: return '4';
    case 5: return '5';
    case 6: return '6';
    case 7: return '7';
    case 8: return '8';
    case 9: return '9';
    case 10: return 'A';
    case 11: return 'B';
    case 12: return 'C';
    case 13: return 'D';
    case 14: return 'E';
    case 15: return 'F';
    default : return '~';
  }
}
// convert hid keycode to ascii and print via usb device CDC (ignore non-printable)
static void process_kbd_report(uint8_t dev_addr, hid_keyboard_report_t const *report)
{
  (void) dev_addr;
  static hid_keyboard_report_t prev_report = { 0, 0, {0} }; // previous report to check key released
  bool flush = false;

  static uint8_t dataset=0;
  if(!dataset)
  {
    data_init_all();
    dataset=1;
    tud_cdc_write_str("DATA INITIALISD");
  }
  static const uint8_t zeroreport[6]={0},delay=16;
  uint8_t keyinfo[6]={0};
  uint8_t report_keycode[6]={0};
  process_kbd_attachment(keyinfo,report);
  if(keyinfo[0])
  {
    if(keyinfo[1])
    {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD,KEYBOARD_MODIFIER_LEFTALT,zeroreport);
      for(uint8_t i=2;i<6;i++)
      {
        report_keycode[0]=numpadkey[keyinfo[i]];
        sleep_ms(delay);
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD,KEYBOARD_MODIFIER_LEFTALT,report_keycode);
        sleep_ms(delay);
        tud_hid_keyboard_report(REPORT_ID_KEYBOARD,KEYBOARD_MODIFIER_LEFTALT,zeroreport);
        tud_cdc_write_str("        pressed unicode          ");
      }
      sleep_ms(delay);
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD,0,zeroreport);
    }
    else 
    {
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD, prev_report.modifier, prev_report.keycode);
      sleep_ms(8);
      tud_hid_keyboard_report(REPORT_ID_KEYBOARD,0,zeroreport);
      tud_cdc_write_str("        send key as it is         ");
    }
  }
  for(uint8_t i=0; i<6; i++)
  {
    uint8_t keycode = report->keycode[i];
    if ( keycode )
    {
      if ( find_key_in_report(&prev_report, keycode) )
      {
        // exist in previous report means the current key is holding
      }else
      {
        // not existed in previous report means the current key is pressed

        bool const is_shift = report->modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT);
        uint8_t ch = keycode2ascii[keycode][is_shift ? 1 : 0];

        if (ch)
        {
          if (ch == '\n') tud_cdc_write("\r", 1);
          //tud_cdc_write(&ch, 1);
          flush = true;
        }
      }
    }
    // TODO example skips key released
  }

  if (flush) tud_cdc_write_flush();

  prev_report = *report;
}

// send mouse report to usb device CDC
static void process_mouse_report(uint8_t dev_addr, hid_mouse_report_t const * report)
{
  //------------- button state  -------------//
  //uint8_t button_changed_mask = report->buttons ^ prev_report.buttons;
  char l = report->buttons & MOUSE_BUTTON_LEFT   ? 'L' : '-';
  char m = report->buttons & MOUSE_BUTTON_MIDDLE ? 'M' : '-';
  char r = report->buttons & MOUSE_BUTTON_RIGHT  ? 'R' : '-';

  char tempbuf[32];
  int count = sprintf(tempbuf, "[%u] %c%c%c %d %d %d\r\n", dev_addr, l, m, r, report->x, report->y, report->wheel);

  tud_cdc_write(tempbuf, count);
  tud_cdc_write_flush();
}

// Invoked when received report from device via interrupt endpoint
void tuh_hid_report_received_cb(uint8_t dev_addr, uint8_t instance, uint8_t const* report, uint16_t len)
{
  (void) len;
  uint8_t const itf_protocol = tuh_hid_interface_protocol(dev_addr, instance);

  switch(itf_protocol)
  {
    case HID_ITF_PROTOCOL_KEYBOARD:
      process_kbd_report(dev_addr, (hid_keyboard_report_t const*) report );
    break;

    case HID_ITF_PROTOCOL_MOUSE:
      process_mouse_report(dev_addr, (hid_mouse_report_t const*) report );
    break;

    default: break;
  }

  // continue to request to receive report
  if ( !tuh_hid_receive_report(dev_addr, instance) )
  {
    tud_cdc_write_str("Error: cannot request report\r\n");
  }
}
