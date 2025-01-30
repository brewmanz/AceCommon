#line 2 "PrintStrTest.ino"
#ifdef NANO_OLD_BOOTLOADER
 #define BAUDRATE 38400 // old boot loader drops @ 57K6
#else
 #define BAUDRATE 115200 // ESP8266 default of 74880 not supported on Linux
#endif

#include <AUnit.h>
#include <AceCommon.h>

using namespace aunit;
using namespace ace_common;

//----------------------------------------------------------------------------
// Helpers
//----------------------------------------------------------------------------

static const int BUF_SIZE = 300;
static char STRING[BUF_SIZE];

static void fillString() {
  for (size_t i = 0; i < BUF_SIZE; i++) {
    char c = (i % 10) + '0';
    STRING[i] = c;
  }
  STRING[BUF_SIZE - 1] = '\0';
}

static void truncateString(int length) {
  STRING[length] = '\0';
}

//----------------------------------------------------------------------------
// Test that PrintStr<SiZE> can handle strings >= 255 but <= 65534.
// In practice, the SIZE is limited by the maximum stack size, which can be
// far smaller than the amount of static RAM avaiable. For example. ESP8266 has
// a max stack size of 4kB, even though it has 80kB of RAM.
//----------------------------------------------------------------------------

test(PrintStrTest, startsWith_KeyInFlash) {
  PrintStr<30> printStr;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  const __FlashStringHelper* pFSH;
  bool found;

  // key 'aei'
  pFSH = F("aei");
  // startsWith Key 'aei' false
  found = printStr.startsWith(pFSH);
  assertEqual(false, found);

  // key 'a'
  pFSH = F("a");
  // startsWith Key true
  found = printStr.startsWith(pFSH);
  assertEqual(true, found);

  // key 'ab'
  pFSH = F("ab");
  // startsWith Key true
  found = printStr.startsWith(pFSH);
  assertEqual(true, found);

  // key 'abd'
  pFSH = F("abd");
  // startsWith Key false
  found = printStr.startsWith(pFSH);
  assertEqual(false, found);

  // key 'y'
  pFSH = F("y");
  // startsWith Key false
  found = printStr.startsWith(pFSH);
  assertEqual(false, found);

  // key 'yz'
  pFSH = F("yz");
  // startsWith Key false
  found = printStr.startsWith(pFSH);
  assertEqual(false, found);
}
test(PrintStrTest, startsWith_KeyInPrintStr) {
  PrintStr<30> printStr, printKey;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  bool found;

  // key 'aei'
  printKey.flush();
  n = printKey.print(F("aei"));
  assertEqual(3, n);
  assertEqual(n, (int)printKey.length());
  // startsWith Key false
  found = printStr.startsWith(printKey);
  assertEqual(false, found);

  // key 'a'
  printKey.flush();
  printKey.print('a');
  // startsWith Key true
  found = printStr.startsWith(printKey);
  assertEqual(true, found);

  // key 'ab'
  printKey.print('b');
  // startsWith Key true
  found = printStr.startsWith(printKey);
  assertEqual(true, found);

  // key 'abd'
  printKey.print('d');
  // startsWith Key false
  found = printStr.startsWith(printKey);
  assertEqual(false, found);

  // key 'y'
  printKey.flush();
  printKey.print('y');
  // startsWith Key false
  found = printStr.startsWith(printKey);
  assertEqual(false, found);

  // key 'yz'
  printKey.print('z');
  // startsWith Key false
  found = printStr.startsWith(printKey);
  assertEqual(false, found);
}

test(PrintStrTest, startsWith_KeyInRAM) {
  PrintStr<30> printStr, printKey;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  bool found;

  // key 'aei'
  printKey.flush();
  n = printKey.print(F("aei"));
  assertEqual(3, n);
  assertEqual(n, (int)printKey.length());
  // startsWith Key false
  found = printStr.startsWith(printKey.cstr());
  assertEqual(false, found);

  // key 'a'
  printKey.flush();
  printKey.print('a');
  // startsWith Key true
  found = printStr.startsWith(printKey.cstr());
  assertEqual(true, found);

  // key 'ab'
  printKey.print('b');
  // startsWith Key true
  found = printStr.startsWith(printKey.cstr());
  assertEqual(true, found);

  // key 'abd'
  printKey.print('d');
  // startsWith Key false
  found = printStr.startsWith(printKey.cstr());
  assertEqual(false, found);

  // key 'y'
  printKey.flush();
  printKey.print('y');
  // startsWith Key false
  found = printStr.startsWith(printKey.cstr());
  assertEqual(false, found);

  // key 'yz'
  printKey.print('z');
  // startsWith Key false
  found = printStr.startsWith(printKey.cstr());
  assertEqual(false, found);
}

test(PrintStrTest, indexOf_KeyInFlash) {
  PrintStr<30> printStr;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  const __FlashStringHelper* pFSH;
  // key 'aei'
  pFSH = F("aei");
  // indexOf Key 'aei' not found
  n = printStr.indexOf(pFSH);
  assertEqual(-1, n);

  // key 'a'
  pFSH = F("a");
  // indexOf Key at start
  n = printStr.indexOf(pFSH);
  assertEqual(0, n);

  // key 'ab'
  pFSH = F("ab");
  // indexOf Key at start
  n = printStr.indexOf(pFSH);
  assertEqual(0, n);

  // key 'abd'
  pFSH = F("abd");
  // indexOf Key not found
  n = printStr.indexOf(pFSH);
  assertEqual(-1, n);

  // key 'y'
  pFSH = F("y");
  // indexOf Key near end
  n = printStr.indexOf(pFSH);
  assertEqual(24, n);

  // key 'yz'
  pFSH = F("yz");
  // indexOf Key near end
  n = printStr.indexOf(pFSH);
  assertEqual(24, n);

  // key 'yza'
  pFSH = F("yza");
  // indexOf Key not found
  n = printStr.indexOf(pFSH);
  assertEqual(-1, n);
}
test(PrintStrTest, indexOf_KeyInPrintStr) {
  PrintStr<30> printStr, printKey;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  // key 'aei'
  printKey.flush();
  n = printKey.print(F("aei"));
  assertEqual(3, n);
  assertEqual(n, (int)printKey.length());
  // indexOf Key 'aei' not found
  n = printStr.indexOf(printKey);
  assertEqual(-1, n);

  // key 'a'
  printKey.flush();
  printKey.print('a');
  // indexOf Key at start
  n = printStr.indexOf(printKey);
  assertEqual(0, n);

  // key 'ab'
  printKey.print('b');
  // indexOf Key at start
  n = printStr.indexOf(printKey);
  assertEqual(0, n);

  // key 'abd'
  printKey.print('d');
  // indexOf Key not found
  n = printStr.indexOf(printKey);
  assertEqual(-1, n);

  // key 'y'
  printKey.flush();
  printKey.print('y');
  // indexOf Key near end
  n = printStr.indexOf(printKey);
  assertEqual(24, n);

  // key 'yz'
  printKey.print('z');
  // indexOf Key near end
  n = printStr.indexOf(printKey);
  assertEqual(24, n);

  // key 'yza'
  printKey.print('a');
  // indexOf Key not found
  n = printStr.indexOf(printKey);
  assertEqual(-1, n);
}
test(PrintStrTest, indexOf_KeyInRAM) {
  PrintStr<30> printStr, printKey;
  int n = printStr.print(F("abcdefghijklmnopqrstuvwxyz"));
  assertEqual(26, n);
  assertEqual(n, (int)printStr.length());

  // key 'aei'
  printKey.flush();
  n = printKey.print(F("aei"));
  assertEqual(3, n);
  assertEqual(n, (int)printKey.length());
  // indexOf Key 'aei' not found
  n = printStr.indexOf(printKey.cstr());
  assertEqual(-1, n);

  // key 'a'
  printKey.flush();
  printKey.print('a');
  // indexOf Key at start
  n = printStr.indexOf(printKey.cstr());
  assertEqual(0, n);

  // key 'ab'
  printKey.print('b');
  // indexOf Key at start
  n = printStr.indexOf(printKey.cstr());
  assertEqual(0, n);

  // key 'abd'
  printKey.print('d');
  // indexOf Key not found
  n = printStr.indexOf(printKey.cstr());
  assertEqual(-1, n);

  // key 'y'
  printKey.flush();
  printKey.print('y');
  // indexOf Key near end
  n = printStr.indexOf(printKey.cstr());
  assertEqual(24, n);

  // key 'yz'
  printKey.print('z');
  // indexOf Key near end
  n = printStr.indexOf(printKey.cstr());
  assertEqual(24, n);

  // key 'yza'
  printKey.print('a');
  // indexOf Key not found
  n = printStr.indexOf(printKey.cstr());
  assertEqual(-1, n);
}

test(PrintStrTest, flush) {
  PrintStr<10> printStr;
  size_t n = printStr.print('a');
  assertEqual("a", printStr.cstr());
  assertEqual((size_t) 1, n);
  assertEqual((size_t) n, printStr.length());

  printStr.flush();
  assertEqual("", printStr.cstr());
  assertEqual((size_t) 0, printStr.length());

  n = printStr.print("zyx");
  assertEqual("zyx", printStr.cstr());
  assertEqual((size_t) 3, n);
  assertEqual((size_t) n, printStr.length());

  printStr.flush();
  assertEqual("", printStr.cstr());
  assertEqual((size_t) 0, printStr.length());
}

test(PrintStrTest, print_underSized) {
  fillString();
  PrintStr<310> printStr;
  size_t n = printStr.print(STRING);

  // Verify that the entire STRING got copied
  assertEqual((size_t) BUF_SIZE - 1, n);
  assertEqual((size_t) BUF_SIZE - 1, printStr.length());
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrTest, print_overSized) {
  fillString();
  PrintStr<270> printStr;
  size_t n = printStr.print(STRING);

  // Verify that the STRING was truncated to 269
  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrTest, write_overSized) {
  fillString();
  PrintStr<270> printStr;
  size_t n = printStr.write((const uint8_t*) STRING, BUF_SIZE - 1);

  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

//----------------------------------------------------------------------------
// Test PrintStrN(size) which creates the char array buffer on the heap.
//----------------------------------------------------------------------------

test(PrintStrNTest, flush) {
  PrintStrN printStr(10);
  size_t n = printStr.print('a');
  assertEqual((size_t) 1, n);
  assertEqual((size_t) 1, printStr.length());

  printStr.flush();
  assertEqual("", printStr.cstr());
}

test(PrintStrNTest, print_underSized) {
  fillString();
  PrintStrN printStr(310);
  size_t n = printStr.print(STRING);

  // Verify that the entire STRING got copied
  assertEqual((size_t) BUF_SIZE - 1, n);
  assertEqual((size_t) BUF_SIZE - 1, printStr.length());
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrNTest, print_overSized) {
  fillString();
  PrintStrN printStr(270);
  size_t n = printStr.print(STRING);

  // Verify that the STRING was truncated to 269
  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

test(PrintStrNTest, write_overSized) {
  fillString();
  PrintStrN printStr(270);
  size_t n = printStr.write((const uint8_t*) STRING, BUF_SIZE - 1);

  assertEqual((size_t) 269, n);
  assertEqual((size_t) 269, printStr.length());
  truncateString(269);
  assertEqual(STRING, printStr.cstr());
}

//----------------------------------------------------------------------------
// setup() and loop()
//----------------------------------------------------------------------------

void setup() {
#ifndef EPOXY_DUINO
  delay(1000); // wait for stability on some boards to prevent garbage Serial
#endif

  SERIAL_PORT_MONITOR.begin(BAUDRATE);
  while(!SERIAL_PORT_MONITOR); // for the Arduino Leonardo/Micro only

#if defined(EPOXY_DUINO)
  Serial.setLineModeUnix();
#endif
}

void loop() {
  aunit::TestRunner::run();
}
